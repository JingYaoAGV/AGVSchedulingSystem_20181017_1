/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AGVFactory/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[80];
    char stringdata0[1003];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 18), // "update_progressBar"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 15), // "app_stateChange"
QT_MOC_LITERAL(4, 47, 5), // "state"
QT_MOC_LITERAL(5, 53, 10), // "IntoSySucc"
QT_MOC_LITERAL(6, 64, 2), // "id"
QT_MOC_LITERAL(7, 67, 19), // "diconnectFromSystem"
QT_MOC_LITERAL(8, 87, 13), // "missionfromId"
QT_MOC_LITERAL(9, 101, 14), // "missionTowrite"
QT_MOC_LITERAL(10, 116, 5), // "agvid"
QT_MOC_LITERAL(11, 122, 10), // "hasmission"
QT_MOC_LITERAL(12, 133, 8), // "agvinfor"
QT_MOC_LITERAL(13, 142, 9), // "multiUsed"
QT_MOC_LITERAL(14, 152, 13), // "writeToServer"
QT_MOC_LITERAL(15, 166, 12), // "update_store"
QT_MOC_LITERAL(16, 179, 12), // "start_nodeID"
QT_MOC_LITERAL(17, 192, 10), // "end_nodeID"
QT_MOC_LITERAL(18, 203, 11), // "start_layer"
QT_MOC_LITERAL(19, 215, 9), // "end_layer"
QT_MOC_LITERAL(20, 225, 11), // "kuweixiugai"
QT_MOC_LITERAL(21, 237, 8), // "huoliang"
QT_MOC_LITERAL(22, 246, 11), // "dianliangdi"
QT_MOC_LITERAL(23, 258, 12), // "selectChange"
QT_MOC_LITERAL(24, 271, 4), // "save"
QT_MOC_LITERAL(25, 276, 4), // "load"
QT_MOC_LITERAL(26, 281, 15), // "open_recentFile"
QT_MOC_LITERAL(27, 297, 19), // "open_missionManager"
QT_MOC_LITERAL(28, 317, 7), // "prepare"
QT_MOC_LITERAL(29, 325, 3), // "run"
QT_MOC_LITERAL(30, 329, 15), // "agv_information"
QT_MOC_LITERAL(31, 345, 4), // "stop"
QT_MOC_LITERAL(32, 350, 17), // "set_documentDirty"
QT_MOC_LITERAL(33, 368, 17), // "set_topoViewDirty"
QT_MOC_LITERAL(34, 386, 19), // "reset_documentDirty"
QT_MOC_LITERAL(35, 406, 18), // "set_progressBarMax"
QT_MOC_LITERAL(36, 425, 3), // "max"
QT_MOC_LITERAL(37, 429, 4), // "text"
QT_MOC_LITERAL(38, 434, 16), // "set_progressStep"
QT_MOC_LITERAL(39, 451, 5), // "value"
QT_MOC_LITERAL(40, 457, 13), // "show_nodeName"
QT_MOC_LITERAL(41, 471, 4), // "flag"
QT_MOC_LITERAL(42, 476, 18), // "wms_state_chanaged"
QT_MOC_LITERAL(43, 495, 9), // "connected"
QT_MOC_LITERAL(44, 505, 12), // "valueChanged"
QT_MOC_LITERAL(45, 518, 11), // "QtProperty*"
QT_MOC_LITERAL(46, 530, 8), // "property"
QT_MOC_LITERAL(47, 539, 9), // "itemMoved"
QT_MOC_LITERAL(48, 549, 14), // "QGraphicsItem*"
QT_MOC_LITERAL(49, 564, 4), // "item"
QT_MOC_LITERAL(50, 569, 11), // "showAgvInfo"
QT_MOC_LITERAL(51, 581, 8), // "xiansudu"
QT_MOC_LITERAL(52, 590, 8), // "jiaosudu"
QT_MOC_LITERAL(53, 599, 7), // "Warning"
QT_MOC_LITERAL(54, 607, 15), // "call_dialog_add"
QT_MOC_LITERAL(55, 623, 18), // "call_dialog_remove"
QT_MOC_LITERAL(56, 642, 18), // "call_dialog_enable"
QT_MOC_LITERAL(57, 661, 19), // "call_dialog_disable"
QT_MOC_LITERAL(58, 681, 18), // "call_dialog_adjust"
QT_MOC_LITERAL(59, 700, 9), // "add_store"
QT_MOC_LITERAL(60, 710, 12), // "remove_store"
QT_MOC_LITERAL(61, 723, 12), // "enable_store"
QT_MOC_LITERAL(62, 736, 13), // "disable_store"
QT_MOC_LITERAL(63, 750, 12), // "adjust_store"
QT_MOC_LITERAL(64, 763, 12), // "serial_write"
QT_MOC_LITERAL(65, 776, 4), // "data"
QT_MOC_LITERAL(66, 781, 11), // "serial_read"
QT_MOC_LITERAL(67, 793, 23), // "onSystemTrayIconClicked"
QT_MOC_LITERAL(68, 817, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(69, 851, 6), // "reason"
QT_MOC_LITERAL(70, 858, 16), // "addAGVintoSystem"
QT_MOC_LITERAL(71, 875, 16), // "removeFromSystem"
QT_MOC_LITERAL(72, 892, 11), // "stopAgvSent"
QT_MOC_LITERAL(73, 904, 12), // "xuanzhongAGV"
QT_MOC_LITERAL(74, 917, 17), // "get_agvAndMission"
QT_MOC_LITERAL(75, 935, 12), // "whichMission"
QT_MOC_LITERAL(76, 948, 12), // "agvInforList"
QT_MOC_LITERAL(77, 961, 16), // "addAGVasFangzhen"
QT_MOC_LITERAL(78, 978, 13), // "receivFromTcp"
QT_MOC_LITERAL(79, 992, 10) // "writeToTcp"

    },
    "MainWindow\0update_progressBar\0\0"
    "app_stateChange\0state\0IntoSySucc\0id\0"
    "diconnectFromSystem\0missionfromId\0"
    "missionTowrite\0agvid\0hasmission\0"
    "agvinfor\0multiUsed\0writeToServer\0"
    "update_store\0start_nodeID\0end_nodeID\0"
    "start_layer\0end_layer\0kuweixiugai\0"
    "huoliang\0dianliangdi\0selectChange\0"
    "save\0load\0open_recentFile\0open_missionManager\0"
    "prepare\0run\0agv_information\0stop\0"
    "set_documentDirty\0set_topoViewDirty\0"
    "reset_documentDirty\0set_progressBarMax\0"
    "max\0text\0set_progressStep\0value\0"
    "show_nodeName\0flag\0wms_state_chanaged\0"
    "connected\0valueChanged\0QtProperty*\0"
    "property\0itemMoved\0QGraphicsItem*\0"
    "item\0showAgvInfo\0xiansudu\0jiaosudu\0"
    "Warning\0call_dialog_add\0call_dialog_remove\0"
    "call_dialog_enable\0call_dialog_disable\0"
    "call_dialog_adjust\0add_store\0remove_store\0"
    "enable_store\0disable_store\0adjust_store\0"
    "serial_write\0data\0serial_read\0"
    "onSystemTrayIconClicked\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0addAGVintoSystem\0removeFromSystem\0"
    "stopAgvSent\0xuanzhongAGV\0get_agvAndMission\0"
    "whichMission\0agvInforList\0addAGVasFangzhen\0"
    "receivFromTcp\0writeToTcp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      62,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  324,    2, 0x06 /* Public */,
       3,    1,  327,    2, 0x06 /* Public */,
       5,    1,  330,    2, 0x06 /* Public */,
       7,    1,  333,    2, 0x06 /* Public */,
       8,    4,  336,    2, 0x06 /* Public */,
       9,    4,  345,    2, 0x06 /* Public */,
      12,    1,  354,    2, 0x06 /* Public */,
      13,    0,  357,    2, 0x06 /* Public */,
      14,    2,  358,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    4,  363,    2, 0x08 /* Private */,
      20,    2,  372,    2, 0x08 /* Private */,
      22,    0,  377,    2, 0x08 /* Private */,
      23,    0,  378,    2, 0x08 /* Private */,
      24,    0,  379,    2, 0x08 /* Private */,
      25,    0,  380,    2, 0x08 /* Private */,
      26,    0,  381,    2, 0x08 /* Private */,
      27,    0,  382,    2, 0x08 /* Private */,
      28,    0,  383,    2, 0x08 /* Private */,
      29,    0,  384,    2, 0x08 /* Private */,
      30,    0,  385,    2, 0x08 /* Private */,
      31,    0,  386,    2, 0x08 /* Private */,
      32,    0,  387,    2, 0x08 /* Private */,
      33,    0,  388,    2, 0x08 /* Private */,
      34,    0,  389,    2, 0x08 /* Private */,
      35,    2,  390,    2, 0x08 /* Private */,
      38,    1,  395,    2, 0x08 /* Private */,
      40,    1,  398,    2, 0x08 /* Private */,
      42,    1,  401,    2, 0x08 /* Private */,
      44,    2,  404,    2, 0x08 /* Private */,
      44,    2,  409,    2, 0x08 /* Private */,
      44,    2,  414,    2, 0x08 /* Private */,
      44,    2,  419,    2, 0x08 /* Private */,
      44,    2,  424,    2, 0x08 /* Private */,
      44,    2,  429,    2, 0x08 /* Private */,
      44,    2,  434,    2, 0x08 /* Private */,
      44,    2,  439,    2, 0x08 /* Private */,
      47,    1,  444,    2, 0x08 /* Private */,
      50,    3,  447,    2, 0x08 /* Private */,
      53,    1,  454,    2, 0x08 /* Private */,
      54,    0,  457,    2, 0x08 /* Private */,
      55,    0,  458,    2, 0x08 /* Private */,
      56,    0,  459,    2, 0x08 /* Private */,
      57,    0,  460,    2, 0x08 /* Private */,
      58,    0,  461,    2, 0x08 /* Private */,
      59,    0,  462,    2, 0x08 /* Private */,
      60,    0,  463,    2, 0x08 /* Private */,
      61,    0,  464,    2, 0x08 /* Private */,
      62,    0,  465,    2, 0x08 /* Private */,
      63,    0,  466,    2, 0x08 /* Private */,
      64,    1,  467,    2, 0x08 /* Private */,
      66,    0,  470,    2, 0x08 /* Private */,
      67,    1,  471,    2, 0x08 /* Private */,
      70,    1,  474,    2, 0x08 /* Private */,
      71,    1,  477,    2, 0x08 /* Private */,
      72,    1,  480,    2, 0x08 /* Private */,
      73,    8,  483,    2, 0x08 /* Private */,
      74,    1,  500,    2, 0x08 /* Private */,
      75,    4,  503,    2, 0x08 /* Private */,
      76,    1,  512,    2, 0x0a /* Public */,
      77,    2,  515,    2, 0x0a /* Public */,
      78,    3,  520,    2, 0x0a /* Public */,
      79,    1,  527,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::ULongLong, QMetaType::Bool,   10,   11,    6,    2,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QByteArray,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   16,   17,   18,   19,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
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
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   36,   37,
    QMetaType::Void, QMetaType::Int,   39,
    QMetaType::Void, QMetaType::Bool,   41,
    QMetaType::Void, QMetaType::Bool,   43,
    QMetaType::Void, 0x80000000 | 45, QMetaType::Double,   46,   39,
    QMetaType::Void, 0x80000000 | 45, QMetaType::QString,   46,   39,
    QMetaType::Void, 0x80000000 | 45, QMetaType::QColor,   46,   39,
    QMetaType::Void, 0x80000000 | 45, QMetaType::QFont,   46,   39,
    QMetaType::Void, 0x80000000 | 45, QMetaType::QPoint,   46,   39,
    QMetaType::Void, 0x80000000 | 45, QMetaType::QSize,   46,   39,
    QMetaType::Void, 0x80000000 | 45, QMetaType::Bool,   46,   39,
    QMetaType::Void, 0x80000000 | 45, QMetaType::Int,   46,   39,
    QMetaType::Void, 0x80000000 | 48,   49,
    QMetaType::Void, 0x80000000 | 48, QMetaType::Int, QMetaType::Int,   49,   51,   52,
    QMetaType::Void, QMetaType::Int,    6,
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
    QMetaType::Void, QMetaType::QByteArray,   65,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 68,   69,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Char, QMetaType::QByteArray,    2,    2,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->update_progressBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->app_stateChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->IntoSySucc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->diconnectFromSystem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->missionfromId((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 5: _t->missionTowrite((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< quint64(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 6: _t->agvinfor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->multiUsed(); break;
        case 8: _t->writeToServer((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 9: _t->update_store((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 10: _t->kuweixiugai((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->dianliangdi(); break;
        case 12: _t->selectChange(); break;
        case 13: { bool _r = _t->save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: _t->load(); break;
        case 15: _t->open_recentFile(); break;
        case 16: _t->open_missionManager(); break;
        case 17: _t->prepare(); break;
        case 18: _t->run(); break;
        case 19: _t->agv_information(); break;
        case 20: _t->stop(); break;
        case 21: _t->set_documentDirty(); break;
        case 22: _t->set_topoViewDirty(); break;
        case 23: _t->reset_documentDirty(); break;
        case 24: _t->set_progressBarMax((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 25: _t->set_progressStep((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->show_nodeName((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->wms_state_chanaged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 29: _t->valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 30: _t->valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        case 31: _t->valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2]))); break;
        case 32: _t->valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 33: _t->valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QSize(*)>(_a[2]))); break;
        case 34: _t->valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 35: _t->valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 36: _t->itemMoved((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        case 37: _t->showAgvInfo((*reinterpret_cast< QGraphicsItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 38: _t->Warning((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->call_dialog_add(); break;
        case 40: _t->call_dialog_remove(); break;
        case 41: _t->call_dialog_enable(); break;
        case 42: _t->call_dialog_disable(); break;
        case 43: _t->call_dialog_adjust(); break;
        case 44: _t->add_store(); break;
        case 45: _t->remove_store(); break;
        case 46: _t->enable_store(); break;
        case 47: _t->disable_store(); break;
        case 48: _t->adjust_store(); break;
        case 49: _t->serial_write((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 50: _t->serial_read(); break;
        case 51: _t->onSystemTrayIconClicked((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 52: _t->addAGVintoSystem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->removeFromSystem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 54: _t->stopAgvSent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 55: _t->xuanzhongAGV((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 56: _t->get_agvAndMission((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 57: _t->whichMission((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 58: _t->agvInforList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 59: _t->addAGVasFangzhen((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 60: _t->receivFromTcp((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< char(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 61: _t->writeToTcp((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 36:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        case 37:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::update_progressBar)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::app_stateChange)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::IntoSySucc)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::diconnectFromSystem)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(bool , bool , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::missionfromId)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(int , bool , quint64 , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::missionTowrite)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::agvinfor)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::multiUsed)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (MainWindow::*_t)(int , QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::writeToServer)) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 62)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 62;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 62)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 62;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::update_progressBar(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::app_stateChange(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::IntoSySucc(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::diconnectFromSystem(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::missionfromId(bool _t1, bool _t2, int _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::missionTowrite(int _t1, bool _t2, quint64 _t3, bool _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::agvinfor(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::multiUsed()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void MainWindow::writeToServer(int _t1, QByteArray _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
