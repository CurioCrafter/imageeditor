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
struct qt_meta_tag_ZN4core6ILayerE_t {};
} // unnamed namespace

template <> constexpr inline auto core::ILayer::qt_create_metaobjectdata<qt_meta_tag_ZN4core6ILayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::ILayer",
        "nameChanged",
        "",
        "name",
        "visibilityChanged",
        "visible",
        "opacityChanged",
        "opacity",
        "transformChanged",
        "transform"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'nameChanged'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'visibilityChanged'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Signal 'opacityChanged'
        QtMocHelpers::SignalData<void(qreal)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QReal, 7 },
        }}),
        // Signal 'transformChanged'
        QtMocHelpers::SignalData<void(const QTransform &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QTransform, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ILayer, qt_meta_tag_ZN4core6ILayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::ILayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core6ILayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core6ILayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core6ILayerE_t>.metaTypes,
    nullptr
} };

void core::ILayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ILayer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->nameChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->visibilityChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->opacityChanged((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1]))); break;
        case 3: _t->transformChanged((*reinterpret_cast< std::add_pointer_t<QTransform>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ILayer::*)(const QString & )>(_a, &ILayer::nameChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ILayer::*)(bool )>(_a, &ILayer::visibilityChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ILayer::*)(qreal )>(_a, &ILayer::opacityChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ILayer::*)(const QTransform & )>(_a, &ILayer::transformChanged, 3))
            return;
    }
}

const QMetaObject *core::ILayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::ILayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core6ILayerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int core::ILayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void core::ILayer::nameChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void core::ILayer::visibilityChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void core::ILayer::opacityChanged(qreal _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void core::ILayer::transformChanged(const QTransform & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN4core11RasterLayerE_t {};
} // unnamed namespace

template <> constexpr inline auto core::RasterLayer::qt_create_metaobjectdata<qt_meta_tag_ZN4core11RasterLayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::RasterLayer",
        "imageChanged",
        "",
        "sizeChanged",
        "size"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'imageChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sizeChanged'
        QtMocHelpers::SignalData<void(const QSize &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QSize, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RasterLayer, qt_meta_tag_ZN4core11RasterLayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::RasterLayer::staticMetaObject = { {
    QMetaObject::SuperData::link<ILayer::staticMetaObject>(),
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
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->imageChanged(); break;
        case 1: _t->sizeChanged((*reinterpret_cast< std::add_pointer_t<QSize>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (RasterLayer::*)()>(_a, &RasterLayer::imageChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (RasterLayer::*)(const QSize & )>(_a, &RasterLayer::sizeChanged, 1))
            return;
    }
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
    return ILayer::qt_metacast(_clname);
}

int core::RasterLayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ILayer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void core::RasterLayer::imageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void core::RasterLayer::sizeChanged(const QSize & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
