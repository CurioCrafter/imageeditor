#include "document.h"
#include "raster_layer.h"
#include <algorithm>

namespace core {

Document::Document(const String& name)
    : m_name(name)
    , m_size(800, 600)
    , m_activeLayer(nullptr)
{
    // Create a default background layer
    auto backgroundLayer = std::make_shared<RasterLayer>("Background");
    backgroundLayer->resize(m_size.width, m_size.height);
    backgroundLayer->fill(255, 255, 255, 255); // White background
    addLayer(backgroundLayer);
}

Document::~Document()
{
}

void Document::setSize(const Size& size)
{
    if (size != m_size) {
        m_size = size;
        
        // Resize all layers
        for (auto& layer : m_layers) {
            if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
                rasterLayer->resize(size.width, size.height);
            }
        }
    }
}

void Document::addLayer(std::shared_ptr<Layer> layer)
{
    if (layer) {
        m_layers.push_back(layer);
        if (!m_activeLayer) {
            m_activeLayer = layer;
        }
    }
}

void Document::removeLayer(std::shared_ptr<Layer> layer)
{
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        if (m_activeLayer == layer) {
            m_activeLayer = nullptr;
        }
        m_layers.erase(it);
        
        // Set a new active layer if needed
        if (m_activeLayer == nullptr && !m_layers.empty()) {
            m_activeLayer = m_layers.back();
        }
    }
}

void Document::moveLayer(std::shared_ptr<Layer> layer, int newIndex)
{
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        auto layerPtr = *it;
        m_layers.erase(it);
        
        if (newIndex >= m_layers.size()) {
            m_layers.push_back(layerPtr);
        } else {
            m_layers.insert(m_layers.begin() + newIndex, layerPtr);
        }
    }
}

void Document::setActiveLayer(std::shared_ptr<Layer> layer)
{
    if (std::find(m_layers.begin(), m_layers.end(), layer) != m_layers.end()) {
        m_activeLayer = layer;
    }
}

std::shared_ptr<Layer> Document::getLayerAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        return m_layers[index];
    }
    return nullptr;
}

void Document::render(unsigned char* outputBuffer, int outputWidth, int outputHeight)
{
    if (!outputBuffer) return;
    
    // Clear output buffer
    std::fill(outputBuffer, outputBuffer + outputWidth * outputHeight * 4, 0);
    
    // Render layers from bottom to top
    for (const auto& layer : m_layers) {
        if (layer->isVisible() && layer->getOpacity() > 0.0f) {
            // TODO: Implement proper layer rendering with alpha blending
            // For now, just a placeholder
        }
    }
}

bool Document::loadFromFile(const String& filename)
{
    // TODO: Implement file loading
    return false;
}

bool Document::saveToFile(const String& filename) const
{
    // TODO: Implement file saving
    return false;
}

} // namespace core
