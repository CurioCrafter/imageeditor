#include "brush_engine.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace core {

BrushEngine::BrushEngine()
    : m_strokeActive(false)
{
}

void BrushEngine::beginStroke(float x, float y, float pressure, float tilt)
{
    m_currentStroke.clear();
    m_strokeActive = true;
    
    BrushStroke stroke;
    stroke.x = x;
    stroke.y = y;
    stroke.pressure = pressure;
    stroke.tilt = tilt;
    stroke.timestamp = 0.0f; // TODO: Add proper timing
    
    m_currentStroke.push_back(stroke);
}

void BrushEngine::addPoint(float x, float y, float pressure, float tilt)
{
    if (!m_strokeActive) return;
    
    BrushStroke stroke;
    stroke.x = x;
    stroke.y = y;
    stroke.pressure = pressure;
    stroke.tilt = tilt;
    stroke.timestamp = 0.0f; // TODO: Add proper timing
    
    m_currentStroke.push_back(stroke);
}

void BrushEngine::endStroke()
{
    m_strokeActive = false;
    m_currentStroke.clear();
}

void BrushEngine::paintOnLayer(unsigned char* layerData, int layerWidth, int layerHeight,
                               unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (m_currentStroke.size() < 2) return;
    
    // Draw brush points along the stroke path
    for (size_t i = 0; i < m_currentStroke.size(); ++i) {
        const auto& stroke = m_currentStroke[i];
        drawBrushPoint(layerData, layerWidth, layerHeight, 
                      stroke.x, stroke.y, stroke.pressure, stroke.tilt, r, g, b, a);
    }
    
    // Interpolate between points for smooth strokes
    for (size_t i = 1; i < m_currentStroke.size(); ++i) {
        const auto& prev = m_currentStroke[i - 1];
        const auto& curr = m_currentStroke[i];
        
        float dx = curr.x - prev.x;
        float dy = curr.y - prev.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance > m_settings.spacing * m_settings.size) {
            int steps = static_cast<int>(distance / (m_settings.spacing * m_settings.size));
            for (int j = 1; j < steps; ++j) {
                float t = static_cast<float>(j) / steps;
                float x = prev.x + dx * t;
                float y = prev.y + dy * t;
                float pressure = prev.pressure + (curr.pressure - prev.pressure) * t;
                float tilt = prev.tilt + (curr.tilt - prev.tilt) * t;
                
                drawBrushPoint(layerData, layerWidth, layerHeight, x, y, pressure, tilt, r, g, b, a);
            }
        }
    }
}

void BrushEngine::drawBrushPoint(unsigned char* layerData, int layerWidth, int layerHeight,
                                float x, float y, float pressure, float tilt,
                                unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    int centerX = static_cast<int>(x);
    int centerY = static_cast<int>(y);
    int brushSize = static_cast<int>(m_settings.size * pressure);
    
    if (brushSize <= 0) return;
    
    // Create brush mask
    std::vector<float> brushMask;
    createBrushMask(brushMask, brushSize, m_settings.hardness);
    
    // Apply brush to layer
    for (int dy = -brushSize; dy <= brushSize; ++dy) {
        for (int dx = -brushSize; dx <= brushSize; ++dx) {
            int layerX = centerX + dx;
            int layerY = centerY + dy;
            
            if (layerX >= 0 && layerX < layerWidth && layerY >= 0 && layerY < layerHeight) {
                int maskIndex = (dy + brushSize) * (2 * brushSize + 1) + (dx + brushSize);
                if (maskIndex >= 0 && maskIndex < static_cast<int>(brushMask.size())) {
                    float maskValue = brushMask[maskIndex];
                    if (maskValue > 0.0f) {
                        int pixelIndex = (layerY * layerWidth + layerX) * 4;
                        
                        // Blend with existing pixel
                        float alpha = maskValue * m_settings.opacity * (a / 255.0f);
                        float invAlpha = 1.0f - alpha;
                        
                        layerData[pixelIndex] = static_cast<unsigned char>(r * alpha + layerData[pixelIndex] * invAlpha);
                        layerData[pixelIndex + 1] = static_cast<unsigned char>(g * alpha + layerData[pixelIndex + 1] * invAlpha);
                        layerData[pixelIndex + 2] = static_cast<unsigned char>(b * alpha + layerData[pixelIndex + 2] * invAlpha);
                        layerData[pixelIndex + 3] = static_cast<unsigned char>(255 * alpha + layerData[pixelIndex + 3] * invAlpha);
                    }
                }
            }
        }
    }
}

float BrushEngine::calculateBrushAlpha(float distance, float pressure, float tilt)
{
    float normalizedDistance = distance / m_settings.size;
    if (normalizedDistance >= 1.0f) return 0.0f;
    
    // Simple circular falloff
    float alpha = 1.0f - normalizedDistance;
    alpha *= pressure; // Apply pressure sensitivity
    
    return std::clamp(alpha, 0.0f, 1.0f);
}

void BrushEngine::createBrushMask(std::vector<float>& mask, float size, float hardness)
{
    int brushSize = static_cast<int>(size);
    int maskSize = 2 * brushSize + 1;
    mask.resize(maskSize * maskSize);
    
    for (int y = 0; y < maskSize; ++y) {
        for (int x = 0; x < maskSize; ++x) {
            float dx = static_cast<float>(x - brushSize) / size;
            float dy = static_cast<float>(y - brushSize) / size;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            if (distance <= 1.0f) {
                float alpha = calculateBrushAlpha(distance * size, 1.0f, 0.0f);
                
                // Apply hardness
                if (hardness < 1.0f) {
                    float softness = 1.0f - hardness;
                    if (distance > hardness) {
                        alpha *= (1.0f - distance) / softness;
                    }
                }
                
                mask[y * maskSize + x] = alpha;
            } else {
                mask[y * maskSize + x] = 0.0f;
            }
        }
    }
}

void BrushEngine::loadPreset(const String& name)
{
    // TODO: Implement preset loading
    std::cout << "Loading brush preset: " << name << std::endl;
}

void BrushEngine::savePreset(const String& name)
{
    // TODO: Implement preset saving
    std::cout << "Saving brush preset: " << name << std::endl;
}

StringList BrushEngine::availablePresets() const
{
    // TODO: Return actual available presets
    return {"Default", "Soft", "Hard", "Airbrush"};
}

} // namespace core
