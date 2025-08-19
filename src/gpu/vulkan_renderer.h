#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <QObject>
#include <QSize>
#include <QImage>

namespace gpu {

class VulkanRenderer : public QObject
{
    Q_OBJECT

public:
    explicit VulkanRenderer(QObject* parent = nullptr);
    ~VulkanRenderer();

    // Initialization and cleanup
    bool initialize();
    void cleanup();
    bool isInitialized() const { return m_initialized; }

    // Device capabilities
    bool supportsComputeShaders() const;
    bool supportsRayTracing() const;
    QString getDeviceName() const;
    QString getDriverVersion() const;

    // Rendering operations
    bool beginFrame();
    void endFrame();
    bool renderImage(const QImage& image, const QRect& destRect);
    bool renderLayer(const QImage& layer, const QRect& destRect, float opacity = 1.0f);
    bool applyFilter(const QImage& input, QImage& output, const QString& filterName);
    bool compositeLayers(const std::vector<QImage>& layers, QImage& result);

    // GPU compute operations
    bool dispatchComputeShader(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);
    bool updateComputeBuffer(uint32_t binding, const void* data, size_t size);
    bool waitForComputeCompletion();

    // Performance monitoring
    double getLastFrameTime() const { return m_lastFrameTime; }
    double getAverageFrameTime() const { return m_averageFrameTime; }
    uint64_t getFrameCount() const { return m_frameCount; }

signals:
    void deviceLost();
    void renderError(const QString& error);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
    
    bool m_initialized = false;
    double m_lastFrameTime = 0.0;
    double m_averageFrameTime = 0.0;
    uint64_t m_frameCount = 0;

    // Private helper methods
    bool createInstance();
    bool selectPhysicalDevice();
    bool createLogicalDevice();
    bool createSwapchain();
    bool createRenderPass();
    bool createFramebuffers();
    bool createCommandPool();
    bool createSyncObjects();
    bool createShaders();
    bool createPipelines();
    
    void updatePerformanceMetrics();
    void cleanupResources();
};

} // namespace gpu
