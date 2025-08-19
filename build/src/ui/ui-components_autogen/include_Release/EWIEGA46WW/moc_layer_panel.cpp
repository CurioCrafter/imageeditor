/****************************************************************************
** Meta object code from reading C++ file 'layer_panel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/ui/layer_panel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'layer_panel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN2ui10LayerPanelE_t {};
} // unnamed namespace

template <> constexpr inline auto ui::LayerPanel::qt_create_metaobjectdata<qt_meta_tag_ZN2ui10LayerPanelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ui::LayerPanel",
        "layerSelectionChanged",
        "",
        "layerIndex",
        "layerVisibilityChanged",
        "visible",
        "layerAdded",
        "layerDeleted",
        "onAddLayer",
        "onDeleteLayer",
        "onDuplicateLayer",
        "onLayerVisibilityChanged",
        "QListWidgetItem*",
        "item"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'layerSelectionChanged'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'layerVisibilityChanged'
        QtMocHelpers::SignalData<void(int, bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Bool, 5 },
        }}),
        // Signal 'layerAdded'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'layerDeleted'
        QtMocHelpers::SignalData<void(int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onAddLayer'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeleteLayer'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDuplicateLayer'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLayerVisibilityChanged'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LayerPanel, qt_meta_tag_ZN2ui10LayerPanelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ui::LayerPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui10LayerPanelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui10LayerPanelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2ui10LayerPanelE_t>.metaTypes,
    nullptr
} };

void ui::LayerPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LayerPanel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->layerSelectionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->layerVisibilityChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 2: _t->layerAdded((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->layerDeleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onAddLayer(); break;
        case 5: _t->onDeleteLayer(); break;
        case 6: _t->onDuplicateLayer(); break;
        case 7: _t->onLayerVisibilityChanged((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int )>(_a, &LayerPanel::layerSelectionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int , bool )>(_a, &LayerPanel::layerVisibilityChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int )>(_a, &LayerPanel::layerAdded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayerPanel::*)(int )>(_a, &LayerPanel::layerDeleted, 3))
            return;
    }
}

const QMetaObject *ui::LayerPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ui::LayerPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui10LayerPanelE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ui::LayerPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ui::LayerPanel::layerSelectionChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ui::LayerPanel::layerVisibilityChanged(int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void ui::LayerPanel::layerAdded(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void ui::LayerPanel::layerDeleted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
