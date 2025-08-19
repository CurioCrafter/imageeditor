#pragma once

#include <QWidget>
#include <QToolButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QGroupBox>

namespace ui {

class ToolPanel : public QWidget {
    Q_OBJECT

public:
    explicit ToolPanel(QWidget* parent = nullptr);

signals:
    void toolChanged(int toolId);

private slots:
    void onToolSelected(QAbstractButton* button);

private:
    void setupUI();
    void setupConnections();

    QButtonGroup* m_toolGroup;
    QToolButton* m_moveTool;
    QToolButton* m_brushTool;
    QToolButton* m_eraserTool;
    QToolButton* m_eyedropperTool;
    QToolButton* m_rectSelectTool;
    QToolButton* m_ellipseSelectTool;
    QToolButton* m_lassoSelectTool;
    QToolButton* m_textTool;
    QToolButton* m_gradientTool;
    QToolButton* m_cloneStampTool;
    QToolButton* m_healingBrushTool;
    QToolButton* m_magicWandTool;
    QToolButton* m_cropTool;
    QToolButton* m_blurTool;
    QToolButton* m_sharpenTool;
    QToolButton* m_dodgeTool;
    QToolButton* m_burnTool;
    QToolButton* m_spongeTool;
};

} // namespace ui
