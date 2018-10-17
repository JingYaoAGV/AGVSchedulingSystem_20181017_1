/****************************************************************************
** Meta object code from reading C++ file 'wmsinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/wmsinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wmsinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WMSInterface_t {
    QByteArrayData data[13];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WMSInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WMSInterface_t qt_meta_stringdata_WMSInterface = {
    {
QT_MOC_LITERAL(0, 0, 12), // "WMSInterface"
QT_MOC_LITERAL(1, 13, 11), // "new_request"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 2), // "id"
QT_MOC_LITERAL(4, 29, 14), // "QPair<int,int>"
QT_MOC_LITERAL(5, 44, 8), // "priority"
QT_MOC_LITERAL(6, 53, 13), // "inquiry_state"
QT_MOC_LITERAL(7, 67, 14), // "delete_request"
QT_MOC_LITERAL(8, 82, 16), // "mission_finished"
QT_MOC_LITERAL(9, 99, 12), // "is_connected"
QT_MOC_LITERAL(10, 112, 8), // "readData"
QT_MOC_LITERAL(11, 121, 9), // "connected"
QT_MOC_LITERAL(12, 131, 12) // "disconnected"

    },
    "WMSInterface\0new_request\0\0id\0"
    "QPair<int,int>\0priority\0inquiry_state\0"
    "delete_request\0mission_finished\0"
    "is_connected\0readData\0connected\0"
    "disconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WMSInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   54,    2, 0x06 /* Public */,
       6,    1,   63,    2, 0x06 /* Public */,
       7,    1,   66,    2, 0x06 /* Public */,
       8,    1,   69,    2, 0x06 /* Public */,
       9,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   75,    2, 0x0a /* Public */,
      11,    0,   76,    2, 0x0a /* Public */,
      12,    0,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 4, 0x80000000 | 4, QMetaType::Int,    3,    2,    2,    5,
    QMetaType::Void, QMetaType::ULongLong,    3,
    QMetaType::Void, QMetaType::ULongLong,    3,
    QMetaType::Void, QMetaType::ULongLong,    3,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WMSInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WMSInterface *_t = static_cast<WMSInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->new_request((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QPair<int,int>(*)>(_a[2])),(*reinterpret_cast< QPair<int,int>(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->inquiry_state((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 2: _t->delete_request((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 3: _t->mission_finished((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 4: _t->is_connected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->readData(); break;
        case 6: _t->connected(); break;
        case 7: _t->disconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WMSInterface::*_t)(quint64 , QPair<int,int> , QPair<int,int> , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WMSInterface::new_request)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (WMSInterface::*_t)(quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WMSInterface::inquiry_state)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (WMSInterface::*_t)(quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WMSInterface::delete_request)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (WMSInterface::*_t)(quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WMSInterface::mission_finished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (WMSInterface::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WMSInterface::is_connected)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject WMSInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WMSInterface.data,
      qt_meta_data_WMSInterface,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WMSInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WMSInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WMSInterface.stringdata0))
        return static_cast<void*>(const_cast< WMSInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int WMSInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void WMSInterface::new_request(quint64 _t1, QPair<int,int> _t2, QPair<int,int> _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WMSInterface::inquiry_state(quint64 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WMSInterface::delete_request(quint64 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WMSInterface::mission_finished(quint64 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WMSInterface::is_connected(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
