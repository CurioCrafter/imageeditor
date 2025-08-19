#include "document.h"
#include "layer.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>

namespace core {

// Document implementation
Document::Document(int width, int height, ColorMode mode, QObject* parent)
    : QObject(parent)
    , m_name("Untitled")
    , m_size(width, height)
    , m_colorMode(mode)
    , m_createdDate(QDateTime::currentDateTime())
    , m_modifiedDate(QDateTime::currentDateTime())
    , m_activeLayerIndex(-1)
    , m_renderQuality(Normal)
    , m_gpuAcceleration(true)
    , m_cacheValid(false)
{
}

Document::Document(const QSize& size, ColorMode mode, QObject* parent)
    : Document(size.width(), size.height(), mode, parent)
{
}

Document::~Document() = default;

void Document::setName(const QString& name) {
    if (m_name != name) {
        m_name = name;
        updateModifiedDate();
    }
}

void Document::setSize(const QSize& size) {
    if (m_size != size) {
        m_size = size;
        invalidateCache();
        emit documentSizeChanged(size);
        updateModifiedDate();
    }
}

void Document::setColorMode(ColorMode mode) {
    if (m_colorMode != mode) {
        m_colorMode = mode;
        invalidateCache();
        updateModifiedDate();
    }
}

void Document::addLayer(LayerPtr layer, int index) {
    if (!layer) return;
    
    if (index < 0 || index > static_cast<int>(m_layers.size())) {
        index = m_layers.size();
    }
    
    m_layers.insert(m_layers.begin() + index, layer);
    
    if (m_activeLayerIndex < 0) {
        setActiveLayer(0);
    }
    
    invalidateCache();
    emit layerAdded(index);
    updateModifiedDate();
}

void Document::removeLayer(int index) {
    if (index < 0 || index >= static_cast<int>(m_layers.size())) return;
    
    LayerPtr layer = m_layers[index];
    m_layers.erase(m_layers.begin() + index);
    
    // Update active layer index
    if (m_activeLayerIndex == index) {
        if (m_layers.empty()) {
            m_activeLayerIndex = -1;
        } else {
            m_activeLayerIndex = std::min(m_activeLayerIndex, static_cast<int>(m_layers.size()) - 1);
        }
        emit activeLayerChanged(m_activeLayerIndex);
    } else if (m_activeLayerIndex > index) {
        m_activeLayerIndex--;
        emit activeLayerChanged(m_activeLayerIndex);
    }
    
    invalidateCache();
    emit layerRemoved(index);
    updateModifiedDate();
}

void Document::removeLayer(LayerPtr layer) {
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        int index = std::distance(m_layers.begin(), it);
        removeLayer(index);
    }
}

void Document::moveLayer(int fromIndex, int toIndex) {
    if (fromIndex < 0 || fromIndex >= static_cast<int>(m_layers.size()) ||
        toIndex < 0 || toIndex >= static_cast<int>(m_layers.size())) {
        return;
    }
    
    if (fromIndex == toIndex) return;
    
    LayerPtr layer = m_layers[fromIndex];
    m_layers.erase(m_layers.begin() + fromIndex);
    m_layers.insert(m_layers.begin() + toIndex, layer);
    
    // Update active layer index
    if (m_activeLayerIndex == fromIndex) {
        m_activeLayerIndex = toIndex;
        emit activeLayerChanged(m_activeLayerIndex);
    } else if (m_activeLayerIndex > fromIndex && m_activeLayerIndex <= toIndex) {
        m_activeLayerIndex--;
        emit activeLayerChanged(m_activeLayerIndex);
    } else if (m_activeLayerIndex < fromIndex && m_activeLayerIndex >= toIndex) {
        m_activeLayerIndex++;
        emit activeLayerChanged(m_activeLayerIndex);
    }
    
    invalidateCache();
    emit layerMoved(fromIndex, toIndex);
    updateModifiedDate();
}

void Document::duplicateLayer(int index) {
    if (index < 0 || index >= static_cast<int>(m_layers.size())) return;
    
    LayerPtr original = m_layers[index];
    LayerPtr duplicate = original->duplicate();
    duplicate->setName(original->getName() + " Copy");
    
    addLayer(duplicate, index + 1);
}

void Document::mergeDown(int index) {
    if (index <= 0 || index >= static_cast<int>(m_layers.size())) return;
    
    LayerPtr topLayer = m_layers[index];
    LayerPtr bottomLayer = m_layers[index - 1];
    
    if (topLayer && bottomLayer) {
        bottomLayer->merge({topLayer});
        removeLayer(index);
    }
}

void Document::mergeVisible() {
    // Implementation for merging all visible layers
    invalidateCache();
    updateModifiedDate();
}

void Document::flattenImage() {
    // Implementation for flattening all layers
    invalidateCache();
    updateModifiedDate();
}

std::vector<Document::LayerPtr> Document::getLayers() const {
    return m_layers;
}

Document::LayerPtr Document::getLayerAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_layers.size())) {
        return nullptr;
    }
    return m_layers[index];
}

Document::LayerPtr Document::getActiveLayer() const {
    if (m_activeLayerIndex < 0 || m_activeLayerIndex >= static_cast<int>(m_layers.size())) {
        return nullptr;
    }
    return m_layers[m_activeLayerIndex];
}

void Document::setActiveLayer(int index) {
    if (index < -1 || index >= static_cast<int>(m_layers.size())) return;
    
    if (m_activeLayerIndex != index) {
        m_activeLayerIndex = index;
        emit activeLayerChanged(index);
    }
}

void Document::setActiveLayer(LayerPtr layer) {
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        int index = std::distance(m_layers.begin(), it);
        setActiveLayer(index);
    }
}

void Document::showLayer(int index, bool show) {
    if (auto layer = getLayerAt(index)) {
        layer->setVisible(show);
        invalidateCache();
        emit layerChanged(index);
        updateModifiedDate();
    }
}

void Document::lockLayer(int index, bool lock) {
    if (auto layer = getLayerAt(index)) {
        layer->setLocked(lock);
        invalidateCache();
        emit layerChanged(index);
        updateModifiedDate();
    }
}

void Document::setLayerOpacity(int index, float opacity) {
    if (auto layer = getLayerAt(index)) {
        layer->setOpacity(opacity);
        invalidateCache();
        emit layerChanged(index);
        updateModifiedDate();
    }
}

void Document::setLayerBlendMode(int index, BlendMode mode) {
    if (auto layer = getLayerAt(index)) {
        layer->setBlendMode(mode);
        invalidateCache();
        emit layerChanged(index);
        updateModifiedDate();
    }
}

QImage Document::render(const QRect& viewport) {
    if (viewport.isNull()) {
        viewport = QRect(QPoint(0, 0), m_size);
    }
    
    QImage result(viewport.size(), QImage::Format_ARGB32_Premultiplied);
    result.fill(Qt::transparent);
    
    QPainter painter(&result);
    render(&painter, viewport);
    
    return result;
}

QImage Document::renderThumbnail(const QSize& size) {
    QImage fullImage = render();
    return fullImage.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void Document::render(QPainter* painter, const QRect& viewport) {
    if (!painter) return;
    
    QRect bounds = viewport.isNull() ? QRect(QPoint(0, 0), m_size) : viewport;
    
    // Render layers from bottom to top
    for (const auto& layer : m_layers) {
        if (layer && layer->isVisible()) {
            layer->render(painter, bounds);
        }
    }
}

void Document::rotate(double angle, const QPointF& center) {
    // Implementation for rotating the entire document
    invalidateCache();
    updateModifiedDate();
}

void Document::scale(double factor, const QPointF& center) {
    // Implementation for scaling the entire document
    invalidateCache();
    updateModifiedDate();
}

void Document::flipHorizontal() {
    // Implementation for horizontal flip
    invalidateCache();
    updateModifiedDate();
}

void Document::flipVertical() {
    // Implementation for vertical flip
    invalidateCache();
    updateModifiedDate();
}

void Document::setSelection(const QPainterPath& path) {
    m_selection = path;
    // Emit selection changed signal if needed
}

void Document::clearSelection() {
    m_selection = QPainterPath();
    // Emit selection changed signal if needed
}

QPainterPath Document::getSelection() const {
    return m_selection;
}

void Document::beginUndoGroup(const QString& name) {
    m_currentGroupName = name;
    m_currentGroup.clear();
}

void Document::endUndoGroup() {
    if (!m_currentGroup.empty()) {
        // Add grouped actions to undo stack
        m_undoStack.insert(m_undoStack.end(), 
                          std::make_move_iterator(m_currentGroup.begin()),
                          std::make_move_iterator(m_currentGroup.end()));
        m_currentGroup.clear();
        m_currentGroupName.clear();
        m_redoStack.clear();
        emit undoStackChanged();
    }
}

void Document::addUndoAction(std::unique_ptr<UndoAction> action) {
    if (action) {
        if (!m_currentGroupName.isEmpty()) {
            m_currentGroup.push_back(std::move(action));
        } else {
            m_undoStack.push_back(std::move(action));
            m_redoStack.clear();
            emit undoStackChanged();
        }
    }
}

bool Document::canUndo() const {
    return !m_undoStack.empty();
}

bool Document::canRedo() const {
    return !m_redoStack.empty();
}

void Document::undo() {
    if (!canUndo()) return;
    
    auto action = std::move(m_undoStack.back());
    m_undoStack.pop_back();
    
    action->undo(this);
    m_redoStack.push_back(std::move(action));
    
    invalidateCache();
    emit undoStackChanged();
    updateModifiedDate();
}

void Document::redo() {
    if (!canRedo()) return;
    
    auto action = std::move(m_redoStack.back());
    m_redoStack.pop_back();
    
    action->redo(this);
    m_undoStack.push_back(std::move(action));
    
    invalidateCache();
    emit undoStackChanged();
    updateModifiedDate();
}

bool Document::loadFromFile(const QString& filename) {
    // Implementation for loading document from file
    Q_UNUSED(filename)
    return false;
}

bool Document::saveToFile(const QString& filename) const {
    // Implementation for saving document to file
    Q_UNUSED(filename)
    return false;
}

bool Document::exportToFile(const QString& filename, const QSize& size) const {
    // Implementation for exporting document to file
    Q_UNUSED(filename)
    Q_UNUSED(size)
    return false;
}

void Document::setRenderQuality(RenderQuality quality) {
    if (m_renderQuality != quality) {
        m_renderQuality = quality;
        invalidateCache();
        emit renderQualityChanged(quality);
    }
}

Document::RenderQuality Document::getRenderQuality() const {
    return m_renderQuality;
}

void Document::enableGPUAcceleration(bool enable) {
    if (m_gpuAcceleration != enable) {
        m_gpuAcceleration = enable;
        invalidateCache();
    }
}

bool Document::isGPUAccelerationEnabled() const {
    return m_gpuAcceleration;
}

void Document::updateModifiedDate() {
    m_modifiedDate = QDateTime::currentDateTime();
    emit documentModified();
}

void Document::invalidateCache() {
    m_cacheValid = false;
    m_renderCache.clear();
}

void Document::optimizeLayerOrder() {
    // Implementation for optimizing layer rendering order
}

QImage Document::compositeLayers(const QRect& bounds) {
    // Implementation for compositing layers
    Q_UNUSED(bounds)
    return QImage();
}

// Undo action implementations
AddLayerAction::AddLayerAction(int index, LayerPtr layer)
    : m_index(index)
    , m_layer(layer)
{
}

void AddLayerAction::undo(Document* doc) {
    if (doc) {
        doc->removeLayer(m_index);
    }
}

void AddLayerAction::redo(Document* doc) {
    if (doc) {
        doc->addLayer(m_layer, m_index);
    }
}

QString AddLayerAction::getDescription() const {
    return QString("Add Layer '%1'").arg(m_layer ? m_layer->getName() : "Unknown");
}

RemoveLayerAction::RemoveLayerAction(int index, LayerPtr layer)
    : m_index(index)
    , m_layer(layer)
{
}

void RemoveLayerAction::undo(Document* doc) {
    if (doc) {
        doc->addLayer(m_layer, m_index);
    }
}

void RemoveLayerAction::redo(Document* doc) {
    if (doc) {
        doc->removeLayer(m_index);
    }
}

QString RemoveLayerAction::getDescription() const {
    return QString("Remove Layer '%1'").arg(m_layer ? m_layer->getName() : "Unknown");
}

} // namespace core
