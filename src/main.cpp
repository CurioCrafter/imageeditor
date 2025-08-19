#include <iostream>
#include <memory>

#include <QApplication>
#include <QMainWindow>
#include "ui/main_window.h"
#include "core/engine.h"
#include "core/document.h"

int main(int argc, char* argv[])
{
    // Create Qt application
    QApplication app(argc, argv);
    
    // Initialize core engine
    core::Engine engine;
    if (!engine.initialize()) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return 1;
    }
    
    // Create and show main window
    ui::MainWindow window;
    window.show();
    
    // Run Qt event loop
    return app.exec();
}
