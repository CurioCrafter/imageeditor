#pragma once

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace core {
    class Engine;
    class Document;
}

namespace ui {

class CanvasView;
class LayerPanel;
class ToolPanel;
class ColorPanel;
class BrushPanel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    // Core engine access
    void setEngine(core::Engine* engine);
    core::Engine* engine() const { return m_engine; }

protected:
    void closeEvent(QCloseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private slots:
    // File menu actions
    void newDocument();
    void openDocument();
    void saveDocument();
    void saveAsDocument();
    void exportDocument();
    void quit();

    // Edit menu actions
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void preferences();

    // View menu actions
    void showLayerPanel(bool show);
    void showToolPanel(bool show);
    void showColorPanel(bool show);
    void showBrushPanel(bool show);
    void zoomIn();
    void zoomOut();
    void fitToWindow();
    void actualSize();

    // Help menu actions
    void about();
    void aboutQt();

private:
    void loadImageFile(const QString& filePath);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createDockWidgets();
    void createCentralWidget();
    void setupConnections();
    void updateWindowTitle();

    // Core components
    core::Engine* m_engine;
    core::Document* m_currentDocument;

    // UI components
    CanvasView* m_canvasView;
    // UI panels
    LayerPanel* m_layerPanel;
    ToolPanel* m_toolPanel;
    ColorPanel* m_colorPanel;
    BrushPanel* m_brushPanel;

    // Menus
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_viewMenu;
    QMenu* m_toolsMenu;
    QMenu* m_filterMenu;
    QMenu* m_helpMenu;

    // Toolbars
    QToolBar* m_fileToolBar;
    QToolBar* m_editToolBar;
    QToolBar* m_toolsToolBar;
    QToolBar* m_viewToolBar;

    // Actions
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_exportAction;
    QAction* m_quitAction;
    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_cutAction;
    QAction* m_copyAction;
    QAction* m_pasteAction;
    QAction* m_preferencesAction;
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    QAction* m_fitToWindowAction;
    QAction* m_actualSizeAction;
    QAction* m_aboutAction;
    QAction* m_aboutQtAction;
};

} // namespace ui
