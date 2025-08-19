#pragma once

#include <QObject>
#include <QString>
#include <QSize>
#include <QDateTime>
#include <QImage>
#include <QPainterPath>
#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <chrono>

namespace core {

// Forward declarations
class Layer;
class UndoAction;

// Type aliases
using LayerPtr = std::shared_ptr<Layer>;

// Enums
enum class ColorMode {
    RGB,
    RGBA,
    Grayscale,
    Indexed
};

enum class RenderQuality {
    Draft,
    Normal,
    High
};

/**
 * @brief The Document class represents an image document with multiple layers
 * 
 * This class provides a complete document management system with:
 * - Multiple layer support (raster, adjustment, text)
 * - Non-destructive editing
 * - Undo/redo system
 * - GPU acceleration
 * - Performance optimization
 */
class Document : public QObject
{
    Q_OBJECT

public:
    explicit Document(QObject* parent = nullptr);
    explicit Document(const QSize& size, ColorMode colorMode = ColorMode::RGBA, QObject* parent = nullptr);
    ~Document();

    // Document properties
    QString getName() const { return m_name; }
    void setName(const QString& name);
    
    QSize getSize() const { return m_size; }
    void setSize(const QSize& size);
    
    ColorMode getColorMode() const { return m_colorMode; }
    void setColorMode(ColorMode mode);
    
    QDateTime getCreatedDate() const { return m_createdDate; }
    QDateTime getModifiedDate() const { return m_modifiedDate; }

    // Layer management
    int getLayerCount() const { return static_cast<int>(m_layers.size()); }
    const std::vector<LayerPtr>& getLayers() const { return m_layers; }
    LayerPtr getLayerAt(int index) const;
    LayerPtr getActiveLayer() const;
    int getActiveLayerIndex() const { return m_activeLayerIndex; }
    
    void addLayer(LayerPtr layer, int index = -1);
    void removeLayer(int index);
    void moveLayer(int fromIndex, int toIndex);
    void duplicateLayer(int index);
    void mergeDown(int index);
    void mergeVisible();
    void flattenImage();
    void setActiveLayer(int index);
    
    // Layer operations
    void showLayer(int index, bool show);
    void lockLayer(int index, bool lock);
    void setLayerOpacity(int index, float opacity);
    void setLayerBlendMode(int index, const QString& blendMode);

    // Rendering
    QImage render(const QRect& bounds = QRect());
    QImage renderThumbnail(const QSize& size = QSize(128, 128));
    void render(QPainter* painter, const QRect& bounds = QRect());

    // Image transformations
    void rotate(double angle);
    void scale(const QSize& newSize);
    void flipHorizontal();
    void flipVertical();

    // Selection management
    void setSelection(const QPainterPath& selection);
    void clearSelection();
    QPainterPath getSelection() const;

    // Undo/redo system
    void beginUndoGroup(const QString& name = QString());
    void endUndoGroup();
    void addUndoAction(std::unique_ptr<UndoAction> action);
    bool canUndo() const { return !m_undoStack.empty(); }
    bool canRedo() const { return !m_redoStack.empty(); }
    void undo();
    void redo();
    
    // File I/O
    bool loadFromFile(const QString& filename);
    bool saveToFile(const QString& filename) const;
    bool exportToFile(const QString& filename, const QSize& size = QSize()) const;
    
    // Performance optimization
    void setRenderQuality(RenderQuality quality);
    RenderQuality getRenderQuality() const;
    void enableGPUAcceleration(bool enable);
    bool isGPUAccelerationEnabled() const;
    
    // Signals
signals:
    void layerAdded(int index);
    void layerRemoved(int index);
    void layerMoved(int fromIndex, int toIndex);
    void layerChanged(int index);
    void activeLayerChanged(int index);
    void documentSizeChanged(const QSize& newSize);
    void documentModified();
    void renderQualityChanged(RenderQuality quality);
    void undoStackChanged();

private:
    // Private implementation
    struct DocumentImpl;
    std::unique_ptr<DocumentImpl> d;
    
    // Internal methods
    void updateModifiedDate();
    void invalidateCache();
    void optimizeLayerOrder();
    QImage compositeLayers(const QRect& bounds);
    
    // Member variables
    QString m_name;
    QSize m_size;
    ColorMode m_colorMode;
    QDateTime m_createdDate;
    QDateTime m_modifiedDate;
    
    std::vector<LayerPtr> m_layers;
    int m_activeLayerIndex;
    
    // Rendering state
    RenderQuality m_renderQuality;
    bool m_gpuAcceleration;
    
    // Cache management
    mutable std::unordered_map<QString, QImage> m_renderCache;
    mutable std::mutex m_cacheMutex;
    std::atomic<bool> m_cacheValid;
    
    // Selection state
    QPainterPath m_selection;
    
    // Undo/redo system
    std::vector<std::unique_ptr<UndoAction>> m_undoStack;
    std::vector<std::unique_ptr<UndoAction>> m_redoStack;
    std::vector<std::unique_ptr<UndoAction>> m_currentGroup;
    QString m_currentGroupName;
    
    // Performance monitoring
    mutable std::chrono::steady_clock::time_point m_lastRenderTime;
    mutable double m_averageRenderTime;
    mutable int m_renderCount;
};

// Undo action base class
class UndoAction {
public:
    virtual ~UndoAction() = default;
    virtual void undo(Document* doc) = 0;
    virtual void redo(Document* doc) = 0;
    virtual QString getDescription() const = 0;
};

// Layer operation undo actions
class AddLayerAction : public UndoAction {
public:
    AddLayerAction(int index, LayerPtr layer);
    void undo(Document* doc) override;
    void redo(Document* doc) override;
    QString getDescription() const override;
private:
    int m_index;
    LayerPtr m_layer;
};

class RemoveLayerAction : public UndoAction {
public:
    RemoveLayerAction(int index, LayerPtr layer);
    void undo(Document* doc) override;
    void redo(Document* doc) override;
    QString getDescription() const override;
private:
    int m_index;
    LayerPtr m_layer;
};

} // namespace core
