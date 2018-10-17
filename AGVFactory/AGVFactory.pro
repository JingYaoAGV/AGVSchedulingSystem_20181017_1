#-------------------------------------------------
#
# Project created by QtCreator 2015-10-30T19:53:17
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
QT       += serialport
CONFIG   += C++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AGVFactory
TEMPLATE = app

INCLUDEPATH +=src
SOURCES += main.cpp\
        mainwindow.cpp \
    centralscene.cpp \
    agv.cpp \
    topoview.cpp \
    global.cpp \
    missionmanager.cpp \
    trafficmanager.cpp \
    requestmanager.cpp \
    wmsinterface.cpp \
    sweptarea.cpp \
    agvinformation.cpp \
    agvremovewizard.cpp \
    src/dl_dxf.cpp \
    src/dl_writer_ascii.cpp \
    test_creationclass.cpp \
    ncentralview.cpp \
    lineitem.cpp \
    arcitem.cpp \
    rectitem.cpp \
    change_combo.cpp \
    tcpserver.cpp \
    tcpsocket.cpp \
    threadhandle.cpp

HEADERS  += mainwindow.h \
    centralscene.h \
    agv.h \
    topoview.h \
    global.h \
    missionmanager.h \
    trafficmanager.h \
    requestmanager.h \
    wmsinterface.h \
    sweptarea.h \
    agvinformation.h \
    agvremovewizard.h \
    src/dl_attributes.h \
    src/dl_codes.h \
    src/dl_creationadapter.h \
    src/dl_creationinterface.h \
    src/dl_dxf.h \
    src/dl_entities.h \
    src/dl_exception.h \
    src/dl_extrusion.h \
    src/dl_global.h \
    src/dl_writer.h \
    src/dl_writer_ascii.h \
    test_creationclass.h \
    block_obj.h \
    ncentralview.h \
    lineitem.h \
    arcitem.h \
    rectitem.h \
    change_combo.h \
    hashtable.h \
    hashset.h \
    tcpserver.h \
    tcpsocket.h \
    threadhandle.h

FORMS    += \
    request.ui \
    agvinformation.ui


RESOURCES += \
    images.qrc

RC_FILE = appicon.rc
include(../qtpropertybrowser/src/qtpropertybrowser.pri)

