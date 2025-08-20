#include "main_window_vulkan.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWindow>
#include <QWidget>
#include <QTimer>
#include <QVulkanInstance>
#include <QWidget>
#include <QVBoxLayout>

#include "../core/engine.h"
#include "../gpu/vulkan_window.h"

MainWindowVulkan::MainWindowVulkan(core::Engine* engine, QWidget* parent)
    : QMainWindow(parent)
    , m_engine(engine)
    , m_centralWidget(nullptr)
    , m_layout(nullptr)
    , m_statusLabel(nullptr)
    , m_vulkanAvailable(false)
{
    qDebug() << "MainWindowVulkan constructor starting...";
    setWindowTitle("Advanced Image Editor - CPU Mode");
    resize(1024, 768);
    
    setupUI();
    if (!attemptVulkan()) {
        fallbackToCPU();
    }
    
    qDebug() << "MainWindowVulkan constructor completed successfully";
}

MainWindowVulkan::~MainWindowVulkan()
{
    qDebug() << "MainWindowVulkan destructor called";
}

void MainWindowVulkan::setupUI()
{
    qDebug() << "Setting up UI...";
    
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_layout = new QVBoxLayout(m_centralWidget);
    m_layout->setContentsMargins(20, 20, 20, 20);
    
    // Welcome label
    QLabel* welcomeLabel = new QLabel("Advanced Image Editor", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #2c3e50; padding: 20px; }");
    m_layout->addWidget(welcomeLabel);
    
    // Status label
    m_statusLabel = new QLabel("Initializing...", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("QLabel { font-size: 16px; color: #7f8c8d; padding: 10px; }");
    m_layout->addWidget(m_statusLabel);
    
    // Add some spacing
    m_layout->addSpacing(20);
    
    // Info label
    QLabel* infoLabel = new QLabel("This is a CPU-based image editor.\nGPU acceleration is not available.", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("QLabel { font-size: 14px; color: #95a5a6; padding: 10px; }");
    m_layout->addWidget(infoLabel);
    
    // Add stretch to push everything to the top
    m_layout->addStretch();
    
    qDebug() << "UI setup completed";
}

bool MainWindowVulkan::attemptVulkan()
{
    qDebug() << "Attempting to initialize Vulkan...";
#if defined(QT_NO_OPENGL) // irrelevant but keep guards possible
#endif
    // Create Vulkan instance
    m_vulkanInstance = new QVulkanInstance();
#ifdef QT_DEBUG
    m_vulkanInstance->setLayers({QStringLiteral("VK_LAYER_KHRONOS_validation")});
#endif
    if (!m_vulkanInstance->create()) {
        qWarning() << "QVulkanInstance creation failed";
        delete m_vulkanInstance;
        m_vulkanInstance = nullptr;
        return false;
    }

    // Create QVulkanWindow and wrap into a QWidget container
    m_vulkanWindow = new VulkanWindow(m_engine);
    m_vulkanWindow->setVulkanInstance(m_vulkanInstance);
    QWidget* container = QWidget::createWindowContainer(m_vulkanWindow, this);
    container->setMinimumSize(640, 480);
    container->setFocusPolicy(Qt::StrongFocus);

    // Replace central layout content to show Vulkan view prominently
    m_layout->insertWidget(0, container, 1);
    m_vulkanContainer = container;

    m_statusLabel->setText("Vulkan mode active - Rendering");
    m_statusLabel->setStyleSheet("QLabel { font-size: 16px; color: #3498db; padding: 10px; }");
    m_vulkanAvailable = true;
    return true;
}

void MainWindowVulkan::fallbackToCPU()
{
    qDebug() << "Using CPU fallback mode";
    m_vulkanAvailable = false;
    
    // Show fallback UI
    m_statusLabel->setText("CPU mode active - Ready for use");
    m_statusLabel->setStyleSheet("QLabel { font-size: 16px; color: #27ae60; padding: 10px; }");
    
    onVulkanUnavailable();
}

void MainWindowVulkan::onVulkanAvailable()
{
    qDebug() << "Vulkan is now available";
    // This should never be called in CPU-only mode
}

void MainWindowVulkan::onVulkanUnavailable()
{
    qDebug() << "Running in CPU mode - GPU acceleration not available";
    // TODO: Initialize CPU-based image editing features
}
