/****************************************************************************
** Meta object code from reading C++ file 'ncentralview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/ncentralview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ncentralview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_nCentralView_t {
    QByteArrayData data[14];
    char stringdata0[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nCentralView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nCentralView_t qt_meta_stringdata_nCentralView = {
    {
QT_MOC_LITERAL(0, 0, 12), // "nCentralView"
QT_MOC_LITERAL(1, 13, 6), // "zoomIn"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 7), // "zoomOut"
QT_MOC_LITERAL(4, 29, 7), // "sratate"
QT_MOC_LITERAL(5, 37, 7), // "aratate"
QT_MOC_LITERAL(6, 45, 3), // "vup"
QT_MOC_LITERAL(7, 49, 5), // "vdown"
QT_MOC_LITERAL(8, 55, 5), // "vleft"
QT_MOC_LITERAL(9, 61, 6), // "vright"
QT_MOC_LITERAL(10, 68, 4), // "zoom"
QT_MOC_LITERAL(11, 73, 11), // "scaleFactor"
QT_MOC_LITERAL(12, 85, 9), // "translate"
QT_MOC_LITERAL(13, 95, 5) // "delta"

    },
    "nCentralView\0zoomIn\0\0zoomOut\0sratate\0"
    "aratate\0vup\0vdown\0vleft\0vright\0zoom\0"
    "scaleFactor\0translate\0delta"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nCentralView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    0,   67,    2, 0x0a /* Public */,
       6,    0,   68,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    0,   70,    2, 0x0a /* Public */,
       9,    0,   71,    2, 0x0a /* Public */,
      10,    1,   72,    2, 0x0a /* Public */,
      12,    1,   75,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   11,
    QMetaType::Void, QMetaType::QPointF,   13,

       0        // eod
};

void nCentralView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        nCentralView *_t = static_cast<nCentralView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->zoomIn(); break;
        case 1: _t->zoomOut(); break;
        case 2: _t->sratate(); break;
        case 3: _t->aratate(); break;
        case 4: _t->vup(); break;
        case 5: _t->vdown(); break;
        case 6: _t->vleft(); break;
        case 7: _t->vright(); break;
        case 8: _t->zoom((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->translate((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject nCentralView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_nCentralView.data,
      qt_meta_data_nCentralView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *nCentralView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nCentralView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_nCentralView.stringdata0))
        return static_cast<void*>(const_cast< nCentralView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int nCentralView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
