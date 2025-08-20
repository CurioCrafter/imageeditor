#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <memory>

class QVulkanInstance;
class QWidget;
class VulkanWindow;

namespace core { class Engine; }

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
    void fallbackToCPU();
    bool attemptVulkan();

    core::Engine* m_engine;
    QWidget* m_centralWidget;
    QVBoxLayout* m_layout;
    QLabel* m_statusLabel;
    QWidget* m_vulkanContainer = nullptr;
    QVulkanInstance* m_vulkanInstance = nullptr;
    VulkanWindow* m_vulkanWindow = nullptr;
    
    bool m_vulkanAvailable;
};
