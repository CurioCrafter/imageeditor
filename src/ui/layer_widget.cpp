#include "layer_widget.h"
#include <QHeaderView>
#include <QSlider>
#include <QComboBox>
#include <QCheckBox>
#include <QDebug>

namespace ui {

LayerWidget::LayerWidget(QWidget* parent)
    : QWidget(parent)
    , m_selectedLayerIndex(-1)
{
    setupUI();
    createContextMenu();
    
    // Add a default background layer
    addLayer("Background", true);
}

LayerWidget::~LayerWidget() = default;

void LayerWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(5);
    
    // Title
    QLabel* titleLabel = new QLabel("Layers", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; color: white; }");
    mainLayout->addWidget(titleLabel);
    
    // Layer tree
    m_layerTree = new QTreeWidget(this);
    m_layerTree->setHeaderLabels({"", "Name", "Opacity", "Blend Mode"});
    m_layerTree->setAlternatingRowColors(true);
    m_layerTree->setSelectionMode(QAbstractItemView::SingleSelection);
    m_layerTree->setRootIsDecorated(false);
    m_layerTree->setItemsExpandable(false);
    
    // Set column widths
    m_layerTree->header()->setStretchLastSection(false);
    m_layerTree->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_layerTree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_layerTree->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_layerTree->header()->setSectionResizeMode(3, QHeaderView::Fixed);
    
    m_layerTree->setColumnWidth(0, 30);  // Visibility
    m_layerTree->setColumnWidth(2, 60);  // Opacity
    m_layerTree->setColumnWidth(3, 80);  // Blend Mode
    
    mainLayout->addWidget(m_layerTree);
    
    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(5);
    
    m_addLayerButton = new QPushButton("+", this);
    m_addLayerButton->setFixedSize(24, 24);
    m_addLayerButton->setToolTip("Add Layer");
    m_addLayerButton->setStyleSheet("QPushButton { font-weight: bold; background-color: #0078d4; color: white; border: none; border-radius: 12px; } QPushButton:hover { background-color: #106ebe; }");
    
    m_removeLayerButton = new QPushButton("-", this);
    m_removeLayerButton->setFixedSize(24, 24);
    m_removeLayerButton->setToolTip("Remove Layer");
    m_removeLayerButton->setStyleSheet("QPushButton { font-weight: bold; background-color: #d83b01; color: white; border: none; border-radius: 12px; } QPushButton:hover { background-color: #b83300; }");
    
    m_duplicateLayerButton = new QPushButton("⧉", this);
    m_duplicateLayerButton->setFixedSize(24, 24);
    m_duplicateLayerButton->setToolTip("Duplicate Layer");
    m_duplicateLayerButton->setStyleSheet("QPushButton { font-weight: bold; background-color: #107c10; color: white; border: none; border-radius: 12px; } QPushButton:hover { background-color: #0e6e0e; }");
    
    buttonLayout->addWidget(m_addLayerButton);
    buttonLayout->addWidget(m_removeLayerButton);
    buttonLayout->addWidget(m_duplicateLayerButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(m_layerTree, &QTreeWidget::itemSelectionChanged, this, &LayerWidget::onLayerSelectionChanged);
    connect(m_addLayerButton, &QPushButton::clicked, this, &LayerWidget::onAddLayerClicked);
    connect(m_removeLayerButton, &QPushButton::clicked, this, &LayerWidget::onRemoveLayerClicked);
    connect(m_duplicateLayerButton, &QPushButton::clicked, this, &LayerWidget::onDuplicateLayerClicked);
    connect(m_layerTree, &QTreeWidget::customContextMenuRequested, this, &LayerWidget::onContextMenuRequested);
    
    // Enable context menu
    m_layerTree->setContextMenuPolicy(Qt::CustomContextMenu);
}

void LayerWidget::createContextMenu()
{
    m_contextMenu = new QMenu(this);
    
    m_addLayerAction = new QAction("Add Layer", this);
    m_removeLayerAction = new QAction("Remove Layer", this);
    m_duplicateLayerAction = new QAction("Duplicate Layer", this);
    m_propertiesAction = new QAction("Properties", this);
    
    m_contextMenu->addAction(m_addLayerAction);
    m_contextMenu->addAction(m_removeLayerAction);
    m_contextMenu->addAction(m_duplicateLayerAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(m_propertiesAction);
    
    connect(m_addLayerAction, &QAction::triggered, this, &LayerWidget::onAddLayerClicked);
    connect(m_removeLayerAction, &QAction::triggered, this, &LayerWidget::onRemoveLayerClicked);
    connect(m_duplicateLayerAction, &QAction::triggered, this, &LayerWidget::onDuplicateLayerClicked);
    connect(m_propertiesAction, &QAction::triggered, this, &LayerWidget::onLayerPropertiesClicked);
}

void LayerWidget::addLayer(const QString& name, bool visible)
{
    LayerInfo layerInfo;
    layerInfo.name = name;
    layerInfo.visible = visible;
    layerInfo.opacity = 1.0;
    layerInfo.blendMode = "Normal";
    
    // Create tree item
    QTreeWidgetItem* item = new QTreeWidgetItem(m_layerTree);
    
    // Visibility checkbox
    QCheckBox* visibilityCheckBox = new QCheckBox();
    visibilityCheckBox->setChecked(visible);
    visibilityCheckBox->setStyleSheet("QCheckBox { margin: 2px; }");
    m_layerTree->setItemWidget(item, 0, visibilityCheckBox);
    
    // Name
    item->setText(1, name);
    
    // Opacity slider
    QSlider* opacitySlider = new QSlider(Qt::Horizontal);
    opacitySlider->setRange(0, 100);
    opacitySlider->setValue(100);
    opacitySlider->setStyleSheet("QSlider::groove:horizontal { height: 8px; background: #444; border-radius: 4px; } QSlider::handle:horizontal { background: #0078d4; border: 1px solid #0078d4; width: 16px; margin: -4px 0; border-radius: 8px; }");
    m_layerTree->setItemWidget(item, 2, opacitySlider);
    
    // Blend mode combo
    QComboBox* blendCombo = new QComboBox();
    blendCombo->addItems({"Normal", "Multiply", "Screen", "Overlay", "Soft Light", "Hard Light", "Color Dodge", "Color Burn", "Darken", "Lighten", "Difference", "Exclusion", "Hue", "Saturation", "Color", "Luminosity"});
    blendCombo->setStyleSheet("QComboBox { background-color: #444; color: white; border: 1px solid #666; border-radius: 3px; padding: 2px; } QComboBox::drop-down { border: none; } QComboBox::down-arrow { image: none; border-left: 5px solid transparent; border-right: 5px solid transparent; border-top: 5px solid white; }");
    m_layerTree->setItemWidget(item, 3, blendCombo);
    
    layerInfo.treeItem = item;
    m_layers.append(layerInfo);
    
    // Connect signals
    connect(visibilityCheckBox, &QCheckBox::toggled, this, [this, index = m_layers.size() - 1](bool checked) {
        setLayerVisible(index, checked);
    });
    
    connect(opacitySlider, &QSlider::valueChanged, this, [this, index = m_layers.size() - 1](int value) {
        setLayerOpacity(index, value / 100.0);
    });
    
    connect(blendCombo, QOverload<const QString&>::of(&QComboBox::currentTextChanged), this, [this, index = m_layers.size() - 1](const QString& text) {
        setLayerBlendMode(index, text);
    });
    
    // Select the new layer
    selectLayer(m_layers.size() - 1);
    
    qDebug() << "Added layer:" << name << "Total layers:" << m_layers.size();
}

void LayerWidget::removeLayer(int index)
{
    if (index < 0 || index >= m_layers.size()) return;
    
    // Remove from tree
    QTreeWidgetItem* item = m_layers[index].treeItem;
    m_layerTree->takeTopLevelItem(m_layerTree->indexOfTopLevelItem(item));
    delete item;
    
    // Remove from list
    m_layers.removeAt(index);
    
    // Update indices for remaining layers
    for (int i = index; i < m_layers.size(); ++i) {
        // Reconnect signals with new indices
        // This is a simplified approach - in a real app you'd use a more robust system
    }
    
    // Select a valid layer
    if (m_layers.isEmpty()) {
        m_selectedLayerIndex = -1;
    } else {
        selectLayer(qMin(index, m_layers.size() - 1));
    }
    
    // Emit signal to notify canvas that layers changed
    emit layerSelectionChanged(m_selectedLayerIndex);
    
    qDebug() << "Removed layer at index:" << index << "Total layers:" << m_layers.size();
}

void LayerWidget::clearLayers()
{
    m_layerTree->clear();
    m_layers.clear();
    m_selectedLayerIndex = -1;
}

void LayerWidget::setLayerVisible(int index, bool visible)
{
    if (index < 0 || index >= m_layers.size()) return;
    
    m_layers[index].visible = visible;
    emit layerVisibilityChanged(index, visible);
}

void LayerWidget::setLayerOpacity(int index, qreal opacity)
{
    if (index < 0 || index >= m_layers.size()) return;
    
    m_layers[index].opacity = opacity;
    emit layerOpacityChanged(index, opacity);
}

void LayerWidget::setLayerBlendMode(int index, const QString& blendMode)
{
    if (index < 0 || index >= m_layers.size()) return;
    
    m_layers[index].blendMode = blendMode;
    emit layerBlendModeChanged(index, blendMode);
}

int LayerWidget::getSelectedLayerIndex() const
{
    return m_selectedLayerIndex;
}

int LayerWidget::getLayerCount() const
{
    return m_layers.size();
}

void LayerWidget::selectLayer(int index)
{
    if (index < 0 || index >= m_layers.size()) return;
    
    m_selectedLayerIndex = index;
    m_layerTree->setCurrentItem(m_layers[index].treeItem);
    emit layerSelectionChanged(index);
}

void LayerWidget::onLayerSelectionChanged()
{
    QList<QTreeWidgetItem*> selectedItems = m_layerTree->selectedItems();
    if (selectedItems.isEmpty()) {
        m_selectedLayerIndex = -1;
        return;
    }
    
    QTreeWidgetItem* selectedItem = selectedItems.first();
    for (int i = 0; i < m_layers.size(); ++i) {
        if (m_layers[i].treeItem == selectedItem) {
            m_selectedLayerIndex = i;
            emit layerSelectionChanged(i);
            break;
        }
    }
}

void LayerWidget::onLayerVisibilityToggled()
{
    // Handled by individual checkboxes
}

void LayerWidget::onLayerOpacityChanged()
{
    // Handled by individual sliders
}

void LayerWidget::onLayerBlendModeChanged()
{
    // Handled by individual combos
}

void LayerWidget::onAddLayerClicked()
{
    addLayer("New Layer", true);
}

void LayerWidget::onRemoveLayerClicked()
{
    if (m_selectedLayerIndex >= 0 && m_selectedLayerIndex < m_layers.size()) {
        removeLayer(m_selectedLayerIndex);
    }
}

void LayerWidget::onDuplicateLayerClicked()
{
    if (m_selectedLayerIndex >= 0 && m_selectedLayerIndex < m_layers.size()) {
        const LayerInfo& original = m_layers[m_selectedLayerIndex];
        addLayer(original.name + " Copy", original.visible);
        
        // Copy properties
        int newIndex = m_layers.size() - 1;
        setLayerOpacity(newIndex, original.opacity);
        setLayerBlendMode(newIndex, original.blendMode);
    }
}

void LayerWidget::onLayerPropertiesClicked()
{
    if (m_selectedLayerIndex >= 0 && m_selectedLayerIndex < m_layers.size()) {
        qDebug() << "Layer properties for:" << m_layers[m_selectedLayerIndex].name;
        // TODO: Show properties dialog
    }
}

void LayerWidget::onContextMenuRequested(const QPoint& pos)
{
    QTreeWidgetItem* item = m_layerTree->itemAt(pos);
    if (item) {
        // Find the layer index
        for (int i = 0; i < m_layers.size(); ++i) {
            if (m_layers[i].treeItem == item) {
                m_selectedLayerIndex = i;
                break;
            }
        }
    }
    
    m_contextMenu->exec(m_layerTree->mapToGlobal(pos));
}

void LayerWidget::updateLayerItem(int index)
{
    if (index < 0 || index >= m_layers.size()) return;
    
    const LayerInfo& layer = m_layers[index];
    QTreeWidgetItem* item = layer.treeItem;
    
    if (item) {
        // Update visibility checkbox
        QCheckBox* checkbox = qobject_cast<QCheckBox*>(m_layerTree->itemWidget(item, 0));
        if (checkbox) checkbox->setChecked(layer.visible);
        
        // Update opacity slider
        QSlider* slider = qobject_cast<QSlider*>(m_layerTree->itemWidget(item, 2));
        if (slider) slider->setValue(qRound(layer.opacity * 100));
        
        // Update blend mode combo
        QComboBox* combo = qobject_cast<QComboBox*>(m_layerTree->itemWidget(item, 3));
        if (combo) combo->setCurrentText(layer.blendMode);
    }
}

} // namespace ui
