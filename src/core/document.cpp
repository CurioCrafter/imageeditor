#include "document.h"
#include "layer.h"
#include <QPainter>
#include <QFileInfo>
#include <QDebug>
#include <algorithm>

namespace core {

Document::Document(int width, int height, QObject* parent)
    : QObject(parent)
    , m_width(width)
    , m_height(height)
    , m_colorMode(ColorMode::RGBA8)
    , m_modified(false)
{
    // Create default background layer
    auto background = std::make_shared<RasterLayer>(width, height, "Background");
    background->fill(Qt::white);
    addLayer(background);
    
    m_createdDate = QDateTime::currentDateTime();
    m_modifiedDate = m_createdDate;
}

Document::~Document() = default;

void Document::addLayer(LayerPtr layer, int index)
{
    if (!layer) return;
    
    if (index < 0 || index > static_cast<int>(m_layers.size())) {
        m_layers.push_back(layer);
    } else {
        m_layers.insert(m_layers.begin() + index, layer);
    }
    
    if (!m_activeLayer) {
        m_activeLayer = layer;
    }
    
    invalidateCache();
    updateModifiedDate();
    emit layerAdded(m_layers.size() - 1);
}

void Document::removeLayer(int index)
{
    if (index < 0 || index >= static_cast<int>(m_layers.size())) return;
    
    auto layer = m_layers[index];
    if (layer == m_activeLayer) {
        m_activeLayer = nullptr;
    }
    
    m_layers.erase(m_layers.begin() + index);
    
    // Set new active layer if needed
    if (!m_activeLayer && !m_layers.empty()) {
        m_activeLayer = m_layers.back();
    }
    
    invalidateCache();
    updateModifiedDate();
    emit layerRemoved(index);
}

void Document::moveLayer(int from, int to)
{
    if (from < 0 || from >= static_cast<int>(m_layers.size()) ||
        to < 0 || to >= static_cast<int>(m_layers.size()) ||
        from == to) {
        return;
    }
    
    auto layer = m_layers[from];
    m_layers.erase(m_layers.begin() + from);
    m_layers.insert(m_layers.begin() + to, layer);
    
    invalidateCache();
    updateModifiedDate();
    emit layerMoved(from, to);
}

LayerPtr Document::getLayerAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_layers.size())) {
        return m_layers[index];
    }
    return nullptr;
}

LayerPtr Document::getLayerByName(const QString& name) const
{
    auto it = std::find_if(m_layers.begin(), m_layers.end(),
        [&name](const LayerPtr& layer) {
            return layer->getName() == name;
        });
    
    return (it != m_layers.end()) ? *it : nullptr;
}

void Document::setActiveLayer(int index)
{
    auto layer = getLayerAt(index);
    if (layer) {
        m_activeLayer = layer;
        emit activeLayerChanged(index);
    }
}

QImage Document::render(const QRect& viewport) const
{
    QRect renderRect = viewport.isNull() ? QRect(0, 0, m_width, m_height) : viewport;
    QImage result(renderRect.size(), QImage::Format_ARGB32_Premultiplied);
    result.fill(Qt::transparent);
    
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Render layers from bottom to top
    for (const auto& layer : m_layers) {
        if (!layer->isVisible()) continue;
        
        QImage layerImage = layer->render();
        if (layerImage.isNull()) continue;
        
        // Apply blend mode and opacity
        painter.setOpacity(layer->getOpacity());
        painter.setCompositionMode(blendModeToQPainter(layer->getBlendMode()));
        
        QRect layerBounds = layer->getBounds().toRect();
        if (renderRect.intersects(layerBounds)) {
            painter.drawImage(layerBounds.topLeft() - renderRect.topLeft(), layerImage);
        }
    }
    
    return result;
}

void Document::render(QPainter* painter, const QRect& viewport) const
{
    if (!painter) return;
    
    QImage rendered = render(viewport);
    painter->drawImage(viewport.topLeft(), rendered);
}

bool Document::loadFromFile(const QString& filename)
{
    QFileInfo info(filename);
    if (!info.exists()) return false;
    
    // TODO: Implement actual file loading
    // For now, just update metadata
    m_filename = filename;
    m_name = info.baseName();
    updateModifiedDate();
    
    return true;
}

bool Document::saveToFile(const QString& filename) const
{
    // TODO: Implement actual file saving
    // For now, just pretend it worked
    const_cast<Document*>(this)->m_filename = filename;
    const_cast<Document*>(this)->m_modified = false;
    
    return true;
}

bool Document::exportImage(const QString& filename, const QString& format) const
{
    QImage image = render();
    return image.save(filename, format.toUtf8().constData());
}

void Document::undo()
{
    if (!canUndo()) return;
    
    // TODO: Implement undo
    invalidateCache();
    emit documentChanged();
}

void Document::redo()
{
    if (!canRedo()) return;
    
    // TODO: Implement redo
    invalidateCache();
    emit documentChanged();
}

void Document::resize(int width, int height)
{
    if (width == m_width && height == m_height) return;
    
    m_width = width;
    m_height = height;
    
    // TODO: Resize all layers
    
    invalidateCache();
    updateModifiedDate();
    emit sizeChanged(QSize(width, height));
}

void Document::crop(const QRect& rect)
{
    // TODO: Implement cropping
    invalidateCache();
    updateModifiedDate();
    emit documentChanged();
}

QPainter::CompositionMode Document::blendModeToQPainter(BlendMode mode) const
{
    switch (mode) {
        case BlendMode::Normal: return QPainter::CompositionMode_SourceOver;
        case BlendMode::Multiply: return QPainter::CompositionMode_Multiply;
        case BlendMode::Screen: return QPainter::CompositionMode_Screen;
        case BlendMode::Overlay: return QPainter::CompositionMode_Overlay;
        case BlendMode::SoftLight: return QPainter::CompositionMode_SoftLight;
        case BlendMode::HardLight: return QPainter::CompositionMode_HardLight;
        case BlendMode::ColorDodge: return QPainter::CompositionMode_ColorDodge;
        case BlendMode::ColorBurn: return QPainter::CompositionMode_ColorBurn;
        case BlendMode::Darken: return QPainter::CompositionMode_Darken;
        case BlendMode::Lighten: return QPainter::CompositionMode_Lighten;
        case BlendMode::Difference: return QPainter::CompositionMode_Difference;
        case BlendMode::Exclusion: return QPainter::CompositionMode_Exclusion;
        default: return QPainter::CompositionMode_SourceOver;
    }
}

void Document::invalidateCache()
{
    // Clear any cached render data
    // This would be called when layers change
}

void Document::updateModifiedDate()
{
    m_modifiedDate = QDateTime::currentDateTime();
    m_modified = true;
    emit modifiedChanged(true);
}

} // namespace core
