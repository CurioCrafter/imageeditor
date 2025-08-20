#include "vulkan_renderer.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QVulkanInstance>
#include <QVulkanDeviceFunctions>
#include <QVulkanFunctions>

// Correct Vulkan/MSVC include order per Khronos spec
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.h>

namespace gpu {

struct VulkanRenderer::Impl
{
    // Vulkan instance and device
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue computeQueue = VK_NULL_HANDLE;
    
    // Surface and swapchain
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    
    // Render pass and framebuffers
    VkRenderPass renderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> framebuffers;
    
    // Command buffers and pools
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;
    
    // Synchronization
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    
    // Shaders and pipelines
    VkShaderModule vertexShader = VK_NULL_HANDLE;
    VkShaderModule fragmentShader = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    VkPipeline computePipeline = VK_NULL_HANDLE;
    
    // Buffers and textures
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
    
    // Descriptor sets and layouts
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets;
    
    // Current frame tracking
    size_t currentFrame = 0;
    uint32_t currentImageIndex = 0;
    
    // Performance tracking
    QElapsedTimer frameTimer;
    double frameTime = 0.0;
    
    // Device properties
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    VkPhysicalDeviceMemoryProperties memoryProperties;
};

VulkanRenderer::VulkanRenderer(QObject* parent)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{
}

VulkanRenderer::~VulkanRenderer()
{
    cleanup();
}

bool VulkanRenderer::initialize()
{
    if (m_initialized) {
        return true;
    }
    
    qDebug() << "Initializing Vulkan renderer...";
    
    // Create Vulkan instance
    if (!createInstance()) {
        qCritical() << "Failed to create Vulkan instance";
        return false;
    }
    
    // Select physical device
    if (!selectPhysicalDevice()) {
        qCritical() << "Failed to select physical device";
        return false;
    }
    
    // Create logical device
    if (!createLogicalDevice()) {
        qCritical() << "Failed to create logical device";
        return false;
    }
    
    // Create command pool
    if (!createCommandPool()) {
        qCritical() << "Failed to create command pool";
        return false;
    }
    
    // Create synchronization objects
    if (!createSyncObjects()) {
        qCritical() << "Failed to create synchronization objects";
        return false;
    }
    
    // Create shaders
    if (!createShaders()) {
        qCritical() << "Failed to create shaders";
        return false;
    }
    
    // Create pipelines
    if (!createPipelines()) {
        qCritical() << "Failed to create pipelines";
        return false;
    }
    
    m_initialized = true;
    qDebug() << "Vulkan renderer initialized successfully";
    qDebug() << "Device:" << getDeviceName();
    qDebug() << "Driver:" << getDriverVersion();
    
    return true;
}

void VulkanRenderer::cleanup()
{
    if (!m_initialized) {
        return;
    }
    
    vkDeviceWaitIdle(m_impl->device);
    
    // Cleanup resources
    cleanupResources();
    
    // Destroy device
    if (m_impl->device) {
        vkDestroyDevice(m_impl->device, nullptr);
        m_impl->device = VK_NULL_HANDLE;
    }
    
    // Destroy instance
    if (m_impl->instance) {
        vkDestroyInstance(m_impl->instance, nullptr);
        m_impl->instance = VK_NULL_HANDLE;
    }
    
    m_initialized = false;
    qDebug() << "Vulkan renderer cleaned up";
}

bool VulkanRenderer::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Advanced Image Editor";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Advanced Image Editor Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    // Enable validation layers in debug builds
    std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    
    // Enable extensions
    std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    if (vkCreateInstance(&createInfo, nullptr, &m_impl->instance) != VK_SUCCESS) {
        qCritical() << "Failed to create Vulkan instance";
        return false;
    }
    
    return true;
}

bool VulkanRenderer::selectPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_impl->instance, &deviceCount, nullptr);
    
    if (deviceCount == 0) {
        qCritical() << "Failed to find GPUs with Vulkan support";
        return false;
    }
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_impl->instance, &deviceCount, devices.data());
    
    // Select the first suitable device (prefer discrete GPUs)
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        
        // Check if device supports required features
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(device, &features);
        
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
            properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            
            // Check queue families
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
            
            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
            
            bool hasGraphicsQueue = false;
            bool hasComputeQueue = false;
            
            for (uint32_t i = 0; i < queueFamilyCount; i++) {
                if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    hasGraphicsQueue = true;
                }
                if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                    hasComputeQueue = true;
                }
            }
            
            if (hasGraphicsQueue && hasComputeQueue) {
                m_impl->physicalDevice = device;
                m_impl->deviceProperties = properties;
                m_impl->deviceFeatures = features;
                
                vkGetPhysicalDeviceMemoryProperties(device, &m_impl->memoryProperties);
                
                qDebug() << "Selected device:" << properties.deviceName;
                return true;
            }
        }
    }
    
    qCritical() << "Failed to find suitable GPU";
    return false;
}

bool VulkanRenderer::createLogicalDevice()
{
    // Find queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_impl->physicalDevice, &queueFamilyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_impl->physicalDevice, &queueFamilyCount, queueFamilies.data());
    
    uint32_t graphicsQueueFamily = UINT32_MAX;
    uint32_t computeQueueFamily = UINT32_MAX;
    
    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueFamily = i;
        }
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            computeQueueFamily = i;
        }
    }
    
    if (graphicsQueueFamily == UINT32_MAX) {
        qCritical() << "Failed to find graphics queue family";
        return false;
    }
    
    // Create device queues
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {graphicsQueueFamily, computeQueueFamily};
    
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    // Device features
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.fillModeNonSolid = VK_TRUE;
    
    // Create logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    
    // Enable extensions
    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    if (vkCreateDevice(m_impl->physicalDevice, &createInfo, nullptr, &m_impl->device) != VK_SUCCESS) {
        qCritical() << "Failed to create logical device";
        return false;
    }
    
    // Get queue handles
    vkGetDeviceQueue(m_impl->device, graphicsQueueFamily, 0, &m_impl->graphicsQueue);
    if (computeQueueFamily != UINT32_MAX) {
        vkGetDeviceQueue(m_impl->device, computeQueueFamily, 0, &m_impl->computeQueue);
    }
    
    return true;
}

bool VulkanRenderer::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = 0; // Graphics queue family
    
    if (vkCreateCommandPool(m_impl->device, &poolInfo, nullptr, &m_impl->commandPool) != VK_SUCCESS) {
        qCritical() << "Failed to create command pool";
        return false;
    }
    
    return true;
}

bool VulkanRenderer::createSyncObjects()
{
    m_impl->imageAvailableSemaphores.resize(2);
    m_impl->renderFinishedSemaphores.resize(2);
    m_impl->inFlightFences.resize(2);
    
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    for (size_t i = 0; i < 2; i++) {
        if (vkCreateSemaphore(m_impl->device, &semaphoreInfo, nullptr, &m_impl->imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(m_impl->device, &semaphoreInfo, nullptr, &m_impl->renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_impl->device, &fenceInfo, nullptr, &m_impl->inFlightFences[i]) != VK_SUCCESS) {
            qCritical() << "Failed to create synchronization objects";
            return false;
        }
    }
    
    return true;
}

bool VulkanRenderer::createShaders()
{
    // TODO: Implement shader compilation
    return true;
}

bool VulkanRenderer::createPipelines()
{
    // TODO: Implement pipeline creation
    return true;
}

bool VulkanRenderer::beginFrame()
{
    if (!m_initialized) {
        return false;
    }
    
    m_impl->frameTimer.start();
    
    // Wait for previous frame
    vkWaitForFences(m_impl->device, 1, &m_impl->inFlightFences[m_impl->currentFrame], VK_TRUE, UINT64_MAX);
    
    return true;
}

void VulkanRenderer::endFrame()
{
    if (!m_initialized) {
        return;
    }
    
    // Update performance metrics
    m_impl->frameTime = m_impl->frameTimer.nsecsElapsed() / 1000000.0; // Convert to milliseconds
    
    // Reset fence
    vkResetFences(m_impl->device, 1, &m_impl->inFlightFences[m_impl->currentFrame]);
    
    // Move to next frame
    m_impl->currentFrame = (m_impl->currentFrame + 1) % 2;
}

bool VulkanRenderer::renderImage(const QImage& image, const QRect& destRect)
{
    if (!m_initialized) {
        return false;
    }
    
    // TODO: Implement actual image rendering
    // This would involve:
    // 1. Creating a texture from the QImage
    // 2. Recording commands to render the texture
    // 3. Submitting the command buffer
    
    return true;
}

bool VulkanRenderer::renderLayer(const QImage& layer, const QRect& destRect, float opacity)
{
    if (!m_initialized) {
        return false;
    }
    
    // TODO: Implement layer rendering with opacity
    return true;
}

bool VulkanRenderer::applyFilter(const QImage& input, QImage& output, const QString& filterName)
{
    if (!m_initialized) {
        return false;
    }
    
    // TODO: Implement GPU-accelerated filters
    // This would use compute shaders for image processing
    return true;
}

bool VulkanRenderer::compositeLayers(const std::vector<QImage>& layers, QImage& result)
{
    if (!m_initialized) {
        return false;
    }
    
    // TODO: Implement GPU-accelerated layer compositing
    return true;
}

bool VulkanRenderer::supportsComputeShaders() const
{
    return m_initialized && m_impl->computeQueue != VK_NULL_HANDLE;
}

bool VulkanRenderer::supportsRayTracing() const
{
    // TODO: Check for ray tracing extensions
    return false;
}

QString VulkanRenderer::getDeviceName() const
{
    if (m_initialized) {
        return QString::fromUtf8(m_impl->deviceProperties.deviceName);
    }
    return QString();
}

QString VulkanRenderer::getDriverVersion() const
{
    if (m_initialized) {
        uint32_t major = VK_VERSION_MAJOR(m_impl->deviceProperties.driverVersion);
        uint32_t minor = VK_VERSION_MINOR(m_impl->deviceProperties.driverVersion);
        uint32_t patch = VK_VERSION_PATCH(m_impl->deviceProperties.driverVersion);
        return QString("%1.%2.%3").arg(major).arg(minor).arg(patch);
    }
    return QString();
}

bool VulkanRenderer::dispatchComputeShader(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{
    if (!supportsComputeShaders()) {
        return false;
    }
    
    // TODO: Implement compute shader dispatch
    return true;
}

bool VulkanRenderer::updateComputeBuffer(uint32_t binding, const void* data, size_t size)
{
    if (!supportsComputeShaders()) {
        return false;
    }
    
    // TODO: Implement buffer updates
    return true;
}

bool VulkanRenderer::waitForComputeCompletion()
{
    if (!supportsComputeShaders()) {
        return false;
    }
    
    // TODO: Implement compute completion wait
    return true;
}

void VulkanRenderer::cleanupResources()
{
    // Cleanup swapchain resources
    if (m_impl->swapchain) {
        for (auto imageView : m_impl->swapchainImageViews) {
            vkDestroyImageView(m_impl->device, imageView, nullptr);
        }
        vkDestroySwapchainKHR(m_impl->device, m_impl->swapchain, nullptr);
    }
    
    // Cleanup framebuffers
    for (auto framebuffer : m_impl->framebuffers) {
        vkDestroyFramebuffer(m_impl->device, framebuffer, nullptr);
    }
    
    // Cleanup render pass
    if (m_impl->renderPass) {
        vkDestroyRenderPass(m_impl->device, m_impl->renderPass, nullptr);
    }
    
    // Cleanup command pool
    if (m_impl->commandPool) {
        vkDestroyCommandPool(m_impl->device, m_impl->commandPool, nullptr);
    }
    
    // Cleanup synchronization objects
    for (size_t i = 0; i < 2; i++) {
        if (m_impl->imageAvailableSemaphores[i]) {
            vkDestroySemaphore(m_impl->device, m_impl->imageAvailableSemaphores[i], nullptr);
        }
        if (m_impl->renderFinishedSemaphores[i]) {
            vkDestroySemaphore(m_impl->device, m_impl->renderFinishedSemaphores[i], nullptr);
        }
        if (m_impl->inFlightFences[i]) {
            vkDestroyFence(m_impl->device, m_impl->inFlightFences[i], nullptr);
        }
    }
    
    // Cleanup shaders
    if (m_impl->vertexShader) {
        vkDestroyShaderModule(m_impl->device, m_impl->vertexShader, nullptr);
    }
    if (m_impl->fragmentShader) {
        vkDestroyShaderModule(m_impl->device, m_impl->fragmentShader, nullptr);
    }
    
    // Cleanup pipelines
    if (m_impl->graphicsPipeline) {
        vkDestroyPipeline(m_impl->device, m_impl->graphicsPipeline, nullptr);
    }
    if (m_impl->computePipeline) {
        vkDestroyPipeline(m_impl->device, m_impl->computePipeline, nullptr);
    }
    
    // Cleanup buffers
    if (m_impl->vertexBuffer) {
        vkDestroyBuffer(m_impl->device, m_impl->vertexBuffer, nullptr);
    }
    if (m_impl->vertexBufferMemory) {
        vkFreeMemory(m_impl->device, m_impl->vertexBufferMemory, nullptr);
    }
    if (m_impl->indexBuffer) {
        vkDestroyBuffer(m_impl->device, m_impl->indexBuffer, nullptr);
    }
    if (m_impl->indexBufferMemory) {
        vkFreeMemory(m_impl->device, m_impl->indexBufferMemory, nullptr);
    }
    
    // Cleanup descriptor resources
    if (m_impl->descriptorPool) {
        vkDestroyDescriptorPool(m_impl->device, m_impl->descriptorPool, nullptr);
    }
    if (m_impl->descriptorSetLayout) {
        vkDestroyDescriptorSetLayout(m_impl->device, m_impl->descriptorSetLayout, nullptr);
    }
}

} // namespace gpu
