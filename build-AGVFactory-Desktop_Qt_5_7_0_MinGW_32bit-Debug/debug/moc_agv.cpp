/****************************************************************************
** Meta object code from reading C++ file 'agv.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/agv.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'agv.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AGV_t {
    QByteArrayData data[72];
    char stringdata0[676];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AGV_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AGV_t qt_meta_stringdata_AGV = {
    {
QT_MOC_LITERAL(0, 0, 3), // "AGV"
QT_MOC_LITERAL(1, 4, 9), // "agv_moved"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 14), // "QGraphicsItem*"
QT_MOC_LITERAL(4, 30, 11), // "state_moved"
QT_MOC_LITERAL(5, 42, 12), // "state_change"
QT_MOC_LITERAL(6, 55, 17), // "state_change_PT20"
QT_MOC_LITERAL(7, 73, 12), // "serial_write"
QT_MOC_LITERAL(8, 86, 4), // "data"
QT_MOC_LITERAL(9, 91, 9), // "tcp_write"
QT_MOC_LITERAL(10, 101, 13), // "mission_abort"
QT_MOC_LITERAL(11, 115, 7), // "agvname"
QT_MOC_LITERAL(12, 123, 12), // "report_error"
QT_MOC_LITERAL(13, 136, 12), // "TestFeedback"
QT_MOC_LITERAL(14, 149, 10), // "AGVCommand"
QT_MOC_LITERAL(15, 160, 3), // "com"
QT_MOC_LITERAL(16, 164, 7), // "dongzuo"
QT_MOC_LITERAL(17, 172, 9), // "fangxiang"
QT_MOC_LITERAL(18, 182, 5), // "agvid"
QT_MOC_LITERAL(19, 188, 16), // "dianliangjinggao"
QT_MOC_LITERAL(20, 205, 7), // "AGVWarn"
QT_MOC_LITERAL(21, 213, 6), // "WarnID"
QT_MOC_LITERAL(22, 220, 15), // "missionwancheng"
QT_MOC_LITERAL(23, 236, 4), // "AGV*"
QT_MOC_LITERAL(24, 241, 5), // "set_x"
QT_MOC_LITERAL(25, 247, 1), // "x"
QT_MOC_LITERAL(26, 249, 5), // "set_y"
QT_MOC_LITERAL(27, 255, 1), // "y"
QT_MOC_LITERAL(28, 257, 10), // "set_center"
QT_MOC_LITERAL(29, 268, 6), // "center"
QT_MOC_LITERAL(30, 275, 12), // "set_rotation"
QT_MOC_LITERAL(31, 288, 6), // "rotate"
QT_MOC_LITERAL(32, 295, 12), // "set_dashLine"
QT_MOC_LITERAL(33, 308, 8), // "lineType"
QT_MOC_LITERAL(34, 317, 8), // "set_size"
QT_MOC_LITERAL(35, 326, 1), // "w"
QT_MOC_LITERAL(36, 328, 1), // "h"
QT_MOC_LITERAL(37, 330, 12), // "set_penWidth"
QT_MOC_LITERAL(38, 343, 12), // "set_penColor"
QT_MOC_LITERAL(39, 356, 5), // "color"
QT_MOC_LITERAL(40, 362, 14), // "set_brushColor"
QT_MOC_LITERAL(41, 377, 8), // "set_name"
QT_MOC_LITERAL(42, 386, 4), // "name"
QT_MOC_LITERAL(43, 391, 12), // "set_selected"
QT_MOC_LITERAL(44, 404, 8), // "recvData"
QT_MOC_LITERAL(45, 413, 10), // "frame_type"
QT_MOC_LITERAL(46, 424, 4), // "sign"
QT_MOC_LITERAL(47, 429, 14), // "show_errorInfo"
QT_MOC_LITERAL(48, 444, 15), // "changeForkState"
QT_MOC_LITERAL(49, 460, 5), // "value"
QT_MOC_LITERAL(50, 466, 13), // "get_forkState"
QT_MOC_LITERAL(51, 480, 11), // "setLastFork"
QT_MOC_LITERAL(52, 492, 12), // "get_lastFork"
QT_MOC_LITERAL(53, 505, 12), // "get_previous"
QT_MOC_LITERAL(54, 518, 3), // "Pre"
QT_MOC_LITERAL(55, 522, 11), // "set_AGVpath"
QT_MOC_LITERAL(56, 534, 10), // "QList<int>"
QT_MOC_LITERAL(57, 545, 4), // "path"
QT_MOC_LITERAL(58, 550, 7), // "_center"
QT_MOC_LITERAL(59, 558, 2), // "_y"
QT_MOC_LITERAL(60, 561, 7), // "_rotate"
QT_MOC_LITERAL(61, 569, 10), // "_pen_width"
QT_MOC_LITERAL(62, 580, 10), // "_pen_color"
QT_MOC_LITERAL(63, 591, 12), // "_brush_color"
QT_MOC_LITERAL(64, 604, 5), // "_name"
QT_MOC_LITERAL(65, 610, 10), // "_pen_style"
QT_MOC_LITERAL(66, 621, 6), // "_power"
QT_MOC_LITERAL(67, 628, 6), // "_state"
QT_MOC_LITERAL(68, 635, 9), // "_comstate"
QT_MOC_LITERAL(69, 645, 9), // "_jiaosudu"
QT_MOC_LITERAL(70, 655, 9), // "_xiansudu"
QT_MOC_LITERAL(71, 665, 10) // "_commstata"

    },
    "AGV\0agv_moved\0\0QGraphicsItem*\0state_moved\0"
    "state_change\0state_change_PT20\0"
    "serial_write\0data\0tcp_write\0mission_abort\0"
    "agvname\0report_error\0TestFeedback\0"
    "AGVCommand\0com\0dongzuo\0fangxiang\0agvid\0"
    "dianliangjinggao\0AGVWarn\0WarnID\0"
    "missionwancheng\0AGV*\0set_x\0x\0set_y\0y\0"
    "set_center\0center\0set_rotation\0rotate\0"
    "set_dashLine\0lineType\0set_size\0w\0h\0"
    "set_penWidth\0set_penColor\0color\0"
    "set_brushColor\0set_name\0name\0set_selected\0"
    "recvData\0frame_type\0sign\0show_errorInfo\0"
    "changeForkState\0value\0get_forkState\0"
    "setLastFork\0get_lastFork\0get_previous\0"
    "Pre\0set_AGVpath\0QList<int>\0path\0_center\0"
    "_y\0_rotate\0_pen_width\0_pen_color\0"
    "_brush_color\0_name\0_pen_style\0_power\0"
    "_state\0_comstate\0_jiaosudu\0_xiansudu\0"
    "_commstata"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AGV[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
      14,  278, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  174,    2, 0x06 /* Public */,
       4,    3,  177,    2, 0x06 /* Public */,
       5,    0,  184,    2, 0x06 /* Public */,
       6,    0,  185,    2, 0x06 /* Public */,
       7,    1,  186,    2, 0x06 /* Public */,
       9,    1,  189,    2, 0x06 /* Public */,
      10,    1,  192,    2, 0x06 /* Public */,
      12,    2,  195,    2, 0x06 /* Public */,
      13,    4,  200,    2, 0x06 /* Public */,
      19,    0,  209,    2, 0x06 /* Public */,
      20,    1,  210,    2, 0x06 /* Public */,
      22,    3,  213,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      24,    1,  220,    2, 0x0a /* Public */,
      26,    1,  223,    2, 0x0a /* Public */,
      28,    1,  226,    2, 0x0a /* Public */,
      30,    1,  229,    2, 0x0a /* Public */,
      32,    1,  232,    2, 0x0a /* Public */,
      34,    2,  235,    2, 0x0a /* Public */,
      37,    1,  240,    2, 0x0a /* Public */,
      38,    1,  243,    2, 0x0a /* Public */,
      40,    1,  246,    2, 0x0a /* Public */,
      41,    1,  249,    2, 0x0a /* Public */,
      43,    0,  252,    2, 0x0a /* Public */,
      44,    2,  253,    2, 0x0a /* Public */,
      44,    3,  258,    2, 0x0a /* Public */,
      47,    0,  265,    2, 0x0a /* Public */,
      48,    1,  266,    2, 0x0a /* Public */,
      50,    0,  269,    2, 0x0a /* Public */,
      51,    1,  270,    2, 0x0a /* Public */,
      52,    0,  273,    2, 0x0a /* Public */,
      53,    0,  274,    2, 0x0a /* Public */,
      55,    1,  275,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::Char,    2,    2,
    QMetaType::Void, 0x80000000 | 14, QMetaType::Int, QMetaType::Int, QMetaType::Int,   15,   16,   17,   18,
    QMetaType::Void,
    QMetaType::Int, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, 0x80000000 | 23,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QReal,   25,
    QMetaType::Void, QMetaType::QReal,   27,
    QMetaType::Void, QMetaType::QPoint,   29,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   35,   36,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::QColor,   39,
    QMetaType::Void, QMetaType::QColor,   39,
    QMetaType::Void, QMetaType::QString,   42,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Char, QMetaType::QByteArray,   45,    8,
    QMetaType::Void, QMetaType::Char, QMetaType::QByteArray, QMetaType::Bool,   45,    8,   46,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   49,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   49,
    QMetaType::Int,
    0x80000000 | 54,
    QMetaType::Void, 0x80000000 | 56,   57,

 // properties: name, type, flags
      58, QMetaType::QPoint, 0x00095103,
      59, QMetaType::Int, 0x00095103,
      60, QMetaType::Int, 0x00095003,
      61, QMetaType::Int, 0x00095003,
      62, QMetaType::QColor, 0x00095003,
      63, QMetaType::QColor, 0x00095003,
      64, QMetaType::QString, 0x00095001,
      65, QMetaType::Int, 0x00095001,
      66, QMetaType::Int, 0x00095001,
      67, QMetaType::QString, 0x00095001,
      68, QMetaType::QString, 0x00095001,
      69, QMetaType::Int, 0x00095001,
      70, QMetaType::Int, 0x00095001,
      71, QMetaType::Bool, 0x00095001,

       0        // eod
};

void AGV::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AGV *_t = static_cast<AGV *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->agv_moved((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        case 1: _t->state_moved((*reinterpret_cast< QGraphicsItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->state_change(); break;
        case 3: _t->state_change_PT20(); break;
        case 4: _t->serial_write((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->tcp_write((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->mission_abort((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->report_error((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2]))); break;
        case 8: _t->TestFeedback((*reinterpret_cast< AGVCommand(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 9: _t->dianliangjinggao(); break;
        case 10: { int _r = _t->AGVWarn((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: _t->missionwancheng((*reinterpret_cast< quint64(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< AGV*(*)>(_a[3]))); break;
        case 12: _t->set_x((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 13: _t->set_y((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 14: _t->set_center((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 15: _t->set_rotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->set_dashLine((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->set_size((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->set_penWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->set_penColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 20: _t->set_brushColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 21: _t->set_name((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->set_selected(); break;
        case 23: _t->recvData((*reinterpret_cast< char(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 24: _t->recvData((*reinterpret_cast< char(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 25: _t->show_errorInfo(); break;
        case 26: _t->changeForkState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: { int _r = _t->get_forkState();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 28: _t->setLastFork((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: { int _r = _t->get_lastFork();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 30: { Pre _r = _t->get_previous();
            if (_a[0]) *reinterpret_cast< Pre*>(_a[0]) = _r; }  break;
        case 31: _t->set_AGVpath((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AGV* >(); break;
            }
            break;
        case 31:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AGV::*_t)(QGraphicsItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::agv_moved)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AGV::*_t)(QGraphicsItem * , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::state_moved)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AGV::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::state_change)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AGV::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::state_change_PT20)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AGV::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::serial_write)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AGV::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::tcp_write)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AGV::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::mission_abort)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (AGV::*_t)(QString , char );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::report_error)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (AGV::*_t)(AGVCommand , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::TestFeedback)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (AGV::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::dianliangjinggao)) {
                *result = 9;
                return;
            }
        }
        {
            typedef int (AGV::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::AGVWarn)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (AGV::*_t)(quint64 , QString , AGV * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGV::missionwancheng)) {
                *result = 11;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        AGV *_t = static_cast<AGV *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QPoint*>(_v) = _t->get_center(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->y(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->rotation(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->get_penWidth(); break;
        case 4: *reinterpret_cast< QColor*>(_v) = _t->get_penColor(); break;
        case 5: *reinterpret_cast< QColor*>(_v) = _t->get_brushColor(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->get_name(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->get_penStyle(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->get_powerPercent(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->ReadmissionState(); break;
        case 10: *reinterpret_cast< QString*>(_v) = _t->ReadCommandState(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->get_jiaosudu(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->get_xiansudu(); break;
        case 13: *reinterpret_cast< bool*>(_v) = _t->communicateState(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        AGV *_t = static_cast<AGV *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_center(*reinterpret_cast< QPoint*>(_v)); break;
        case 1: _t->set_y(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->set_rotation(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->set_penWidth(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->set_penColor(*reinterpret_cast< QColor*>(_v)); break;
        case 5: _t->set_brushColor(*reinterpret_cast< QColor*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject AGV::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AGV.data,
      qt_meta_data_AGV,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AGV::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AGV::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AGV.stringdata0))
        return static_cast<void*>(const_cast< AGV*>(this));
    if (!strcmp(_clname, "QGraphicsRectItem"))
        return static_cast< QGraphicsRectItem*>(const_cast< AGV*>(this));
    return QObject::qt_metacast(_clname);
}

int AGV::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 14;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void AGV::agv_moved(QGraphicsItem * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AGV::state_moved(QGraphicsItem * _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AGV::state_change()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void AGV::state_change_PT20()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void AGV::serial_write(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AGV::tcp_write(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AGV::mission_abort(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AGV::report_error(QString _t1, char _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AGV::TestFeedback(AGVCommand _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AGV::dianliangjinggao()
{
    QMetaObject::activate(this, &staticMetaObject, 9, Q_NULLPTR);
}

// SIGNAL 10
int AGV::AGVWarn(int _t1)
{
    int _t0 = int();
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
    return _t0;
}

// SIGNAL 11
void AGV::missionwancheng(quint64 _t1, QString _t2, AGV * _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_END_MOC_NAMESPACE
