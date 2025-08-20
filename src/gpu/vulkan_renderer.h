#pragma once

#include <QVulkanWindowRenderer>
#include <QVulkanDeviceFunctions>
#include <QVulkanFunctions>
#include <memory>
#include <vector>
#include <QSize>
#include <QImage>

namespace core { class Engine; }

class VulkanRenderer : public QVulkanWindowRenderer
{
public:
    explicit VulkanRenderer(QVulkanWindow* window, core::Engine* engine);
    ~VulkanRenderer();

    // QVulkanWindowRenderer interface
    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;
    void startNextFrame() override;

    // Device capabilities
    bool supportsComputeShaders() const;
    bool supportsRayTracing() const;
    QString getDeviceName() const;
    QString getDriverVersion() const;

    // Rendering operations
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

private:
    QVulkanWindow* m_window;
    core::Engine* m_engine;
    QVulkanDeviceFunctions* m_deviceFunctions = nullptr;
    
    double m_lastFrameTime = 0.0;
    double m_averageFrameTime = 0.0;
    uint64_t m_frameCount = 0;

    // Private helper methods
    void createPipelines();
    void createShaders();
    void updatePerformanceMetrics();
};
