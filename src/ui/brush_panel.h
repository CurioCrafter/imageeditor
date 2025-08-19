#pragma once

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

namespace ui {

class BrushPanel : public QWidget {
    Q_OBJECT

public:
    explicit BrushPanel(QWidget* parent = nullptr);

private:
    void setupUI();
    void setupConnections();

    QSlider* m_sizeSlider;
    QSlider* m_hardnessSlider;
    QSlider* m_opacitySlider;
    QSlider* m_flowSlider;
    QSlider* m_spacingSlider;
    
    QSpinBox* m_sizeSpinBox;
    QSpinBox* m_hardnessSpinBox;
    QSpinBox* m_opacitySpinBox;
    QSpinBox* m_flowSpinBox;
    QSpinBox* m_spacingSpinBox;
    
    QCheckBox* m_pressureSensitiveCheck;
    QCheckBox* m_tiltSensitiveCheck;
};

} // namespace ui
