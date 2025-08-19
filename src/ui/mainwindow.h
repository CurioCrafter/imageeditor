#pragma once

#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QActionGroup>
#include <memory>

namespace core {
class Document;
class Engine;
}

namespace ui {

class CanvasWidget;
class LayerPanel;
class ToolPanel;
class ColorPanel;
class HistoryPanel;
class PropertiesPanel;
class AIPanel;

/**
 * @brief Main application window with dockable panels and canvas
 * 
 * The MainWindow provides:
 * - Central canvas for image editing
 * - Dockable panels for layers, tools, colors, etc.
 * - Menu system with standard image editing operations
 * - Toolbar with common tools and actions
 * - Status bar with document information
 * - Support for multiple document tabs
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    // Document management
    core::Document* currentDocument() const;
    void setCurrentDocument(core::Document* document);
    
    // Panel management
    void showPanel(const QString& panelName);
    void hidePanel(const QString& panelName);
    void togglePanel(const QString& panelName);
    bool isPanelVisible(const QString& panelName) const;
    
    // Canvas operations
    void zoomIn();
    void zoomOut();
    void zoomToFit();
    void zoomToActualSize();
    void setZoomLevel(double zoom);
    double zoomLevel() const;
    
    // File operations
    void newDocument();
    void openDocument();
    void saveDocument();
    void saveDocumentAs();
    void exportDocument();
    void closeDocument();
    
    // Edit operations
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();
    void deselectAll();
    
    // View operations
    void showGrid(bool show);
    void showGuides(bool show);
    void showRulers(bool show);
    void toggleFullScreen();

protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void onDocumentChanged(core::Document* document);
    void onDocumentSaved(const QString& filePath);
    void onDocumentClosed();
    void onErrorOccurred(const QString& error);
    void onPerformanceMetricsUpdated(double frameTime, double memoryUsage);
    
    // Menu actions
    void onNewDocument();
    void onOpenDocument();
    void onSaveDocument();
    void onSaveDocumentAs();
    void onExportDocument();
    void onCloseDocument();
    void onExit();
    
    void onUndo();
    void onRedo();
    void onCut();
    void onCopy();
    void onPaste();
    void onSelectAll();
    void onDeselectAll();
    
    void onZoomIn();
    void onZoomOut();
    void onZoomToFit();
    void onZoomToActualSize();
    
    void onShowGrid();
    void onShowGuides();
    void onShowRulers();
    void onToggleFullScreen();
    
    void onAbout();
    void onPreferences();
    void onHelp();

private:
    // Core components
    core::Engine* m_engine;
    core::Document* m_currentDocument;
    
    // UI components
    CanvasWidget* m_canvas;
    QTabWidget* m_documentTabs;
    
    // Dockable panels
    QDockWidget* m_layerDock;
    QDockWidget* m_toolDock;
    QDockWidget* m_colorDock;
    QDockWidget* m_historyDock;
    QDockWidget* m_propertiesDock;
    QDockWidget* m_aiDock;
    
    // Panel widgets
    LayerPanel* m_layerPanel;
    ToolPanel* m_toolPanel;
    ColorPanel* m_colorPanel;
    HistoryPanel* m_historyPanel;
    PropertiesPanel* m_propertiesPanel;
    AIPanel* m_aiPanel;
    
    // Menu system
    QMenuBar* m_menuBar;
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_viewMenu;
    QMenu* m_layerMenu;
    QMenu* m_filterMenu;
    QMenu* m_aiMenu;
    QMenu* m_helpMenu;
    
    // Toolbars
    QToolBar* m_mainToolBar;
    QToolBar* m_toolToolBar;
    QToolBar* m_viewToolBar;
    
    // Actions
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_exportAction;
    QAction* m_closeAction;
    QAction* m_exitAction;
    
    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_cutAction;
    QAction* m_copyAction;
    QAction* m_pasteAction;
    QAction* m_selectAllAction;
    QAction* m_deselectAllAction;
    
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    QAction* m_zoomFitAction;
    QAction* m_zoomActualAction;
    
    QAction* m_showGridAction;
    QAction* m_showGuidesAction;
    QAction* m_showRulersAction;
    QAction* m_fullScreenAction;
    
    QAction* m_aboutAction;
    QAction* m_preferencesAction;
    QAction* m_helpAction;
    
    // Status bar
    QStatusBar* m_statusBar;
    QLabel* m_zoomLabel;
    QLabel* m_positionLabel;
    QLabel* m_sizeLabel;
    QLabel* m_colorLabel;
    QLabel* m_performanceLabel;
    
    void createActions();
    void createMenus();
    void createToolBars();
    void createDockWidgets();
    void createStatusBar();
    void setupConnections();
    void setupShortcuts();
    
    void updateActions();
    void updateStatusBar();
    void updateWindowTitle();
    
    void loadSettings();
    void saveSettings();
    
    void showPreferencesDialog();
    void showAboutDialog();
    void showHelpDialog();
};

} // namespace ui
