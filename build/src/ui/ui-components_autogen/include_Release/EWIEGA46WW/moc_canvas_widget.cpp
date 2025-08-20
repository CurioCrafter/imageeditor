/****************************************************************************
** Meta object code from reading C++ file 'canvas_widget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src/ui/canvas_widget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'canvas_widget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN2ui12CanvasWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ui::CanvasWidget::qt_create_metaobjectdata<qt_meta_tag_ZN2ui12CanvasWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ui::CanvasWidget",
        "ToolType",
        "Brush",
        "Eraser",
        "Line",
        "Rectangle",
        "Ellipse",
        "Selection",
        "Lasso",
        "MagicWand",
        "Move",
        "Zoom",
        "Pan"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ToolType'
        QtMocHelpers::EnumData<ToolType>(1, 1, QMC::EnumFlags{}).add({
            {    2, ToolType::Brush },
            {    3, ToolType::Eraser },
            {    4, ToolType::Line },
            {    5, ToolType::Rectangle },
            {    6, ToolType::Ellipse },
            {    7, ToolType::Selection },
            {    8, ToolType::Lasso },
            {    9, ToolType::MagicWand },
            {   10, ToolType::Move },
            {   11, ToolType::Zoom },
            {   12, ToolType::Pan },
        }),
    };
    return QtMocHelpers::metaObjectData<CanvasWidget, qt_meta_tag_ZN2ui12CanvasWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ui::CanvasWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui12CanvasWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui12CanvasWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN2ui12CanvasWidgetE_t>.metaTypes,
    nullptr
} };

void ui::CanvasWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CanvasWidget *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *ui::CanvasWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ui::CanvasWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN2ui12CanvasWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ui::CanvasWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
