#include "raster_layer.h"
#include <algorithm>
#include <cstring>

namespace core {

RasterLayer::RasterLayer(const String& name)
    : Layer(name)
    , m_dataWidth(0)
    , m_dataHeight(0)
{
    m_size = Size(0, 0);
}

RasterLayer::~RasterLayer()
{
}

void RasterLayer::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (x < 0 || y < 0 || x >= m_dataWidth || y >= m_dataHeight) return;
    
    int index = (y * m_dataWidth + x) * 4;
    m_pixelData[index] = r;
    m_pixelData[index + 1] = g;
    m_pixelData[index + 2] = b;
    m_pixelData[index + 3] = a;
}

void RasterLayer::getPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const
{
    if (x < 0 || y < 0 || x >= m_dataWidth || y >= m_dataHeight) {
        r = g = b = a = 0;
        return;
    }
    
    int index = (y * m_dataWidth + x) * 4;
    r = m_pixelData[index];
    g = m_pixelData[index + 1];
    b = m_pixelData[index + 2];
    a = m_pixelData[index + 3];
}

void RasterLayer::clear()
{
    std::fill(m_pixelData.begin(), m_pixelData.end(), 0);
}

void RasterLayer::fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    for (size_t i = 0; i < m_pixelData.size(); i += 4) {
        m_pixelData[i] = r;
        m_pixelData[i + 1] = g;
        m_pixelData[i + 2] = b;
        m_pixelData[i + 3] = a;
    }
}

void RasterLayer::resize(int width, int height)
{
    if (width <= 0 || height <= 0) return;
    
    m_dataWidth = width;
    m_dataHeight = height;
    m_size = Size(width, height);
    
    // Resize pixel data (RGBA format)
    m_pixelData.resize(width * height * 4);
    m_pixelData.assign(m_pixelData.size(), 0); // Clear with transparent black
}

void RasterLayer::render(void* target, const void* bounds)
{
    if (!target || m_pixelData.empty()) return;
    
    // TODO: Implement proper rendering with alpha blending
    // For now, just a placeholder implementation
}

bool RasterLayer::loadFromFile(const String& filename)
{
    // TODO: Implement file loading
    return false;
}

bool RasterLayer::saveToFile(const String& filename) const
{
    // TODO: Implement file saving
    return false;
}

} // namespace core
