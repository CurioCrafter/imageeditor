#include "main_window.h"
#include "canvas_view.h"
#include "layer_panel.h"
#include "tool_panel.h"
#include "color_panel.h"
#include "brush_panel.h"
#include "../core/engine.h"
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMimeData>
#include <QUrl>
#include <QPixmap>
#include <QImageReader>
#include <QFileInfo>
#include <QFile>
#include <QSplitter>

namespace ui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_engine(nullptr)
    , m_currentDocument(nullptr)
    , m_canvasView(nullptr)
    , m_layerPanel(nullptr)
    , m_toolPanel(nullptr)
    , m_colorPanel(nullptr)
    , m_brushPanel(nullptr)

{
    setWindowTitle("Advanced Image Editor");
    resize(1200, 800);
    
    createActions();
    createMenus();
    createToolBars();
    createDockWidgets();
    createCentralWidget();
    setupConnections();
    
    // Enable drag and drop
    setAcceptDrops(true);
    
    // Set up status bar
    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    // Cleanup will be handled by Qt's parent-child system
}

void MainWindow::setEngine(core::Engine* engine)
{
    m_engine = engine;
    if (m_canvasView && m_engine && m_engine->getCurrentDocument()) {
        m_canvasView->setDocument(m_engine->getCurrentDocument().get());
    }
    if (m_layerPanel && m_engine && m_engine->getCurrentDocument()) {
        m_layerPanel->setDocument(m_engine->getCurrentDocument().get());
    }

    // Keep canvas synced with document changes
    if (m_engine && m_engine->getCurrentDocument() && m_canvasView) {
        auto doc = m_engine->getCurrentDocument();
        connect(doc.get(), &core::Document::documentChanged, this, [this, doc]() {
            if (m_canvasView) m_canvasView->setDocument(doc.get());
        });
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // TODO: Check for unsaved changes
    event->accept();
}

void MainWindow::createActions()
{
    // File actions
    m_newAction = new QAction("&New", this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setStatusTip("Create a new document");
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newDocument);
    
    m_openAction = new QAction("&Open...", this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip("Open an existing document");
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openDocument);
    
    m_saveAction = new QAction("&Save", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip("Save the current document");
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveDocument);
    
    m_saveAsAction = new QAction("Save &As...", this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAction->setStatusTip("Save the current document with a new name");
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAsDocument);
    
    m_exportAction = new QAction("&Export...", this);
    m_exportAction->setStatusTip("Export the current document");
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportDocument);
    
    m_quitAction = new QAction("&Quit", this);
    m_quitAction->setShortcut(QKeySequence::Quit);
    m_quitAction->setStatusTip("Quit the application");
    connect(m_quitAction, &QAction::triggered, this, &MainWindow::quit);
    
    // Edit actions
    m_undoAction = new QAction("&Undo", this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_undoAction->setStatusTip("Undo the last action");
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::undo);
    
    m_redoAction = new QAction("&Redo", this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_redoAction->setStatusTip("Redo the last undone action");
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::redo);
    
    m_cutAction = new QAction("Cu&t", this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    m_cutAction->setStatusTip("Cut the selected content");
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::cut);
    
    m_copyAction = new QAction("&Copy", this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_copyAction->setStatusTip("Copy the selected content");
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::copy);
    
    m_pasteAction = new QAction("&Paste", this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    m_pasteAction->setStatusTip("Paste the copied content");
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::paste);
    
    m_preferencesAction = new QAction("&Preferences...", this);
    m_preferencesAction->setStatusTip("Configure application preferences");
    connect(m_preferencesAction, &QAction::triggered, this, &MainWindow::preferences);
    
    // View actions
    m_zoomInAction = new QAction("Zoom &In", this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    m_zoomInAction->setStatusTip("Zoom in on the canvas");
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    
    m_zoomOutAction = new QAction("Zoom &Out", this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    m_zoomOutAction->setStatusTip("Zoom out on the canvas");
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    
    m_fitToWindowAction = new QAction("&Fit to Window", this);
    m_fitToWindowAction->setShortcut(QKeySequence("Ctrl+0"));
    m_fitToWindowAction->setStatusTip("Fit the canvas to the window");
    connect(m_fitToWindowAction, &QAction::triggered, this, &MainWindow::fitToWindow);
    
    m_actualSizeAction = new QAction("&Actual Size", this);
    m_actualSizeAction->setShortcut(QKeySequence("Ctrl+1"));
    m_actualSizeAction->setStatusTip("Show the canvas at actual size");
    connect(m_actualSizeAction, &QAction::triggered, this, &MainWindow::actualSize);
    
    // Help actions
    m_aboutAction = new QAction("&About", this);
    m_aboutAction->setStatusTip("Show information about the application");
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);
    
    m_aboutQtAction = new QAction("About &Qt", this);
    m_aboutQtAction->setStatusTip("Show information about Qt");
    connect(m_aboutQtAction, &QAction::triggered, this, &MainWindow::aboutQt);
}

void MainWindow::createMenus()
{
    // File menu
    m_fileMenu = menuBar()->addMenu("&File");
    m_fileMenu->addAction(m_newAction);
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exportAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);
    
    // Edit menu
    m_editMenu = menuBar()->addMenu("&Edit");
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_cutAction);
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addAction(m_pasteAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_preferencesAction);
    
    // View menu
    m_viewMenu = menuBar()->addMenu("&View");
    m_viewMenu->addAction(m_zoomInAction);
    m_viewMenu->addAction(m_zoomOutAction);
    m_viewMenu->addAction(m_fitToWindowAction);
    m_viewMenu->addAction(m_actualSizeAction);
    
    // Help menu
    m_helpMenu = menuBar()->addMenu("&Help");
    m_helpMenu->addAction(m_aboutAction);
    m_helpMenu->addAction(m_aboutQtAction);
}

void MainWindow::createToolBars()
{
    // File toolbar
    m_fileToolBar = addToolBar("File");
    m_fileToolBar->addAction(m_newAction);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    
    // Edit toolbar
    m_editToolBar = addToolBar("Edit");
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_cutAction);
    m_editToolBar->addAction(m_copyAction);
    m_editToolBar->addAction(m_pasteAction);
    
    // View toolbar
    m_viewToolBar = addToolBar("View");
    m_viewToolBar->addAction(m_zoomInAction);
    m_viewToolBar->addAction(m_zoomOutAction);
    m_viewToolBar->addAction(m_fitToWindowAction);
    m_viewToolBar->addAction(m_actualSizeAction);
}

void MainWindow::createDockWidgets()
{
    // Create dock widgets for each panel
    QDockWidget* layerDock = new QDockWidget(tr("Layers"), this);
    layerDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_layerPanel = new LayerPanel(this);
    layerDock->setWidget(m_layerPanel);
    addDockWidget(Qt::RightDockWidgetArea, layerDock);

    QDockWidget* toolDock = new QDockWidget(tr("Tools"), this);
    toolDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_toolPanel = new ToolPanel(this);
    toolDock->setWidget(m_toolPanel);
    addDockWidget(Qt::LeftDockWidgetArea, toolDock);

    QDockWidget* colorDock = new QDockWidget(tr("Color"), this);
    colorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_colorPanel = new ColorPanel(this);
    colorDock->setWidget(m_colorPanel);
    addDockWidget(Qt::RightDockWidgetArea, colorDock);

    QDockWidget* brushDock = new QDockWidget(tr("Brush"), this);
    brushDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_brushPanel = new BrushPanel(this);
    brushDock->setWidget(m_brushPanel);
    addDockWidget(Qt::RightDockWidgetArea, brushDock);
}

void MainWindow::createCentralWidget()
{
    m_canvasView = new CanvasView(this);
    setCentralWidget(m_canvasView);
}

void MainWindow::setupConnections()
{
    // Connect tool panel to canvas view
    if (m_toolPanel && m_canvasView) {
        connect(m_toolPanel, &ToolPanel::toolChanged, this, [this](int toolId) {
            m_canvasView->setCurrentTool(static_cast<ui::Tool>(toolId));
        });
    }
    
    // Connect layer panel to canvas view
    if (m_layerPanel && m_canvasView) {
        connect(m_layerPanel, &LayerPanel::layerSelectionChanged, this, [this](int layerIndex) {
            if (!m_engine || !m_engine->getCurrentDocument()) return;
            m_engine->getCurrentDocument()->setActiveLayer(layerIndex);
            if (m_canvasView) m_canvasView->setDocument(m_engine->getCurrentDocument().get());
        });
    }
    
    // Connect color panel to canvas view
    if (m_colorPanel && m_canvasView) {
        connect(m_colorPanel, &ColorPanel::colorChanged, this, [this](const QColor& color) {
            m_canvasView->setBrushColor(color);
        });
        
        // Connect eyedropper to color panel
        connect(m_canvasView, &CanvasView::colorPicked, m_colorPanel, &ColorPanel::setColor);
    }
    
    // Connect brush panel to canvas view (when implemented)
    // TODO: Connect brush settings to canvas view
}

void MainWindow::updateWindowTitle()
{
    QString title = "Advanced Image Editor";
    if (m_currentDocument) {
        title += " - [Document Name]"; // TODO: Add actual document name
    }
    setWindowTitle(title);
}

// File menu actions
void MainWindow::newDocument()
{
    // TODO: Implement new document creation
    statusBar()->showMessage("New document created", 2000);
}

void MainWindow::openDocument()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Document", "", "Image Files (*.png *.jpg *.jpeg *.tiff *.psd *.kdoc)");
    if (!fileName.isEmpty()) {
        // TODO: Implement document opening
        statusBar()->showMessage("Opened: " + fileName, 2000);
    }
}

void MainWindow::saveDocument()
{
    // TODO: Implement document saving
    statusBar()->showMessage("Document saved", 2000);
}

void MainWindow::saveAsDocument()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Document As", "", "Image Files (*.png *.jpg *.jpeg *.tiff *.psd *.kdoc)");
    if (!fileName.isEmpty()) {
        // TODO: Implement document saving
        statusBar()->showMessage("Saved as: " + fileName, 2000);
    }
}

void MainWindow::exportDocument()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Export Document", "", "Image Files (*.png *.jpg *.jpeg *.tiff)");
    if (!fileName.isEmpty()) {
        // TODO: Implement document export
        statusBar()->showMessage("Exported to: " + fileName, 2000);
    }
}

void MainWindow::quit()
{
    QApplication::quit();
}

// Edit menu actions
void MainWindow::undo()
{
    // TODO: Implement undo
    statusBar()->showMessage("Undo", 1000);
}

void MainWindow::redo()
{
    // TODO: Implement redo
    statusBar()->showMessage("Redo", 1000);
}

void MainWindow::cut()
{
    // TODO: Implement cut
    statusBar()->showMessage("Cut", 1000);
}

void MainWindow::copy()
{
    // TODO: Implement copy
    statusBar()->showMessage("Copy", 1000);
}

void MainWindow::paste()
{
    // TODO: Implement paste
    statusBar()->showMessage("Paste", 1000);
}

void MainWindow::preferences()
{
    // TODO: Implement preferences dialog
    QMessageBox::information(this, "Preferences", "Preferences dialog not implemented yet.");
}

// View menu actions
void MainWindow::zoomIn()
{
    if (m_canvasView) {
        // TODO: Implement zoom in
        statusBar()->showMessage("Zoomed in", 1000);
    }
}

void MainWindow::zoomOut()
{
    if (m_canvasView) {
        // TODO: Implement zoom out
        statusBar()->showMessage("Zoomed out", 1000);
    }
}

void MainWindow::fitToWindow()
{
    if (m_canvasView) {
        // TODO: Implement fit to window
        statusBar()->showMessage("Fitted to window", 1000);
    }
}

void MainWindow::actualSize()
{
    if (m_canvasView) {
        // TODO: Implement actual size
        statusBar()->showMessage("Actual size", 1000);
    }
}

// Help menu actions
void MainWindow::about()
{
    QMessageBox::about(this, "About Advanced Image Editor",
        "Advanced Image Editor v1.0.0\n\n"
        "A professional-grade image editing application with AI-powered tools.\n\n"
        "Built with Qt6 and modern C++.");
}

void MainWindow::aboutQt()
{
    QApplication::aboutQt();
}

// View menu actions
void MainWindow::showLayerPanel(bool show)
{
    // TODO: Implement layer panel visibility toggle
    Q_UNUSED(show)
}

void MainWindow::showToolPanel(bool show)
{
    // TODO: Implement tool panel visibility toggle
    Q_UNUSED(show)
}

void MainWindow::showColorPanel(bool show)
{
    // TODO: Implement color panel visibility toggle
    Q_UNUSED(show)
}

void MainWindow::showBrushPanel(bool show)
{
    // TODO: Implement brush panel visibility toggle
    Q_UNUSED(show)
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
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

void MainWindow::dropEvent(QDropEvent* event)
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

void MainWindow::loadImageFile(const QString& filePath)
{
    QImageReader reader(filePath);
    QImage image = reader.read();
    
    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Could not load image: " + reader.errorString());
        return;
    }
    
    // Create a new document with the image size
    if (m_engine) {
        auto document = m_engine->createNewDocument(QFileInfo(filePath).baseName().toStdString());
        if (document) {
            // Set the loaded image to the canvas
            m_currentDocument = document.get();
            if (m_canvasView) {
                m_canvasView->setDocument(document.get());
                m_canvasView->setCanvasImage(image);
            }
            if (m_layerPanel) {
                m_layerPanel->setDocument(document.get());
            }
            statusBar()->showMessage("Loaded: " + QFileInfo(filePath).fileName());
        }
    }
}

} // namespace ui
