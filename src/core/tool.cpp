#include "tool.h"
#include <QWidget>
#include <QApplication>

namespace core {

// Tool base class implementation
Tool::Tool(const QString& name, QObject* parent)
    : QObject(parent)
    , m_name(name)
    , m_description("A tool for image editing")
    , m_iconPath("")
    , m_active(false)
    , m_canvas(nullptr)
{
}

Tool::~Tool() = default;

void Tool::activate(QWidget* canvas) {
    if (m_active) return;
    
    m_canvas = canvas;
    m_active = true;
    onActivated();
    emit activated();
}

void Tool::deactivate() {
    if (!m_active) return;
    
    onDeactivated();
    m_active = false;
    m_canvas = nullptr;
    emit deactivated();
}

void Tool::mousePressEvent(const ToolEvent& event) {
    // Default implementation does nothing
    Q_UNUSED(event)
}

void Tool::mouseMoveEvent(const ToolEvent& event) {
    // Default implementation does nothing
    Q_UNUSED(event)
}

void Tool::mouseReleaseEvent(const ToolEvent& event) {
    // Default implementation does nothing
    Q_UNUSED(event)
}

void Tool::keyPressEvent(QKeyEvent* event) {
    // Default implementation does nothing
    Q_UNUSED(event)
}

void Tool::keyReleaseEvent(QKeyEvent* event) {
    // Default implementation does nothing
    Q_UNUSED(event)
}

void Tool::draw(QPainter& painter, const QPointF& pos) {
    // Default implementation does nothing
    Q_UNUSED(painter)
    Q_UNUSED(pos)
}

void Tool::drawPreview(QPainter& painter, const QRectF& bounds) {
    // Default implementation does nothing
    Q_UNUSED(painter)
    Q_UNUSED(bounds)
}

QCursor Tool::cursor() const {
    return Qt::ArrowCursor;
}

QCursor Tool::cursor(const QPointF& pos) const {
    // Default implementation returns the same cursor for all positions
    Q_UNUSED(pos)
    return cursor();
}

QWidget* Tool::createOptionsWidget() {
    // Default implementation returns nullptr
    return nullptr;
}

void Tool::updateOptions() {
    // Default implementation does nothing
}

void Tool::savePreset(const QString& name) {
    // Default implementation does nothing
    Q_UNUSED(name)
}

void Tool::loadPreset(const QString& name) {
    // Default implementation does nothing
    Q_UNUSED(name)
}

QStringList Tool::getPresetNames() const {
    // Default implementation returns empty list
    return QStringList();
}

void Tool::onActivated() {
    // Default implementation does nothing
}

void Tool::onDeactivated() {
    // Default implementation does nothing
}

void Tool::updateCursor() {
    if (m_canvas) {
        m_canvas->setCursor(cursor());
        emit cursorChanged();
    }
}

// Brush tool implementation
BrushTool::BrushTool(QObject* parent)
    : Tool("Brush", parent)
    , m_size(10.0f)
    , m_opacity(1.0f)
    , m_flow(1.0f)
    , m_hardness(0.5f)
    , m_spacing(0.25f)
    , m_color(Qt::black)
    , m_isDrawing(false)
    , m_lastPos(0, 0)
    , m_pressureSensitivity(true)
    , m_sizePressure(true)
    , m_opacityPressure(true)
    , m_flowPressure(false)
{
    m_description = "Paint with customizable brush strokes";
}

void BrushTool::setSize(float size) {
    m_size = std::max(1.0f, size);
    updateOptions();
}

void BrushTool::setOpacity(float opacity) {
    m_opacity = std::clamp(opacity, 0.0f, 1.0f);
    updateOptions();
}

void BrushTool::setFlow(float flow) {
    m_flow = std::clamp(flow, 0.0f, 1.0f);
    updateOptions();
}

void BrushTool::setHardness(float hardness) {
    m_hardness = std::clamp(hardness, 0.0f, 1.0f);
    updateOptions();
}

void BrushTool::setSpacing(float spacing) {
    m_spacing = std::clamp(spacing, 0.1f, 2.0f);
    updateOptions();
}

void BrushTool::setColor(const QColor& color) {
    m_color = color;
    updateOptions();
}

void BrushTool::setPressureSensitivity(bool enabled) {
    m_pressureSensitivity = enabled;
    updateOptions();
}

void BrushTool::setSizePressure(bool enabled) {
    m_sizePressure = enabled;
    updateOptions();
}

void BrushTool::setOpacityPressure(bool enabled) {
    m_opacityPressure = enabled;
    updateOptions();
}

void BrushTool::setFlowPressure(bool enabled) {
    m_flowPressure = enabled;
    updateOptions();
}

void BrushTool::mousePressEvent(const ToolEvent& event) {
    beginStroke(event.pos);
}

void BrushTool::mouseMoveEvent(const ToolEvent& event) {
    continueStroke(event.pos);
}

void BrushTool::mouseReleaseEvent(const ToolEvent& event) {
    endStroke();
}

void BrushTool::draw(QPainter& painter, const QPointF& pos) {
    if (m_isDrawing) {
        drawBrushStroke(painter, m_lastPos, pos);
    }
}

void BrushTool::beginStroke(const QPointF& pos) {
    m_isDrawing = true;
    m_lastPos = pos;
    m_strokePoints.clear();
    m_strokePoints.push_back(pos);
}

void BrushTool::continueStroke(const QPointF& pos) {
    if (!m_isDrawing) return;
    
    // Add point if it's far enough from the last one
    float distance = QLineF(m_lastPos, pos).length();
    if (distance >= m_size * m_spacing) {
        m_strokePoints.push_back(pos);
        m_lastPos = pos;
    }
}

void BrushTool::endStroke() {
    m_isDrawing = false;
    m_strokePoints.clear();
}

void BrushTool::drawBrushStroke(QPainter& painter, const QPointF& from, const QPointF& to) {
    painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setOpacity(m_opacity);
    painter.drawLine(from, to);
}

QPointF BrushTool::interpolateStroke(const QPointF& p0, const QPointF& p1, const QPointF& p2, const QPointF& p3) {
    // Catmull-Rom spline interpolation for smooth strokes
    float t = 0.5f;
    float t2 = t * t;
    float t3 = t2 * t;
    
    return 0.5f * (
        (-t3 + 2*t2 - t) * p0 +
        (3*t3 - 5*t2 + 2) * p1 +
        (-3*t3 + 4*t2 + t) * p2 +
        (t3 - t2) * p3
    );
}

// Selection tool implementation
SelectionTool::SelectionTool(SelectionType type, QObject* parent)
    : Tool("Selection", parent)
    , m_selectionType(type)
    , m_feather(0.0f)
    , m_antiAlias(true)
    , m_isSelecting(false)
    , m_startPos(0, 0)
    , m_currentPos(0, 0)
{
    m_description = "Select areas of the image";
}

void SelectionTool::setSelectionType(SelectionType type) {
    m_selectionType = type;
    updateOptions();
}

void SelectionTool::setFeather(float feather) {
    m_feather = std::max(0.0f, feather);
    updateOptions();
}

void SelectionTool::setAntiAlias(bool antiAlias) {
    m_antiAlias = antiAlias;
    updateOptions();
}

void SelectionTool::mousePressEvent(const ToolEvent& event) {
    m_isSelecting = true;
    m_startPos = event.pos;
    m_currentPos = event.pos;
    m_selectionPath = QPainterPath();
}

void SelectionTool::mouseMoveEvent(const ToolEvent& event) {
    if (!m_isSelecting) return;
    
    m_currentPos = event.pos;
    updateSelection();
}

void SelectionTool::mouseReleaseEvent(const ToolEvent& event) {
    if (!m_isSelecting) return;
    
    m_currentPos = event.pos;
    finalizeSelection();
    m_isSelecting = false;
}

void SelectionTool::draw(QPainter& painter, const QPointF& pos) {
    // Draw selection preview
    Q_UNUSED(pos)
}

void SelectionTool::drawPreview(QPainter& painter, const QRectF& bounds) {
    if (m_selectionPath.isEmpty()) return;
    
    painter.setPen(QPen(Qt::blue, 2.0, Qt::DashLine));
    painter.setBrush(Qt::transparent);
    painter.drawPath(m_selectionPath);
}

void SelectionTool::updateSelection() {
    m_selectionPath = QPainterPath();
    
    switch (m_selectionType) {
        case SelectionType::Rectangular: {
            QRectF rect(m_startPos, m_currentPos);
            rect = rect.normalized();
            m_selectionPath.addRect(rect);
            break;
        }
        case SelectionType::Elliptical: {
            QRectF rect(m_startPos, m_currentPos);
            rect = rect.normalized();
            m_selectionPath.addEllipse(rect);
            break;
        }
        case SelectionType::Lasso:
            // Lasso selection would be implemented here
            break;
        default:
            break;
    }
}

void SelectionTool::finalizeSelection() {
    updateSelection();
    // Here you would apply the selection to the document
}

// Move tool implementation
MoveTool::MoveTool(QObject* parent)
    : Tool("Move", parent)
    , m_isMoving(false)
    , m_startPos(0, 0)
    , m_lastPos(0, 0)
    , m_offset(0, 0)
{
    m_description = "Move layers and selections";
}

void MoveTool::mousePressEvent(const ToolEvent& event) {
    m_isMoving = true;
    m_startPos = event.pos;
    m_lastPos = event.pos;
}

void MoveTool::mouseMoveEvent(const ToolEvent& event) {
    if (!m_isMoving) return;
    
    updateMove(event.pos);
}

void MoveTool::mouseReleaseEvent(const ToolEvent& event) {
    if (!m_isMoving) return;
    
    endMove();
}

QCursor MoveTool::cursor() const {
    return m_isMoving ? Qt::ClosedHandCursor : Qt::OpenHandCursor;
}

void MoveTool::startMove(const QPointF& pos) {
    m_startPos = pos;
    m_lastPos = pos;
    m_offset = QPointF(0, 0);
}

void MoveTool::updateMove(const QPointF& pos) {
    m_offset += pos - m_lastPos;
    m_lastPos = pos;
    
    // Here you would update the document's active layer position
}

void MoveTool::endMove() {
    m_isMoving = false;
    // Here you would finalize the move operation
}

} // namespace core
