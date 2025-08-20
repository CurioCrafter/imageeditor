#include "vulkan_window.h"
#include "vulkan_renderer.h"
#include "../core/engine.h"
#include <QDebug>

VulkanWindow::VulkanWindow(core::Engine* engine, QWindow* parent)
    : QVulkanWindow(parent)
    , m_engine(engine)
{
    qDebug() << "VulkanWindow created";
}

VulkanWindow::~VulkanWindow()
{
    qDebug() << "VulkanWindow destroyed";
}

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
    qDebug() << "Creating VulkanRenderer";
    return new VulkanRenderer(this, m_engine);
}

// Override event handlers to emit signals when appropriate
void VulkanWindow::exposeEvent(QExposeEvent* event)
{
    QVulkanWindow::exposeEvent(event);
    if (isExposed()) {
        qDebug() << "VulkanWindow exposed";
    }
}

void VulkanWindow::resizeEvent(QResizeEvent* event)
{
    QVulkanWindow::resizeEvent(event);
    qDebug() << "VulkanWindow resized to" << event->size();
}
