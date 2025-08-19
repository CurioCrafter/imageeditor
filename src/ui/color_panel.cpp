#include "color_panel.h"
#include <QColorDialog>

namespace ui {

ColorPanel::ColorPanel(QWidget* parent)
    : QWidget(parent)
    , m_red(255)
    , m_green(0)
    , m_blue(0)
    , m_alpha(255)
{
    setupUI();
    setupConnections();
    updateColorButton();
}

void ColorPanel::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    
    // Color button
    m_colorButton = new QPushButton(this);
    m_colorButton->setMinimumHeight(40);
    mainLayout->addWidget(m_colorButton);
    
    // RGB sliders group
    auto rgbGroup = new QGroupBox("RGB", this);
    auto rgbLayout = new QVBoxLayout(rgbGroup);
    
    // Red
    auto redLayout = new QHBoxLayout();
    redLayout->addWidget(new QLabel("R:", this));
    
    m_redSlider = new QSlider(Qt::Horizontal, this);
    m_redSlider->setRange(0, 255);
    m_redSlider->setValue(m_red);
    redLayout->addWidget(m_redSlider);
    
    m_redSpinBox = new QSpinBox(this);
    m_redSpinBox->setRange(0, 255);
    m_redSpinBox->setValue(m_red);
    redLayout->addWidget(m_redSpinBox);
    rgbLayout->addLayout(redLayout);
    
    // Green
    auto greenLayout = new QHBoxLayout();
    greenLayout->addWidget(new QLabel("G:", this));
    
    m_greenSlider = new QSlider(Qt::Horizontal, this);
    m_greenSlider->setRange(0, 255);
    m_greenSlider->setValue(m_green);
    greenLayout->addWidget(m_greenSlider);
    
    m_greenSpinBox = new QSpinBox(this);
    m_greenSpinBox->setRange(0, 255);
    m_greenSpinBox->setValue(m_green);
    greenLayout->addWidget(m_greenSpinBox);
    rgbLayout->addLayout(greenLayout);
    
    // Blue
    auto blueLayout = new QHBoxLayout();
    blueLayout->addWidget(new QLabel("B:", this));
    
    m_blueSlider = new QSlider(Qt::Horizontal, this);
    m_blueSlider->setRange(0, 255);
    m_blueSlider->setValue(m_blue);
    blueLayout->addWidget(m_blueSlider);
    
    m_blueSpinBox = new QSpinBox(this);
    m_blueSpinBox->setRange(0, 255);
    m_blueSpinBox->setValue(m_blue);
    blueLayout->addWidget(m_blueSpinBox);
    rgbLayout->addLayout(blueLayout);
    
    mainLayout->addWidget(rgbGroup);
    
    // Alpha slider group
    auto alphaGroup = new QGroupBox("Alpha", this);
    auto alphaLayout = new QHBoxLayout(alphaGroup);
    
    alphaLayout->addWidget(new QLabel("A:", this));
    
    m_alphaSlider = new QSlider(Qt::Horizontal, this);
    m_alphaSlider->setRange(0, 255);
    m_alphaSlider->setValue(m_alpha);
    alphaLayout->addWidget(m_alphaSlider);
    
    m_alphaSpinBox = new QSpinBox(this);
    m_alphaSpinBox->setRange(0, 255);
    m_alphaSpinBox->setValue(m_alpha);
    alphaLayout->addWidget(m_alphaSpinBox);
    
    mainLayout->addWidget(alphaGroup);
    mainLayout->addStretch();
}

void ColorPanel::setupConnections()
{
    connect(m_colorButton, &QPushButton::clicked, this, &ColorPanel::onColorButtonClicked);
    
    connect(m_redSlider, &QSlider::valueChanged, this, &ColorPanel::onSliderChanged);
    connect(m_greenSlider, &QSlider::valueChanged, this, &ColorPanel::onSliderChanged);
    connect(m_blueSlider, &QSlider::valueChanged, this, &ColorPanel::onSliderChanged);
    connect(m_alphaSlider, &QSlider::valueChanged, this, &ColorPanel::onSliderChanged);
    
    connect(m_redSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPanel::onSpinBoxChanged);
    connect(m_greenSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPanel::onSpinBoxChanged);
    connect(m_blueSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPanel::onSpinBoxChanged);
    connect(m_alphaSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPanel::onSpinBoxChanged);
}

void ColorPanel::onColorButtonClicked()
{
    QColor currentColor(m_red, m_green, m_blue, m_alpha);
    QColor newColor = QColorDialog::getColor(currentColor, this, "Select Color");
    
    if (newColor.isValid()) {
        m_red = newColor.red();
        m_green = newColor.green();
        m_blue = newColor.blue();
        m_alpha = newColor.alpha();
        
        updateSliders();
        updateColorButton();
        emit colorChanged(getColor());
    }
}

void ColorPanel::onSliderChanged()
{
    m_red = m_redSlider->value();
    m_green = m_greenSlider->value();
    m_blue = m_blueSlider->value();
    m_alpha = m_alphaSlider->value();
    
    updateSpinBoxes();
    updateColorButton();
    emit colorChanged(getColor());
}

void ColorPanel::onSpinBoxChanged()
{
    m_red = m_redSpinBox->value();
    m_green = m_greenSpinBox->value();
    m_blue = m_blueSpinBox->value();
    m_alpha = m_alphaSpinBox->value();
    
    updateSliders();
    updateColorButton();
    emit colorChanged(getColor());
}

void ColorPanel::updateColorButton()
{
    QColor color(m_red, m_green, m_blue, m_alpha);
    QString style = QString("background-color: %1; border: 1px solid black;").arg(color.name());
    m_colorButton->setStyleSheet(style);
}

void ColorPanel::updateSliders()
{
    m_redSlider->setValue(m_red);
    m_greenSlider->setValue(m_green);
    m_blueSlider->setValue(m_blue);
    m_alphaSlider->setValue(m_alpha);
}

void ColorPanel::updateSpinBoxes()
{
    m_redSpinBox->setValue(m_red);
    m_greenSpinBox->setValue(m_green);
    m_blueSpinBox->setValue(m_blue);
    m_alphaSpinBox->setValue(m_alpha);
}

void ColorPanel::setColor(const QColor& color)
{
    m_red = color.red();
    m_green = color.green();
    m_blue = color.blue();
    m_alpha = color.alpha();
    
    updateSliders();
    updateSpinBoxes();
    updateColorButton();
}

QColor ColorPanel::getColor() const
{
    return QColor(m_red, m_green, m_blue, m_alpha);
}

} // namespace ui
