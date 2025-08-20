#include <iostream>
#include <memory>

#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include "ui/main_window_vulkan.h"
#include "core/engine.h"
#include "core/document.h"

int main(int argc, char* argv[])
{
    // Create Qt application
    QApplication app(argc, argv);
    
    qDebug() << "Starting Advanced Image Editor with Vulkan support...";
    
    // Initialize core engine
    core::Engine engine;
    if (!engine.initialize()) {
        qCritical() << "Failed to initialize engine";
        return 1;
    }
    
    // Create and show main window with Vulkan support
    MainWindowVulkan window(&engine);
    window.show();
    
    qDebug() << "Main window created, starting event loop...";
    
    // Run Qt event loop
    return app.exec();
}
