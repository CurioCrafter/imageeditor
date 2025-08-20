#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

namespace core { class Engine; }
class VulkanWindow;

class MainWindowVulkan : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowVulkan(core::Engine* engine, QWidget* parent = nullptr);
    ~MainWindowVulkan();

private slots:
    void onVulkanAvailable();
    void onVulkanUnavailable();

private:
    void setupUI();
    void tryVulkan();
    void fallbackToCPU();

    core::Engine* m_engine;
    VulkanWindow* m_vulkanWindow;
    QWidget* m_centralWidget;
    QVBoxLayout* m_layout;
    QLabel* m_statusLabel;
    QPushButton* m_retryButton;
    
    bool m_vulkanAvailable;
};
