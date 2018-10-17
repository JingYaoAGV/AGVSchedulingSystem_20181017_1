/****************************************************************************
** Meta object code from reading C++ file 'requestmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/requestmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'requestmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RequestManager_t {
    QByteArrayData data[14];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RequestManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RequestManager_t qt_meta_stringdata_RequestManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "RequestManager"
QT_MOC_LITERAL(1, 15, 7), // "request"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 14), // "QPair<int,int>"
QT_MOC_LITERAL(4, 39, 12), // "serial_write"
QT_MOC_LITERAL(5, 52, 4), // "data"
QT_MOC_LITERAL(6, 57, 8), // "recvData"
QT_MOC_LITERAL(7, 66, 23), // "QPair<QByteArray,uchar>"
QT_MOC_LITERAL(8, 90, 4), // "addr"
QT_MOC_LITERAL(9, 95, 16), // "app_stateChanged"
QT_MOC_LITERAL(10, 112, 5), // "state"
QT_MOC_LITERAL(11, 118, 20), // "request_accomplished"
QT_MOC_LITERAL(12, 139, 5), // "start"
QT_MOC_LITERAL(13, 145, 3) // "end"

    },
    "RequestManager\0request\0\0QPair<int,int>\0"
    "serial_write\0data\0recvData\0"
    "QPair<QByteArray,uchar>\0addr\0"
    "app_stateChanged\0state\0request_accomplished\0"
    "start\0end"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RequestManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       4,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   47,    2, 0x0a /* Public */,
       9,    1,   50,    2, 0x0a /* Public */,
      11,    2,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QByteArray,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   12,   13,

       0        // eod
};

void RequestManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RequestManager *_t = static_cast<RequestManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->request((*reinterpret_cast< QPair<int,int>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->serial_write((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->recvData((*reinterpret_cast< QPair<QByteArray,uchar>(*)>(_a[1]))); break;
        case 3: _t->app_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->request_accomplished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RequestManager::*_t)(QPair<int,int> , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RequestManager::request)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RequestManager::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RequestManager::serial_write)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject RequestManager::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RequestManager.data,
      qt_meta_data_RequestManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RequestManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RequestManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RequestManager.stringdata0))
        return static_cast<void*>(const_cast< RequestManager*>(this));
    return QWidget::qt_metacast(_clname);
}

int RequestManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void RequestManager::request(QPair<int,int> _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RequestManager::serial_write(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
