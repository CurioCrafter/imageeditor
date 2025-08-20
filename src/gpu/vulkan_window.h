#pragma once

#include <QVulkanWindow>
#include <QVulkanInstance>
#include <QExposeEvent>
#include <QResizeEvent>

namespace core { class Engine; }

class VulkanWindow : public QVulkanWindow
{
    Q_OBJECT

public:
    explicit VulkanWindow(core::Engine* engine, QWindow* parent = nullptr);
    ~VulkanWindow();

    QVulkanWindowRenderer* createRenderer() override;

signals:
    void frameCompleted();
    void vulkanError(const QString& error);

protected:
    void exposeEvent(QExposeEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    core::Engine* m_engine;
};
