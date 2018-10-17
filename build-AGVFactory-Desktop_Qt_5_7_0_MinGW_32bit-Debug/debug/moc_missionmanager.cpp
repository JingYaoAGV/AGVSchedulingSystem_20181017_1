/****************************************************************************
** Meta object code from reading C++ file 'missionmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/missionmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'missionmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MissionManager_t {
    QByteArrayData data[63];
    char stringdata0[772];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MissionManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MissionManager_t qt_meta_stringdata_MissionManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MissionManager"
QT_MOC_LITERAL(1, 15, 20), // "mission_accomplished"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 5), // "start"
QT_MOC_LITERAL(4, 43, 3), // "end"
QT_MOC_LITERAL(5, 47, 19), // "MissionStateChanged"
QT_MOC_LITERAL(6, 67, 15), // "mission_deleted"
QT_MOC_LITERAL(7, 83, 2), // "id"
QT_MOC_LITERAL(8, 86, 13), // "wms_connected"
QT_MOC_LITERAL(9, 100, 12), // "start_nodeID"
QT_MOC_LITERAL(10, 113, 10), // "end_nodeID"
QT_MOC_LITERAL(11, 124, 14), // "huoliangxiugai"
QT_MOC_LITERAL(12, 139, 12), // "loadTopoView"
QT_MOC_LITERAL(13, 152, 9), // "TopoView*"
QT_MOC_LITERAL(14, 162, 8), // "topoview"
QT_MOC_LITERAL(15, 171, 6), // "addAGV"
QT_MOC_LITERAL(16, 178, 4), // "AGV*"
QT_MOC_LITERAL(17, 183, 3), // "agv"
QT_MOC_LITERAL(18, 187, 9), // "removeAGV"
QT_MOC_LITERAL(19, 197, 13), // "action_create"
QT_MOC_LITERAL(20, 211, 11), // "action_edit"
QT_MOC_LITERAL(21, 223, 11), // "action_hang"
QT_MOC_LITERAL(22, 235, 13), // "action_delete"
QT_MOC_LITERAL(23, 249, 14), // "action_xunhuan"
QT_MOC_LITERAL(24, 264, 13), // "action_jinzhi"
QT_MOC_LITERAL(25, 278, 11), // "action_Test"
QT_MOC_LITERAL(26, 290, 16), // "action_Chushihua"
QT_MOC_LITERAL(27, 307, 15), // "action_TestStop"
QT_MOC_LITERAL(28, 323, 25), // "action_QianjinZhenngchang"
QT_MOC_LITERAL(29, 349, 20), // "action_QianjinJiansu"
QT_MOC_LITERAL(30, 370, 23), // "action_HoutuiZhengchang"
QT_MOC_LITERAL(31, 394, 19), // "action_HoutuiJiansu"
QT_MOC_LITERAL(32, 414, 14), // "action_Zuoguai"
QT_MOC_LITERAL(33, 429, 14), // "action_Youguai"
QT_MOC_LITERAL(34, 444, 21), // "action_Tingchedengdai"
QT_MOC_LITERAL(35, 466, 18), // "action_recvCommand"
QT_MOC_LITERAL(36, 485, 10), // "AGVCommand"
QT_MOC_LITERAL(37, 496, 3), // "com"
QT_MOC_LITERAL(38, 500, 7), // "Dongzuo"
QT_MOC_LITERAL(39, 508, 9), // "Fangxiang"
QT_MOC_LITERAL(40, 518, 5), // "agvid"
QT_MOC_LITERAL(41, 524, 19), // "action_nodeDistance"
QT_MOC_LITERAL(42, 544, 5), // "index"
QT_MOC_LITERAL(43, 550, 11), // "new_request"
QT_MOC_LITERAL(44, 562, 14), // "QPair<int,int>"
QT_MOC_LITERAL(45, 577, 4), // "info"
QT_MOC_LITERAL(46, 582, 8), // "priority"
QT_MOC_LITERAL(47, 591, 9), // "startInfo"
QT_MOC_LITERAL(48, 601, 7), // "endInfo"
QT_MOC_LITERAL(49, 609, 13), // "inquiry_state"
QT_MOC_LITERAL(50, 623, 2), // "ID"
QT_MOC_LITERAL(51, 626, 12), // "stop_request"
QT_MOC_LITERAL(52, 639, 12), // "report_error"
QT_MOC_LITERAL(53, 652, 10), // "error_code"
QT_MOC_LITERAL(54, 663, 16), // "mission_finished"
QT_MOC_LITERAL(55, 680, 14), // "MissionArrange"
QT_MOC_LITERAL(56, 695, 17), // "MissionResetOrFin"
QT_MOC_LITERAL(57, 713, 10), // "hasmission"
QT_MOC_LITERAL(58, 724, 9), // "missionid"
QT_MOC_LITERAL(59, 734, 5), // "reset"
QT_MOC_LITERAL(60, 740, 9), // "jinzhiAGV"
QT_MOC_LITERAL(61, 750, 15), // "setMissionState"
QT_MOC_LITERAL(62, 766, 5) // "state"

    },
    "MissionManager\0mission_accomplished\0"
    "\0start\0end\0MissionStateChanged\0"
    "mission_deleted\0id\0wms_connected\0"
    "start_nodeID\0end_nodeID\0huoliangxiugai\0"
    "loadTopoView\0TopoView*\0topoview\0addAGV\0"
    "AGV*\0agv\0removeAGV\0action_create\0"
    "action_edit\0action_hang\0action_delete\0"
    "action_xunhuan\0action_jinzhi\0action_Test\0"
    "action_Chushihua\0action_TestStop\0"
    "action_QianjinZhenngchang\0"
    "action_QianjinJiansu\0action_HoutuiZhengchang\0"
    "action_HoutuiJiansu\0action_Zuoguai\0"
    "action_Youguai\0action_Tingchedengdai\0"
    "action_recvCommand\0AGVCommand\0com\0"
    "Dongzuo\0Fangxiang\0agvid\0action_nodeDistance\0"
    "index\0new_request\0QPair<int,int>\0info\0"
    "priority\0startInfo\0endInfo\0inquiry_state\0"
    "ID\0stop_request\0report_error\0error_code\0"
    "mission_finished\0MissionArrange\0"
    "MissionResetOrFin\0hasmission\0missionid\0"
    "reset\0jinzhiAGV\0setMissionState\0state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MissionManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  204,    2, 0x06 /* Public */,
       5,    0,  209,    2, 0x06 /* Public */,
       6,    1,  210,    2, 0x06 /* Public */,
       8,    1,  213,    2, 0x06 /* Public */,
       5,    2,  216,    2, 0x06 /* Public */,
      11,    2,  221,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,  226,    2, 0x0a /* Public */,
      15,    1,  229,    2, 0x0a /* Public */,
      18,    1,  232,    2, 0x0a /* Public */,
      19,    0,  235,    2, 0x0a /* Public */,
      20,    0,  236,    2, 0x0a /* Public */,
      21,    0,  237,    2, 0x0a /* Public */,
      22,    0,  238,    2, 0x0a /* Public */,
      23,    0,  239,    2, 0x0a /* Public */,
      24,    0,  240,    2, 0x0a /* Public */,
      25,    0,  241,    2, 0x0a /* Public */,
      26,    0,  242,    2, 0x0a /* Public */,
      27,    0,  243,    2, 0x0a /* Public */,
      28,    0,  244,    2, 0x0a /* Public */,
      29,    0,  245,    2, 0x0a /* Public */,
      30,    0,  246,    2, 0x0a /* Public */,
      31,    0,  247,    2, 0x0a /* Public */,
      32,    0,  248,    2, 0x0a /* Public */,
      33,    0,  249,    2, 0x0a /* Public */,
      34,    0,  250,    2, 0x0a /* Public */,
      35,    4,  251,    2, 0x0a /* Public */,
      41,    1,  260,    2, 0x0a /* Public */,
      43,    2,  263,    2, 0x0a /* Public */,
      43,    1,  268,    2, 0x2a /* Public | MethodCloned */,
      43,    4,  271,    2, 0x0a /* Public */,
      49,    1,  280,    2, 0x0a /* Public */,
      51,    1,  283,    2, 0x0a /* Public */,
      52,    2,  286,    2, 0x0a /* Public */,
      54,    1,  291,    2, 0x0a /* Public */,
      55,    2,  294,    2, 0x0a /* Public */,
      56,    4,  299,    2, 0x0a /* Public */,
      60,    0,  308,    2, 0x0a /* Public */,
      61,    3,  309,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULongLong,    7,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 36, QMetaType::Int, QMetaType::Int, QMetaType::Int,   37,   38,   39,   40,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, 0x80000000 | 44, QMetaType::Int,   45,   46,
    QMetaType::Void, 0x80000000 | 44,   45,
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 44, 0x80000000 | 44, QMetaType::Int,    7,   47,   48,   46,
    QMetaType::Void, QMetaType::ULongLong,   50,
    QMetaType::Void, QMetaType::ULongLong,   50,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Char,   50,   53,
    QMetaType::Void, QMetaType::ULongLong,   50,
    QMetaType::Void, 0x80000000 | 16, QMetaType::Bool,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::ULongLong, QMetaType::Bool,   40,   57,   58,   59,
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, 0x80000000 | 16,    7,   62,   17,

       0        // eod
};

void MissionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MissionManager *_t = static_cast<MissionManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mission_accomplished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->MissionStateChanged(); break;
        case 2: _t->mission_deleted((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 3: _t->wms_connected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->MissionStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->huoliangxiugai((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->loadTopoView((*reinterpret_cast< TopoView*(*)>(_a[1]))); break;
        case 7: _t->addAGV((*reinterpret_cast< AGV*(*)>(_a[1]))); break;
        case 8: _t->removeAGV((*reinterpret_cast< AGV*(*)>(_a[1]))); break;
        case 9: _t->action_create(); break;
        case 10: _t->action_edit(); break;
        case 11: _t->action_hang(); break;
        case 12: _t->action_delete(); break;
        case 13: _t->action_xunhuan(); break;
        case 14: _t->action_jinzhi(); break;
        case 15: _t->action_Test(); break;
        case 16: _t->action_Chushihua(); break;
        case 17: _t->action_TestStop(); break;
        case 18: _t->action_QianjinZhenngchang(); break;
        case 19: _t->action_QianjinJiansu(); break;
        case 20: _t->action_HoutuiZhengchang(); break;
        case 21: _t->action_HoutuiJiansu(); break;
        case 22: _t->action_Zuoguai(); break;
        case 23: _t->action_Youguai(); break;
        case 24: _t->action_Tingchedengdai(); break;
        case 25: _t->action_recvCommand((*reinterpret_cast< AGVCommand(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 26: _t->action_nodeDistance((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->new_request((*reinterpret_cast< QPair<int,int>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 28: _t->new_request((*reinterpret_cast< QPair<int,int>(*)>(_a[1]))); break;
        case 29: _t->new_request((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QPair<int,int>(*)>(_a[2])),(*reinterpret_cast< QPair<int,int>(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 30: _t->inquiry_state((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 31: _t->stop_request((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 32: _t->report_error((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 33: _t->mission_finished((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 34: _t->MissionArrange((*reinterpret_cast< AGV*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 35: _t->MissionResetOrFin((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< quint64(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 36: _t->jinzhiAGV(); break;
        case 37: _t->setMissionState((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< AGV*(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TopoView* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AGV* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AGV* >(); break;
            }
            break;
        case 34:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AGV* >(); break;
            }
            break;
        case 37:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AGV* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MissionManager::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MissionManager::mission_accomplished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MissionManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MissionManager::MissionStateChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MissionManager::*_t)(quint64 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MissionManager::mission_deleted)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MissionManager::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MissionManager::wms_connected)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MissionManager::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MissionManager::MissionStateChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MissionManager::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MissionManager::huoliangxiugai)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject MissionManager::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MissionManager.data,
      qt_meta_data_MissionManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MissionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MissionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MissionManager.stringdata0))
        return static_cast<void*>(const_cast< MissionManager*>(this));
    return QWidget::qt_metacast(_clname);
}

int MissionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void MissionManager::mission_accomplished(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MissionManager::MissionStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MissionManager::mission_deleted(quint64 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MissionManager::wms_connected(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MissionManager::MissionStateChanged(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MissionManager::huoliangxiugai(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
