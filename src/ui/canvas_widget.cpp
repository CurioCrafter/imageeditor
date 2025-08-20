#include "canvas_widget.h"
#include "../core/document.h"
#include "../core/tool.h"
#include <QPainter>
#include <QWheelEvent>
#include <QtMath>
#include <QTimer>
#include <algorithm>
#include <execution>

namespace ui {

// Canvas implementation with CPU rendering (Vulkan integration planned)
class CanvasWidget::Impl {
public:
    // Document and rendering
    core::Document* document = nullptr;
    QImage renderBuffer;
    
    // Viewport state
    QTransform viewTransform;
    double zoomLevel = 1.0;
    QPointF panOffset;
    QPointF rotationCenter;
    double rotation = 0.0;
    
    // Tile cache for large documents
    struct Tile {
        QRect bounds;
        QImage image;
        bool dirty = true;
        std::chrono::steady_clock::time_point lastAccess;
    };
    
    static constexpr int TILE_SIZE = 256;
    mutable std::unordered_map<uint64_t, Tile> tileCache;
    mutable std::mutex tileCacheMutex;
    
    // Performance monitoring
    std::chrono::steady_clock::time_point lastFrameTime;
    double averageFrameTime = 0.0;
    int frameCount = 0;
    
    // Tool interaction
    core::Tool* activeTool = nullptr;
    bool isDrawing = false;
    QPointF lastMousePos;
    std::vector<QPointF> currentStroke;
    
    // Display options
    bool showGrid = false;
    bool showGuides = true;
    bool showPixelGrid = false;
    float gridSpacing = 50.0f;
    QColor gridColor{128, 128, 128, 64};
    
    // Render quality settings
    enum RenderQuality { Draft, Normal, High };
    RenderQuality quality = Normal;
    bool useGPU = true;
    
    uint64_t getTileKey(int x, int y) const {
        return (static_cast<uint64_t>(x) << 32) | static_cast<uint64_t>(y);
    }
    
    QRect getTileBounds(int tileX, int tileY) const {
        return QRect(tileX * TILE_SIZE, tileY * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    }
    
    void invalidateTiles(const QRect& rect) {
        std::lock_guard<std::mutex> lock(tileCacheMutex);
        
        int startX = rect.left() / TILE_SIZE;
        int endX = (rect.right() + TILE_SIZE - 1) / TILE_SIZE;
        int startY = rect.top() / TILE_SIZE;
        int endY = (rect.bottom() + TILE_SIZE - 1) / TILE_SIZE;
        
        for (int y = startY; y < endY; ++y) {
            for (int x = startX; x < endX; ++x) {
                uint64_t key = getTileKey(x, y);
                if (auto it = tileCache.find(key); it != tileCache.end()) {
                    it->second.dirty = true;
                }
            }
        }
    }
    
    QImage renderTile(int tileX, int tileY) const {
        if (!document) return QImage();
        
        QRect tileBounds = getTileBounds(tileX, tileY);
        QImage tile(TILE_SIZE, TILE_SIZE, QImage::Format_ARGB32_Premultiplied);
        tile.fill(Qt::transparent);
        
        QPainter painter(&tile);
        painter.setRenderHint(QPainter::Antialiasing, quality != Draft);
        painter.translate(-tileBounds.topLeft());
        
        // Render document layers to this tile
        document->render(&painter, tileBounds);
        
        return tile;
    }
};

CanvasWidget::CanvasWidget(QWidget* parent)
    : QWidget(parent)
    , d(std::make_unique<Impl>())
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    
    // Enable high-performance rendering
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    
    // Setup update timer for smooth animations
    auto updateTimer = new QTimer(this);
    updateTimer->setInterval(16); // 60 FPS
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        if (d->isDrawing) {
            update();
        }
    });
    updateTimer->start();
}

CanvasWidget::~CanvasWidget() = default;

void CanvasWidget::setDocument(core::Document* doc) {
    if (d->document == doc) return;
    
    d->document = doc;
    d->tileCache.clear();
    
    if (doc) {
        // Connect to document signals
        connect(doc, &core::Document::layerChanged, this, [this](int layerIndex) {
            d->invalidateTiles(QRect(QPoint(0, 0), d->document->getSize()));
            update();
        });
        
        connect(doc, &core::Document::documentSizeChanged, this, [this](const QSize& newSize) {
            d->tileCache.clear();
            update();
        });
    }
    
    update();
}

void CanvasWidget::setActiveTool(core::Tool* tool) {
    if (d->activeTool == tool) return;
    
    if (d->activeTool) {
        d->activeTool->deactivate();
    }
    
    d->activeTool = tool;
    
    if (tool) {
        tool->activate(this);
        setCursor(tool->cursor());
    }
}

void CanvasWidget::setZoomLevel(double zoom) {
    zoom = std::clamp(zoom, 0.01, 100.0);
    if (qFuzzyCompare(d->zoomLevel, zoom)) return;
    
    // Zoom around viewport center
    QPointF center = rect().center();
    QPointF worldBefore = d->viewTransform.inverted().map(center);
    
    d->zoomLevel = zoom;
    updateTransform();
    
    QPointF worldAfter = d->viewTransform.inverted().map(center);
    d->panOffset += worldAfter - worldBefore;
    
    updateTransform();
    update();
    
    emit zoomChanged(zoom);
}

void CanvasWidget::zoomIn() {
    setZoomLevel(d->zoomLevel * 1.25);
}

void CanvasWidget::zoomOut() {
    setZoomLevel(d->zoomLevel / 1.25);
}

void CanvasWidget::fitToWindow() {
    if (!d->document) return;
    
    QSizeF docSize = d->document->getSize();
    QSizeF viewSize = size();
    
    double scaleX = viewSize.width() / docSize.width();
    double scaleY = viewSize.height() / docSize.height();
    double scale = std::min(scaleX, scaleY) * 0.9; // 90% to leave margin
    
    d->zoomLevel = scale;
    d->panOffset = QPointF(0, 0);
    d->rotation = 0;
    
    updateTransform();
    update();
}

void CanvasWidget::resetView() {
    d->zoomLevel = 1.0;
    d->panOffset = QPointF(0, 0);
    d->rotation = 0;
    updateTransform();
    update();
}

void CanvasWidget::initializeWidget() {
    // Initialize widget for high-performance rendering
    
    // Setup composite shader for layer blending
    // d->compositeShader = new QOpenGLShaderProgram(this); // Removed OpenGL shader
    
    // const char* vertexShader = R"( // Removed OpenGL shader
    //     #version 330 core
    //     layout(location = 0) in vec2 position;
    //     layout(location = 1) in vec2 texCoord;
    //     out vec2 fragTexCoord;
    //     uniform mat4 transform;
        
    //     void main() {
    //         gl_Position = transform * vec4(position, 0.0, 1.0);
    //         fragTexCoord = texCoord;
    //     }
    // )";
    
    // const char* fragmentShader = R"( // Removed OpenGL shader
    //     #version 330 core
    //     in vec2 fragTexCoord;
    //     out vec4 fragColor;
    //     uniform sampler2D texture0;
    //     uniform float opacity;
        
    //     void main() {
    //         fragColor = texture(texture0, fragTexCoord);
    //         fragColor.a *= opacity;
    //     }
    // )";
    
    // d->compositeShader->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader); // Removed OpenGL shader
    // d->compositeShader->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader); // Removed OpenGL shader
    // d->compositeShader->link(); // Removed OpenGL shader
}

void CanvasWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    auto startTime = std::chrono::steady_clock::now();
    
    // Clear background with QPainter
    QPainter painter(this);
    painter.fillRect(rect(), QColor(51, 51, 51, 255)); // Dark gray background
    
    if (!d->document) return;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, d->quality != Impl::Draft);
    
    // Apply view transform
    painter.setTransform(d->viewTransform);
    
    // Calculate visible tiles
    QRectF visibleRect = d->viewTransform.inverted().mapRect(rect());
    int startX = std::floor(visibleRect.left() / Impl::TILE_SIZE);
    int endX = std::ceil(visibleRect.right() / Impl::TILE_SIZE);
    int startY = std::floor(visibleRect.top() / Impl::TILE_SIZE);
    int endY = std::ceil(visibleRect.bottom() / Impl::TILE_SIZE);
    
    // Render visible tiles
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            uint64_t key = d->getTileKey(x, y);
            
            // Get or render tile
            auto& tile = d->tileCache[key];
            if (tile.dirty || tile.image.isNull()) {
                tile.bounds = d->getTileBounds(x, y);
                tile.image = d->renderTile(x, y);
                tile.dirty = false;
            }
            tile.lastAccess = std::chrono::steady_clock::now();
            
            // Draw tile
            painter.drawImage(tile.bounds, tile.image);
        }
    }
    
    // Draw grid overlay
    if (d->showGrid && d->zoomLevel > 0.25) {
        drawGrid(painter);
    }
    
    // Draw guides
    if (d->showGuides) {
        drawGuides(painter);
    }
    
    // Draw active tool overlay
    if (d->activeTool && d->isDrawing) {
        painter.save();
        // d->activeTool->draw(painter, d->lastMousePos);
        painter.restore();
    }
    
    // Update performance metrics
    auto endTime = std::chrono::steady_clock::now();
    auto frameTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    d->averageFrameTime = d->averageFrameTime * 0.9 + frameTime * 0.1;
    d->frameCount++;
    
    // Clean up old tiles if cache is too large
    if (d->tileCache.size() > 100) {
        cleanTileCache();
    }
}

void CanvasWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateTransform();
}

void CanvasWidget::mousePressEvent(QMouseEvent* event) {
    d->lastMousePos = event->pos();
    QPointF docPos = mapToDocument(event->pos());
    
    if (d->activeTool) {
        d->isDrawing = true;
        d->currentStroke.clear();
        d->currentStroke.push_back(docPos);
        
        // Tool event handling would go here
        // core::ToolEvent toolEvent;
        // toolEvent.pos = docPos;
        // toolEvent.pressure = 1.0f;
        // toolEvent.buttons = event->buttons();
        // toolEvent.modifiers = event->modifiers();
        // d->activeTool->mousePressEvent(toolEvent);
    }
    
    update();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* event) {
    QPointF docPos = mapToDocument(event->pos());
    
    if (d->isDrawing && d->activeTool) {
        // Smooth stroke interpolation
        if (!d->currentStroke.empty()) {
            QPointF lastPos = d->currentStroke.back();
            float distance = QLineF(lastPos, docPos).length();
            
            if (distance > 2.0) { // Minimum distance for new point
                d->currentStroke.push_back(docPos);
                
                // Invalidate affected tiles
                QRectF dirtyRect(lastPos, docPos);
                dirtyRect = dirtyRect.normalized().adjusted(-50, -50, 50, 50);
                d->invalidateTiles(dirtyRect.toRect());
            }
        }
    }
    
    // Pan with middle mouse button
    if (event->buttons() & Qt::MiddleButton) {
        QPointF delta = event->pos() - d->lastMousePos;
        d->panOffset += delta;
        updateTransform();
    }
    
    d->lastMousePos = event->pos();
    
    if (d->isDrawing) {
        update();
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (d->isDrawing && d->activeTool) {
        QPointF docPos = mapToDocument(event->pos());
        
        d->isDrawing = false;
        d->currentStroke.clear();
    }
    
    update();
}

void CanvasWidget::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        // Zoom with Ctrl+Wheel
        double delta = event->angleDelta().y() / 120.0;
        double factor = std::pow(1.1, delta);
        
        // Zoom around mouse position
        QPointF mousePos = event->position();
        QPointF worldBefore = d->viewTransform.inverted().map(mousePos);
        
        d->zoomLevel *= factor;
        d->zoomLevel = std::clamp(d->zoomLevel, 0.01, 100.0);
        updateTransform();
        
        QPointF worldAfter = d->viewTransform.inverted().map(mousePos);
        d->panOffset += (worldAfter - worldBefore) * d->zoomLevel;
        updateTransform();
        
        event->accept();
    } else {
        // Scroll
        d->panOffset += QPointF(event->angleDelta().x(), event->angleDelta().y()) * 0.5;
        updateTransform();
    }
    
    update();
}

void CanvasWidget::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Space:
            // Temporary hand tool
            setCursor(Qt::OpenHandCursor);
            break;
        case Qt::Key_R:
            if (event->modifiers() & Qt::ControlModifier) {
                // Reset rotation
                d->rotation = 0;
                updateTransform();
                update();
            }
            break;
        case Qt::Key_0:
            if (event->modifiers() & Qt::ControlModifier) {
                fitToWindow();
            }
            break;
        case Qt::Key_1:
            if (event->modifiers() & Qt::ControlModifier) {
                resetView();
            }
            break;
    }
    
    if (d->activeTool) {
        // d->activeTool->keyPressEvent(event);
    }
}

void CanvasWidget::updateTransform() {
    d->viewTransform = QTransform();
    
    // Center the document
    QSizeF docSize = d->document ? d->document->getSize() : QSizeF(800, 600);
    QPointF center = QPointF(docSize.width() / 2, docSize.height() / 2);
    
    // Apply transformations in order: scale, rotate, translate
    d->viewTransform.translate(rect().center().x(), rect().center().y());
    d->viewTransform.scale(d->zoomLevel, d->zoomLevel);
    d->viewTransform.rotate(d->rotation);
    d->viewTransform.translate(-center.x() - d->panOffset.x(), -center.y() - d->panOffset.y());
}

void CanvasWidget::drawGrid(QPainter& painter) {
    if (!d->document) return;
    
    QSizeF docSize = d->document->getSize();
    painter.setPen(QPen(d->gridColor, 1.0));
    
    // Draw vertical lines
    for (float x = 0; x <= docSize.width(); x += d->gridSpacing) {
        painter.drawLine(QPointF(x, 0), QPointF(x, docSize.height()));
    }
    
    // Draw horizontal lines
    for (float y = 0; y <= docSize.height(); y += d->gridSpacing) {
        painter.drawLine(QPointF(0, y), QPointF(docSize.width(), y));
    }
}

void CanvasWidget::drawGuides(QPainter& painter) {
    // Guide drawing implementation would go here
    // This would draw user-defined guides for alignment
}

void CanvasWidget::cleanTileCache() {
    std::lock_guard<std::mutex> lock(d->tileCacheMutex);
    
    auto now = std::chrono::steady_clock::now();
    auto threshold = std::chrono::seconds(30); // 30 seconds
    
    for (auto it = d->tileCache.begin(); it != d->tileCache.end();) {
        if (now - it->second.lastAccess > threshold) {
            it = d->tileCache.erase(it);
        } else {
            ++it;
        }
    }
}

QPointF CanvasWidget::mapToDocument(const QPoint& screenPos) const {
    return d->viewTransform.inverted().map(screenPos);
}

QPoint CanvasWidget::mapFromDocument(const QPointF& docPos) const {
    return d->viewTransform.map(docPos).toPoint();
}

// Getter implementations
core::Document* CanvasWidget::getDocument() const { return d->document; }
core::Tool* CanvasWidget::getActiveTool() const { return d->activeTool; }
double CanvasWidget::getZoomLevel() const { return d->zoomLevel; }
QPointF CanvasWidget::getPanOffset() const { return d->panOffset; }
CanvasWidget::RenderQuality CanvasWidget::getRenderQuality() const { return static_cast<RenderQuality>(d->quality); }
double CanvasWidget::getAverageFrameTime() const { return d->averageFrameTime; }
int CanvasWidget::getFrameCount() const { return d->frameCount; }

// Setter implementations
void CanvasWidget::setPanOffset(const QPointF& offset) {
    d->panOffset = offset;
    updateTransform();
    update();
}

void CanvasWidget::setShowGrid(bool show) {
    d->showGrid = show;
    update();
}

void CanvasWidget::setShowGuides(bool show) {
    d->showGuides = show;
    update();
}

void CanvasWidget::setShowPixelGrid(bool show) {
    d->showPixelGrid = show;
    update();
}

void CanvasWidget::setGridSpacing(float spacing) {
    d->gridSpacing = spacing;
    update();
}

void CanvasWidget::setGridColor(const QColor& color) {
    d->gridColor = color;
    update();
}

void CanvasWidget::setRenderQuality(RenderQuality quality) {
    d->quality = static_cast<Impl::RenderQuality>(quality);
    update();
}

void CanvasWidget::keyReleaseEvent(QKeyEvent* event) {
    // Handle key release events
    QWidget::keyReleaseEvent(event);
}

} // namespace ui
