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
    m_layerList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_layerList->setDragEnabled(true);
    m_layerList->setAcceptDrops(true);
    m_layerList->setDropIndicatorShown(true);
    m_layerList->setDragDropMode(QAbstractItemView::InternalMove);
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
    connect(m_layerList, &QListWidget::currentRowChanged, this, [this](int row){
        emit layerSelectionChanged(row);
    });

    // Handle drag-and-drop reorder: map UI rows (top=0) to document indices (top=last)
    connect(m_layerList->model(), &QAbstractItemModel::rowsMoved, this,
            [this](const QModelIndex&, int from, int, const QModelIndex&, int to){
                if (!m_document) return;
                int count = m_document->getLayerCount();
                int docFrom = count - 1 - from;
                int docTo = count - 1 - (to > from ? to - 1 : to);
                m_document->moveLayer(docFrom, docTo);
                refreshLayers();
            });
}

void LayerPanel::onAddLayer()
{
    if (!m_document) return;
    auto newLayer = std::make_shared<core::RasterLayer>(
        m_document->width(), m_document->height(), Qt::transparent);
    newLayer->setName("New Layer");
    m_document->addLayer(newLayer, -1);
    // Select new topmost
    m_layerList->setCurrentRow(0);
    refreshLayers();
}

void LayerPanel::onDeleteLayer()
{
    if (!m_document) return;
    int row = m_layerList->currentRow();
    if (row < 0) return;
    int docIndex = m_document->getLayerCount() - 1 - row;
    m_document->removeLayer(docIndex);
    refreshLayers();
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
    if (!m_document || !item) return;
    int indexFromTop = m_layerList->row(item);
    int docIndex = m_document->getLayerCount() - 1 - indexFromTop;
    auto layer = m_document->getLayerAt(docIndex);
    if (layer) {
        layer->setVisible(item->checkState() == Qt::Checked);
        emit layerSelectionChanged(docIndex);
    }
}

void LayerPanel::updateLayerList()
{
    if (!m_document) return;
    
    m_layerList->clear();
    const auto& layers = m_document->getLayers();
    // UI shows topmost first: layers are stored bottom-to-top, so iterate reverse
    for (int i = static_cast<int>(layers.size()) - 1; i >= 0; --i) {
        auto item = new QListWidgetItem(layers[i]->getName(), m_layerList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setCheckState(layers[i]->isVisible() ? Qt::Checked : Qt::Unchecked);
        m_layerList->addItem(item);
    }
    if (!layers.empty()) {
        m_layerList->setCurrentRow(0); // select topmost as active
    }
}

} // namespace ui
