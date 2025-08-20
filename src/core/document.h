#pragma once

#include <QObject>
#include <QString>
#include <QSize>
#include <QRect>
#include <QImage>
#include <QDateTime>
#include <QPainter>
#include <memory>
#include <vector>

namespace core {

// Forward declarations
class Layer;
class RasterLayer;
class AdjustmentLayer;
class TextLayer;

// Type definitions
using LayerPtr = std::shared_ptr<Layer>;

// Color modes
enum class ColorMode {
    RGBA8,      // 8-bit per channel RGBA
    RGBA16,     // 16-bit per channel RGBA
    RGBA32F,    // 32-bit float per channel RGBA
    CMYK8,      // 8-bit per channel CMYK
    Lab,        // LAB color space
    Grayscale   // Grayscale
};

// Blend modes (defined in layer.h)
enum class BlendMode;

/**
 * @brief Document class represents an image document with layers
 */
class Document : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new Document with specified dimensions
     */
    explicit Document(int width, int height, QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~Document();

    // === Properties ===
    
    /**
     * @brief Get document name
     */
    QString getName() const { return m_name; }
    
    /**
     * @brief Set document name
     */
    void setName(const QString& name) { m_name = name; }
    
    /**
     * @brief Get document size
     */
    QSize size() const { return QSize(m_width, m_height); }
    
    /**
     * @brief Get document size (alias for size())
     */
    QSize getSize() const { return QSize(m_width, m_height); }
    
    /**
     * @brief Get document width
     */
    int width() const { return m_width; }
    
    /**
     * @brief Get document height
     */
    int height() const { return m_height; }
    
    /**
     * @brief Get color mode
     */
    ColorMode getColorMode() const { return m_colorMode; }
    
    /**
     * @brief Set color mode
     */
    void setColorMode(ColorMode mode) { m_colorMode = mode; }
    
    // === Layer Management ===
    
    /**
     * @brief Add a layer to the document
     * @param layer The layer to add
     * @param index Position to insert (-1 for top)
     */
    void addLayer(LayerPtr layer, int index = -1);
    
    /**
     * @brief Remove a layer by index
     */
    void removeLayer(int index);
    
    /**
     * @brief Move a layer from one position to another
     */
    void moveLayer(int from, int to);
    
    /**
     * @brief Get all layers
     */
    const std::vector<LayerPtr>& getLayers() const { return m_layers; }
    
    /**
     * @brief Get layer at specific index
     */
    LayerPtr getLayerAt(int index) const;
    
    /**
     * @brief Get layer by name
     */
    LayerPtr getLayerByName(const QString& name) const;
    
    /**
     * @brief Get active layer
     */
    LayerPtr getActiveLayer() const { return m_activeLayer; }
    
    /**
     * @brief Set active layer by index
     */
    void setActiveLayer(int index);
    
    /**
     * @brief Get layer count
     */
    int getLayerCount() const { return static_cast<int>(m_layers.size()); }
    
    // === Rendering ===
    
    /**
     * @brief Render the document to an image
     * @param viewport Optional viewport to render (null = entire document)
     */
    QImage render(const QRect& viewport = QRect()) const;
    
    /**
     * @brief Render the document to a painter
     */
    void render(QPainter* painter, const QRect& viewport = QRect()) const;
    
    // === File Operations ===
    
    /**
     * @brief Load document from file
     */
    bool loadFromFile(const QString& filename);
    
    /**
     * @brief Save document to file
     */
    bool saveToFile(const QString& filename) const;
    
    /**
     * @brief Export document as image
     */
    bool exportImage(const QString& filename, const QString& format = "PNG") const;
    
    /**
     * @brief Get filename
     */
    QString getFilename() const { return m_filename; }
    
    // === History ===
    
    /**
     * @brief Check if can undo
     */
    bool canUndo() const { return false; } // TODO: Implement
    
    /**
     * @brief Check if can redo
     */
    bool canRedo() const { return false; } // TODO: Implement
    
    /**
     * @brief Undo last operation
     */
    void undo();
    
    /**
     * @brief Redo last undone operation
     */
    void redo();
    
    // === Modifications ===
    
    /**
     * @brief Check if document is modified
     */
    bool isModified() const { return m_modified; }
    
    /**
     * @brief Get creation date
     */
    QDateTime getCreatedDate() const { return m_createdDate; }
    
    /**
     * @brief Get modification date
     */
    QDateTime getModifiedDate() const { return m_modifiedDate; }
    
    // === Document Operations ===
    
    /**
     * @brief Resize the document
     */
    void resize(int width, int height);
    
    /**
     * @brief Crop the document
     */
    void crop(const QRect& rect);
    
signals:
    /**
     * @brief Emitted when a layer is added
     */
    void layerAdded(int index);
    
    /**
     * @brief Emitted when a layer is removed
     */
    void layerRemoved(int index);
    
    /**
     * @brief Emitted when a layer is moved
     */
    void layerMoved(int from, int to);
    
    /**
     * @brief Emitted when a layer changes
     */
    void layerChanged(int index);
    
    /**
     * @brief Emitted when active layer changes
     */
    void activeLayerChanged(int index);
    
    /**
     * @brief Emitted when document size changes
     */
    void sizeChanged(const QSize& size);
    
    /**
     * @brief Emitted when document size changes (alias)
     */
    void documentSizeChanged(const QSize& size);
    
    /**
     * @brief Emitted when document is modified
     */
    void modifiedChanged(bool modified);
    
    /**
     * @brief General document change signal
     */
    void documentChanged();

protected:
    /**
     * @brief Invalidate render cache
     */
    void invalidateCache();
    
    /**
     * @brief Update modification date
     */
    void updateModifiedDate();

private:
    /**
     * @brief Convert blend mode to QPainter composition mode
     */
    QPainter::CompositionMode blendModeToQPainter(BlendMode mode) const;

private:
    // Document properties
    QString m_name;
    QString m_filename;
    int m_width;
    int m_height;
    ColorMode m_colorMode;
    
    // Layers
    std::vector<LayerPtr> m_layers;
    LayerPtr m_activeLayer;
    
    // State
    bool m_modified;
    QDateTime m_createdDate;
    QDateTime m_modifiedDate;
    
    // Cache (for performance)
    mutable QImage m_cachedRender;
    mutable bool m_cacheValid = false;
};

} // namespace core