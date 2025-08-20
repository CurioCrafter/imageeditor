/****************************************************************************
** Meta object code from reading C++ file 'document.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/core/document.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'document.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN4core8DocumentE_t {};
} // unnamed namespace

template <> constexpr inline auto core::Document::qt_create_metaobjectdata<qt_meta_tag_ZN4core8DocumentE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::Document",
        "nameChanged",
        "",
        "name",
        "sizeChanged",
        "size",
        "backgroundColorChanged",
        "color",
        "modifiedChanged",
        "modified",
        "layerTreeChanged",
        "documentChanged",
        "selectionChanged",
        "onLayerTreeChanged",
        "onCommandExecuted",
        "ICommand*",
        "command",
        "onCommandUndone",
        "onCommandRedone"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'nameChanged'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'sizeChanged'
        QtMocHelpers::SignalData<void(const QSize &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QSize, 5 },
        }}),
        // Signal 'backgroundColorChanged'
        QtMocHelpers::SignalData<void(const QColor &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QColor, 7 },
        }}),
        // Signal 'modifiedChanged'
        QtMocHelpers::SignalData<void(bool)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Signal 'layerTreeChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'documentChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'selectionChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onLayerTreeChanged'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCommandExecuted'
        QtMocHelpers::SlotData<void(ICommand *)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 15, 16 },
        }}),
        // Slot 'onCommandUndone'
        QtMocHelpers::SlotData<void(ICommand *)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 15, 16 },
        }}),
        // Slot 'onCommandRedone'
        QtMocHelpers::SlotData<void(ICommand *)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 15, 16 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Document, qt_meta_tag_ZN4core8DocumentE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::Document::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core8DocumentE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core8DocumentE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core8DocumentE_t>.metaTypes,
    nullptr
} };

void core::Document::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Document *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->nameChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->sizeChanged((*reinterpret_cast< std::add_pointer_t<QSize>>(_a[1]))); break;
        case 2: _t->backgroundColorChanged((*reinterpret_cast< std::add_pointer_t<QColor>>(_a[1]))); break;
        case 3: _t->modifiedChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->layerTreeChanged(); break;
        case 5: _t->documentChanged(); break;
        case 6: _t->selectionChanged(); break;
        case 7: _t->onLayerTreeChanged(); break;
        case 8: _t->onCommandExecuted((*reinterpret_cast< std::add_pointer_t<ICommand*>>(_a[1]))); break;
        case 9: _t->onCommandUndone((*reinterpret_cast< std::add_pointer_t<ICommand*>>(_a[1]))); break;
        case 10: _t->onCommandRedone((*reinterpret_cast< std::add_pointer_t<ICommand*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Document::*)(const QString & )>(_a, &Document::nameChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Document::*)(const QSize & )>(_a, &Document::sizeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Document::*)(const QColor & )>(_a, &Document::backgroundColorChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Document::*)(bool )>(_a, &Document::modifiedChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Document::*)()>(_a, &Document::layerTreeChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Document::*)()>(_a, &Document::documentChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Document::*)()>(_a, &Document::selectionChanged, 6))
            return;
    }
}

const QMetaObject *core::Document::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::Document::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core8DocumentE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int core::Document::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void core::Document::nameChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void core::Document::sizeChanged(const QSize & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void core::Document::backgroundColorChanged(const QColor & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void core::Document::modifiedChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void core::Document::layerTreeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void core::Document::documentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void core::Document::selectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
namespace {
struct qt_meta_tag_ZN4core6CanvasE_t {};
} // unnamed namespace

template <> constexpr inline auto core::Canvas::qt_create_metaobjectdata<qt_meta_tag_ZN4core6CanvasE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::Canvas",
        "viewportChanged",
        "",
        "viewport",
        "viewTransformChanged",
        "transform",
        "zoomChanged",
        "zoom",
        "panOffsetChanged",
        "offset",
        "selectionRectChanged",
        "rect",
        "gridSettingsChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'viewportChanged'
        QtMocHelpers::SignalData<void(const QRect &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QRect, 3 },
        }}),
        // Signal 'viewTransformChanged'
        QtMocHelpers::SignalData<void(const QTransform &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QTransform, 5 },
        }}),
        // Signal 'zoomChanged'
        QtMocHelpers::SignalData<void(double)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 7 },
        }}),
        // Signal 'panOffsetChanged'
        QtMocHelpers::SignalData<void(const QPoint &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QPoint, 9 },
        }}),
        // Signal 'selectionRectChanged'
        QtMocHelpers::SignalData<void(const QRect &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QRect, 11 },
        }}),
        // Signal 'gridSettingsChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Canvas, qt_meta_tag_ZN4core6CanvasE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::Canvas::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core6CanvasE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core6CanvasE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core6CanvasE_t>.metaTypes,
    nullptr
} };

void core::Canvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Canvas *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->viewportChanged((*reinterpret_cast< std::add_pointer_t<QRect>>(_a[1]))); break;
        case 1: _t->viewTransformChanged((*reinterpret_cast< std::add_pointer_t<QTransform>>(_a[1]))); break;
        case 2: _t->zoomChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->panOffsetChanged((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 4: _t->selectionRectChanged((*reinterpret_cast< std::add_pointer_t<QRect>>(_a[1]))); break;
        case 5: _t->gridSettingsChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(const QRect & )>(_a, &Canvas::viewportChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(const QTransform & )>(_a, &Canvas::viewTransformChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(double )>(_a, &Canvas::zoomChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(const QPoint & )>(_a, &Canvas::panOffsetChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(const QRect & )>(_a, &Canvas::selectionRectChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)()>(_a, &Canvas::gridSettingsChanged, 5))
            return;
    }
}

const QMetaObject *core::Canvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::Canvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core6CanvasE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int core::Canvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void core::Canvas::viewportChanged(const QRect & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void core::Canvas::viewTransformChanged(const QTransform & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void core::Canvas::zoomChanged(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void core::Canvas::panOffsetChanged(const QPoint & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void core::Canvas::selectionRectChanged(const QRect & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void core::Canvas::gridSettingsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
namespace {
struct qt_meta_tag_ZN4core9SelectionE_t {};
} // unnamed namespace

template <> constexpr inline auto core::Selection::qt_create_metaobjectdata<qt_meta_tag_ZN4core9SelectionE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::Selection",
        "selectionChanged",
        "",
        "maskChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'selectionChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'maskChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Selection, qt_meta_tag_ZN4core9SelectionE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::Selection::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9SelectionE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9SelectionE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core9SelectionE_t>.metaTypes,
    nullptr
} };

void core::Selection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Selection *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->selectionChanged(); break;
        case 1: _t->maskChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Selection::*)()>(_a, &Selection::selectionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Selection::*)()>(_a, &Selection::maskChanged, 1))
            return;
    }
}

const QMetaObject *core::Selection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::Selection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9SelectionE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int core::Selection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void core::Selection::selectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void core::Selection::maskChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
