#pragma once

#include <QObject>
#include <QString>
#include <QSize>
#include <QPoint>
#include <QPointF>
#include <QImage>
#include <QPainter>
#include <QTransform>
#include <QDateTime>
#include <QColor>
#include <QFont>
#include <QVariant>
#include <memory>
#include <vector>

namespace core {

// Forward declarations
class Document;

// Enhanced blend mode enumeration
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
    Exclusion,
    Hue,
    Saturation,
    Color,
    Luminosity,
    Dissolve,
    Behind,
    Clear,
    Add,
    Subtract,
    Divide,
    LinearBurn,
    LinearDodge,
    VividLight,
    LinearLight,
    PinLight,
    HardMix
};

// Layer type enumeration
enum class LayerType {
    Raster,         // Pixel-based layer
    Vector,         // Vector graphics layer
    Adjustment,     // Adjustment/filter layer
    Text,           // Text layer
    Shape,          // Shape layer
    SmartObject,    // Smart object layer
    Group           // Layer group
};

// Layer mask information
struct LayerMask {
    QImage mask;
    bool enabled = false;
    bool linked = true;
    QPoint offset{0, 0};
    float density = 1.0f;
    float feather = 0.0f;
};

// Layer effects - FIXED: No duplicate definitions
struct LayerEffects {
    // Effect parameters
    struct DropShadow {
        bool enabled = false;
        QColor color{Qt::black};
        float opacity = 0.75f;
        float angle = 120.0f;
        float distance = 5.0f;
        float spread = 0.0f;
        float size = 5.0f;
        bool useGlobalLight = true;
        bool antiAlias = true;
    } dropShadow;
    
    struct InnerShadow {
        bool enabled = false;
        QColor color{Qt::black};
        float opacity = 0.75f;
        float angle = 120.0f;
        float distance = 5.0f;
        float choke = 0.0f;
        float size = 5.0f;
        bool useGlobalLight = true;
        bool antiAlias = true;
    } innerShadow;
    
    struct OuterGlow {
        bool enabled = false;
        QColor color{Qt::yellow};
        float opacity = 0.75f;
        float spread = 0.0f;
        float size = 5.0f;
    } outerGlow;
    
    struct InnerGlow {
        bool enabled = false;
        QColor color{Qt::yellow};
        float opacity = 0.75f;
        float choke = 0.0f;
        float size = 5.0f;
    } innerGlow;
    
    struct Stroke {
        bool enabled = false;
        QColor color{Qt::black};
        float size = 1.0f;
        enum Position { Outside, Inside, Center } position = Outside;
    } stroke;
};

// Base Layer class with Qt6 integration
class Layer : public QObject {
    Q_OBJECT
public:
    explicit Layer(const QString& name = "Layer", QObject* parent = nullptr);
    virtual ~Layer();

    // Basic properties
    QString getName() const { return m_name; }
    void setName(const QString& name);
    
    bool isVisible() const { return m_visible; }
    void setVisible(bool visible);
    
    bool isLocked() const { return m_locked; }
    void setLocked(bool locked);
    
    float getOpacity() const { return m_opacity; }
    void setOpacity(float opacity);
    
    BlendMode getBlendMode() const { return m_blendMode; }
    void setBlendMode(BlendMode mode);
    
    LayerType getType() const { return m_type; }
    
    // Hierarchy
    void addChild(std::shared_ptr<Layer> child);
    void removeChild(std::shared_ptr<Layer> child);
    std::vector<std::shared_ptr<Layer>> getChildren() const { return m_children; }
    
    Layer* getParent() const { return m_parent; }
    void setParent(Layer* parent) { m_parent = parent; }
    
    // Transform
    QPointF getPosition() const { return m_position; }
    void setPosition(const QPointF& pos);
    
    QSize getSize() const { return m_size; }
    void setSize(const QSize& size);
    
    QTransform getTransform() const { return m_transform; }
    void setTransform(const QTransform& transform);
    
    // Layer mask
    const LayerMask& getMask() const { return m_mask; }
    void setMask(const LayerMask& mask);
    void enableMask(bool enable);
    void linkMask(bool link);
    
    // Layer effects
    const LayerEffects& getEffects() const { return m_effects; }
    void setEffects(const LayerEffects& effects);
    
    // Rendering
    virtual QImage render(const QSize& size = QSize()) = 0;
    virtual void render(QPainter* painter, const QRect& bounds = QRect()) = 0;
    
    // Layer operations
    virtual void duplicate();
    virtual void merge(const std::vector<std::shared_ptr<Layer>>& layers);
    virtual void rasterize();
    
    // Utility methods
    QRectF getBounds() const;
    bool contains(const QPointF& point) const;
    bool intersects(const QRectF& rect) const;
    
    // Metadata
    QDateTime getCreatedDate() const { return m_createdDate; }
    QDateTime getModifiedDate() const { return m_modifiedDate; }
    void updateModifiedDate();

protected:
    // Protected members for derived classes
    QString m_name;
    bool m_visible;
    bool m_locked;
    float m_opacity;
    BlendMode m_blendMode;
    LayerType m_type;
    QPointF m_position;
    QSize m_size;
    QTransform m_transform;
    
    std::vector<std::shared_ptr<Layer>> m_children;
    Layer* m_parent;
    
    LayerMask m_mask;
    LayerEffects m_effects;
    
    QDateTime m_createdDate;
    QDateTime m_modifiedDate;
    
    // Internal methods
    virtual void onPropertyChanged();
    void notifyParentOfChange();

signals:
    void propertyChanged();
    void visibilityChanged(bool visible);
    void opacityChanged(float opacity);
    void blendModeChanged(BlendMode mode);
    void positionChanged(const QPointF& position);
    void sizeChanged(const QSize& size);
    void transformChanged(const QTransform& transform);
    void maskChanged();
    void effectsChanged();
};

// Raster layer implementation
class RasterLayer : public Layer {
    Q_OBJECT
public:
    RasterLayer(int width, int height, const QColor& fillColor = Qt::transparent, QObject* parent = nullptr);
    RasterLayer(const QImage& image, QObject* parent = nullptr);
    
    // Image data access
    QImage getImage() const { return m_image; }
    void setImage(const QImage& image);
    
    // Pixel manipulation
    QColor getPixel(int x, int y) const;
    void setPixel(int x, int y, const QColor& color);
    void fill(const QColor& color);
    void clear();
    
    // Rendering
    QImage render(const QSize& size = QSize()) override;
    void render(QPainter* painter, const QRect& bounds = QRect()) override;
    
    // Layer operations
    void duplicate() override;
    void merge(const std::vector<std::shared_ptr<Layer>>& layers) override;
    void rasterize() override;
    
    // Image processing
    void applyFilter(class Filter* filter);
    void adjustBrightnessContrast(float brightness, float contrast);
    void adjustHueSaturation(float hue, float saturation, float lightness);
    void adjustLevels(float blackPoint, float whitePoint, float gamma);
    
    // Selection operations
    void selectAll();
    void clearSelection();
    void invertSelection();
    void expandSelection(int pixels);
    void contractSelection(int pixels);
    
    // Copy/paste operations
    void copy(const QRect& bounds);
    void paste(const QImage& image, const QPoint& position);
    void cut(const QRect& bounds);
    
    // Transform operations
    void rotate(double angle, const QPointF& center = QPointF());
    void scale(double factor, const QPointF& center = QPointF());
    void flipHorizontal();
    void flipVertical();
    void skew(double horizontal, double vertical);

private:
    QImage m_image;
    QImage m_originalImage; // For undo/redo
    QRect m_selection;
    QImage m_clipboard;
    
    void updateImageBounds();
    void applyTransform(const QTransform& transform);
};

// Adjustment layer for non-destructive editing
class AdjustmentLayer : public Layer {
    Q_OBJECT
public:
    enum class AdjustmentType {
        BrightnessContrast,
        HueSaturation,
        ColorBalance,
        Curves,
        Levels,
        PhotoFilter,
        ChannelMixer,
        GradientMap,
        Invert,
        Threshold,
        Posterize,
        SelectiveColor
    };
    
    AdjustmentLayer(AdjustmentType type, QObject* parent = nullptr);
    
    // Adjustment parameters
    AdjustmentType getAdjustmentType() const { return m_adjustmentType; }
    void setAdjustmentType(AdjustmentType type);
    void setParameters(const QVariantMap& params);
    QVariantMap getParameters() const { return m_parameters; }
    void setParameter(const QString& key, const QVariant& value);
    
    // Rendering
    QImage render(const QSize& size = QSize()) override;
    void render(QPainter* painter, const QRect& bounds = QRect()) override;
    
    // Layer operations
    void duplicate() override;
    void rasterize() override;
    
    // Adjustment application
    QImage applyAdjustment(const QImage& input) const;

private:
    AdjustmentType m_adjustmentType;
    QVariantMap m_parameters;
};

// Text layer for typography
class TextLayer : public Layer {
    Q_OBJECT
public:
    TextLayer(const QString& text = "", QObject* parent = nullptr);
    
    // Text properties
    QString getText() const { return m_text; }
    void setText(const QString& text);
    
    QFont getFont() const { return m_font; }
    void setFont(const QFont& font);
    
    QColor getColor() const { return m_color; }
    void setColor(const QColor& color);
    
    // Text formatting
    void setAlignment(Qt::Alignment alignment);
    Qt::Alignment getAlignment() const { return m_alignment; }
    
    void setLineSpacing(float spacing);
    float getLineSpacing() const { return m_lineSpacing; }
    
    // Rendering
    QImage render(const QSize& size = QSize()) override;
    void render(QPainter* painter, const QRect& bounds = QRect()) override;
    
    // Layer operations
    void duplicate() override;
    void rasterize() override;

private:
    QString m_text;
    QFont m_font;
    QColor m_color;
    Qt::Alignment m_alignment;
    float m_lineSpacing;
    
    void updateTextBounds();
};

} // namespace core
