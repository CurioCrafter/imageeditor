#include "main_window_vulkan.h"
#include <QVulkanInstance>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

// Only include GPU headers when GPU module is available
#ifdef GPU_AVAILABLE
#include "../gpu/vulkan_window.h"
#endif

#include "../core/engine.h"

MainWindowVulkan::MainWindowVulkan(core::Engine* engine, QWidget* parent)
    : QMainWindow(parent)
    , m_engine(engine)
    , m_vulkanWindow(nullptr)
    , m_centralWidget(nullptr)
    , m_layout(nullptr)
    , m_statusLabel(nullptr)
    , m_retryButton(nullptr)
    , m_vulkanAvailable(false)
{
    setWindowTitle("Advanced Image Editor - Vulkan");
    resize(1024, 768);
    
    setupUI();
    
#ifdef GPU_AVAILABLE
    tryVulkan();
#else
    fallbackToCPU();
#endif
}

MainWindowVulkan::~MainWindowVulkan()
{
}

void MainWindowVulkan::setupUI()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_layout = new QVBoxLayout(m_centralWidget);
    m_layout->setContentsMargins(0, 0, 0, 0);
    
    m_statusLabel = new QLabel("Initializing...", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("QLabel { font-size: 16px; padding: 20px; }");
    m_layout->addWidget(m_statusLabel);
    
    m_retryButton = new QPushButton("Retry Vulkan", this);
    m_retryButton->setVisible(false);
    connect(m_retryButton, &QPushButton::clicked, this, &MainWindowVulkan::tryVulkan);
    m_layout->addWidget(m_retryButton);
}

void MainWindowVulkan::tryVulkan()
{
#ifdef GPU_AVAILABLE
    qDebug() << "Attempting to initialize Vulkan...";
    
    // Create Vulkan instance
    QVulkanInstance* instance = new QVulkanInstance();
    
    // Enable validation layers in debug builds
    QByteArrayList layers;
    layers << "VK_LAYER_KHRONOS_validation";
    instance->setLayers(layers);
    
    if (instance->create()) {
        qDebug() << "Vulkan instance created successfully";
        
        // Create Vulkan window
        m_vulkanWindow = new VulkanWindow(m_engine, nullptr);
        m_vulkanWindow->setVulkanInstance(instance);
        
        // Embed the Vulkan window in a widget container
        QWidget* vulkanContainer = QWidget::createWindowContainer(m_vulkanWindow, m_centralWidget);
        
        // Replace the status label with the Vulkan container
        m_layout->removeWidget(m_statusLabel);
        m_statusLabel->setVisible(false);
        m_layout->addWidget(vulkanContainer);
        
        // Show the Vulkan window
        m_vulkanWindow->show();
        
        m_vulkanAvailable = true;
        onVulkanAvailable();
        
        qDebug() << "Vulkan rendering initialized successfully";
        
    } else {
        qWarning() << "Failed to create Vulkan instance:" << instance->errorCode();
        fallbackToCPU();
    }
#else
    qDebug() << "GPU module not available, using CPU fallback";
    fallbackToCPU();
#endif
}

void MainWindowVulkan::fallbackToCPU()
{
    m_vulkanAvailable = false;
    
    // Remove any existing Vulkan window
    if (m_vulkanWindow) {
        m_layout->removeWidget(QWidget::createWindowContainer(m_vulkanWindow));
        m_vulkanWindow->setVisible(false);
        m_vulkanWindow = nullptr;
    }
    
    // Show fallback UI
    m_statusLabel->setText("GPU acceleration not available - using CPU fallback");
    m_statusLabel->setVisible(true);
    
#ifdef GPU_AVAILABLE
    m_retryButton->setVisible(true);
#endif
    
    onVulkanUnavailable();
}

void MainWindowVulkan::onVulkanAvailable()
{
    qDebug() << "Vulkan is now available";
    // TODO: Enable GPU-accelerated features in the UI
}

void MainWindowVulkan::onVulkanUnavailable()
{
    qDebug() << "GPU acceleration not available, using CPU fallback";
    // TODO: Disable GPU-accelerated features in the UI
}
