
#include "engine.h"
#include "document.h"
#include "brush_engine.h"
#include <QDebug>
#include <QString>

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
        
        qDebug() << "Engine initialized successfully";
        return true;
    } catch (const std::exception& e) {
        qCritical() << "Failed to initialize engine:" << e.what();
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
    
    qDebug() << "Engine shutdown complete";
}

std::shared_ptr<Document> Engine::createNewDocument(const std::string& name)
{
    try {
        // Convert std::string to QString for Document constructor
        auto document = std::make_shared<Document>(800, 600);
        document->setName(QString::fromStdString(name));
        m_currentDocument = document;
        qDebug() << "Created new document:" << QString::fromStdString(name);
        return document;
    } catch (const std::exception& e) {
        qCritical() << "Failed to create document:" << e.what();
        return nullptr;
    }
}

bool Engine::saveDocument(const std::string& filename)
{
    if (!m_currentDocument) {
        qWarning() << "No document to save";
        return false;
    }
    
    try {
        // Convert std::string to QString
        bool success = m_currentDocument->saveToFile(QString::fromStdString(filename));
        if (success) {
            qDebug() << "Document saved to:" << QString::fromStdString(filename);
        } else {
            qWarning() << "Failed to save document";
        }
        return success;
    } catch (const std::exception& e) {
        qCritical() << "Exception while saving document:" << e.what();
        return false;
    }
}

bool Engine::loadDocument(const std::string& filename)
{
    try {
        auto document = std::make_shared<Document>(800, 600);
        // Convert std::string to QString
        bool success = document->loadFromFile(QString::fromStdString(filename));
        if (success) {
            m_currentDocument = document;
            qDebug() << "Document loaded from:" << QString::fromStdString(filename);
        } else {
            qWarning() << "Failed to load document";
        }
        return success;
    } catch (const std::exception& e) {
        qCritical() << "Exception while loading document:" << e.what();
        return false;
    }
}

void Engine::closeCurrentDocument()
{
    if (m_currentDocument) {
        qDebug() << "Closing document:" << m_currentDocument->getName();
        m_currentDocument.reset();
    }
}

} // namespace core
