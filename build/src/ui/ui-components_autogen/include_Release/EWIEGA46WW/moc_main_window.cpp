/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/ui/main_window.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2ui10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto ui::MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN2ui10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ui::MainWindow",
        "newDocument",
        "",
        "openDocument",
        "saveDocument",
        "saveAsDocument",
        "exportDocument",
        "quit",
        "undo",
        "redo",
        "cut",
        "copy",
        "paste",
        "preferences",
        "showLayerPanel",
        "show",
        "showToolPanel",
        "showColorPanel",
        "showBrushPanel",
        "zoomIn",
        "zoomOut",
        "fitToWindow",
        "actualSize",
        "about",
        "aboutQt"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'newDocument'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openDocument'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveDocument'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveAsDocument'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'exportDocument'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'quit'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'undo'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'redo'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'cut'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'copy'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'paste'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'preferences'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showLayerPanel'
        QtMocHelpers::SlotData<void(bool)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
        // Slot 'showToolPanel'
        QtMocHelpers::SlotData<void(bool)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
        // Slot 'showColorPanel'
        QtMocHelpers::SlotData<void(bool)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
        // Slot 'showBrushPanel'
        QtMocHelpers::SlotData<void(bool)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
        // Slot 'zoomIn'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'zoomOut'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'fitToWindow'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'actualSize'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'about'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'aboutQt'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN2ui10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ui::MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2ui10MainWindowE_t>.metaTypes,
    nullptr
} };

void ui::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->newDocument(); break;
        case 1: _t->openDocument(); break;
        case 2: _t->saveDocument(); break;
        case 3: _t->saveAsDocument(); break;
        case 4: _t->exportDocument(); break;
        case 5: _t->quit(); break;
        case 6: _t->undo(); break;
        case 7: _t->redo(); break;
        case 8: _t->cut(); break;
        case 9: _t->copy(); break;
        case 10: _t->paste(); break;
        case 11: _t->preferences(); break;
        case 12: _t->showLayerPanel((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->showToolPanel((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 14: _t->showColorPanel((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 15: _t->showBrushPanel((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 16: _t->zoomIn(); break;
        case 17: _t->zoomOut(); break;
        case 18: _t->fitToWindow(); break;
        case 19: _t->actualSize(); break;
        case 20: _t->about(); break;
        case 21: _t->aboutQt(); break;
        default: ;
        }
    }
}

const QMetaObject *ui::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ui::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ui::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 22;
    }
    return _id;
}
QT_WARNING_POP
