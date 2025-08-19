#include "brush_panel.h"

namespace ui {

BrushPanel::BrushPanel(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

void BrushPanel::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    
    // Size group
    auto sizeGroup = new QGroupBox("Size", this);
    auto sizeLayout = new QHBoxLayout(sizeGroup);
    
    sizeLayout->addWidget(new QLabel("Size:", this));
    
    m_sizeSlider = new QSlider(Qt::Horizontal, this);
    m_sizeSlider->setRange(1, 100);
    m_sizeSlider->setValue(10);
    sizeLayout->addWidget(m_sizeSlider);
    
    m_sizeSpinBox = new QSpinBox(this);
    m_sizeSpinBox->setRange(1, 100);
    m_sizeSpinBox->setValue(10);
    sizeLayout->addWidget(m_sizeSpinBox);
    
    mainLayout->addWidget(sizeGroup);
    
    // Hardness group
    auto hardnessGroup = new QGroupBox("Hardness", this);
    auto hardnessLayout = new QHBoxLayout(hardnessGroup);
    
    hardnessLayout->addWidget(new QLabel("Hardness:", this));
    
    m_hardnessSlider = new QSlider(Qt::Horizontal, this);
    m_hardnessSlider->setRange(0, 100);
    m_hardnessSlider->setValue(80);
    hardnessLayout->addWidget(m_hardnessSlider);
    
    m_hardnessSpinBox = new QSpinBox(this);
    m_hardnessSpinBox->setRange(0, 100);
    m_hardnessSpinBox->setValue(80);
    hardnessLayout->addWidget(m_hardnessSpinBox);
    
    mainLayout->addWidget(hardnessGroup);
    
    // Opacity group
    auto opacityGroup = new QGroupBox("Opacity", this);
    auto opacityLayout = new QHBoxLayout(opacityGroup);
    
    opacityLayout->addWidget(new QLabel("Opacity:", this));
    
    m_opacitySlider = new QSlider(Qt::Horizontal, this);
    m_opacitySlider->setRange(0, 100);
    m_opacitySlider->setValue(100);
    opacityLayout->addWidget(m_opacitySlider);
    
    m_opacitySpinBox = new QSpinBox(this);
    m_opacitySpinBox->setRange(0, 100);
    m_opacitySpinBox->setValue(100);
    opacityLayout->addWidget(m_opacitySpinBox);
    
    mainLayout->addWidget(opacityGroup);
    
    // Flow group
    auto flowGroup = new QGroupBox("Flow", this);
    auto flowLayout = new QHBoxLayout(flowGroup);
    
    flowLayout->addWidget(new QLabel("Flow:", this));
    
    m_flowSlider = new QSlider(Qt::Horizontal, this);
    m_flowSlider->setRange(0, 100);
    m_flowSlider->setValue(100);
    flowLayout->addWidget(m_flowSlider);
    
    m_flowSpinBox = new QSpinBox(this);
    m_flowSpinBox->setRange(0, 100);
    m_flowSpinBox->setValue(100);
    flowLayout->addWidget(m_flowSpinBox);
    
    mainLayout->addWidget(flowGroup);
    
    // Spacing group
    auto spacingGroup = new QGroupBox("Spacing", this);
    auto spacingLayout = new QHBoxLayout(spacingGroup);
    
    spacingLayout->addWidget(new QLabel("Spacing:", this));
    
    m_spacingSlider = new QSlider(Qt::Horizontal, this);
    m_spacingSlider->setRange(0, 100);
    m_spacingSlider->setValue(25);
    spacingLayout->addWidget(m_spacingSlider);
    
    m_spacingSpinBox = new QSpinBox(this);
    m_spacingSpinBox->setRange(0, 100);
    m_spacingSpinBox->setValue(25);
    spacingLayout->addWidget(m_spacingSpinBox);
    
    mainLayout->addWidget(spacingGroup);
    
    // Options group
    auto optionsGroup = new QGroupBox("Options", this);
    auto optionsLayout = new QVBoxLayout(optionsGroup);
    
    m_pressureSensitiveCheck = new QCheckBox("Pressure Sensitive", this);
    m_pressureSensitiveCheck->setChecked(true);
    optionsLayout->addWidget(m_pressureSensitiveCheck);
    
    m_tiltSensitiveCheck = new QCheckBox("Tilt Sensitive", this);
    m_tiltSensitiveCheck->setChecked(false);
    optionsLayout->addWidget(m_tiltSensitiveCheck);
    
    mainLayout->addWidget(optionsGroup);
    mainLayout->addStretch();
}

void BrushPanel::setupConnections()
{
    // Connect sliders to spin boxes
    connect(m_sizeSlider, &QSlider::valueChanged, m_sizeSpinBox, &QSpinBox::setValue);
    connect(m_sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_sizeSlider, &QSlider::setValue);
    
    connect(m_hardnessSlider, &QSlider::valueChanged, m_hardnessSpinBox, &QSpinBox::setValue);
    connect(m_hardnessSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_hardnessSlider, &QSlider::setValue);
    
    connect(m_opacitySlider, &QSlider::valueChanged, m_opacitySpinBox, &QSpinBox::setValue);
    connect(m_opacitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_opacitySlider, &QSlider::setValue);
    
    connect(m_flowSlider, &QSlider::valueChanged, m_flowSpinBox, &QSpinBox::setValue);
    connect(m_flowSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_flowSlider, &QSlider::setValue);
    
    connect(m_spacingSlider, &QSlider::valueChanged, m_spacingSpinBox, &QSpinBox::setValue);
    connect(m_spacingSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_spacingSlider, &QSlider::setValue);
}

} // namespace ui
