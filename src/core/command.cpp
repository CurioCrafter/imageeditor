#include "command.h"
#include "document.h"
#include "layer.h"
#include <QPainter>
#include <QDebug>

namespace core {

// CommandManager implementation
CommandManager::CommandManager(QObject* parent)
    : QObject(parent)
    , m_macroActive(false)
{
}

CommandManager::~CommandManager() = default;

bool CommandManager::executeCommand(std::unique_ptr<ICommand> command, Document* document)
{
    if (!command || !document) return false;
    
    // Execute the command and store it for undo
    if (command->execute(document)) {
        m_undoStack.push_back({std::move(command), command->description(), false});
        return true;
    }
    return false;
}

bool CommandManager::undo(Document* document)
{
    if (m_undoStack.empty() || !document) return false;
    
    auto& entry = m_undoStack.back();
    if (entry.command->undo(document)) {
        // Move to redo stack
        m_redoStack.push_back({std::move(entry.command), entry.description, entry.isMacro});
        m_undoStack.pop_back();
        return true;
    }
    return false;
}

bool CommandManager::redo(Document* document)
{
    if (m_redoStack.empty() || !document) return false;
    
    auto& entry = m_redoStack.back();
    if (entry.command->execute(document)) {
        // Move back to undo stack
        m_undoStack.push_back({std::move(entry.command), entry.description, entry.isMacro});
        m_redoStack.pop_back();
        return true;
    }
    return false;
}

bool CommandManager::canUndo() const
{
    return !m_undoStack.empty();
}

bool CommandManager::canRedo() const
{
    return !m_redoStack.empty();
}

void CommandManager::clear()
{
    m_undoStack.clear();
    m_redoStack.clear();
}

void CommandManager::beginMacro(const QString& description)
{
    Q_UNUSED(description)
    m_macroActive = true;
    // TODO: Implement macro support
}

void CommandManager::endMacro()
{
    m_macroActive = false;
    // TODO: Implement macro support
}

bool CommandManager::isMacroActive() const
{
    return m_macroActive;
}

// BrushStrokeCommand implementation
BrushStrokeCommand::BrushStrokeCommand(const QPoint& start, const QPoint& end, 
                                     const QColor& color, int size, int layerIndex)
    : m_start(start)
    , m_end(end)
    , m_color(color)
    , m_size(size)
    , m_layerIndex(layerIndex)
{
}

bool BrushStrokeCommand::execute(Document* document)
{
    if (!document) {
        qDebug() << "BrushStrokeCommand: Document is null";
        return false;
    }
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) {
        qDebug() << "BrushStrokeCommand: Layer is null at index" << m_layerIndex;
        return false;
    }
    
    // Store original content for undo
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        try {
            m_originalLayerContent = rasterLayer->image();
            
            // Check if image is valid
            if (rasterLayer->image().isNull()) {
                qDebug() << "BrushStrokeCommand: Layer image is null";
                return false;
            }
            
            qDebug() << "BrushStrokeCommand: Drawing on layer" << m_layerIndex << "image size:" << rasterLayer->image().size();
            
            // Draw to the layer
            QPainter painter(&rasterLayer->image());
            if (!painter.isActive()) {
                qDebug() << "BrushStrokeCommand: Failed to create painter";
                return false;
            }
            
            painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(m_start, m_end);
            painter.end();
            
            qDebug() << "BrushStrokeCommand: Successfully drew line from" << m_start << "to" << m_end;
            return true;
        } catch (const std::exception& e) {
            qDebug() << "BrushStrokeCommand: Exception during drawing:" << e.what();
            return false;
        }
    } else {
        qDebug() << "BrushStrokeCommand: Layer is not a RasterLayer";
    }
    return false;
}

bool BrushStrokeCommand::undo(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        rasterLayer->setImage(m_originalLayerContent);
        return true;
    }
    return false;
}

QString BrushStrokeCommand::description() const
{
    return "Brush Stroke";
}

bool BrushStrokeCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool BrushStrokeCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

// DrawLineCommand
DrawLineCommand::DrawLineCommand(const QPoint& start, const QPoint& end, 
                               const QColor& color, int size, int layerIndex)
    : m_start(start)
    , m_end(end)
    , m_color(color)
    , m_size(size)
    , m_layerIndex(layerIndex)
{
}

bool DrawLineCommand::execute(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        m_originalLayerContent = rasterLayer->image();
        
        QPainter painter(&rasterLayer->image());
        painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(m_start, m_end);
        painter.end();
        
        return true;
    }
    return false;
}

bool DrawLineCommand::undo(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        rasterLayer->setImage(m_originalLayerContent);
        return true;
    }
    return false;
}

QString DrawLineCommand::description() const
{
    return "Draw Line";
}

bool DrawLineCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool DrawLineCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

// DrawRectangleCommand
DrawRectangleCommand::DrawRectangleCommand(const QRect& rect, const QColor& color, 
                                         int size, double rotation, int layerIndex)
    : m_rect(rect)
    , m_color(color)
    , m_size(size)
    , m_rotation(rotation)
    , m_layerIndex(layerIndex)
{
}

bool DrawRectangleCommand::execute(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        m_originalLayerContent = rasterLayer->image();
        
        QPainter painter(&rasterLayer->image());
        painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap));
        painter.setBrush(Qt::NoBrush);
        
        if (m_rotation != 0.0) {
            QPoint center = m_rect.center();
            painter.translate(center);
            painter.rotate(m_rotation);
            painter.translate(-center);
        }
        
        painter.drawRect(m_rect);
        painter.end();
        
        return true;
    }
    return false;
}

bool DrawRectangleCommand::undo(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        rasterLayer->setImage(m_originalLayerContent);
        return true;
    }
    return false;
}

QString DrawRectangleCommand::description() const
{
    return "Draw Rectangle";
}

bool DrawRectangleCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool DrawRectangleCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

// DrawEllipseCommand
DrawEllipseCommand::DrawEllipseCommand(const QRect& rect, const QColor& color, 
                                     int size, double rotation, int layerIndex)
    : m_rect(rect)
    , m_color(color)
    , m_size(size)
    , m_rotation(rotation)
    , m_layerIndex(layerIndex)
{
}

bool DrawEllipseCommand::execute(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        m_originalLayerContent = rasterLayer->image();
        
        QPainter painter(&rasterLayer->image());
        painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap));
        painter.setBrush(Qt::NoBrush);
        
        if (m_rotation != 0.0) {
            QPoint center = m_rect.center();
            painter.translate(center);
            painter.rotate(m_rotation);
            painter.translate(-center);
        }
        
        painter.drawEllipse(m_rect);
        painter.end();
        
        return true;
    }
    return false;
}

bool DrawEllipseCommand::undo(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        rasterLayer->setImage(m_originalLayerContent);
        return true;
    }
    return false;
}

QString DrawEllipseCommand::description() const
{
    return "Draw Ellipse";
}

bool DrawEllipseCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool DrawEllipseCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

// EraseCommand implementation
EraseCommand::EraseCommand(const QPoint& start, const QPoint& end, int size, int layerIndex)
    : m_start(start)
    , m_end(end)
    , m_size(size)
    , m_layerIndex(layerIndex)
{
}

bool EraseCommand::execute(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        m_originalLayerContent = rasterLayer->image();
        
        QPainter painter(&rasterLayer->image());
        painter.setPen(QPen(Qt::white, m_size, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(m_start, m_end);
        painter.end();
        
        return true;
    }
    return false;
}

bool EraseCommand::undo(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    if (auto rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer)) {
        rasterLayer->setImage(m_originalLayerContent);
        return true;
    }
    return false;
}

QString EraseCommand::description() const
{
    return "Erase";
}

bool EraseCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool EraseCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

// AddLayerCommand implementation
AddLayerCommand::AddLayerCommand(const QString& name, int index, const QSize& size)
    : m_name(name)
    , m_index(index)
    , m_size(size)
    , m_actualIndex(index)
{
}

bool AddLayerCommand::execute(Document* document)
{
    if (!document) return false;
    auto newLayer = std::make_shared<RasterLayer>(m_name, m_size);
    document->addLayer(newLayer, m_index);
    return true;
}

bool AddLayerCommand::undo(Document* document)
{
    if (!document) return false;
    document->removeLayer(m_index);
    return true;
}

QString AddLayerCommand::description() const
{
    return QString("Add Layer: %1").arg(m_name);
}

bool AddLayerCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool AddLayerCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

// RemoveLayerCommand implementation
RemoveLayerCommand::RemoveLayerCommand(int index)
    : m_index(index)
{
}

bool RemoveLayerCommand::execute(Document* document)
{
    if (!document) return false;
    m_removedLayer = document->getLayer(m_index);
    m_originalIndex = m_index;
    document->removeLayer(m_index);
    return true;
}

bool RemoveLayerCommand::undo(Document* document)
{
    if (!document) return false;
    if (m_removedLayer) {
        document->addLayer(m_removedLayer, m_originalIndex);
    }
    return true;
}

QString RemoveLayerCommand::description() const
{
    return "Remove Layer";
}

bool RemoveLayerCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool RemoveLayerCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

// MoveLayerCommand implementation
MoveLayerCommand::MoveLayerCommand(int fromIndex, int toIndex)
    : m_fromIndex(fromIndex)
    , m_toIndex(toIndex)
{
}

bool MoveLayerCommand::execute(Document* document)
{
    if (!document) return false;
    m_originalFromIndex = m_fromIndex;
    m_originalToIndex = m_toIndex;
    document->moveLayer(m_fromIndex, m_toIndex);
    return true;
}

bool MoveLayerCommand::undo(Document* document)
{
    if (!document) return false;
    document->moveLayer(m_toIndex, m_originalFromIndex);
    return true;
}

QString MoveLayerCommand::description() const
{
    return "Move Layer";
}

bool MoveLayerCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool MoveLayerCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

// LayerPropertyCommand implementation
LayerPropertyCommand::LayerPropertyCommand(int layerIndex, PropertyType property, 
                                         const QVariant& newValue, const QVariant& oldValue)
    : m_layerIndex(layerIndex)
    , m_property(property)
    , m_newValue(newValue)
    , m_oldValue(oldValue)
{
}

bool LayerPropertyCommand::execute(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    switch (m_property) {
        case Visibility:
            layer->setVisible(m_newValue.toBool());
            break;
        case Opacity:
            layer->setOpacity(m_newValue.toReal());
            break;
        case Name:
            layer->setName(m_newValue.toString());
            break;
        default:
            return false;
    }
    return true;
}

bool LayerPropertyCommand::undo(Document* document)
{
    if (!document) return false;
    
    auto layer = document->getLayer(m_layerIndex);
    if (!layer) return false;
    
    switch (m_property) {
        case Visibility:
            layer->setVisible(m_oldValue.toBool());
            break;
        case Opacity:
            layer->setOpacity(m_oldValue.toReal());
            break;
        case Name:
            layer->setName(m_oldValue.toString());
            break;
        default:
            return false;
    }
    return true;
}

QString LayerPropertyCommand::description() const
{
    return "Change Layer Property";
}

bool LayerPropertyCommand::canMergeWith(const ICommand* other) const
{
    Q_UNUSED(other)
    return false;
}

bool LayerPropertyCommand::mergeWith(const ICommand* other)
{
    Q_UNUSED(other)
    return false;
}

} // namespace core