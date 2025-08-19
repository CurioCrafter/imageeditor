#pragma once

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

namespace ui {

class ColorPanel : public QWidget {
    Q_OBJECT

public:
    explicit ColorPanel(QWidget* parent = nullptr);
    
    void setColor(const QColor& color);
    QColor getColor() const;

signals:
    void colorChanged(const QColor& color);

private slots:
    void onColorButtonClicked();
    void onSliderChanged();
    void onSpinBoxChanged();

private:
    void setupUI();
    void setupConnections();
    void updateColorButton();
    void updateSliders();
    void updateSpinBoxes();

    QPushButton* m_colorButton;
    QSlider* m_redSlider;
    QSlider* m_greenSlider;
    QSlider* m_blueSlider;
    QSlider* m_alphaSlider;
    
    QSpinBox* m_redSpinBox;
    QSpinBox* m_greenSpinBox;
    QSpinBox* m_blueSpinBox;
    QSpinBox* m_alphaSpinBox;
    
    // Current color values
    int m_red, m_green, m_blue, m_alpha;
};

} // namespace ui
