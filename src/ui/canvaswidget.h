#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <memory>

namespace core {
class Document;
class Layer;
class Tool;
class Selection;
}

namespace ui {

class CanvasView;
class CanvasRenderer;
class ToolCursor;

/**
 * @brief Canvas widget for displaying and editing images
 * 
 * The CanvasWidget provides:
 * - High-performance rendering of layered images
 * - Zoom, pan, and navigation controls
 * - Tool interaction and cursor management
 * - Selection visualization and manipulation
 * - Real-time preview of effects and adjustments
 * - GPU acceleration support
 */
class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    enum ViewMode {
        Normal,         // Standard editing view
        FullScreen,     // Full screen editing
        Presentation,   // Presentation mode
        Proof          // Color proof view
    };
    
    enum NavigationMode {
        Pan,           // Pan the canvas
        Zoom,          // Zoom in/out
        Rotate,        // Rotate the canvas
        Hand           // Hand tool navigation
    };

    CanvasWidget(QWidget* parent = nullptr);
    ~CanvasWidget();

    // Document management
    void setDocument(core::Document* document);
    core::Document* document() const { return m_document; }
    
    // View management
    void setViewMode(ViewMode mode);
    ViewMode viewMode() const { return m_viewMode; }
    
    void setNavigationMode(NavigationMode mode);
    NavigationMode navigationMode() const { return m_navigationMode; }
    
    // Zoom and pan
    void setZoomLevel(double zoom);
    double zoomLevel() const { return m_zoomLevel; }
    
    void setPanOffset(const QPoint& offset);
    QPoint panOffset() const { return m_panOffset; }
    
    void zoomIn();
    void zoomOut();
    void zoomToFit();
    void zoomToActualSize();
    void zoomToSelection();
    
    void centerOnPoint(const QPoint& point);
    void centerOnSelection();
    
    // Canvas display
    void setShowGrid(bool show);
    bool showGrid() const { return m_showGrid; }
    
    void setShowGuides(bool show);
    bool showGuides() const { return m_showGuides; }
    
    void setShowRulers(bool show);
    bool showRulers() const { return m_showRulers; }
    
    void setShowSelection(bool show);
    bool showSelection() const { return m_showSelection; }
    
    void setShowToolCursor(bool show);
    bool showToolCursor() const { return m_showToolCursor; }
    
    // Performance
    void setRenderQuality(int quality); // 0-100
    int renderQuality() const { return m_renderQuality; }
    
    void setGPUAcceleration(bool enable);
    bool isGPUAccelerated() const { return m_gpuAccelerated; }
    
    void setTileSize(int tileSize);
    int tileSize() const { return m_tileSize; }
    
    // Export
    QPixmap exportView(const QRect& rect = QRect());
    QPixmap exportDocument();
    
    // Selection
    QRect selectionBounds() const;
    void setSelection(const QRect& rect);
    void clearSelection();
    
    // Tools
    void setActiveTool(core::Tool* tool);
    core::Tool* activeTool() const { return m_activeTool; }
    
    // Cursor management
    void setCursor(const QCursor& cursor);
    void setToolCursor(const QCursor& cursor);
    void hideToolCursor();
    
    // Refresh and update
    void refresh();
    void updateView();
    void invalidateCache();

signals:
    void zoomChanged(double zoom);
    void panChanged(const QPoint& offset);
    void viewModeChanged(ViewMode mode);
    void navigationModeChanged(NavigationMode mode);
    void selectionChanged(const QRect& selection);
    void toolChanged(core::Tool* tool);
    void documentClicked(const QPoint& position);
    void documentDoubleClicked(const QPoint& position);
    void contextMenuRequested(const QPoint& position);

protected:
    // Event handling
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    
    // Drag and drop
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private slots:
    void onDocumentChanged(core::Document* document);
    void onLayerChanged();
    void onSelectionChanged();
    void onToolChanged(core::Tool* tool);
    void onRenderComplete();
    void onPerformanceUpdate();

private:
    // Core components
    core::Document* m_document;
    core::Tool* m_activeTool;
    
    // View state
    ViewMode m_viewMode;
    NavigationMode m_navigationMode;
    double m_zoomLevel;
    QPoint m_panOffset;
    QPoint m_lastMousePos;
    
    // Display options
    bool m_showGrid;
    bool m_showGuides;
    bool m_showRulers;
    bool m_showSelection;
    bool m_showToolCursor;
    
    // Performance settings
    int m_renderQuality;
    bool m_gpuAccelerated;
    int m_tileSize;
    
    // Rendering
    CanvasRenderer* m_renderer;
    QTimer* m_renderTimer;
    QTimer* m_performanceTimer;
    
    // UI components
    QScrollArea* m_scrollArea;
    CanvasView* m_canvasView;
    ToolCursor* m_toolCursor;
    
    // Interaction state
    bool m_isPanning;
    bool m_isZooming;
    bool m_isSelecting;
    QPoint m_dragStart;
    QRect m_selectionRect;
    
    // Coordinate conversion
    QPoint documentToView(const QPoint& docPos) const;
    QPoint viewToDocument(const QPoint& viewPos) const;
    QRect documentToView(const QRect& docRect) const;
    QRect viewToDocument(const QRect& viewRect) const;
    
    // Event processing
    void processMousePress(QMouseEvent* event);
    void processMouseMove(QMouseEvent* event);
    void processMouseRelease(QMouseEvent* event);
    void processWheel(QWheelEvent* event);
    
    // Navigation
    void startPan(const QPoint& pos);
    void updatePan(const QPoint& pos);
    void endPan();
    
    void startZoom(const QPoint& pos, double factor);
    void updateZoom(const QPoint& pos, double factor);
    void endZoom();
    
    // Rendering
    void scheduleRender();
    void updateRenderQuality();
    void updateTileSize();
    
    // Performance monitoring
    void startPerformanceMonitoring();
    void stopPerformanceMonitoring();
    void updatePerformanceMetrics();
    
    // Grid and guides
    void drawGrid(QPainter& painter);
    void drawGuides(QPainter& painter);
    void drawRulers(QPainter& painter);
    void drawSelection(QPainter& painter);
    
    // Cursor management
    void updateToolCursor();
    void showToolCursorAt(const QPoint& pos);
    void hideToolCursor();
};

} // namespace ui
