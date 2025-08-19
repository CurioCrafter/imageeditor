#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "core/engine.h"
#include "core/document.h"
#include "core/brush_engine.h"

class SimpleImageEditor : public QMainWindow {
    Q_OBJECT

public:
    SimpleImageEditor(core::Engine* engine, QWidget* parent = nullptr)
        : QMainWindow(parent), m_engine(engine), m_document(nullptr)
    {
        setWindowTitle("Advanced Image Editor - Simple UI");
        setGeometry(100, 100, 1200, 800);
        
        createUI();
        setupConnections();
        
        // Create a default document
        if (m_engine) {
            m_engine->createNewDocument(800, 600);
            m_document = m_engine->currentDocument();
            updateCanvas();
        }
    }

private slots:
    void newDocument() {
        if (m_engine) {
            m_engine->createNewDocument(800, 600);
            m_document = m_engine->currentDocument();
            updateCanvas();
            statusBar()->showMessage("New document created", 2000);
        }
    }
    
    void openDocument() {
        QString fileName = QFileDialog::getOpenFileName(this,
            "Open Image", "", "Images (*.png *.jpg *.jpeg *.bmp *.tiff)");
        if (!fileName.isEmpty()) {
            statusBar()->showMessage("Opening: " + fileName, 2000);
            // TODO: Implement file loading
        }
    }
    
    void saveDocument() {
        QString fileName = QFileDialog::getSaveFileName(this,
            "Save Image", "", "PNG (*.png);;JPEG (*.jpg);;BMP (*.bmp);;TIFF (*.tiff)");
        if (!fileName.isEmpty()) {
            if (m_engine) {
                m_engine->saveDocument(fileName.toStdString());
                statusBar()->showMessage("Saved: " + fileName, 2000);
            }
        }
    }
    
    void about() {
        QMessageBox::about(this, "About Advanced Image Editor",
            "Advanced Image Editor v1.0.0\n\n"
            "A professional-grade image editor with:\n"
            "- Layer-based editing\n"
            "- Advanced brush engine\n"
            "- AI-powered tools\n"
            "- GPU acceleration\n\n"
            "Built with C++ and Qt6");
    }

private:
    void createUI() {
        // Create central widget
        QWidget* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        
        // Create canvas
        m_canvasView = new QGraphicsView(this);
        m_canvasScene = new QGraphicsScene(this);
        m_canvasView->setScene(m_canvasScene);
        m_canvasView->setRenderHint(QPainter::Antialiasing);
        m_canvasView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        m_canvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_canvasView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        
        mainLayout->addWidget(m_canvasView);
        
        // Create toolbar
        QHBoxLayout* toolbarLayout = new QHBoxLayout();
        
        QPushButton* newBtn = new QPushButton("New", this);
        QPushButton* openBtn = new QPushButton("Open", this);
        QPushButton* saveBtn = new QPushButton("Save", this);
        
        toolbarLayout->addWidget(newBtn);
        toolbarLayout->addWidget(openBtn);
        toolbarLayout->addWidget(saveBtn);
        toolbarLayout->addStretch();
        
        mainLayout->addLayout(toolbarLayout);
        
        // Connect buttons
        connect(newBtn, &QPushButton::clicked, this, &SimpleImageEditor::newDocument);
        connect(openBtn, &QPushButton::clicked, this, &SimpleImageEditor::openDocument);
        connect(saveBtn, &QPushButton::clicked, this, &SimpleImageEditor::saveDocument);
        
        // Create menu bar
        createMenuBar();
        
        // Create status bar
        statusBar()->showMessage("Ready");
    }
    
    void createMenuBar() {
        QMenuBar* menuBar = this->menuBar();
        
        // File menu
        QMenu* fileMenu = menuBar->addMenu("&File");
        fileMenu->addAction("&New", this, &SimpleImageEditor::newDocument, QKeySequence::New);
        fileMenu->addAction("&Open...", this, &SimpleImageEditor::openDocument, QKeySequence::Open);
        fileMenu->addAction("&Save", this, &SimpleImageEditor::saveDocument, QKeySequence::Save);
        fileMenu->addSeparator();
        fileMenu->addAction("E&xit", this, &QWidget::close, QKeySequence::Quit);
        
        // Help menu
        QMenu* helpMenu = menuBar->addMenu("&Help");
        helpMenu->addAction("&About", this, &SimpleImageEditor::about);
    }
    
    void setupConnections() {
        // TODO: Add more connections as needed
    }
    
    void updateCanvas() {
        if (!m_document) return;
        
        m_canvasScene->clear();
        
        // Create a simple canvas representation
        int width = m_document->width();
        int height = m_document->height();
        
        // Create a white background
        QPixmap pixmap(width, height);
        pixmap.fill(Qt::white);
        
        // Add to scene
        QGraphicsPixmapItem* item = m_canvasScene->addPixmap(pixmap);
        
        // Set scene rect
        m_canvasScene->setSceneRect(0, 0, width, height);
        
        // Fit view to scene
        m_canvasView->fitInView(item, Qt::KeepAspectRatio);
        
        statusBar()->showMessage(QString("Canvas: %1x%2").arg(width).arg(height));
    }
    
    void resizeEvent(QResizeEvent* event) override {
        QMainWindow::resizeEvent(event);
        if (m_canvasView && m_canvasScene->items().size() > 0) {
            m_canvasView->fitInView(m_canvasScene->items().first(), Qt::KeepAspectRatio);
        }
    }

private:
    core::Engine* m_engine;
    core::Document* m_document;
    QGraphicsView* m_canvasView;
    QGraphicsScene* m_canvasScene;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    // Initialize core engine
    if (!core::Engine::initialize()) {
        QMessageBox::critical(nullptr, "Error", "Failed to initialize core engine!");
        return 1;
    }
    
    core::Engine* engine = core::Engine::instance();
    if (!engine) {
        QMessageBox::critical(nullptr, "Error", "Failed to get engine instance!");
        return 1;
    }
    
    // Create and show main window
    SimpleImageEditor editor(engine);
    editor.show();
    
    // Run application
    int result = app.exec();
    
    // Cleanup
    core::Engine::shutdown();
    
    return result;
}

#include "main_qt.moc"
