#pragma once

#include <QObject>
#include <QString>
#include <QPoint>
#include <QRect>
#include <QColor>
#include <QImage>
#include <QVariant>
#include <memory>

namespace core {

class Document;

/**
 * @brief Base interface for all commands in the application
 * 
 * Commands implement the Command pattern for undo/redo functionality.
 * Every editing operation should be wrapped in a command object.
 */
class ICommand {
public:
    virtual ~ICommand() = default;
    
    /**
     * @brief Execute the command
     * @param document The document to operate on
     * @return true if successful, false otherwise
     */
    virtual bool execute(Document* document) = 0;
    
    /**
     * @brief Undo the command
     * @param document The document to operate on
     * @return true if successful, false otherwise
     */
    virtual bool undo(Document* document) = 0;
    
    /**
     * @brief Get a human-readable description of the command
     * @return Command description
     */
    virtual QString description() const = 0;
    
    /**
     * @brief Check if this command can be merged with another
     * @param other The command to potentially merge with
     * @return true if commands can be merged
     */
    virtual bool canMergeWith(const ICommand* other) const = 0;
    
    /**
     * @brief Merge this command with another
     * @param other The command to merge with
     * @return true if merge was successful
     */
    virtual bool mergeWith(const ICommand* other) = 0;
};

/**
 * @brief Command for drawing a brush stroke
 */
class BrushStrokeCommand : public ICommand {
public:
    BrushStrokeCommand(const QPoint& start, const QPoint& end, 
                      const QColor& color, int size, int layerIndex);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    QPoint m_start;
    QPoint m_end;
    QColor m_color;
    int m_size;
    int m_layerIndex;
    
    // Store the original layer content for undo
    QImage m_originalLayerContent;
    QRect m_affectedRegion;
};

/**
 * @brief Command for drawing a line
 */
class DrawLineCommand : public ICommand {
public:
    DrawLineCommand(const QPoint& start, const QPoint& end, 
                   const QColor& color, int size, int layerIndex);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    QPoint m_start;
    QPoint m_end;
    QColor m_color;
    int m_size;
    int m_layerIndex;
    
    QImage m_originalLayerContent;
    QRect m_affectedRegion;
};

/**
 * @brief Command for drawing a rectangle
 */
class DrawRectangleCommand : public ICommand {
public:
    DrawRectangleCommand(const QRect& rect, const QColor& color, 
                       int size, double rotation, int layerIndex);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    QRect m_rect;
    QColor m_color;
    int m_size;
    double m_rotation;
    int m_layerIndex;
    
    QImage m_originalLayerContent;
    QRect m_affectedRegion;
};

/**
 * @brief Command for drawing an ellipse
 */
class DrawEllipseCommand : public ICommand {
public:
    DrawEllipseCommand(const QRect& rect, const QColor& color, 
                      int size, double rotation, int layerIndex);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    QRect m_rect;
    QColor m_color;
    int m_size;
    double m_rotation;
    int m_layerIndex;
    
    QImage m_originalLayerContent;
    QRect m_affectedRegion;
};

/**
 * @brief Command for erasing content
 */
class EraseCommand : public ICommand {
public:
    EraseCommand(const QPoint& start, const QPoint& end, 
                int size, int layerIndex);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    QPoint m_start;
    QPoint m_end;
    int m_size;
    int m_layerIndex;
    
    QImage m_originalLayerContent;
    QRect m_affectedRegion;
};

/**
 * @brief Command for adding a layer
 */
class AddLayerCommand : public ICommand {
public:
    AddLayerCommand(const QString& name, int index, const QSize& size);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    QString m_name;
    int m_index;
    QSize m_size;
    int m_actualIndex; // Store where the layer was actually added
};

/**
 * @brief Command for removing a layer
 */
class RemoveLayerCommand : public ICommand {
public:
    RemoveLayerCommand(int index);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    int m_index;
    std::shared_ptr<class ILayer> m_removedLayer;
    int m_originalIndex;
};

/**
 * @brief Command for moving a layer
 */
class MoveLayerCommand : public ICommand {
public:
    MoveLayerCommand(int fromIndex, int toIndex);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    int m_fromIndex;
    int m_toIndex;
    int m_originalFromIndex;
    int m_originalToIndex;
};

/**
 * @brief Command for changing layer properties
 */
class LayerPropertyCommand : public ICommand {
public:
    enum PropertyType {
        Visibility,
        Opacity,
        BlendMode,
        Name
    };
    
    LayerPropertyCommand(int layerIndex, PropertyType property, 
                       const QVariant& newValue, const QVariant& oldValue);
    
    bool execute(Document* document) override;
    bool undo(Document* document) override;
    QString description() const override;
    bool canMergeWith(const ICommand* other) const override;
    bool mergeWith(const ICommand* other) override;

private:
    int m_layerIndex;
    PropertyType m_property;
    QVariant m_newValue;
    QVariant m_oldValue;
};

/**
 * @brief Command manager that handles the undo/redo stack
 */
class CommandManager : public QObject {
    Q_OBJECT

public:
    explicit CommandManager(QObject* parent = nullptr);
    ~CommandManager();

    /**
     * @brief Execute a command and add it to the undo stack
     * @param command The command to execute
     * @param document The document to execute the command on
     * @return true if successful
     */
    bool executeCommand(std::unique_ptr<ICommand> command, Document* document);
    
    /**
     * @brief Undo the last command
     * @param document The document to undo the command on
     * @return true if successful
     */
    bool undo(Document* document);
    
    /**
     * @brief Redo the last undone command
     * @param document The document to redo the command on
     * @return true if successful
     */
    bool redo(Document* document);
    
    /**
     * @brief Check if undo is available
     * @return true if undo is possible
     */
    bool canUndo() const;
    
    /**
     * @brief Check if redo is available
     * @return true if redo is possible
     */
    bool canRedo() const;
    
    /**
     * @brief Clear the undo/redo history
     */
    void clear();
    
    /**
     * @brief Begin a macro command (multiple commands as one)
     * @param description Description of the macro
     */
    void beginMacro(const QString& description);
    
    /**
     * @brief End the current macro
     */
    void endMacro();
    
    /**
     * @brief Check if a macro is currently active
     * @return true if macro is active
     */
    bool isMacroActive() const;

signals:
    void commandExecuted(ICommand* command);
    void commandUndone(ICommand* command);
    void commandRedone(ICommand* command);
    void undoStackChanged();

private:
    struct CommandEntry {
        std::unique_ptr<ICommand> command;
        QString description;
        bool isMacro;
    };
    
    std::vector<CommandEntry> m_undoStack;
    std::vector<CommandEntry> m_redoStack;
    std::vector<CommandEntry> m_currentMacro;
    bool m_macroActive;
    
    void addToUndoStack(std::unique_ptr<ICommand> command, const QString& description = QString());
    void clearRedoStack();
    void executeMacro();
};

} // namespace core
