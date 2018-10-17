/****************************************************************************
** Meta object code from reading C++ file 'trafficmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/trafficmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trafficmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TrafficManager_t {
    QByteArrayData data[21];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrafficManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrafficManager_t qt_meta_stringdata_TrafficManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "TrafficManager"
QT_MOC_LITERAL(1, 15, 14), // "AGVstateChange"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 4), // "AGV*"
QT_MOC_LITERAL(4, 36, 19), // "MissionStateChanged"
QT_MOC_LITERAL(5, 56, 12), // "start_nodeID"
QT_MOC_LITERAL(6, 69, 10), // "end_nodeID"
QT_MOC_LITERAL(7, 80, 11), // "start_layer"
QT_MOC_LITERAL(8, 92, 9), // "end_layer"
QT_MOC_LITERAL(9, 102, 20), // "updateGlobalPlanning"
QT_MOC_LITERAL(10, 123, 12), // "report_error"
QT_MOC_LITERAL(11, 136, 7), // "agvname"
QT_MOC_LITERAL(12, 144, 10), // "error_code"
QT_MOC_LITERAL(13, 155, 13), // "mission_abort"
QT_MOC_LITERAL(14, 169, 2), // "id"
QT_MOC_LITERAL(15, 172, 15), // "someInforDelate"
QT_MOC_LITERAL(16, 188, 5), // "agvid"
QT_MOC_LITERAL(17, 194, 15), // "multisystemused"
QT_MOC_LITERAL(18, 210, 8), // "pauseAgv"
QT_MOC_LITERAL(19, 219, 7), // "pointID"
QT_MOC_LITERAL(20, 227, 6) // "keepOn"

    },
    "TrafficManager\0AGVstateChange\0\0AGV*\0"
    "MissionStateChanged\0start_nodeID\0"
    "end_nodeID\0start_layer\0end_layer\0"
    "updateGlobalPlanning\0report_error\0"
    "agvname\0error_code\0mission_abort\0id\0"
    "someInforDelate\0agvid\0multisystemused\0"
    "pauseAgv\0pointID\0keepOn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrafficManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       4,    4,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   73,    2, 0x09 /* Protected */,
      10,    2,   74,    2, 0x09 /* Protected */,
      13,    1,   79,    2, 0x09 /* Protected */,
      15,    1,   82,    2, 0x09 /* Protected */,
      17,    0,   85,    2, 0x0a /* Public */,
      18,    2,   86,    2, 0x0a /* Public */,
      20,    1,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    5,    6,    7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,   11,   12,
    QMetaType::Void, QMetaType::ULongLong,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   16,   19,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void TrafficManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrafficManager *_t = static_cast<TrafficManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->AGVstateChange((*reinterpret_cast< AGV*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->MissionStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 2: _t->updateGlobalPlanning(); break;
        case 3: _t->report_error((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 4: _t->mission_abort((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 5: _t->someInforDelate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->multisystemused(); break;
        case 7: _t->pauseAgv((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->keepOn((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AGV* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TrafficManager::*_t)(AGV * , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrafficManager::AGVstateChange)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TrafficManager::*_t)(int , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TrafficManager::MissionStateChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TrafficManager::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TrafficManager.data,
      qt_meta_data_TrafficManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TrafficManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrafficManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TrafficManager.stringdata0))
        return static_cast<void*>(const_cast< TrafficManager*>(this));
    return QWidget::qt_metacast(_clname);
}

int TrafficManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TrafficManager::AGVstateChange(AGV * _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TrafficManager::MissionStateChanged(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
