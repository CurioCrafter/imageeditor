#pragma once

#include <memory>
#include <string>

// Forward declarations
namespace core {
    class Document;
    class BrushEngine;
}

namespace core {

class Engine {
public:
    Engine();
    ~Engine();

    // Engine lifecycle
    bool initialize();
    void shutdown();

    // Document management
    std::shared_ptr<Document> createNewDocument(const std::string& name = "Untitled");
    bool saveDocument(const std::string& filename);
    bool loadDocument(const std::string& filename);
    void closeCurrentDocument();
    
    std::shared_ptr<Document> getCurrentDocument() const { return m_currentDocument; }
    BrushEngine* getBrushEngine() const { return m_brushEngine; }

private:
    std::shared_ptr<Document> m_currentDocument;
    BrushEngine* m_brushEngine;
};

} // namespace core
