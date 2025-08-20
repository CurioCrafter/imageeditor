#include "canvas_view.h"
#include <QPainter>
#include <QScrollBar>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QImageReader>
#include <QFile>
#include "../core/layer.h"

namespace ui {

CanvasView::CanvasView(QWidget* parent)
    : QGraphicsView(parent)
    , m_document(nullptr)
    , m_scene(new QGraphicsScene(this))
    , m_currentTool(Tool::Move)
    , m_isDrawing(false)
    , m_isSelecting(false)
    , m_zoomFactor(1.0)
    , m_updateTimer(new QTimer(this))
    , m_brushColor(Qt::black)
    , m_brushSize(10)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setDragMode(QGraphicsView::RubberBandDrag);
    
    // Setup update timer
    m_updateTimer->setInterval(16); // ~60 FPS
    m_updateTimer->setSingleShot(false);
    
    // Set default cursor
    updateCursor();
    
    // Enable drag and drop
    setAcceptDrops(true);
    
    // Initialize default canvas
    m_canvasImage = QImage(800, 600, QImage::Format_ARGB32);
    m_canvasImage.fill(Qt::white);
    updateCanvasPixmap();
}

CanvasView::~CanvasView()
{
}

void CanvasView::setDocument(core::Document* document)
{
    // Disconnect from old document if any
    if (m_document) {
        disconnect(m_document, nullptr, this, nullptr);
    }
    m_document = document;
    if (m_document) {
        // Connect to document change signals to refresh view
        connect(m_document, &core::Document::layerAdded,        this, [this](int){ updateFromDocument(); });
        connect(m_document, &core::Document::layerRemoved,      this, [this](int){ updateFromDocument(); });
        connect(m_document, &core::Document::layerMoved,        this, [this](int, int){ updateFromDocument(); });
        connect(m_document, &core::Document::layerChanged,      this, [this](int){ updateFromDocument(); });
        connect(m_document, &core::Document::activeLayerChanged,this, [this](int){ updateFromDocument(); });
        connect(m_document, &core::Document::documentChanged,   this, [this](){ updateFromDocument(); });
        connect(m_document, &core::Document::sizeChanged,       this, [this](const QSize&){ updateFromDocument(); });

        // Update scene rect
        const QSize docSize = m_document->getSize();
        m_scene->setSceneRect(0, 0, docSize.width(), docSize.height());
        fitToView();
        updateFromDocument();
    }
}

void CanvasView::zoomIn()
{
    m_zoomFactor *= 1.2;
    scale(1.2, 1.2);
}

void CanvasView::zoomOut()
{
    m_zoomFactor /= 1.2;
    scale(1.0/1.2, 1.0/1.2);
}

void CanvasView::fitToView()
{
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    m_zoomFactor = 1.0;
}

void CanvasView::resetZoom()
{
    m_zoomFactor = 1.0;
    resetTransform();
}

void CanvasView::setCanvasImage(const QImage& image)
{
    m_canvasImage = image;
    updateCanvasPixmap();
}

void CanvasView::updateCanvasPixmap()
{
    m_canvasPixmap = QPixmap::fromImage(m_canvasImage);
    m_scene->clear();
    m_scene->addPixmap(m_canvasPixmap);
    m_scene->setSceneRect(m_canvasImage.rect());
}

void CanvasView::updateFromDocument()
{
    if (!m_document) return;
    // Render the whole document using CPU composition for now
    QImage rendered = m_document->render();
    if (!rendered.isNull()) {
        setCanvasImage(rendered);
    }
}

core::RasterLayer* CanvasView::activeRasterLayer()
{
    if (!m_document) return nullptr;
    auto active = m_document->getActiveLayer();
    if (!active) return nullptr;
    return dynamic_cast<core::RasterLayer*>(active.get());
}

void CanvasView::drawBrushStroke(const QPointF& from, const QPointF& to)
{
    QPainter painter(&m_canvasImage);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    // Set up brush
    QPen pen(m_brushColor);
    pen.setWidth(m_brushSize);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    
    // Draw line
    if (from == to) {
        // Single dot
        painter.drawPoint(from);
    } else {
        // Line stroke
        painter.drawLine(from, to);
    }
    
    // Persist stroke into active raster layer if available (layer position aware)
    if (auto* layer = activeRasterLayer()) {
        QImage layerImg = layer->getImage();
        QPainter lp(&layerImg);
        lp.setRenderHint(QPainter::Antialiasing, true);
        QPen pen(m_brushColor);
        pen.setWidth(m_brushSize);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        lp.setPen(pen);
        // Map scene coords to layer-local by subtracting layer position
        QPointF lfrom = from - layer->getPosition();
        QPointF lto   = to   - layer->getPosition();
        if (lfrom == lto) {
            lp.drawPoint(lfrom);
        } else {
            lp.drawLine(lfrom, lto);
        }
        lp.end();
        layer->setImage(layerImg);
    }
    updateCanvasPixmap();
}

void CanvasView::drawEraserStroke(const QPointF& from, const QPointF& to)
{
    QPainter painter(&m_canvasImage);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    
    // Set up eraser brush
    QPen pen(Qt::transparent);
    pen.setWidth(m_brushSize);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    
    // Draw eraser stroke
    if (from == to) {
        // Single dot
        painter.drawEllipse(from, m_brushSize/2.0, m_brushSize/2.0);
    } else {
        // Line stroke
        painter.drawLine(from, to);
    }
    
    // Persist erase into active raster layer if available (layer position aware)
    if (auto* layer = activeRasterLayer()) {
        QImage layerImg = layer->getImage();
        QPainter lp(&layerImg);
        lp.setRenderHint(QPainter::Antialiasing, true);
        lp.setCompositionMode(QPainter::CompositionMode_Clear);
        QPen pen(Qt::transparent);
        pen.setWidth(m_brushSize);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        lp.setPen(pen);
        QPointF lfrom = from - layer->getPosition();
        QPointF lto   = to   - layer->getPosition();
        if (lfrom == lto) {
            lp.drawEllipse(lfrom, m_brushSize/2.0, m_brushSize/2.0);
        } else {
            lp.drawLine(lfrom, lto);
        }
        lp.end();
        layer->setImage(layerImg);
    }
    updateCanvasPixmap();
}

void CanvasView::updateCursor()
{
    switch (m_currentTool) {
        case Tool::Move:
            setCursor(Qt::OpenHandCursor);
            break;
        case Tool::Brush:
            setCursor(Qt::CrossCursor);
            break;
        case Tool::Eraser:
            setCursor(Qt::CrossCursor);
            break;
        case Tool::Eyedropper:
            setCursor(Qt::CrossCursor);
            break;
        case Tool::RectSelect:
            setCursor(Qt::CrossCursor);
            break;
        case Tool::EllipseSelect:
            setCursor(Qt::CrossCursor);
            break;
        case Tool::LassoSelect:
            setCursor(Qt::CrossCursor);
            break;
        case Tool::Text:
            setCursor(Qt::IBeamCursor);
            break;
        case Tool::Gradient:
            setCursor(Qt::CrossCursor);
            break;
    }
}

void CanvasView::mousePressEvent(QMouseEvent* event)
{
    QPointF scenePos = mapToScene(event->pos());
    
    switch (m_currentTool) {
        case Tool::Brush:
            handleBrushTool(event);
            break;
        case Tool::Eraser:
            handleEraserTool(event);
            break;
        case Tool::Eyedropper:
            handleEyedropperTool(event);
            break;
        case Tool::RectSelect:
            handleRectSelectTool(event);
            break;
        case Tool::EllipseSelect:
            handleEllipseSelectTool(event);
            break;
        case Tool::LassoSelect:
            handleLassoSelectTool(event);
            break;
        case Tool::Text:
            handleTextTool(event);
            break;
        case Tool::Gradient:
            handleGradientTool(event);
            break;
        case Tool::CloneStamp:
            handleCloneStampTool(event);
            break;
        case Tool::HealingBrush:
            handleHealingBrushTool(event);
            break;
        case Tool::MagicWand:
            handleMagicWandTool(event);
            break;
        case Tool::Crop:
            handleCropTool(event);
            break;
        case Tool::Blur:
            handleBlurTool(event);
            break;
        case Tool::Sharpen:
            handleSharpenTool(event);
            break;
        case Tool::Dodge:
            handleDodgeTool(event);
            break;
        case Tool::Burn:
            handleBurnTool(event);
            break;
        case Tool::Sponge:
            handleSpongeTool(event);
            break;
        case Tool::Move:
            handleMoveTool(event);
            break;
    }
    
    QGraphicsView::mousePressEvent(event);
}



void CanvasView::mouseReleaseEvent(QMouseEvent* event)
{
    m_isDrawing = false;
    m_isSelecting = false;
    
    QGraphicsView::mouseReleaseEvent(event);
}

void CanvasView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void CanvasView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            zoomIn();
            break;
        case Qt::Key_Minus:
            zoomOut();
            break;
        case Qt::Key_0:
            resetZoom();
            break;
        case Qt::Key_F:
            fitToView();
            break;
        default:
            QGraphicsView::keyPressEvent(event);
            break;
    }
}

void CanvasView::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        QStringList supportedFormats;
        supportedFormats << "png" << "jpg" << "jpeg" << "bmp" << "tiff" << "gif" << "webp";
        
        foreach (const QUrl &url, event->mimeData()->urls()) {
            QString fileName = url.toLocalFile();
            QString extension = QFileInfo(fileName).suffix().toLower();
            
            if (supportedFormats.contains(extension)) {
                event->acceptProposedAction();
                return;
            }
        }
    }
    event->ignore();
}

void CanvasView::dropEvent(QDropEvent* event)
{
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        if (QFile::exists(fileName)) {
            loadImageFile(fileName);
            break; // Load only the first image for now
        }
    }
    event->acceptProposedAction();
}

void CanvasView::loadImageFile(const QString& filePath)
{
    QImageReader reader(filePath);
    QImage image = reader.read();
    
    if (image.isNull()) {
        qDebug() << "Could not load image:" << reader.errorString();
        return;
    }
    
    // Set the loaded image to the canvas
    setCanvasImage(image);
    qDebug() << "Image loaded successfully:" << filePath;
}

void CanvasView::drawBackground(QPainter* painter, const QRectF& rect)
{
    // Checkerboard background to visualize transparency
    const int tile = 16;
    QPixmap checker(tile * 2, tile * 2);
    checker.fill(Qt::lightGray);
    QPainter p(&checker);
    p.fillRect(0, 0, tile, tile, Qt::white);
    p.fillRect(tile, tile, tile, tile, Qt::white);
    p.end();
    painter->fillRect(rect, QBrush(checker));
}

void CanvasView::drawForeground(QPainter* painter, const QRectF& rect)
{
    // Draw rulers
    drawRulers(painter);
    
    // Draw selection
    if (m_isSelecting || !m_selectionRect.isNull()) {
        drawSelection(painter);
    }
}

void CanvasView::drawGrid(QPainter* painter, const QRectF& rect)
{
    // Simple grid drawing
    QPen gridPen(Qt::lightGray, 1, Qt::DotLine);
    painter->setPen(gridPen);
    
    int gridSize = 20;
    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);
    
    QVector<QLineF> lines;
    for (qreal x = left; x < rect.right(); x += gridSize) {
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    }
    for (qreal y = top; y < rect.bottom(); y += gridSize) {
        lines.append(QLineF(rect.left(), y, rect.right(), y));
    }
    
    painter->drawLines(lines);
}

void CanvasView::drawRulers(QPainter* painter)
{
    // Simple ruler drawing
    QPen rulerPen(Qt::black, 1);
    painter->setPen(rulerPen);
    
    QRect viewportRect = this->viewport()->rect();
    QPointF topLeft = mapToScene(viewportRect.topLeft());
    QPointF bottomRight = mapToScene(viewportRect.bottomRight());
    
    // Draw horizontal ruler
    painter->drawLine(0, 0, viewportRect.width(), 0);
    // Draw vertical ruler
    painter->drawLine(0, 0, 0, viewportRect.height());
}

void CanvasView::drawSelection(QPainter* painter)
{
    if (m_selectionRect.isNull()) return;
    
    QPen selectionPen(Qt::blue, 2, Qt::DashLine);
    painter->setPen(selectionPen);
    painter->setBrush(Qt::transparent);
    
    QRectF sceneRect = mapToScene(m_selectionRect).boundingRect();
    painter->drawRect(sceneRect);
}

void CanvasView::handleBrushTool(QMouseEvent* event)
{
    QPointF scenePos = mapToScene(event->pos());
    
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        
        // Start a new stroke
        m_currentStroke = QPainterPath();
        m_currentStroke.moveTo(scenePos);
        
        // Draw initial brush mark
        drawBrushStroke(scenePos, scenePos);
    }
    else if (m_isDrawing && event->buttons() & Qt::LeftButton) {
        // Continue the stroke with smooth interpolation
        QPointF lastScenePos = mapToScene(m_lastMousePos);
        
        // Calculate distance and interpolate points for smooth lines
        qreal distance = QLineF(lastScenePos, scenePos).length();
        int steps = qMax(1, int(distance / (m_brushSize / 4.0))); // Smooth interpolation
        
        for (int i = 1; i <= steps; ++i) {
            qreal t = qreal(i) / qreal(steps);
            QPointF interpolatedPos = lastScenePos + t * (scenePos - lastScenePos);
            drawBrushStroke(lastScenePos, interpolatedPos);
            lastScenePos = interpolatedPos;
        }
        
        m_currentStroke.lineTo(scenePos);
        m_lastMousePos = event->pos();
    }
}

void CanvasView::handleEraserTool(QMouseEvent* event)
{
    QPointF scenePos = mapToScene(event->pos());
    
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        
        // Draw with transparent color (erase)
        drawEraserStroke(scenePos, scenePos);
    }
    else if (m_isDrawing && event->buttons() & Qt::LeftButton) {
        // Continue erasing
        QPointF lastScenePos = mapToScene(m_lastMousePos);
        drawEraserStroke(lastScenePos, scenePos);
        m_lastMousePos = event->pos();
    }
}

void CanvasView::handleEyedropperTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        QPoint imagePos = scenePos.toPoint();
        
        // Check bounds
        if (imagePos.x() >= 0 && imagePos.x() < m_canvasImage.width() &&
            imagePos.y() >= 0 && imagePos.y() < m_canvasImage.height()) {
            
            QColor pickedColor = m_canvasImage.pixelColor(imagePos);
            m_brushColor = pickedColor;
            
            // Emit signal to update color panel
            emit colorPicked(pickedColor);
            
            qDebug() << "Color picked:" << pickedColor.name();
        }
    }
}

void CanvasView::handleRectSelectTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isSelecting = true;
        m_selectionRect = QRect(event->pos(), event->pos());
        qDebug() << "Rect select tool activated at" << event->pos();
    }
}



void CanvasView::handleEllipseSelectTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isSelecting = true;
        m_selectionRect = QRect(event->pos(), event->pos());
        
        // TODO: Implement elliptical selection
        qDebug() << "Ellipse select tool activated at" << event->pos();
    }
}

void CanvasView::handleLassoSelectTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isSelecting = true;
        m_selectionRect = QRect(event->pos(), event->pos());
        
        // TODO: Implement lasso selection
        qDebug() << "Lasso select tool activated at" << event->pos();
    }
}

void CanvasView::handleTextTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // TODO: Implement text tool
        qDebug() << "Text tool activated at" << event->pos();
    }
}

void CanvasView::handleGradientTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // TODO: Implement gradient tool
        qDebug() << "Gradient tool activated at" << event->pos();
    }
}

void CanvasView::handleMoveTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        setDragMode(QGraphicsView::ScrollHandDrag);
        setCursor(Qt::ClosedHandCursor);
        m_lastMousePos = event->pos();
    }
}

void CanvasView::handleCloneStampTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        qDebug() << "Clone stamp tool activated at" << event->pos();
    }
}

void CanvasView::handleHealingBrushTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        qDebug() << "Healing brush tool activated at" << event->pos();
    }
}

void CanvasView::handleMagicWandTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        qDebug() << "Magic wand tool activated at" << scenePos;
        // TODO: Implement magic wand selection
    }
}

void CanvasView::handleCropTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isSelecting = true;
        m_selectionRect = QRect(event->pos(), event->pos());
        qDebug() << "Crop tool activated at" << event->pos();
    }
}

void CanvasView::handleBlurTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        qDebug() << "Blur tool activated at" << event->pos();
    }
}

void CanvasView::handleSharpenTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        qDebug() << "Sharpen tool activated at" << event->pos();
    }
}

void CanvasView::handleDodgeTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        qDebug() << "Dodge tool activated at" << event->pos();
    }
}

void CanvasView::handleBurnTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        qDebug() << "Burn tool activated at" << event->pos();
    }
}

void CanvasView::handleSpongeTool(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_lastMousePos = event->pos();
        qDebug() << "Sponge tool activated at" << event->pos();
    }
}

void CanvasView::mouseMoveEvent(QMouseEvent* event)
{
    QPointF scenePos = mapToScene(event->pos());
    
    switch (m_currentTool) {
        case Tool::Brush:
            if (m_isDrawing) {
                handleBrushTool(event);
            }
            break;
        case Tool::Eraser:
            if (m_isDrawing) {
                handleEraserTool(event);
            }
            break;
        case Tool::RectSelect:
            if (m_isSelecting) {
                m_selectionRect.setBottomRight(event->pos());
                viewport()->update(); // Trigger redraw to show selection
            }
            break;
        case Tool::EllipseSelect:
            if (m_isSelecting) {
                m_selectionRect.setBottomRight(event->pos());
                viewport()->update(); // Trigger redraw to show selection
            }
            break;
        case Tool::LassoSelect:
            if (m_isSelecting) {
                m_selectionRect.setBottomRight(event->pos());
                viewport()->update(); // Trigger redraw to show selection
            }
            break;
        case Tool::Move:
            if (event->buttons() & Qt::LeftButton) {
                // Handle panning
                QPoint delta = event->pos() - m_lastMousePos;
                horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
                verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
                m_lastMousePos = event->pos();
            }
            break;
    }
    
    QGraphicsView::mouseMoveEvent(event);
}

} // namespace ui
