#pragma once

#include <QWidget>
#include <QPainter>
#include <QTransform>
#include <QTimer>
#include <QPointF>
#include <QRect>
#include <QImage>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <chrono>

// Forward declarations
namespace core {
    class Document;
    class Tool;
    class ToolEvent;
}

namespace ui {

// Professional Canvas Widget with GPU acceleration
class CanvasWidget : public QWidget {
    Q_OBJECT
public:
    CanvasWidget(QWidget* parent = nullptr);
    ~CanvasWidget();

    // Document management
    void setDocument(core::Document* document);
    core::Document* getDocument() const;

    // Tool management
    void setActiveTool(core::Tool* tool);
    core::Tool* getActiveTool() const;

    // View controls
    void setZoomLevel(double zoom);
    double getZoomLevel() const;
    void zoomIn();
    void zoomOut();
    void fitToWindow();
    void resetView();
    void setPanOffset(const QPointF& offset);
    QPointF getPanOffset() const;

    // Display options
    void setShowGrid(bool show);
    void setShowGuides(bool show);
    void setShowPixelGrid(bool show);
    void setGridSpacing(float spacing);
    void setGridColor(const QColor& color);

    // Render quality
    enum RenderQuality { Draft, Normal, High };
    void setRenderQuality(RenderQuality quality);
    RenderQuality getRenderQuality() const;

    // Performance monitoring
    double getAverageFrameTime() const;
    int getFrameCount() const;

signals:
    void zoomChanged(double zoom);
    void viewChanged();
    void toolActivated(core::Tool* tool);
    void documentChanged(core::Document* document);

protected:
    // Widget overrides
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    // Event handling
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    // Private implementation
    struct Impl;
    std::unique_ptr<Impl> d;

    // Internal methods
    void initializeWidget();
    void updateTransform();
    void drawGrid(QPainter& painter);
    void drawGuides(QPainter& painter);
    void cleanTileCache();
    QPointF mapToDocument(const QPoint& screenPos) const;
    QPoint mapFromDocument(const QPointF& docPos) const;
};

} // namespace ui
