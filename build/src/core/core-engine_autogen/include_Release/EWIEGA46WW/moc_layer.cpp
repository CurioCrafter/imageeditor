/****************************************************************************
** Meta object code from reading C++ file 'layer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/core/layer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'layer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN4core5LayerE_t {};
} // unnamed namespace

template <> constexpr inline auto core::Layer::qt_create_metaobjectdata<qt_meta_tag_ZN4core5LayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::Layer",
        "propertyChanged",
        "",
        "visibilityChanged",
        "visible",
        "opacityChanged",
        "opacity",
        "blendModeChanged",
        "BlendMode",
        "mode",
        "positionChanged",
        "position",
        "sizeChanged",
        "size",
        "transformChanged",
        "transform",
        "maskChanged",
        "effectsChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'propertyChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'visibilityChanged'
        QtMocHelpers::SignalData<void(bool)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 4 },
        }}),
        // Signal 'opacityChanged'
        QtMocHelpers::SignalData<void(float)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 6 },
        }}),
        // Signal 'blendModeChanged'
        QtMocHelpers::SignalData<void(BlendMode)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'positionChanged'
        QtMocHelpers::SignalData<void(const QPointF &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QPointF, 11 },
        }}),
        // Signal 'sizeChanged'
        QtMocHelpers::SignalData<void(const QSize &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QSize, 13 },
        }}),
        // Signal 'transformChanged'
        QtMocHelpers::SignalData<void(const QTransform &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QTransform, 15 },
        }}),
        // Signal 'maskChanged'
        QtMocHelpers::SignalData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'effectsChanged'
        QtMocHelpers::SignalData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Layer, qt_meta_tag_ZN4core5LayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::Layer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core5LayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core5LayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core5LayerE_t>.metaTypes,
    nullptr
} };

void core::Layer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Layer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->propertyChanged(); break;
        case 1: _t->visibilityChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->opacityChanged((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 3: _t->blendModeChanged((*reinterpret_cast< std::add_pointer_t<BlendMode>>(_a[1]))); break;
        case 4: _t->positionChanged((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1]))); break;
        case 5: _t->sizeChanged((*reinterpret_cast< std::add_pointer_t<QSize>>(_a[1]))); break;
        case 6: _t->transformChanged((*reinterpret_cast< std::add_pointer_t<QTransform>>(_a[1]))); break;
        case 7: _t->maskChanged(); break;
        case 8: _t->effectsChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Layer::*)()>(_a, &Layer::propertyChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Layer::*)(bool )>(_a, &Layer::visibilityChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Layer::*)(float )>(_a, &Layer::opacityChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Layer::*)(BlendMode )>(_a, &Layer::blendModeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Layer::*)(const QPointF & )>(_a, &Layer::positionChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Layer::*)(const QSize & )>(_a, &Layer::sizeChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Layer::*)(const QTransform & )>(_a, &Layer::transformChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Layer::*)()>(_a, &Layer::maskChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Layer::*)()>(_a, &Layer::effectsChanged, 8))
            return;
    }
}

const QMetaObject *core::Layer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::Layer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core5LayerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int core::Layer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void core::Layer::propertyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void core::Layer::visibilityChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void core::Layer::opacityChanged(float _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void core::Layer::blendModeChanged(BlendMode _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void core::Layer::positionChanged(const QPointF & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void core::Layer::sizeChanged(const QSize & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void core::Layer::transformChanged(const QTransform & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void core::Layer::maskChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void core::Layer::effectsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
namespace {
struct qt_meta_tag_ZN4core11RasterLayerE_t {};
} // unnamed namespace

template <> constexpr inline auto core::RasterLayer::qt_create_metaobjectdata<qt_meta_tag_ZN4core11RasterLayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::RasterLayer"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RasterLayer, qt_meta_tag_ZN4core11RasterLayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::RasterLayer::staticMetaObject = { {
    QMetaObject::SuperData::link<Layer::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core11RasterLayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core11RasterLayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core11RasterLayerE_t>.metaTypes,
    nullptr
} };

void core::RasterLayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<RasterLayer *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *core::RasterLayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::RasterLayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core11RasterLayerE_t>.strings))
        return static_cast<void*>(this);
    return Layer::qt_metacast(_clname);
}

int core::RasterLayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Layer::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN4core15AdjustmentLayerE_t {};
} // unnamed namespace

template <> constexpr inline auto core::AdjustmentLayer::qt_create_metaobjectdata<qt_meta_tag_ZN4core15AdjustmentLayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::AdjustmentLayer"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AdjustmentLayer, qt_meta_tag_ZN4core15AdjustmentLayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::AdjustmentLayer::staticMetaObject = { {
    QMetaObject::SuperData::link<Layer::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core15AdjustmentLayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core15AdjustmentLayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core15AdjustmentLayerE_t>.metaTypes,
    nullptr
} };

void core::AdjustmentLayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AdjustmentLayer *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *core::AdjustmentLayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::AdjustmentLayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core15AdjustmentLayerE_t>.strings))
        return static_cast<void*>(this);
    return Layer::qt_metacast(_clname);
}

int core::AdjustmentLayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Layer::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN4core9TextLayerE_t {};
} // unnamed namespace

template <> constexpr inline auto core::TextLayer::qt_create_metaobjectdata<qt_meta_tag_ZN4core9TextLayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::TextLayer"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TextLayer, qt_meta_tag_ZN4core9TextLayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::TextLayer::staticMetaObject = { {
    QMetaObject::SuperData::link<Layer::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9TextLayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9TextLayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core9TextLayerE_t>.metaTypes,
    nullptr
} };

void core::TextLayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TextLayer *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *core::TextLayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::TextLayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9TextLayerE_t>.strings))
        return static_cast<void*>(this);
    return Layer::qt_metacast(_clname);
}

int core::TextLayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Layer::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
