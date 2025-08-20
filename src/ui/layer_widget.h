#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QAction>

namespace ui {

class LayerWidget : public QWidget {
    Q_OBJECT

public:
    explicit LayerWidget(QWidget* parent = nullptr);
    ~LayerWidget();

    // Layer management
    void addLayer(const QString& name, bool visible = true);
    void removeLayer(int index);
    void clearLayers();
    
    // Layer properties
    void setLayerVisible(int index, bool visible);
    void setLayerOpacity(int index, qreal opacity);
    void setLayerBlendMode(int index, const QString& blendMode);
    
    // Selection
    int getSelectedLayerIndex() const;
    void selectLayer(int index);
    
    // Layer count
    int getLayerCount() const;

signals:
    void layerSelectionChanged(int index);
    void layerVisibilityChanged(int index, bool visible);
    void layerOpacityChanged(int index, qreal opacity);
    void layerBlendModeChanged(int index, const QString& blendMode);

private slots:
    void onLayerSelectionChanged();
    void onLayerVisibilityToggled();
    void onLayerOpacityChanged();
    void onLayerBlendModeChanged();
    void onAddLayerClicked();
    void onRemoveLayerClicked();
    void onDuplicateLayerClicked();
    void onLayerPropertiesClicked();
    void onContextMenuRequested(const QPoint& pos);

private:
    void setupUI();
    void createContextMenu();
    void updateLayerItem(int index);
    
    QTreeWidget* m_layerTree;
    QPushButton* m_addLayerButton;
    QPushButton* m_removeLayerButton;
    QPushButton* m_duplicateLayerButton;
    
    QMenu* m_contextMenu;
    QAction* m_addLayerAction;
    QAction* m_removeLayerAction;
    QAction* m_duplicateLayerAction;
    QAction* m_propertiesAction;
    
    struct LayerInfo {
        QString name;
        bool visible;
        qreal opacity;
        QString blendMode;
        QTreeWidgetItem* treeItem;
    };
    
    QList<LayerInfo> m_layers;
    int m_selectedLayerIndex;
};

} // namespace ui
