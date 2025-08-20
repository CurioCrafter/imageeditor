#include <iostream>
#include <memory>

#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "ui/main_window.h"
#include "core/engine.h"
#include "core/document.h"
#include "crash_handler.h"

int main(int argc, char* argv[])
{
    // Install crash handler to write .dmp files next to the executable
    platform::crash::installCrashHandler("image-editor");

    // Create Qt application
    QApplication app(argc, argv);
    
    // Enable verbose Qt plugin logging to help diagnose startup issues
    qputenv("QT_DEBUG_PLUGINS", "1");
    qputenv("QT_LOGGING_RULES", "qt.*=true");

    qDebug() << "Starting Advanced Image Editor...";
    
    // Initialize core engine
    core::Engine engine;
    if (!engine.initialize()) {
        qCritical() << "Failed to initialize engine";
        return 1;
    }
    
    // Create and show full editor window
    ui::MainWindow window;
    window.setEngine(&engine);
    window.show();
    
    qDebug() << "Main window created, starting event loop...";
    
    // Run Qt event loop
    return app.exec();
}
