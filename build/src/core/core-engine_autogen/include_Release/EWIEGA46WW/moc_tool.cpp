/****************************************************************************
** Meta object code from reading C++ file 'tool.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/core/tool.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tool.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN4core4ToolE_t {};
} // unnamed namespace

template <> constexpr inline auto core::Tool::qt_create_metaobjectdata<qt_meta_tag_ZN4core4ToolE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::Tool",
        "activated",
        "",
        "deactivated",
        "optionsChanged",
        "cursorChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'activated'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'deactivated'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'optionsChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cursorChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Tool, qt_meta_tag_ZN4core4ToolE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::Tool::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core4ToolE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core4ToolE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core4ToolE_t>.metaTypes,
    nullptr
} };

void core::Tool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Tool *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->activated(); break;
        case 1: _t->deactivated(); break;
        case 2: _t->optionsChanged(); break;
        case 3: _t->cursorChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Tool::*)()>(_a, &Tool::activated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tool::*)()>(_a, &Tool::deactivated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tool::*)()>(_a, &Tool::optionsChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Tool::*)()>(_a, &Tool::cursorChanged, 3))
            return;
    }
}

const QMetaObject *core::Tool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::Tool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core4ToolE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int core::Tool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void core::Tool::activated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void core::Tool::deactivated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void core::Tool::optionsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void core::Tool::cursorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
namespace {
struct qt_meta_tag_ZN4core9BrushToolE_t {};
} // unnamed namespace

template <> constexpr inline auto core::BrushTool::qt_create_metaobjectdata<qt_meta_tag_ZN4core9BrushToolE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::BrushTool"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BrushTool, qt_meta_tag_ZN4core9BrushToolE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::BrushTool::staticMetaObject = { {
    QMetaObject::SuperData::link<Tool::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9BrushToolE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9BrushToolE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core9BrushToolE_t>.metaTypes,
    nullptr
} };

void core::BrushTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BrushTool *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *core::BrushTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::BrushTool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core9BrushToolE_t>.strings))
        return static_cast<void*>(this);
    return Tool::qt_metacast(_clname);
}

int core::BrushTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tool::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN4core13SelectionToolE_t {};
} // unnamed namespace

template <> constexpr inline auto core::SelectionTool::qt_create_metaobjectdata<qt_meta_tag_ZN4core13SelectionToolE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::SelectionTool"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SelectionTool, qt_meta_tag_ZN4core13SelectionToolE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::SelectionTool::staticMetaObject = { {
    QMetaObject::SuperData::link<Tool::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core13SelectionToolE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core13SelectionToolE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core13SelectionToolE_t>.metaTypes,
    nullptr
} };

void core::SelectionTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SelectionTool *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *core::SelectionTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::SelectionTool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core13SelectionToolE_t>.strings))
        return static_cast<void*>(this);
    return Tool::qt_metacast(_clname);
}

int core::SelectionTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tool::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN4core8MoveToolE_t {};
} // unnamed namespace

template <> constexpr inline auto core::MoveTool::qt_create_metaobjectdata<qt_meta_tag_ZN4core8MoveToolE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "core::MoveTool"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MoveTool, qt_meta_tag_ZN4core8MoveToolE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject core::MoveTool::staticMetaObject = { {
    QMetaObject::SuperData::link<Tool::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core8MoveToolE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core8MoveToolE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4core8MoveToolE_t>.metaTypes,
    nullptr
} };

void core::MoveTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MoveTool *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *core::MoveTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *core::MoveTool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4core8MoveToolE_t>.strings))
        return static_cast<void*>(this);
    return Tool::qt_metacast(_clname);
}

int core::MoveTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Tool::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
