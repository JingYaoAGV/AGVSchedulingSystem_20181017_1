/****************************************************************************
** Meta object code from reading C++ file 'tcpserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/tcpserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TcpServer_t {
    QByteArrayData data[19];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpServer_t qt_meta_stringdata_TcpServer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TcpServer"
QT_MOC_LITERAL(1, 10, 13), // "connectClient"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 8), // "readData"
QT_MOC_LITERAL(4, 34, 14), // "sockDisConnect"
QT_MOC_LITERAL(5, 49, 8), // "sentData"
QT_MOC_LITERAL(6, 58, 5), // "array"
QT_MOC_LITERAL(7, 64, 14), // "sentDisConnect"
QT_MOC_LITERAL(8, 79, 1), // "i"
QT_MOC_LITERAL(9, 81, 15), // "connectToSystem"
QT_MOC_LITERAL(10, 97, 16), // "fangzhenAGVxinxi"
QT_MOC_LITERAL(11, 114, 5), // "clear"
QT_MOC_LITERAL(12, 120, 18), // "sockDisConnectSlot"
QT_MOC_LITERAL(13, 139, 6), // "handle"
QT_MOC_LITERAL(14, 146, 2), // "ip"
QT_MOC_LITERAL(15, 149, 4), // "prot"
QT_MOC_LITERAL(16, 154, 8), // "QThread*"
QT_MOC_LITERAL(17, 163, 2), // "th"
QT_MOC_LITERAL(18, 166, 15) // "receviedFormAGV"

    },
    "TcpServer\0connectClient\0\0readData\0"
    "sockDisConnect\0sentData\0array\0"
    "sentDisConnect\0i\0connectToSystem\0"
    "fangzhenAGVxinxi\0clear\0sockDisConnectSlot\0"
    "handle\0ip\0prot\0QThread*\0th\0receviedFormAGV"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   69,    2, 0x06 /* Public */,
       3,    4,   76,    2, 0x06 /* Public */,
       4,    3,   85,    2, 0x06 /* Public */,
       5,    1,   92,    2, 0x06 /* Public */,
       5,    2,   95,    2, 0x06 /* Public */,
       7,    1,  100,    2, 0x06 /* Public */,
       9,    2,  103,    2, 0x06 /* Public */,
      10,    3,  108,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,  115,    2, 0x0a /* Public */,
      12,    4,  116,    2, 0x09 /* Protected */,
      18,    2,  125,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::UShort,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::UShort, QMetaType::QByteArray,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::UShort,    2,    2,    2,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    2,    2,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Char, QMetaType::QByteArray,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::UShort, 0x80000000 | 16,   13,   14,   15,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    2,    2,

       0        // eod
};

void TcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TcpServer *_t = static_cast<TcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectClient((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const quint16(*)>(_a[3]))); break;
        case 1: _t->readData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 2: _t->sockDisConnect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 3: _t->sentData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->sentData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 5: _t->sentDisConnect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->connectToSystem((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->fangzhenAGVxinxi((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 8: _t->clear(); break;
        case 9: _t->sockDisConnectSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3])),(*reinterpret_cast< QThread*(*)>(_a[4]))); break;
        case 10: _t->receviedFormAGV((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TcpServer::*_t)(const int , const QString & , const quint16 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpServer::connectClient)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TcpServer::*_t)(const int , const QString & , quint16 , const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpServer::readData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (TcpServer::*_t)(int , QString , quint16 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpServer::sockDisConnect)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (TcpServer::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpServer::sentData)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (TcpServer::*_t)(int , QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpServer::sentData)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (TcpServer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpServer::sentDisConnect)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (TcpServer::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpServer::connectToSystem)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (TcpServer::*_t)(int , char , QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TcpServer::fangzhenAGVxinxi)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject TcpServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_TcpServer.data,
      qt_meta_data_TcpServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TcpServer.stringdata0))
        return static_cast<void*>(const_cast< TcpServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int TcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void TcpServer::connectClient(const int _t1, const QString & _t2, const quint16 _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TcpServer::readData(const int _t1, const QString & _t2, quint16 _t3, const QByteArray & _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TcpServer::sockDisConnect(int _t1, QString _t2, quint16 _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TcpServer::sentData(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TcpServer::sentData(int _t1, QByteArray _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TcpServer::sentDisConnect(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void TcpServer::connectToSystem(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void TcpServer::fangzhenAGVxinxi(int _t1, char _t2, QByteArray _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
