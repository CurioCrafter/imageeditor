#pragma once

#include <string>
#include <vector>
#include <memory>

namespace core {

// Core types - independent of Qt
typedef std::string String;

struct Size {
    int width, height;
    Size(int w = 0, int h = 0) : width(w), height(h) {}
    bool operator!=(const Size& other) const { return width != other.width || height != other.height; }
    bool operator==(const Size& other) const { return width == other.width && height == other.height; }
};

struct Point {
    int x, y;
    Point(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
};

struct Color {
    int r, g, b, a;
    Color(int r_ = 0, int g_ = 0, int b_ = 0, int a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}
};

} // namespace core

namespace core {

enum class BlendMode {
    Normal,
    Multiply,
    Screen,
    Overlay,
    SoftLight,
    HardLight,
    ColorDodge,
    ColorBurn,
    Darken,
    Lighten,
    Difference,
    Exclusion
};

class Layer {
public:
    explicit Layer(const String& name = "Layer");
    virtual ~Layer();

    // Basic properties
    String getName() const { return m_name; }
    void setName(const String& name) { m_name = name; }
    
    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }
    
    bool isLocked() const { return m_locked; }
    void setLocked(bool locked) { m_locked = locked; }
    
    float getOpacity() const { return m_opacity; }
    void setOpacity(float opacity) { m_opacity = opacity; }
    
    BlendMode getBlendMode() const { return m_blendMode; }
    void setBlendMode(BlendMode mode) { m_blendMode = mode; }

    // Hierarchy
    void addChild(std::shared_ptr<Layer> child);
    void removeChild(std::shared_ptr<Layer> child);
    std::vector<std::shared_ptr<Layer>> getChildren() const { return m_children; }
    
    Layer* getParent() const { return m_parent; }
    void setParent(Layer* parent) { m_parent = parent; }

    // Rendering
    virtual void render(void* target, const void* bounds) = 0;
    
    // Transform
    Point getPosition() const { return m_position; }
    void setPosition(const Point& pos) { m_position = pos; }
    
    Size getSize() const { return m_size; }
    void setSize(const Size& size) { m_size = size; }

protected:
    String m_name;
    bool m_visible;
    bool m_locked;
    float m_opacity;
    BlendMode m_blendMode;
    Point m_position;
    Size m_size;
    
    std::vector<std::shared_ptr<Layer>> m_children;
    Layer* m_parent;
};

} // namespace core
