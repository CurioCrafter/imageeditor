#pragma once

#include "layer.h"
#include <vector>

namespace core {

class RasterLayer : public Layer {
public:
    explicit RasterLayer(const String& name = "Raster Layer");
    ~RasterLayer() override;

    // Pixel data access
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void getPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const;
    
    // Layer operations
    void clear();
    void fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void resize(int width, int height);
    
    // Rendering
    void render(void* target, const void* bounds) override;
    
    // File I/O
    bool loadFromFile(const String& filename);
    bool saveToFile(const String& filename) const;

private:
    std::vector<unsigned char> m_pixelData;
    int m_dataWidth;
    int m_dataHeight;
};

} // namespace core
