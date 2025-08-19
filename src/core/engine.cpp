
#include "engine.h"
#include "document.h"
#include "brush_engine.h"
#include <iostream>

namespace core {

Engine::Engine()
    : m_currentDocument(nullptr)
    , m_brushEngine(nullptr)
{
}

Engine::~Engine()
{
    shutdown();
}

bool Engine::initialize()
{
    try {
        // Initialize brush engine
        m_brushEngine = new BrushEngine();
        
        // Create a default document
        m_currentDocument = createNewDocument("Untitled");
        
        std::cout << "Engine initialized successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize engine: " << e.what() << std::endl;
        return false;
    }
}

void Engine::shutdown()
{
    if (m_brushEngine) {
        delete m_brushEngine;
        m_brushEngine = nullptr;
    }
    
    m_currentDocument.reset();
    
    std::cout << "Engine shutdown complete" << std::endl;
}

std::shared_ptr<Document> Engine::createNewDocument(const std::string& name)
{
    try {
        auto document = std::make_shared<Document>(name);
        m_currentDocument = document;
        std::cout << "Created new document: " << name << std::endl;
        return document;
    } catch (const std::exception& e) {
        std::cerr << "Failed to create document: " << e.what() << std::endl;
        return nullptr;
    }
}

bool Engine::saveDocument(const std::string& filename)
{
    if (!m_currentDocument) {
        std::cerr << "No document to save" << std::endl;
        return false;
    }
    
    try {
        bool success = m_currentDocument->saveToFile(filename);
        if (success) {
            std::cout << "Document saved to: " << filename << std::endl;
        } else {
            std::cerr << "Failed to save document" << std::endl;
        }
        return success;
    } catch (const std::exception& e) {
        std::cerr << "Exception while saving document: " << e.what() << std::endl;
        return false;
    }
}

bool Engine::loadDocument(const std::string& filename)
{
    try {
        auto document = std::make_shared<Document>("Loaded Document");
        bool success = document->loadFromFile(filename);
        if (success) {
            m_currentDocument = document;
            std::cout << "Document loaded from: " << filename << std::endl;
        } else {
            std::cerr << "Failed to load document" << std::endl;
        }
        return success;
    } catch (const std::exception& e) {
        std::cerr << "Exception while loading document: " << e.what() << std::endl;
        return false;
    }
}

void Engine::closeCurrentDocument()
{
    if (m_currentDocument) {
        std::cout << "Closing document: " << m_currentDocument->getName() << std::endl;
        m_currentDocument.reset();
    }
}

} // namespace core
