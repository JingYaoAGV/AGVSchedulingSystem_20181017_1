/****************************************************************************
** Meta object code from reading C++ file 'agvinformation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/agvinformation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'agvinformation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AgvInformation_t {
    QByteArrayData data[28];
    char stringdata0[298];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AgvInformation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AgvInformation_t qt_meta_stringdata_AgvInformation = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AgvInformation"
QT_MOC_LITERAL(1, 15, 20), // "AgvInformationChange"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 10), // "addintoRun"
QT_MOC_LITERAL(4, 48, 9), // "chushihua"
QT_MOC_LITERAL(5, 58, 13), // "removeFromRun"
QT_MOC_LITERAL(6, 72, 14), // "get_agvmission"
QT_MOC_LITERAL(7, 87, 16), // "ToMissionmanager"
QT_MOC_LITERAL(8, 104, 8), // "pauseAgv"
QT_MOC_LITERAL(9, 113, 5), // "agvid"
QT_MOC_LITERAL(10, 119, 7), // "pointid"
QT_MOC_LITERAL(11, 127, 8), // "keepMove"
QT_MOC_LITERAL(12, 136, 13), // "addIntoSystem"
QT_MOC_LITERAL(13, 150, 16), // "removeFromSystem"
QT_MOC_LITERAL(14, 167, 5), // "pause"
QT_MOC_LITERAL(15, 173, 6), // "keepOn"
QT_MOC_LITERAL(16, 180, 13), // "missionDecide"
QT_MOC_LITERAL(17, 194, 11), // "startWizard"
QT_MOC_LITERAL(18, 206, 10), // "hasMission"
QT_MOC_LITERAL(19, 217, 5), // "reach"
QT_MOC_LITERAL(20, 223, 5), // "start"
QT_MOC_LITERAL(21, 229, 3), // "end"
QT_MOC_LITERAL(22, 233, 10), // "QList<int>"
QT_MOC_LITERAL(23, 244, 16), // "insertIntoSystem"
QT_MOC_LITERAL(24, 261, 8), // "inSystem"
QT_MOC_LITERAL(25, 270, 2), // "id"
QT_MOC_LITERAL(26, 273, 9), // "outSystem"
QT_MOC_LITERAL(27, 283, 14) // "receiveMission"

    },
    "AgvInformation\0AgvInformationChange\0"
    "\0addintoRun\0chushihua\0removeFromRun\0"
    "get_agvmission\0ToMissionmanager\0"
    "pauseAgv\0agvid\0pointid\0keepMove\0"
    "addIntoSystem\0removeFromSystem\0pause\0"
    "keepOn\0missionDecide\0startWizard\0"
    "hasMission\0reach\0start\0end\0QList<int>\0"
    "insertIntoSystem\0inSystem\0id\0outSystem\0"
    "receiveMission"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AgvInformation[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,
       3,    1,  105,    2, 0x06 /* Public */,
       4,    8,  108,    2, 0x06 /* Public */,
       5,    1,  125,    2, 0x06 /* Public */,
       6,    1,  128,    2, 0x06 /* Public */,
       7,    4,  131,    2, 0x06 /* Public */,
       8,    2,  140,    2, 0x06 /* Public */,
      11,    1,  145,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  148,    2, 0x0a /* Public */,
      13,    0,  149,    2, 0x0a /* Public */,
      14,    0,  150,    2, 0x0a /* Public */,
      15,    0,  151,    2, 0x0a /* Public */,
      16,    4,  152,    2, 0x0a /* Public */,
      17,    6,  161,    2, 0x0a /* Public */,
      23,    0,  174,    2, 0x0a /* Public */,
      24,    1,  175,    2, 0x0a /* Public */,
      26,    1,  178,    2, 0x0a /* Public */,
      27,    4,  181,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 22,   18,   19,   20,   21,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,

       0        // eod
};

void AgvInformation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AgvInformation *_t = static_cast<AgvInformation *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->AgvInformationChange(); break;
        case 1: _t->addintoRun((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->chushihua((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 3: _t->removeFromRun((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->get_agvmission((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ToMissionmanager((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 6: _t->pauseAgv((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->keepMove((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->addIntoSystem(); break;
        case 9: _t->removeFromSystem(); break;
        case 10: _t->pause(); break;
        case 11: _t->keepOn(); break;
        case 12: _t->missionDecide((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 13: _t->startWizard((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< QList<int>(*)>(_a[6]))); break;
        case 14: _t->insertIntoSystem(); break;
        case 15: _t->inSystem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->outSystem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->receiveMission((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 5:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AgvInformation::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AgvInformation::AgvInformationChange)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AgvInformation::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AgvInformation::addintoRun)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AgvInformation::*_t)(int , int , int , int , int , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AgvInformation::chushihua)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AgvInformation::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AgvInformation::removeFromRun)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AgvInformation::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AgvInformation::get_agvmission)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AgvInformation::*_t)(int , bool , bool , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AgvInformation::ToMissionmanager)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AgvInformation::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AgvInformation::pauseAgv)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (AgvInformation::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AgvInformation::keepMove)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject AgvInformation::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AgvInformation.data,
      qt_meta_data_AgvInformation,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AgvInformation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AgvInformation::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AgvInformation.stringdata0))
        return static_cast<void*>(const_cast< AgvInformation*>(this));
    return QWidget::qt_metacast(_clname);
}

int AgvInformation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void AgvInformation::AgvInformationChange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AgvInformation::addintoRun(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AgvInformation::chushihua(int _t1, int _t2, int _t3, int _t4, int _t5, int _t6, int _t7, int _t8)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AgvInformation::removeFromRun(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AgvInformation::get_agvmission(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AgvInformation::ToMissionmanager(int _t1, bool _t2, bool _t3, bool _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AgvInformation::pauseAgv(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AgvInformation::keepMove(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
