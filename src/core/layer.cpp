#include "layer.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>

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
    , m_modifiedDate(QDateTime::currentDateTime())
{
}

Layer::~Layer() = default;

void Layer::setName(const QString& name) {
    if (m_name != name) {
        m_name = name;
        onPropertyChanged();
    }
}

void Layer::setVisible(bool visible) {
    if (m_visible != visible) {
        m_visible = visible;
        emit visibilityChanged(visible);
        onPropertyChanged();
    }
}

void Layer::setLocked(bool locked) {
    if (m_locked != locked) {
        m_locked = locked;
        onPropertyChanged();
    }
}

void Layer::setOpacity(float opacity) {
    opacity = std::clamp(opacity, 0.0f, 1.0f);
    if (!qFuzzyCompare(m_opacity, opacity)) {
        m_opacity = opacity;
        emit opacityChanged(opacity);
        onPropertyChanged();
    }
}

void Layer::setBlendMode(BlendMode mode) {
    if (m_blendMode != mode) {
        m_blendMode = mode;
        emit blendModeChanged(mode);
        onPropertyChanged();
    }
}

void Layer::addChild(std::shared_ptr<Layer> child) {
    if (child && child != shared_from_this()) {
        child->setParent(this);
        m_children.push_back(child);
    }
}

void Layer::removeChild(std::shared_ptr<Layer> child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        (*it)->setParent(nullptr);
        m_children.erase(it);
    }
}

void Layer::setPosition(const QPointF& pos) {
    if (m_position != pos) {
        m_position = pos;
        emit positionChanged(pos);
        onPropertyChanged();
    }
}

void Layer::setSize(const QSize& size) {
    if (m_size != size) {
        m_size = size;
        emit sizeChanged(size);
        onPropertyChanged();
    }
}

void Layer::setTransform(const QTransform& transform) {
    if (m_transform != transform) {
        m_transform = transform;
        emit transformChanged(transform);
        onPropertyChanged();
    }
}

void Layer::setMask(const LayerMask& mask) {
    m_mask = mask;
    emit maskChanged();
    onPropertyChanged();
}

void Layer::enableMask(bool enable) {
    if (m_mask.enabled != enable) {
        m_mask.enabled = enable;
        emit maskChanged();
        onPropertyChanged();
    }
}

void Layer::linkMask(bool link) {
    if (m_mask.linked != link) {
        m_mask.linked = link;
        emit maskChanged();
        onPropertyChanged();
    }
}

void Layer::setEffects(const LayerEffects& effects) {
    m_effects = effects;
    emit effectsChanged();
    onPropertyChanged();
}

void Layer::duplicate() {
    // Default implementation creates a shallow copy
    // Derived classes should override this for proper duplication
}

void Layer::merge(const std::vector<std::shared_ptr<Layer>>& layers) {
    // Default implementation does nothing
    // Derived classes should override this for proper merging
    Q_UNUSED(layers)
}

void Layer::rasterize() {
    // Default implementation does nothing
    // Derived classes should override this for proper rasterization
}

QRectF Layer::getBounds() const {
    return QRectF(m_position, m_size);
}

bool Layer::contains(const QPointF& point) const {
    return getBounds().contains(point);
}

bool Layer::intersects(const QRectF& rect) const {
    return getBounds().intersects(rect);
}

void Layer::updateModifiedDate() {
    m_modifiedDate = QDateTime::currentDateTime();
}

void Layer::onPropertyChanged() {
    emit propertyChanged();
    updateModifiedDate();
    notifyParentOfChange();
}

void Layer::notifyParentOfChange() {
    if (m_parent) {
        m_parent->onPropertyChanged();
    }
}

// Raster layer implementation
RasterLayer::RasterLayer(int width, int height, const QColor& fillColor, QObject* parent)
    : Layer("Raster Layer", parent)
    , m_image(width, height, QImage::Format_ARGB32_Premultiplied)
    , m_originalImage(width, height, QImage::Format_ARGB32_Premultiplied)
    , m_selection()
    , m_clipboard()
{
    m_type = LayerType::Raster;
    m_size = QSize(width, height);
    m_image.fill(fillColor);
    m_originalImage = m_image;
}

RasterLayer::RasterLayer(const QImage& image, QObject* parent)
    : Layer("Raster Layer", parent)
    , m_image(image)
    , m_originalImage(image)
    , m_selection()
    , m_clipboard()
{
    m_type = LayerType::Raster;
    m_size = image.size();
}

void RasterLayer::setImage(const QImage& image) {
    m_image = image;
    m_size = image.size();
    updateImageBounds();
    onPropertyChanged();
}

QColor RasterLayer::getPixel(int x, int y) const {
    if (m_image.valid(x, y)) {
        return m_image.pixelColor(x, y);
    }
    return QColor();
}

void RasterLayer::setPixel(int x, int y, const QColor& color) {
    if (m_image.valid(x, y)) {
        m_image.setPixelColor(x, y, color);
        onPropertyChanged();
    }
}

void RasterLayer::fill(const QColor& color) {
    m_image.fill(color);
    onPropertyChanged();
}

void RasterLayer::clear() {
    m_image.fill(Qt::transparent);
    onPropertyChanged();
}

QImage RasterLayer::render(const QSize& size) {
    Q_UNUSED(size)
    return m_image;
}

void RasterLayer::render(QPainter* painter, const QRect& bounds) {
    if (!painter) return;
    
    painter->save();
    painter->setOpacity(m_opacity);
    painter->setTransform(m_transform);
    painter->translate(m_position);
    
    // Apply blend mode (simplified)
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    
    // Draw the image
    painter->drawImage(0, 0, m_image);
    
    painter->restore();
}

void RasterLayer::duplicate() {
    // Create a deep copy of the image
    auto duplicate = std::make_shared<RasterLayer>(m_image, parent());
    duplicate->setName(m_name + " Copy");
    duplicate->setPosition(m_position);
    duplicate->setOpacity(m_opacity);
    duplicate->setBlendMode(m_blendMode);
    duplicate->setVisible(m_visible);
    duplicate->setLocked(m_locked);
    
    // The duplicate should be added to the document by the caller
}

void RasterLayer::merge(const std::vector<std::shared_ptr<Layer>>& layers) {
    // Implementation for merging layers
    Q_UNUSED(layers)
}

void RasterLayer::rasterize() {
    // Raster layers are already rasterized
}

void RasterLayer::applyFilter(Filter* filter) {
    // Implementation for applying filters
    Q_UNUSED(filter)
}

void RasterLayer::adjustBrightnessContrast(float brightness, float contrast) {
    // Implementation for brightness/contrast adjustment
    Q_UNUSED(brightness)
    Q_UNUSED(contrast)
}

void RasterLayer::adjustHueSaturation(float hue, float saturation, float lightness) {
    // Implementation for hue/saturation adjustment
    Q_UNUSED(hue)
    Q_UNUSED(saturation)
    Q_UNUSED(lightness)
}

void RasterLayer::adjustLevels(float blackPoint, float whitePoint, float gamma) {
    // Implementation for levels adjustment
    Q_UNUSED(blackPoint)
    Q_UNUSED(whitePoint)
    Q_UNUSED(gamma)
}

void RasterLayer::selectAll() {
    m_selection = QRect(0, 0, m_size.width(), m_size.height());
}

void RasterLayer::clearSelection() {
    m_selection = QRect();
}

void RasterLayer::invertSelection() {
    // Implementation for inverting selection
}

void RasterLayer::expandSelection(int pixels) {
    // Implementation for expanding selection
    Q_UNUSED(pixels)
}

void RasterLayer::contractSelection(int pixels) {
    // Implementation for contracting selection
    Q_UNUSED(pixels)
}

void RasterLayer::copy(const QRect& bounds) {
    if (bounds.isValid()) {
        m_clipboard = m_image.copy(bounds);
    }
}

void RasterLayer::paste(const QImage& image, const QPoint& position) {
    if (!image.isNull()) {
        QPainter painter(&m_image);
        painter.drawImage(position, image);
        onPropertyChanged();
    }
}

void RasterLayer::cut(const QRect& bounds) {
    if (bounds.isValid()) {
        copy(bounds);
        QPainter painter(&m_image);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.fillRect(bounds, Qt::transparent);
        onPropertyChanged();
    }
}

void RasterLayer::rotate(double angle, const QPointF& center) {
    QTransform transform;
    transform.translate(center.x(), center.y());
    transform.rotate(angle);
    transform.translate(-center.x(), -center.y());
    
    m_image = m_image.transformed(transform, Qt::SmoothTransformation);
    updateImageBounds();
    onPropertyChanged();
}

void RasterLayer::scale(double factor, const QPointF& center) {
    QTransform transform;
    transform.translate(center.x(), center.y());
    transform.scale(factor, factor);
    transform.translate(-center.x(), -center.y());
    
    m_image = m_image.transformed(transform, Qt::SmoothTransformation);
    updateImageBounds();
    onPropertyChanged();
}

void RasterLayer::flipHorizontal() {
    m_image = m_image.mirrored(true, false);
    onPropertyChanged();
}

void RasterLayer::flipVertical() {
    m_image = m_image.mirrored(false, true);
    onPropertyChanged();
}

void RasterLayer::skew(double horizontal, double vertical) {
    QTransform transform;
    transform.shear(horizontal, vertical);
    
    m_image = m_image.transformed(transform, Qt::SmoothTransformation);
    updateImageBounds();
    onPropertyChanged();
}

void RasterLayer::updateImageBounds() {
    m_size = m_image.size();
    emit sizeChanged(m_size);
}

void RasterLayer::applyTransform(const QTransform& transform) {
    m_image = m_image.transformed(transform, Qt::SmoothTransformation);
    updateImageBounds();
    onPropertyChanged();
}

// Adjustment layer implementation
AdjustmentLayer::AdjustmentLayer(AdjustmentType type, QObject* parent)
    : Layer("Adjustment Layer", parent)
    , m_type(type)
{
    m_type = LayerType::Adjustment;
}

void AdjustmentLayer::setParameters(const QVariantMap& params) {
    m_parameters = params;
    onPropertyChanged();
}

QImage AdjustmentLayer::render(const QSize& size) {
    Q_UNUSED(size)
    // Return a placeholder image for adjustment layers
    return QImage(100, 100, QImage::Format_ARGB32_Premultiplied);
}

void AdjustmentLayer::render(QPainter* painter, const QRect& bounds) {
    Q_UNUSED(painter)
    Q_UNUSED(bounds)
    // Adjustment layers don't render directly
}

void AdjustmentLayer::duplicate() {
    auto duplicate = std::make_shared<AdjustmentLayer>(m_type, parent());
    duplicate->setName(m_name + " Copy");
    duplicate->setParameters(m_parameters);
    duplicate->setPosition(m_position);
    duplicate->setOpacity(m_opacity);
    duplicate->setBlendMode(m_blendMode);
    duplicate->setVisible(m_visible);
    duplicate->setLocked(m_locked);
}

void AdjustmentLayer::rasterize() {
    // Implementation for rasterizing adjustment layers
}

QImage AdjustmentLayer::applyAdjustment(const QImage& input) {
    // Implementation for applying adjustments
    Q_UNUSED(input)
    return QImage();
}

// Text layer implementation
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

void TextLayer::setText(const QString& text) {
    if (m_text != text) {
        m_text = text;
        updateTextBounds();
        onPropertyChanged();
    }
}

void TextLayer::setFont(const QFont& font) {
    if (m_font != font) {
        m_font = font;
        updateTextBounds();
        onPropertyChanged();
    }
}

void TextLayer::setColor(const QColor& color) {
    if (m_color != color) {
        m_color = color;
        onPropertyChanged();
    }
}

void TextLayer::setAlignment(Qt::Alignment alignment) {
    if (m_alignment != alignment) {
        m_alignment = alignment;
        updateTextBounds();
        onPropertyChanged();
    }
}

void TextLayer::setLineSpacing(float spacing) {
    if (!qFuzzyCompare(m_lineSpacing, spacing)) {
        m_lineSpacing = spacing;
        updateTextBounds();
        onPropertyChanged();
    }
}

QImage TextLayer::render(const QSize& size) {
    Q_UNUSED(size)
    // Render text to image
    QImage image(m_size, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    
    QPainter painter(&image);
    painter.setFont(m_font);
    painter.setPen(m_color);
    painter.setOpacity(m_opacity);
    
    // Draw text with alignment
    QRect textRect = image.rect();
    painter.drawText(textRect, m_alignment, m_text);
    
    return image;
}

void TextLayer::render(QPainter* painter, const QRect& bounds) {
    if (!painter) return;
    
    painter->save();
    painter->setOpacity(m_opacity);
    painter->setTransform(m_transform);
    painter->translate(m_position);
    
    painter->setFont(m_font);
    painter->setPen(m_color);
    
    // Draw text
    painter->drawText(bounds, m_alignment, m_text);
    
    painter->restore();
}

void TextLayer::duplicate() {
    auto duplicate = std::make_shared<TextLayer>(m_text, parent());
    duplicate->setName(m_name + " Copy");
    duplicate->setFont(m_font);
    duplicate->setColor(m_color);
    duplicate->setAlignment(m_alignment);
    duplicate->setLineSpacing(m_lineSpacing);
    duplicate->setPosition(m_position);
    duplicate->setOpacity(m_opacity);
    duplicate->setBlendMode(m_blendMode);
    duplicate->setVisible(m_visible);
    duplicate->setLocked(m_locked);
}

void TextLayer::rasterize() {
    // Convert text layer to raster layer
    QImage textImage = render();
    auto rasterLayer = std::make_shared<RasterLayer>(textImage, parent());
    rasterLayer->setName(m_name + " (Rasterized)");
    rasterLayer->setPosition(m_position);
    rasterLayer->setOpacity(m_opacity);
    rasterLayer->setBlendMode(m_blendMode);
    rasterLayer->setVisible(m_visible);
    rasterLayer->setLocked(m_locked);
    
    // The rasterized layer should replace this text layer in the document
}

void TextLayer::updateTextBounds() {
    // Calculate text bounds based on font and text content
    QFontMetrics metrics(m_font);
    QSize textSize = metrics.size(Qt::TextSingleLine, m_text);
    m_size = textSize;
    emit sizeChanged(m_size);
}

} // namespace core
