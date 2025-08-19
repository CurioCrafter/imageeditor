#include "tool_panel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QToolButton>
#include <QLabel>
#include <QGroupBox>

namespace ui {

ToolPanel::ToolPanel(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

void ToolPanel::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    
    // Main tools group
    auto mainToolsGroup = new QGroupBox("Main Tools", this);
    auto mainToolsLayout = new QVBoxLayout(mainToolsGroup);
    
    m_toolGroup = new QButtonGroup(this);
    
    m_moveTool = new QToolButton(this);
    m_moveTool->setText("Move");
    m_moveTool->setCheckable(true);
    m_moveTool->setChecked(true);
    m_toolGroup->addButton(m_moveTool, 0);
    mainToolsLayout->addWidget(m_moveTool);
    
    m_brushTool = new QToolButton(this);
    m_brushTool->setText("Brush");
    m_brushTool->setCheckable(true);
    m_toolGroup->addButton(m_brushTool, 1);
    mainToolsLayout->addWidget(m_brushTool);
    
    m_eraserTool = new QToolButton(this);
    m_eraserTool->setText("Eraser");
    m_eraserTool->setCheckable(true);
    m_toolGroup->addButton(m_eraserTool, 2);
    mainToolsLayout->addWidget(m_eraserTool);
    
    m_eyedropperTool = new QToolButton(this);
    m_eyedropperTool->setText("Eyedropper");
    m_eyedropperTool->setCheckable(true);
    m_toolGroup->addButton(m_eyedropperTool, 3);
    mainToolsLayout->addWidget(m_eyedropperTool);
    
    mainLayout->addWidget(mainToolsGroup);
    
    // Selection tools group
    auto selectionToolsGroup = new QGroupBox("Selection Tools", this);
    auto selectionToolsLayout = new QVBoxLayout(selectionToolsGroup);
    
    m_rectSelectTool = new QToolButton(this);
    m_rectSelectTool->setText("Rect Select");
    m_rectSelectTool->setCheckable(true);
    m_toolGroup->addButton(m_rectSelectTool, 4);
    selectionToolsLayout->addWidget(m_rectSelectTool);
    
    m_ellipseSelectTool = new QToolButton(this);
    m_ellipseSelectTool->setText("Ellipse Select");
    m_ellipseSelectTool->setCheckable(true);
    m_toolGroup->addButton(m_ellipseSelectTool, 5);
    selectionToolsLayout->addWidget(m_ellipseSelectTool);
    
    m_lassoSelectTool = new QToolButton(this);
    m_lassoSelectTool->setText("Lasso Select");
    m_lassoSelectTool->setCheckable(true);
    m_toolGroup->addButton(m_lassoSelectTool, 6);
    selectionToolsLayout->addWidget(m_lassoSelectTool);
    
    mainLayout->addWidget(selectionToolsGroup);
    
    // Other tools group
    auto otherToolsGroup = new QGroupBox("Other Tools", this);
    auto otherToolsLayout = new QVBoxLayout(otherToolsGroup);
    
    m_textTool = new QToolButton(this);
    m_textTool->setText("Text");
    m_textTool->setCheckable(true);
    m_toolGroup->addButton(m_textTool, 7);
    otherToolsLayout->addWidget(m_textTool);
    
    m_gradientTool = new QToolButton(this);
    m_gradientTool->setText("Gradient");
    m_gradientTool->setCheckable(true);
    m_toolGroup->addButton(m_gradientTool, 8);
    otherToolsLayout->addWidget(m_gradientTool);
    
    mainLayout->addWidget(otherToolsGroup);
    
    // Advanced Tools Group
    auto advancedToolsGroup = new QGroupBox("Advanced Tools", this);
    auto advancedToolsLayout = new QVBoxLayout(advancedToolsGroup);
    
    m_cloneStampTool = new QToolButton(this);
    m_cloneStampTool->setText("Clone Stamp");
    m_cloneStampTool->setCheckable(true);
    m_toolGroup->addButton(m_cloneStampTool, 9);
    advancedToolsLayout->addWidget(m_cloneStampTool);
    
    m_healingBrushTool = new QToolButton(this);
    m_healingBrushTool->setText("Healing Brush");
    m_healingBrushTool->setCheckable(true);
    m_toolGroup->addButton(m_healingBrushTool, 10);
    advancedToolsLayout->addWidget(m_healingBrushTool);
    
    m_magicWandTool = new QToolButton(this);
    m_magicWandTool->setText("Magic Wand");
    m_magicWandTool->setCheckable(true);
    m_toolGroup->addButton(m_magicWandTool, 11);
    advancedToolsLayout->addWidget(m_magicWandTool);
    
    m_cropTool = new QToolButton(this);
    m_cropTool->setText("Crop");
    m_cropTool->setCheckable(true);
    m_toolGroup->addButton(m_cropTool, 12);
    advancedToolsLayout->addWidget(m_cropTool);
    
    mainLayout->addWidget(advancedToolsGroup);
    
    // Effects Tools Group
    auto effectsToolsGroup = new QGroupBox("Effects Tools", this);
    auto effectsToolsLayout = new QVBoxLayout(effectsToolsGroup);
    
    m_blurTool = new QToolButton(this);
    m_blurTool->setText("Blur");
    m_blurTool->setCheckable(true);
    m_toolGroup->addButton(m_blurTool, 13);
    effectsToolsLayout->addWidget(m_blurTool);
    
    m_sharpenTool = new QToolButton(this);
    m_sharpenTool->setText("Sharpen");
    m_sharpenTool->setCheckable(true);
    m_toolGroup->addButton(m_sharpenTool, 14);
    effectsToolsLayout->addWidget(m_sharpenTool);
    
    m_dodgeTool = new QToolButton(this);
    m_dodgeTool->setText("Dodge");
    m_dodgeTool->setCheckable(true);
    m_toolGroup->addButton(m_dodgeTool, 15);
    effectsToolsLayout->addWidget(m_dodgeTool);
    
    m_burnTool = new QToolButton(this);
    m_burnTool->setText("Burn");
    m_burnTool->setCheckable(true);
    m_toolGroup->addButton(m_burnTool, 16);
    effectsToolsLayout->addWidget(m_burnTool);
    
    m_spongeTool = new QToolButton(this);
    m_spongeTool->setText("Sponge");
    m_spongeTool->setCheckable(true);
    m_toolGroup->addButton(m_spongeTool, 17);
    effectsToolsLayout->addWidget(m_spongeTool);
    
    mainLayout->addWidget(effectsToolsGroup);
    
    mainLayout->addStretch();
}

void ToolPanel::setupConnections()
{
    connect(m_toolGroup, &QButtonGroup::buttonClicked, this, &ToolPanel::onToolSelected);
}

void ToolPanel::onToolSelected(QAbstractButton* button)
{
    if (button) {
        int toolId = m_toolGroup->id(button);
        emit toolChanged(toolId);
    }
}

} // namespace ui
