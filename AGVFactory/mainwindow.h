#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QAction>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QRubberBand>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QColorDialog>
#include <QSettings>
#include <QUndoStack>
#include <QTcpServer>
#include <QProgressBar>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSystemTrayIcon>
#include "centralscene.h"
//#include "centralview.h"
//#include "lineitem.h"
#include "topoview.h"
#include "missionmanager.h"
#include "trafficmanager.h"
#include "requestmanager.h"
#include "sweptarea.h"
#include "agvinformation.h"

#include "test_creationclass.h"
#include "ncentralview.h"
#include "lineitem.h"
#include "arcitem.h"
#include "rectitem.h"
#include "qtpropertymanager.h"
#include "QtVariantPropertyManager"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "change_combo.h"


#include "tcpserver.h"
class QtProperty;
class QLabel;
class AGV;
//class SettingDlg;
enum METHOD {NONE_METHOD,MEASURE};
struct AGV_INFORMATION{
    QString name;
    int startNode;
    QByteArray address;
    int type;
    int chargeNode;
    QStringList chargeLimit;
    AGV_INFORMATION()
    {
       startNode=0;
       chargeNode=0;
       type=0;
    }
};

struct Storage{
    QtProperty *Name;
    //QVector<QtVariantProperty *> layer;
    //QVector<QtVariantProperty *> goods;
    QtVariantProperty * layer;
    QtVariantProperty * storage;
};

class agvxianshi
{
   public:
    QtProperty *item1;
    QtProperty *item11;
    QtProperty *item12;
    QtProperty *item13;
    QtProperty *item14;
    QtProperty *item15;
    QtProperty *item16;
    QtProperty *item17;
    QtProperty *item18;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sleep(int msec);

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    void readdb_update_dockwindow();
    void update_topoview();
    //void update_topoview(int ID_1, int layer_1,int ID_2, int layer_2);
    void createActions();
    void createConnections();
    void createDockWindow();
    void createSceneAndView(string f,int flag);
    void createSceneAndView();//重载函数，目标是直接从数据库读取地图
    void getimgdxfborder(Test_CreationClass *creationClass);
    void getimgdxfborder();//重载函数，目标是直接从数据库读取地图
    void getWorldCoordinateborder(vector<BlockObj> ::iterator myblock, QPointF mypoint);//世界坐标系转换，得到dxf文件边界位置
    void drawDXF(QGraphicsScene *,QGraphicsView *,Test_CreationClass *);
     QPointF setWorldCoordinate(int rows,vector<BlockObj> ::iterator myblock, QPointF mypoint);//世界坐标系转换，得到dxf文件边界位置
    //void readCadMap(QString f);
    void createPropertyWindow();  // 创建属性窗口
    //void drawKuWei(QGraphicsScene *scene, QGraphicsView *, Test_CreationClass *creationClass);
    void updateToolbarUI(QAction *action=NULL);
    bool loadAGVs();
    QStringList get_recentOpenFileName();
    void set_recentOpenFileName(QString currentPath);
    void set_appState(int state);
    // 属性栏函数
    void addProperty(QtProperty *property, const QString &id);
    // 串口通信函数
    bool open_serialPort();
    void close_serialPort();
    QByteArrayList carAdress;
    bool isVerInArea(QPointF,QList<QPointF>);
private slots:
    void update_store(int start_nodeID, int end_nodeID, int start_layer, int end_layer);
    void kuweixiugai(int id,int huoliang);
    void dianliangdi();
    //void addItem(bool check=true);
    void selectChange();
    //void export_to_png();
    bool save();
    void load();
   // void newFile();
    void open_recentFile();
    void open_missionManager();
    void prepare();
    void run();
    void agv_information();
    void stop();
   // void edit_settings();
   // void setKeyLock(bool is_lock){m_bKeyLock = is_lock;}
    void set_documentDirty();
    void set_topoViewDirty(){m_bTopoViewChangeFlag = true;set_documentDirty();}
    void reset_documentDirty();
    void set_progressBarMax(int max, QString text);
    void set_progressStep(int value);
    void show_nodeName(bool flag);
    void wms_state_chanaged(bool connected);
    // 属性栏槽函数
    void valueChanged(QtProperty *property, double value);
    void valueChanged(QtProperty *property, const QString &value);
    void valueChanged(QtProperty *property, const QColor &value);
    void valueChanged(QtProperty *property, const QFont &value);
    void valueChanged(QtProperty *property, const QPoint &value);
    void valueChanged(QtProperty *property, const QSize &value);
    void valueChanged(QtProperty *property, const bool &value);
    void valueChanged(QtProperty *property, const int &value);
    void itemMoved(QGraphicsItem *item);
    void showAgvInfo(QGraphicsItem *item,int xiansudu,int jiaosudu);
    void  Warning(int id);//处理各种报警信息

    //库位管理槽函数
    void call_dialog_add();
    void call_dialog_remove();
    void call_dialog_enable();
    void call_dialog_disable();
    void call_dialog_adjust();
    void add_store();
    void remove_store();
    void enable_store();
    void disable_store();
    void adjust_store();

    // 串口通信槽函数
    void serial_write(QByteArray data);
    void serial_read();
    // 后台运行槽函数
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void addAGVintoSystem(int);
    void removeFromSystem(int);
    void stopAgvSent(int);

    void xuanzhongAGV(int,int,int,int,int,int,int,int);

    void get_agvAndMission(int id);
    void whichMission(int ,bool,bool,bool);
signals:
    //void MissionStatechanged();
    void update_progressBar(int);
    void app_stateChange(int state);
    void IntoSySucc(int id);
    void diconnectFromSystem(int id);
    void missionfromId(bool,bool,int,int);
    void missionTowrite(int agvid,bool hasmission,quint64 id ,bool);
    void agvinfor(int id);
    void multiUsed();
private:
   // bool m_bKeyLock;
    bool m_bDocumentChangeFlag;
    bool m_bTopoViewChangeFlag;
    int m_nAppState;
    bool m_bExportZoomNotRemind;
private:
    //CentralView *m_centralView;
    Test_CreationClass* creationClass;
    nCentralView *m_ncentralView;
    CentralScene *m_centralScene;
    QDockWidget *m_propertyWindow;
    QDockWidget *m_propertyWindow2;
    TopoView *m_topoView;
    QMenuBar *m_centralMenuBar;
    QToolBar *m_centralToolBar;
    MissionManager *m_missionManager;
    AgvInformation *m_agvinformation;
    TrafficManager *m_trafficManager;
    RequestManager *m_requestManager;
    SweptArea      *m_sweptArea;
    QMap<int,QList<int>>m_verInArea;
    QMap<int,QList<QPointF>>m_edge_points;
    QMap<int,QList<QPointF>>m_EquPointsInArea;
    QMap<int,QList<int>>m_collsionEdge;
    QMap<int,QList<int>>m_collsionEdgeVer;
    QMap<int,QList<int>>m_huicheEdges;
    QMap<int,QList<int>>m_commonEdges;
    QList<int>m_rukouEdges;
    QList<int>m_commonPoints;
    QList<int> m_agvInUse;
    QMap<int,int>m_agvidSocketScripte;
    QMap<int,AGV_INFORMATION>m_agvINformation;
    QMap<int,AGV_INFORMATION>m_agvINformationInuse;
    QMap<int,int>m_agv_count;
    QMap<int,Mission>m_agvMission;
    QMap<int,QAction*>m_agvidAction;
    QList<int>m_agvList;
    QList<int>m_agvShowList;//二级菜单
    QProgressBar *m_progressBar;
    QLabel *m_progressBarLabel;
   // SettingDlg *m_settingDlg;
    QAction *action_open;
    QAction *action_save;
   // QAction *action_export;
    QAction *action_exit;
   // QAction *action_undo;
    QAction *action_redo;
    QAction *action_rotate;
    QAction *action_antiRotate;
    QAction *action_up;
    QAction *action_down;
    QAction *action_left;
    QAction *action_right;
   // QAction *action_translateUpDown;
   // QAction *action_translateLeftRight;
    QAction *action_zoomin;
    QAction *action_zoomout;
    QAction *action_addedge;
    QAction *action_measure;
    QAction *action_missionManager;
    QAction *action_requestManager;
    QAction *action_multiSystem;
    QAction *action_prepare;
    QAction *action_run;
    QAction *action_stop;
    QAction *action_communicationSettings;
    QAction *action_showNodeName;
    QAction *action_toolbar;
    QAction *action_propertyWindow;
    QAction *action_about;
    QAction *action_help;
    QAction *action_agvInformation;
    QAction *action_agvInfo;//agv信息栏

    //库位管理菜单栏及侧边栏
    QAction *action_add;
    QAction *action_remove;
    QAction *action_enable;
    QAction *action_disable;
    QAction *action_adjust;
    QDockWidget *dw_storage;
    QString m_strId;
    QString m_strHasstore;
    QString m_strLayer;
    ChangeComboDialog *ccDialog;
    QVector<Storage> Stores;
    QVector<QtProperty *> topItems;
    //拓扑图库位显示
    QList<RectItem *> rectitems;
   // QSpinBox *spin_zoom;
    QMenu   *menu_agvinfo;

    // 属性栏变量
    QtDoublePropertyManager *doubleManager;
    QtDoublePropertyManager *doubleAgvInfor;
    QtStringPropertyManager *stringManager;
    QtStringPropertyManager *stringAgvInfor;
    QtColorPropertyManager *colorManager;
    QtFontPropertyManager *fontManager;
    QtPointPropertyManager *pointManager;
    QtPointPropertyManager *pointAgvInfor;
    QtSizePropertyManager *sizeManager;
    QtSizePropertyManager *sizeAgvInfor;
    QtBoolPropertyManager *boolManager;
    QtBoolPropertyManager *boolAgvInfor;
    QtIntPropertyManager *intManager;
    QtIntPropertyManager *intAgvInfor;
    QtTreePropertyBrowser *propertyEditor;
    QtTreePropertyBrowser *propertyEditor2;
    QtGroupPropertyManager *groupManager;
    QMap<QtProperty *, QString> propertyToId;
    QMap<QString, QtProperty *> idToProperty;
    QMap<QString, bool> idToExpanded;
    QMap<int,agvxianshi>idAgvXinshi;
    QGraphicsItem *m_currentItem;

    //库位信息栏变量
    QtVariantPropertyManager *store_variantmanager;
     QMap<int,QtVariantProperty*>idgoods;
    //QtVariantEditorFactory *variantfactory;
    QtTreePropertyBrowser *store_variantEditor;
//    QVector<QtProperty *> Name;
//    QVector<QtVariantProperty *> layer;
//    QVector<QtVariantProperty *> goods;



    // 串口通信变量
    QSerialPort *serial;
    QMap<QByteArray,AGV*> m_mapIDtoAGV;
    //QMap<int,AGV*> m_IDtoAGV;
    QMap<AGV*,int> m_AGVtoID;
    QMap<int,AGV*> m_IDtoAGV;
    // 后台运行变量
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    QByteArray readbuf;
    bool multi;
    bool agvNumChange;
    //边界
    qreal xleftborder;
    qreal xrightborder;
    qreal ytopborder;
    qreal ybuttomborder;
    qreal zoom;
    QPointF coordinatemove;//坐标偏移


     TcpServer ser;
public slots:
    void agvInforList(bool);

    void addAGVasFangzhen(int,int );

    void receivFromTcp(int,char,QByteArray);

    void writeToTcp(QByteArray);
 signals:
    void  writeToServer(int,QByteArray);
};

#endif // MAINWINDOW_H
