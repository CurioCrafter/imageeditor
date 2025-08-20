#include "layer.h"
#include <QDebug>
#include <QPainter>
#include <QDateTime>

namespace core {

// Base Layer implementation
Layer::Layer(const QString& name, QObject* parent)
    : QObject(parent)
    , m_name(name)
    , m_visible(true)
    , m_locked(false)
    , m_opacity(1.0f)
    , m_blendMode(BlendMode::Normal)
    , m_type(LayerType::Raster)
    , m_position(0, 0)
    , m_size(100, 100)
    , m_transform()
    , m_parent(nullptr)
    , m_createdDate(QDateTime::currentDateTime())
    , m_modifiedDate(m_createdDate)
{
}

Layer::~Layer() = default;

void Layer::setName(const QString& name)
{
    if (m_name != name) {
        m_name = name;
        emit propertyChanged();
        onPropertyChanged();
    }
}

void Layer::setVisible(bool visible)
{
    if (m_visible != visible) {
        m_visible = visible;
        emit visibilityChanged(visible);
        onPropertyChanged();
    }
}

void Layer::setLocked(bool locked)
{
    if (m_locked != locked) {
        m_locked = locked;
        onPropertyChanged();
    }
}

void Layer::setOpacity(float opacity)
{
    if (m_opacity != opacity) {
        m_opacity = opacity;
        emit opacityChanged(opacity);
        onPropertyChanged();
    }
}

void Layer::setBlendMode(BlendMode mode)
{
    if (m_blendMode != mode) {
        m_blendMode = mode;
        emit blendModeChanged(mode);
        onPropertyChanged();
    }
}

void Layer::addChild(std::shared_ptr<Layer> child)
{
    if (child && child.get() != this) {
        child->setParent(this);
        m_children.push_back(child);
        onPropertyChanged();
    }
}

void Layer::removeChild(std::shared_ptr<Layer> child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        (*it)->setParent(nullptr);
        m_children.erase(it);
        onPropertyChanged();
    }
}

void Layer::setPosition(const QPointF& pos)
{
    if (m_position != pos) {
        m_position = pos;
        emit positionChanged(pos);
        onPropertyChanged();
    }
}

void Layer::setSize(const QSize& size)
{
    if (m_size != size) {
        m_size = size;
        emit sizeChanged(size);
        onPropertyChanged();
    }
}

void Layer::setTransform(const QTransform& transform)
{
    if (m_transform != transform) {
        m_transform = transform;
        emit transformChanged(transform);
        onPropertyChanged();
    }
}

void Layer::setMask(const LayerMask& mask)
{
    m_mask = mask;
    emit maskChanged();
    onPropertyChanged();
}

void Layer::enableMask(bool enable)
{
    m_mask.enabled = enable;
    emit maskChanged();
    onPropertyChanged();
}

void Layer::linkMask(bool link)
{
    m_mask.linked = link;
    emit maskChanged();
    onPropertyChanged();
}

void Layer::setEffects(const LayerEffects& effects)
{
    m_effects = effects;
    emit effectsChanged();
    onPropertyChanged();
}

void Layer::duplicate()
{
    // TODO: Implement layer duplication
    qDebug() << "Layer duplication not yet implemented";
}

void Layer::merge(const std::vector<std::shared_ptr<Layer>>& layers)
{
    Q_UNUSED(layers)
    // TODO: Implement layer merging
    qDebug() << "Layer merging not yet implemented";
}

void Layer::rasterize()
{
    // TODO: Implement layer rasterization
    qDebug() << "Layer rasterization not yet implemented";
}

QRectF Layer::getBounds() const
{
    return QRectF(m_position, m_size);
}

bool Layer::contains(const QPointF& point) const
{
    return getBounds().contains(point);
}

bool Layer::intersects(const QRectF& rect) const
{
    return getBounds().intersects(rect);
}

void Layer::updateModifiedDate()
{
    m_modifiedDate = QDateTime::currentDateTime();
}

void Layer::onPropertyChanged()
{
    updateModifiedDate();
    notifyParentOfChange();
}

void Layer::notifyParentOfChange()
{
    if (m_parent) {
        m_parent->onPropertyChanged();
    }
}

// RasterLayer implementation
RasterLayer::RasterLayer(int width, int height, const QColor& fillColor, QObject* parent)
    : Layer("Raster Layer", parent)
{
    m_type = LayerType::Raster;
    m_size = QSize(width, height);
    m_image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    m_image.fill(fillColor);
    m_originalImage = m_image;
}

RasterLayer::RasterLayer(const QImage& image, QObject* parent)
    : Layer("Raster Layer", parent)
{
    m_type = LayerType::Raster;
    m_image = image;
    m_size = image.size();
    m_originalImage = image;
}



void RasterLayer::setImage(const QImage& image)
{
    m_image = image;
    m_size = image.size();
    updateImageBounds();
    onPropertyChanged();
}

QColor RasterLayer::getPixel(int x, int y) const
{
    if (m_image.valid(x, y)) {
        return m_image.pixelColor(x, y);
    }
    return QColor();
}

void RasterLayer::setPixel(int x, int y, const QColor& color)
{
    if (m_image.valid(x, y)) {
        m_image.setPixelColor(x, y, color);
        onPropertyChanged();
    }
}

void RasterLayer::fill(const QColor& color)
{
    m_image.fill(color);
    onPropertyChanged();
}

void RasterLayer::clear()
{
    m_image.fill(Qt::transparent);
    onPropertyChanged();
}

QImage RasterLayer::render(const QSize& size)
{
    Q_UNUSED(size)
    return m_image;
}

void RasterLayer::render(QPainter* painter, const QRect& bounds)
{
    if (painter) {
        painter->drawImage(bounds, m_image);
    }
}

void RasterLayer::duplicate()
{
    // TODO: Implement raster layer duplication
    qDebug() << "Raster layer duplication not yet implemented";
}

void RasterLayer::merge(const std::vector<std::shared_ptr<Layer>>& layers)
{
    Q_UNUSED(layers)
    // TODO: Implement raster layer merging
    qDebug() << "Raster layer merging not yet implemented";
}

void RasterLayer::rasterize()
{
    // Raster layers are already rasterized
    qDebug() << "Raster layer is already rasterized";
}

void RasterLayer::applyFilter(Filter* filter)
{
    Q_UNUSED(filter)
    // TODO: Implement filter application
    qDebug() << "Filter application not yet implemented";
}

void RasterLayer::adjustBrightnessContrast(float brightness, float contrast)
{
    Q_UNUSED(brightness)
    Q_UNUSED(contrast)
    // TODO: Implement brightness/contrast adjustment
    qDebug() << "Brightness/contrast adjustment not yet implemented";
}

void RasterLayer::adjustHueSaturation(float hue, float saturation, float lightness)
{
    Q_UNUSED(hue)
    Q_UNUSED(saturation)
    Q_UNUSED(lightness)
    // TODO: Implement hue/saturation adjustment
    qDebug() << "Hue/saturation adjustment not yet implemented";
}

void RasterLayer::adjustLevels(float blackPoint, float whitePoint, float gamma)
{
    Q_UNUSED(blackPoint)
    Q_UNUSED(whitePoint)
    Q_UNUSED(gamma)
    // TODO: Implement levels adjustment
    qDebug() << "Levels adjustment not yet implemented";
}

void RasterLayer::selectAll()
{
    m_selection = m_image.rect();
    onPropertyChanged();
}

void RasterLayer::clearSelection()
{
    m_selection = QRect();
    onPropertyChanged();
}

void RasterLayer::invertSelection()
{
    // TODO: Implement selection inversion
    qDebug() << "Selection inversion not yet implemented";
}

void RasterLayer::expandSelection(int pixels)
{
    Q_UNUSED(pixels)
    // TODO: Implement selection expansion
    qDebug() << "Selection expansion not yet implemented";
}

void RasterLayer::contractSelection(int pixels)
{
    Q_UNUSED(pixels)
    // TODO: Implement selection contraction
    qDebug() << "Selection contraction not yet implemented";
}

void RasterLayer::copy(const QRect& bounds)
{
    if (bounds.isValid() && m_image.rect().intersects(bounds)) {
        m_clipboard = m_image.copy(bounds);
    }
}

void RasterLayer::paste(const QImage& image, const QPoint& position)
{
    Q_UNUSED(image)
    Q_UNUSED(position)
    // TODO: Implement image pasting
    qDebug() << "Image pasting not yet implemented";
}

void RasterLayer::cut(const QRect& bounds)
{
    copy(bounds);
    // TODO: Implement cutting (copy + clear selection)
    qDebug() << "Image cutting not yet implemented";
}

void RasterLayer::rotate(double angle, const QPointF& center)
{
    Q_UNUSED(angle)
    Q_UNUSED(center)
    // TODO: Implement rotation
    qDebug() << "Image rotation not yet implemented";
}

void RasterLayer::scale(double factor, const QPointF& center)
{
    Q_UNUSED(factor)
    Q_UNUSED(center)
    // TODO: Implement scaling
    qDebug() << "Image scaling not yet implemented";
}

void RasterLayer::flipHorizontal()
{
    m_image = m_image.mirrored(true, false);
    onPropertyChanged();
}

void RasterLayer::flipVertical()
{
    m_image = m_image.mirrored(false, true);
    onPropertyChanged();
}

void RasterLayer::skew(double horizontal, double vertical)
{
    Q_UNUSED(horizontal)
    Q_UNUSED(vertical)
    // TODO: Implement skewing
    qDebug() << "Image skewing not yet implemented";
}

void RasterLayer::updateImageBounds()
{
    m_size = m_image.size();
    emit sizeChanged(m_size);
}

void RasterLayer::applyTransform(const QTransform& transform)
{
    Q_UNUSED(transform)
    // TODO: Implement transform application
    qDebug() << "Transform application not yet implemented";
}

// AdjustmentLayer implementation
AdjustmentLayer::AdjustmentLayer(AdjustmentType type, QObject* parent)
    : Layer("Adjustment Layer", parent)
    , m_adjustmentType(type)
{
    m_type = LayerType::Adjustment;
}

QImage AdjustmentLayer::render(const QSize& size)
{
    Q_UNUSED(size)
    // TODO: Implement adjustment layer rendering
    return QImage();
}

void AdjustmentLayer::render(QPainter* painter, const QRect& bounds)
{
    Q_UNUSED(painter)
    Q_UNUSED(bounds)
    // TODO: Implement adjustment layer rendering
}

void AdjustmentLayer::duplicate()
{
    // TODO: Implement adjustment layer duplication
    qDebug() << "Adjustment layer duplication not yet implemented";
}

void AdjustmentLayer::rasterize()
{
    // TODO: Implement adjustment layer rasterization
    qDebug() << "Adjustment layer rasterization not yet implemented";
}

void AdjustmentLayer::setAdjustmentType(AdjustmentType type)
{
    m_adjustmentType = type;
    onPropertyChanged();
}

void AdjustmentLayer::setParameters(const QVariantMap& params)
{
    m_parameters = params;
    onPropertyChanged();
}

void AdjustmentLayer::setParameter(const QString& key, const QVariant& value)
{
    m_parameters[key] = value;
    onPropertyChanged();
}

QImage AdjustmentLayer::applyAdjustment(const QImage& input) const
{
    Q_UNUSED(input)
    // TODO: Implement adjustment application
    return QImage();
}

// TextLayer implementation
TextLayer::TextLayer(const QString& text, QObject* parent)
    : Layer("Text Layer", parent)
    , m_text(text)
    , m_font("Arial", 12)
    , m_color(Qt::black)
    , m_alignment(Qt::AlignLeft)
    , m_lineSpacing(1.0f)
{
    m_type = LayerType::Text;
    updateTextBounds();
}

QImage TextLayer::render(const QSize& size)
{
    Q_UNUSED(size)
    // TODO: Implement text layer rendering
    return QImage();
}

void TextLayer::render(QPainter* painter, const QRect& bounds)
{
    Q_UNUSED(painter)
    Q_UNUSED(bounds)
    // TODO: Implement text layer rendering
}

void TextLayer::duplicate()
{
    // TODO: Implement text layer duplication
    qDebug() << "Text layer duplication not yet implemented";
}

void TextLayer::rasterize()
{
    // TODO: Implement text layer rasterization
    qDebug() << "Text layer rasterization not yet implemented";
}

void TextLayer::setText(const QString& text)
{
    if (m_text != text) {
        m_text = text;
        updateTextBounds();
        onPropertyChanged();
    }
}

void TextLayer::setFont(const QFont& font)
{
    if (m_font != font) {
        m_font = font;
        updateTextBounds();
        onPropertyChanged();
    }
}

void TextLayer::setColor(const QColor& color)
{
    if (m_color != color) {
        m_color = color;
        onPropertyChanged();
    }
}

void TextLayer::setAlignment(Qt::Alignment alignment)
{
    if (m_alignment != alignment) {
        m_alignment = alignment;
        onPropertyChanged();
    }
}

void TextLayer::setLineSpacing(float spacing)
{
    if (m_lineSpacing != spacing) {
        m_lineSpacing = spacing;
        updateTextBounds();
        onPropertyChanged();
    }
}

void TextLayer::updateTextBounds()
{
    // TODO: Calculate text bounds based on font and text
    qDebug() << "Text bounds calculation not yet implemented";
}

} // namespace core
