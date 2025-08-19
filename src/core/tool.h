#pragma once

#include <QObject>
#include <QPointF>
#include <QPainter>
#include <QPainterPath>
#include <QCursor>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QString>
#include <QSize>
#include <QImage>
#include <QWidget>

namespace core {

// Tool event structure
struct ToolEvent {
    QPointF pos;
    float pressure = 1.0f;
    Qt::MouseButtons buttons = Qt::NoButton;
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;
    QPointF delta;
    QPointF velocity;
};

// Base Tool class for all image editing tools
class Tool : public QObject {
    Q_OBJECT
public:
    explicit Tool(const QString& name = "Tool", QObject* parent = nullptr);
    virtual ~Tool();

    // Tool identification
    QString getName() const { return m_name; }
    QString getDescription() const { return m_description; }
    QString getIconPath() const { return m_iconPath; }
    
    // Tool state
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }
    
    // Tool activation/deactivation
    virtual void activate(QWidget* canvas);
    virtual void deactivate();
    
    // Event handling
    virtual void mousePressEvent(const ToolEvent& event);
    virtual void mouseMoveEvent(const ToolEvent& event);
    virtual void mouseReleaseEvent(const ToolEvent& event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    
    // Drawing
    virtual void draw(QPainter& painter, const QPointF& pos);
    virtual void drawPreview(QPainter& painter, const QRectF& bounds);
    
    // Cursor management
    virtual QCursor cursor() const;
    virtual QCursor cursor(const QPointF& pos) const;
    
    // Tool options
    virtual QWidget* createOptionsWidget();
    virtual void updateOptions();
    
    // Tool presets
    virtual void savePreset(const QString& name);
    virtual void loadPreset(const QString& name);
    virtual QStringList getPresetNames() const;

protected:
    // Protected members for derived classes
    QString m_name;
    QString m_description;
    QString m_iconPath;
    bool m_active;
    QWidget* m_canvas;
    
    // Internal methods
    virtual void onActivated();
    virtual void onDeactivated();
    void updateCursor();

signals:
    void activated();
    void deactivated();
    void optionsChanged();
    void cursorChanged();
};

// Brush tool implementation
class BrushTool : public Tool {
    Q_OBJECT
public:
    BrushTool(QObject* parent = nullptr);
    
    // Brush properties
    void setSize(float size);
    float getSize() const { return m_size; }
    
    void setOpacity(float opacity);
    float getOpacity() const { return m_opacity; }
    
    void setFlow(float flow);
    float getFlow() const { return m_flow; }
    
    void setHardness(float hardness);
    float getHardness() const { return m_hardness; }
    
    void setSpacing(float spacing);
    float getSpacing() const { return m_spacing; }
    
    void setColor(const QColor& color);
    QColor getColor() const { return m_color; }
    
    // Tool overrides
    void mousePressEvent(const ToolEvent& event) override;
    void mouseMoveEvent(const ToolEvent& event) override;
    void mouseReleaseEvent(const ToolEvent& event) override;
    void draw(QPainter& painter, const QPointF& pos) override;
    
    // Brush dynamics
    void setPressureSensitivity(bool enabled);
    void setSizePressure(bool enabled);
    void setOpacityPressure(bool enabled);
    void setFlowPressure(bool enabled);

private:
    // Brush properties
    float m_size;
    float m_opacity;
    float m_flow;
    float m_hardness;
    float m_spacing;
    QColor m_color;
    
    // Brush state
    bool m_isDrawing;
    QPointF m_lastPos;
    std::vector<QPointF> m_strokePoints;
    
    // Dynamics
    bool m_pressureSensitivity;
    bool m_sizePressure;
    bool m_opacityPressure;
    bool m_flowPressure;
    
    // Internal methods
    void beginStroke(const QPointF& pos);
    void continueStroke(const QPointF& pos);
    void endStroke();
    void drawBrushStroke(QPainter& painter, const QPointF& from, const QPointF& to);
    QPointF interpolateStroke(const QPointF& p0, const QPointF& p1, const QPointF& p2, const QPointF& p3);
};

// Selection tool implementation
class SelectionTool : public Tool {
    Q_OBJECT
public:
    enum class SelectionType {
        Rectangular,
        Elliptical,
        Lasso,
        MagicWand,
        QuickSelect
    };
    
    SelectionTool(SelectionType type = SelectionType::Rectangular, QObject* parent = nullptr);
    
    // Selection properties
    void setSelectionType(SelectionType type);
    SelectionType getSelectionType() const { return m_selectionType; }
    
    void setFeather(float feather);
    float getFeather() const { return m_feather; }
    
    void setAntiAlias(bool antiAlias);
    bool getAntiAlias() const { return m_antiAlias; }
    
    // Tool overrides
    void mousePressEvent(const ToolEvent& event) override;
    void mouseMoveEvent(const ToolEvent& event) override;
    void mouseReleaseEvent(const ToolEvent& event) override;
    void draw(QPainter& painter, const QPointF& pos) override;
    void drawPreview(QPainter& painter, const QRectF& bounds) override;

private:
    SelectionType m_selectionType;
    float m_feather;
    bool m_antiAlias;
    
    // Selection state
    bool m_isSelecting;
    QPointF m_startPos;
    QPointF m_currentPos;
    QPainterPath m_selectionPath;
    
    // Internal methods
    void updateSelection();
    void finalizeSelection();
};

// Move tool implementation
class MoveTool : public Tool {
    Q_OBJECT
public:
    MoveTool(QObject* parent = nullptr);
    
    // Tool overrides
    void mousePressEvent(const ToolEvent& event) override;
    void mouseMoveEvent(const ToolEvent& event) override;
    void mouseReleaseEvent(const ToolEvent& event) override;
    QCursor cursor() const override;

private:
    bool m_isMoving;
    QPointF m_startPos;
    QPointF m_lastPos;
    QPointF m_offset;
    
    // Internal methods
    void startMove(const QPointF& pos);
    void updateMove(const QPointF& pos);
    void endMove();
};

} // namespace core
