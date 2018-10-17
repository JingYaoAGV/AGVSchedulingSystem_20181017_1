/****************************************************************************
** Meta object code from reading C++ file 'topoview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/topoview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'topoview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TopoVer_t {
    QByteArrayData data[31];
    char stringdata0[265];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TopoVer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TopoVer_t qt_meta_stringdata_TopoVer = {
    {
QT_MOC_LITERAL(0, 0, 7), // "TopoVer"
QT_MOC_LITERAL(1, 8, 15), // "property_change"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 19), // "QMap<QString,qreal>"
QT_MOC_LITERAL(4, 45, 4), // "prop"
QT_MOC_LITERAL(5, 50, 15), // "position_change"
QT_MOC_LITERAL(6, 66, 14), // "address_change"
QT_MOC_LITERAL(7, 81, 9), // "ver_dirty"
QT_MOC_LITERAL(8, 91, 10), // "set_center"
QT_MOC_LITERAL(9, 102, 6), // "center"
QT_MOC_LITERAL(10, 109, 5), // "set_x"
QT_MOC_LITERAL(11, 115, 1), // "x"
QT_MOC_LITERAL(12, 117, 5), // "set_y"
QT_MOC_LITERAL(13, 123, 1), // "y"
QT_MOC_LITERAL(14, 125, 9), // "set_width"
QT_MOC_LITERAL(15, 135, 5), // "width"
QT_MOC_LITERAL(16, 141, 8), // "set_size"
QT_MOC_LITERAL(17, 150, 1), // "w"
QT_MOC_LITERAL(18, 152, 1), // "h"
QT_MOC_LITERAL(19, 154, 14), // "set_brushColor"
QT_MOC_LITERAL(20, 169, 5), // "color"
QT_MOC_LITERAL(21, 175, 11), // "set_vertype"
QT_MOC_LITERAL(22, 187, 1), // "t"
QT_MOC_LITERAL(23, 189, 13), // "set_callPoint"
QT_MOC_LITERAL(24, 203, 4), // "flag"
QT_MOC_LITERAL(25, 208, 12), // "set_selected"
QT_MOC_LITERAL(26, 221, 7), // "_center"
QT_MOC_LITERAL(27, 229, 6), // "_width"
QT_MOC_LITERAL(28, 236, 12), // "_brush_color"
QT_MOC_LITERAL(29, 249, 9), // "_ver_type"
QT_MOC_LITERAL(30, 259, 5) // "_name"

    },
    "TopoVer\0property_change\0\0QMap<QString,qreal>\0"
    "prop\0position_change\0address_change\0"
    "ver_dirty\0set_center\0center\0set_x\0x\0"
    "set_y\0y\0set_width\0width\0set_size\0w\0h\0"
    "set_brushColor\0color\0set_vertype\0t\0"
    "set_callPoint\0flag\0set_selected\0_center\0"
    "_width\0_brush_color\0_ver_type\0_name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TopoVer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       5,  116, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       5,    1,   82,    2, 0x06 /* Public */,
       6,    1,   85,    2, 0x06 /* Public */,
       7,    0,   88,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   89,    2, 0x0a /* Public */,
      10,    1,   92,    2, 0x0a /* Public */,
      12,    1,   95,    2, 0x0a /* Public */,
      14,    1,   98,    2, 0x0a /* Public */,
      16,    2,  101,    2, 0x0a /* Public */,
      19,    1,  106,    2, 0x0a /* Public */,
      21,    1,  109,    2, 0x0a /* Public */,
      23,    1,  112,    2, 0x0a /* Public */,
      25,    0,  115,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QPointF,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    9,
    QMetaType::Void, QMetaType::QReal,   11,
    QMetaType::Void, QMetaType::QReal,   13,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   17,   18,
    QMetaType::Void, QMetaType::QColor,   20,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Bool,   24,
    QMetaType::Void,

 // properties: name, type, flags
      26, QMetaType::QPoint, 0x00095103,
      27, QMetaType::Int, 0x00095103,
      28, QMetaType::QColor, 0x00095003,
      29, QMetaType::Bool, 0x00095003,
      30, QMetaType::QString, 0x00095001,

       0        // eod
};

void TopoVer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TopoVer *_t = static_cast<TopoVer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->property_change((*reinterpret_cast< QMap<QString,qreal>(*)>(_a[1]))); break;
        case 1: _t->position_change((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 2: _t->address_change((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->ver_dirty(); break;
        case 4: _t->set_center((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 5: _t->set_x((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 6: _t->set_y((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 7: _t->set_width((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->set_size((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->set_brushColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 10: _t->set_vertype((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->set_callPoint((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->set_selected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TopoVer::*_t)(QMap<QString,qreal> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TopoVer::property_change)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TopoVer::*_t)(QPointF );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TopoVer::position_change)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TopoVer::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TopoVer::address_change)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TopoVer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TopoVer::ver_dirty)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TopoVer *_t = static_cast<TopoVer *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QPoint*>(_v) = _t->get_center(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->get_width(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = _t->get_brushColor(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->is_callPoint(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->get_name(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TopoVer *_t = static_cast<TopoVer *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_center(*reinterpret_cast< QPoint*>(_v)); break;
        case 1: _t->set_width(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->set_brushColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: _t->set_callPoint(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject TopoVer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TopoVer.data,
      qt_meta_data_TopoVer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TopoVer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TopoVer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TopoVer.stringdata0))
        return static_cast<void*>(const_cast< TopoVer*>(this));
    if (!strcmp(_clname, "QGraphicsEllipseItem"))
        return static_cast< QGraphicsEllipseItem*>(const_cast< TopoVer*>(this));
    return QObject::qt_metacast(_clname);
}

int TopoVer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TopoVer::property_change(QMap<QString,qreal> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TopoVer::position_change(QPointF _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TopoVer::address_change(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TopoVer::ver_dirty()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
struct qt_meta_stringdata_TopoEdge_t {
    QByteArrayData data[19];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TopoEdge_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TopoEdge_t qt_meta_stringdata_TopoEdge = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TopoEdge"
QT_MOC_LITERAL(1, 9, 15), // "property_change"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 19), // "QMap<QString,qreal>"
QT_MOC_LITERAL(4, 46, 12), // "set_penColor"
QT_MOC_LITERAL(5, 59, 5), // "color"
QT_MOC_LITERAL(6, 65, 12), // "set_penWidth"
QT_MOC_LITERAL(7, 78, 10), // "n_penWidth"
QT_MOC_LITERAL(8, 89, 8), // "set_dash"
QT_MOC_LITERAL(9, 98, 4), // "flag"
QT_MOC_LITERAL(10, 103, 10), // "set_weight"
QT_MOC_LITERAL(11, 114, 1), // "w"
QT_MOC_LITERAL(12, 116, 10), // "_pen_width"
QT_MOC_LITERAL(13, 127, 10), // "_pen_color"
QT_MOC_LITERAL(14, 138, 5), // "_name"
QT_MOC_LITERAL(15, 144, 5), // "_dash"
QT_MOC_LITERAL(16, 150, 7), // "_weight"
QT_MOC_LITERAL(17, 158, 11), // "_start_node"
QT_MOC_LITERAL(18, 170, 9) // "_end_node"

    },
    "TopoEdge\0property_change\0\0QMap<QString,qreal>\0"
    "set_penColor\0color\0set_penWidth\0"
    "n_penWidth\0set_dash\0flag\0set_weight\0"
    "w\0_pen_width\0_pen_color\0_name\0_dash\0"
    "_weight\0_start_node\0_end_node"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TopoEdge[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       7,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   42,    2, 0x0a /* Public */,
       6,    1,   45,    2, 0x0a /* Public */,
       8,    1,   48,    2, 0x0a /* Public */,
      10,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::QReal,   11,

 // properties: name, type, flags
      12, QMetaType::Int, 0x00095003,
      13, QMetaType::QColor, 0x00095003,
      14, QMetaType::QString, 0x00095001,
      15, QMetaType::Bool, 0x00095103,
      16, QMetaType::QReal, 0x00095103,
      17, QMetaType::Int, 0x00095001,
      18, QMetaType::Int, 0x00095001,

       0        // eod
};

void TopoEdge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TopoEdge *_t = static_cast<TopoEdge *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->property_change((*reinterpret_cast< QMap<QString,qreal>(*)>(_a[1]))); break;
        case 1: _t->set_penColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->set_penWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->set_dash((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->set_weight((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TopoEdge::*_t)(QMap<QString,qreal> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TopoEdge::property_change)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        TopoEdge *_t = static_cast<TopoEdge *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->get_penWidth(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = _t->get_penColor(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->get_name(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->get_dash(); break;
        case 4: *reinterpret_cast< qreal*>(_v) = _t->get_weight(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->get_startID(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->get_endID(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        TopoEdge *_t = static_cast<TopoEdge *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_penWidth(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->set_penColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: _t->set_dash(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->set_weight(*reinterpret_cast< qreal*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject TopoEdge::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_TopoEdge.data,
      qt_meta_data_TopoEdge,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TopoEdge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TopoEdge::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TopoEdge.stringdata0))
        return static_cast<void*>(const_cast< TopoEdge*>(this));
    return QGraphicsObject::qt_metacast(_clname);
}

int TopoEdge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TopoEdge::property_change(QMap<QString,qreal> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_TopoView_t {
    QByteArrayData data[6];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TopoView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TopoView_t qt_meta_stringdata_TopoView = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TopoView"
QT_MOC_LITERAL(1, 9, 16), // "step_progressBar"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 9), // "set_dirty"
QT_MOC_LITERAL(4, 37, 16), // "app_stateChanged"
QT_MOC_LITERAL(5, 54, 5) // "state"

    },
    "TopoView\0step_progressBar\0\0set_dirty\0"
    "app_stateChanged\0state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TopoView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       3,    0,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   33,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void TopoView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TopoView *_t = static_cast<TopoView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->step_progressBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->set_dirty(); break;
        case 2: _t->app_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TopoView::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TopoView::step_progressBar)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TopoView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TopoView::set_dirty)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TopoView::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TopoView.data,
      qt_meta_data_TopoView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TopoView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TopoView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TopoView.stringdata0))
        return static_cast<void*>(const_cast< TopoView*>(this));
    return QObject::qt_metacast(_clname);
}

int TopoView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TopoView::step_progressBar(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TopoView::set_dirty()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
