#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QPoint>
#include <QSize>
#include <QTimer>
#include <QApplication>
#include <QCursor>
#include <QPixmap>
#include <QImage>
#include <QPainterPath>

#include "../core/document.h"
namespace core { class RasterLayer; }

namespace ui {

enum class Tool {
    Move = 0,
    Brush = 1,
    Eraser = 2,
    Eyedropper = 3,
    RectSelect = 4,
    EllipseSelect = 5,
    LassoSelect = 6,
    Text = 7,
    Gradient = 8,
    CloneStamp = 9,
    HealingBrush = 10,
    MagicWand = 11,
    Crop = 12,
    Blur = 13,
    Sharpen = 14,
    Dodge = 15,
    Burn = 16,
    Sponge = 17
};

class CanvasView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CanvasView(QWidget* parent = nullptr);
    ~CanvasView();

    void setDocument(core::Document* document);
    core::Document* getDocument() const { return m_document; }

    void setCurrentTool(Tool tool) { m_currentTool = tool; updateCursor(); }
    Tool getCurrentTool() const { return m_currentTool; }
    
    void setBrushColor(const QColor& color) { m_brushColor = color; }
    QColor getBrushColor() const { return m_brushColor; }
    
    void setBrushSize(int size) { m_brushSize = size; }
    int getBrushSize() const { return m_brushSize; }
    
    void setCanvasImage(const QImage& image);
    void loadImageFile(const QString& filePath);

    void zoomIn();
    void zoomOut();
    void fitToView();
    void resetZoom();

signals:
    void colorPicked(const QColor& color);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
    void updateCursor();
    void updateCanvasPixmap();
    void updateFromDocument();
    core::RasterLayer* activeRasterLayer();
    void drawBrushStroke(const QPointF& from, const QPointF& to);
    void drawEraserStroke(const QPointF& from, const QPointF& to);
    void drawGrid(QPainter* painter, const QRectF& rect);
    void drawRulers(QPainter* painter);
    void drawSelection(QPainter* painter);
    void handleBrushTool(QMouseEvent* event);
    void handleEraserTool(QMouseEvent* event);
    void handleEyedropperTool(QMouseEvent* event);
    void handleRectSelectTool(QMouseEvent* event);
    void handleEllipseSelectTool(QMouseEvent* event);
    void handleLassoSelectTool(QMouseEvent* event);
    void handleTextTool(QMouseEvent* event);
    void handleGradientTool(QMouseEvent* event);
    void handleMoveTool(QMouseEvent* event);
    void handleCloneStampTool(QMouseEvent* event);
    void handleHealingBrushTool(QMouseEvent* event);
    void handleMagicWandTool(QMouseEvent* event);
    void handleCropTool(QMouseEvent* event);
    void handleBlurTool(QMouseEvent* event);
    void handleSharpenTool(QMouseEvent* event);
    void handleDodgeTool(QMouseEvent* event);
    void handleBurnTool(QMouseEvent* event);
    void handleSpongeTool(QMouseEvent* event);

private:
    core::Document* m_document;
    QGraphicsScene* m_scene;
    Tool m_currentTool;
    QPoint m_lastMousePos;
    bool m_isDrawing;
    bool m_isSelecting;
    QRect m_selectionRect;
    double m_zoomFactor;
    QTimer* m_updateTimer;
    
    // Canvas image data
    QImage m_canvasImage;
    QPixmap m_canvasPixmap;
    
    // Brush properties
    QColor m_brushColor;
    int m_brushSize;
    QPainterPath m_currentStroke;
};

} // namespace ui
