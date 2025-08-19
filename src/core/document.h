#pragma once

#include "layer.h"
#include <vector>
#include <memory>

namespace core {

// Core types - independent of Qt  
struct DateTime {
    DateTime() {}
    String toString() const { return "Unknown"; }
};

} // namespace core

namespace core {

class Document {
public:
    explicit Document(const String& name = "Untitled");
    ~Document();

    // Document properties
    String getName() const { return m_name; }
    void setName(const String& name) { m_name = name; }
    
    Size getSize() const { return m_size; }
    void setSize(const Size& size);
    
    DateTime getCreatedDate() const { return m_createdDate; }
    DateTime getModifiedDate() const { return m_modifiedDate; }

    // Layer management
    void addLayer(std::shared_ptr<Layer> layer);
    void removeLayer(std::shared_ptr<Layer> layer);
    void moveLayer(std::shared_ptr<Layer> layer, int newIndex);
    void setActiveLayer(std::shared_ptr<Layer> layer);
    
    std::vector<std::shared_ptr<Layer>> getLayers() const { return m_layers; }
    std::shared_ptr<Layer> getActiveLayer() const { return m_activeLayer; }
    std::shared_ptr<Layer> getLayerAt(int index) const;

    // Rendering
    void render(unsigned char* outputBuffer, int outputWidth, int outputHeight);
    
    // File I/O
    bool loadFromFile(const String& filename);
    bool saveToFile(const String& filename) const;

private:
    String m_name;
    Size m_size;
    DateTime m_createdDate;
    DateTime m_modifiedDate;
    
    std::vector<std::shared_ptr<Layer>> m_layers;
    std::shared_ptr<Layer> m_activeLayer;
};

} // namespace core
