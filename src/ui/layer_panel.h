#pragma once

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>

namespace core {
    class Document;
}

namespace ui {

class LayerPanel : public QWidget {
    Q_OBJECT

public:
    explicit LayerPanel(QWidget* parent = nullptr);
    
    void setDocument(core::Document* document);
    void refreshLayers();

signals:
    void layerSelectionChanged(int layerIndex);
    void layerVisibilityChanged(int layerIndex, bool visible);
    void layerAdded(int layerIndex);
    void layerDeleted(int layerIndex);

private slots:
    void onAddLayer();
    void onDeleteLayer();
    void onDuplicateLayer();
    void onLayerVisibilityChanged(QListWidgetItem* item);

private:
    void setupUI();
    void setupConnections();
    void updateLayerList();

    core::Document* m_document;
    QListWidget* m_layerList;
    QPushButton* m_addLayerBtn;
    QPushButton* m_deleteLayerBtn;
    QPushButton* m_duplicateLayerBtn;
};

} // namespace ui
