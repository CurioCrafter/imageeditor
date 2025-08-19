#pragma once

#include <vector>
#include <algorithm>
#include <string>

namespace core {

// Core types - independent of Qt
typedef std::string String;
typedef std::vector<String> StringList;

} // namespace core

namespace core {

struct BrushSettings {
    float size = 10.0f;
    float hardness = 0.8f;
    float opacity = 1.0f;
    float flow = 1.0f;
    float spacing = 0.25f;
    bool pressureSensitive = true;
    bool tiltSensitive = false;
};

struct BrushStroke {
    float x, y;
    float pressure;
    float tilt;
    float timestamp;
};

class BrushEngine {
public:
    BrushEngine();
    ~BrushEngine() = default;

    // Brush settings
    const BrushSettings& settings() const { return m_settings; }
    void setSettings(const BrushSettings& settings) { m_settings = settings; }
    
    // Brush properties
    void setSize(float size) { m_settings.size = size; }
    void setHardness(float hardness) { m_settings.hardness = std::clamp(hardness, 0.0f, 1.0f); }
    void setOpacity(float opacity) { m_settings.opacity = std::clamp(opacity, 0.0f, 1.0f); }
    void setFlow(float flow) { m_settings.flow = std::clamp(flow, 0.0f, 1.0f); }

    // Stroke handling
    void beginStroke(float x, float y, float pressure = 1.0f, float tilt = 0.0f);
    void addPoint(float x, float y, float pressure = 1.0f, float tilt = 0.0f);
    void endStroke();
    
    // Painting
    void paintOnLayer(unsigned char* layerData, int layerWidth, int layerHeight, 
                      unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    
    // Brush presets
    void loadPreset(const String& name);
    void savePreset(const String& name);
    StringList availablePresets() const;

private:
    BrushSettings m_settings;
    std::vector<BrushStroke> m_currentStroke;
    bool m_strokeActive;
    
    // Brush algorithms
    void drawBrushPoint(unsigned char* layerData, int layerWidth, int layerHeight,
                       float x, float y, float pressure, float tilt,
                       unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    
    float calculateBrushAlpha(float distance, float pressure, float tilt);
    void createBrushMask(std::vector<float>& mask, float size, float hardness);
};

} // namespace core
