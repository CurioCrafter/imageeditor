#include "layer_panel.h"
#include "../core/document.h"
#include "../core/layer.h"
#include <QMessageBox>

namespace ui {

LayerPanel::LayerPanel(QWidget* parent)
    : QWidget(parent)
    , m_document(nullptr)
{
    setupUI();
    setupConnections();
}

void LayerPanel::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    
    // Layer list
    m_layerList = new QListWidget(this);
    mainLayout->addWidget(m_layerList);
    
    // Buttons
    auto buttonLayout = new QHBoxLayout();
    
    m_addLayerBtn = new QPushButton("Add", this);
    m_deleteLayerBtn = new QPushButton("Delete", this);
    m_duplicateLayerBtn = new QPushButton("Duplicate", this);
    
    buttonLayout->addWidget(m_addLayerBtn);
    buttonLayout->addWidget(m_deleteLayerBtn);
    buttonLayout->addWidget(m_duplicateLayerBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    // Add some sample layers for now
    m_layerList->addItem("Background");
    m_layerList->addItem("Layer 1");
}

void LayerPanel::setDocument(core::Document* document)
{
    m_document = document;
    refreshLayers();
}

void LayerPanel::refreshLayers()
{
    if (!m_document) {
        m_layerList->clear();
        return;
    }
    
    updateLayerList();
}

void LayerPanel::setupConnections()
{
    connect(m_addLayerBtn, &QPushButton::clicked, this, &LayerPanel::onAddLayer);
    connect(m_deleteLayerBtn, &QPushButton::clicked, this, &LayerPanel::onDeleteLayer);
    connect(m_duplicateLayerBtn, &QPushButton::clicked, this, &LayerPanel::onDuplicateLayer);
    connect(m_layerList, &QListWidget::itemChanged, this, &LayerPanel::onLayerVisibilityChanged);
}

void LayerPanel::onAddLayer()
{
    m_layerList->addItem("New Layer");
}

void LayerPanel::onDeleteLayer()
{
    auto currentItem = m_layerList->currentItem();
    if (currentItem) {
        delete m_layerList->takeItem(m_layerList->row(currentItem));
    }
}

void LayerPanel::onDuplicateLayer()
{
    auto currentItem = m_layerList->currentItem();
    if (currentItem) {
        QString newName = currentItem->text() + " Copy";
        m_layerList->addItem(newName);
    }
}

void LayerPanel::onLayerVisibilityChanged(QListWidgetItem* item)
{
    // TODO: Implement layer visibility toggle
    Q_UNUSED(item)
}

void LayerPanel::updateLayerList()
{
    if (!m_document) return;
    
    m_layerList->clear();
    
    // TODO: Get actual layers from document
    // For now, add placeholder layers
    m_layerList->addItem("Background");
    m_layerList->addItem("Layer 1");
}

} // namespace ui
