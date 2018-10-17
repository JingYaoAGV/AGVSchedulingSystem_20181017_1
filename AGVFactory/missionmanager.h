#ifndef MISSIONMANAGER_H
#define MISSIONMANAGER_H

#include <QWidget>
#include <QDialog>
#include <QTableView>
#include <QDateTime>
#include <QStandardItemModel>
#include <QList>
#include <QPushButton>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QSqlTableModel>
#include "topoview.h"
#include "agv.h"
#include "change_combo.h"
#include <QTime>
#include <QTextEdit>
#include <QtAlgorithms>
#include <QPointF>
#define MISSION_STATE_WAIT "等待"
#define MISSION_STATE_BACK "返回"
#define MISSION_STATE_PICK_UP "取货"
#define MISSION_STATE_DELIVERY "放货"
#define MISSION_STATE_DAOKU "到库"
#define MISSION_STATE_FINISH "完成"
#define MISSION_STATE_ABORT "终止"
#define MISSION_STATE_SUSPEND "挂起"
#define MISSION_STATE_ERROR "异常"





class WMSInterface;

extern QDateTime s_universeStart;
class Mission{
public:
    quint64 m_nID;
    QString m_strAGVID;
    int m_nStartNodeID;
    int m_nEndNodeID;
    int m_nPriority;
    int m_nStartNodeLayer;
    int m_nEndNodeLayer;
    bool m_bReachStartNode;
    bool m_bReachEndNode;
    QDateTime m_startTime;
    QDateTime m_endTime;
    QString m_strState;
    Mission(QString strAGVID="",int nPriority=0){
        m_startTime = QDateTime::currentDateTime();
        m_nID = s_universeStart.msecsTo(m_startTime);
        m_strState = MISSION_STATE_WAIT;
        m_strAGVID = strAGVID;
        m_nPriority = nPriority;
        m_bReachStartNode = false;
        m_bReachEndNode = false;
        m_nStartNodeLayer = 0;
        m_nEndNodeLayer = 0;
        m_nStartNodeID=-1;
        m_nEndNodeID=-1;
    }

};

//-------------------------6.20之前版本，任务流程分配及发出--------------------
class MissionState{
public:
   bool state1;
   bool state2;
   bool state3;
   bool state4;
   bool liuchengstate1;
   bool liuchengstate2;
   bool liuchengstate3;
   bool liuchengstate4;
   MissionState()
   {state1=false;//流程完成标志
       state2=false;
       state3=false;
       state4=false;
       liuchengstate1=false;//流程正在进行中标志
       liuchengstate2=false;
       liuchengstate3=false;
       liuchengstate4=false;
   }
   void clear()
   {state1=false;
    state2=false;
    state3=false;
    state4=false;
    liuchengstate1=false;
    liuchengstate1=false;
    liuchengstate2=false;
    liuchengstate3=false;
    liuchengstate4=false;
   }
   bool test1()
   {if(state1&&state2&&state3&&state4)
         return true;
    else
           return false;
   }
   bool test2()
   {if(state1&&state2)
         return true;
    else
           return false;
   }
 };
//-----------------------------6.20版本，任务分配及发出--------------


class MissionManager : public QWidget
{
    Q_OBJECT
public:
    enum MISSION_PRIORITY {PRIORITY_VERYLOW,PRIORITY_LOW,PRIORITY_MIDDLE,PRIORITY_HIGH,PRIORITY_VERYHIGH};
public:
    explicit MissionManager(QWidget *parent = 0);
    ~MissionManager();
    void createMissionDialog();
    void createTestDialog();//独立测试窗口
    void createTableWidget();
    void createButtons();
    void createConnections();
    void createLayout();
    bool getNextMission(QString AGVName ,Mission &mission);
    bool getNextMission(QString AGVName, Mission &mission,AGV*agv);
    bool getNextMission(QString AGVName, Mission &mission,int startNotEqualTo);
    Mission createMission(int nstartID, int nEndID, QString strAGVID=0, MISSION_PRIORITY nPriority=PRIORITY_MIDDLE);
    void readgoods();
    bool open_database();
    bool save_mission(Mission mission);
    bool update_mission(Mission mission);
    bool delete_mission(Mission mission);
    void setMissionState(Mission mission, QString state);
    void setMissionState(quint64 id,QString state,Mission mission);
    void guidaoAGV2();
    void setMissionState(quint64 id,QString state);
    void MissionToRecord(Mission mission,int chexing);
    void addMission(Mission mission);
    void editMission(Mission mission);
    bool dijkstart(int startID, int endID, QList<int> &path,double &cost);
    void recontruct_path(int startID,int endID,QList<int> &path);

    int MissionDistribution(int startnode);//按最近邻原则寻找任务终点，产生任务
    int MissionDistribution(int startnode,bool flag);
    int MissionAllcoation(int startnode);
    int AGV_KuweiDis();
    bool SearchNextMission(QString AGVName, Mission &mission,AGV*agv);
public:
    QList<AGV*> m_agvList;
    QMap<QString,AGV*> m_agvNameMap;
    QMap<int,QString> m_agvIdName;
    QMap<int,AGV*> m_agvId;
    QMap<AGV*,bool> m_agvState;
    QMap<AGV*,bool> m_agvState1;
    QMap<int,int> idhuoliang;//4.9
    QMap<int,uint>quA;
    QMap<int ,uint>quB;
    QMap<int,uint>songA;
    QMap<int,uint>songB;
    QMap<int,int>idlayer;
    MissionState*Aquhuo;
    MissionState*Bquhuo;
    MissionState*Asonghuo;
    MissionState*Bsonghuo;
    bool guidaostate;
    bool guidaostate2;
    bool upxiang;
    bool downxiang;
    int Azhuangpei;
    int Bzhuangpei;
    int Achaijie;
    int Bchaijie;
    int ABzhuangpei;
    int xiangup;
    int xiangdown;
    int A;
    int B;
    bool Aqu;
    bool Asong;
    bool Bqu;
    bool Bsong;
    int xunhuantimer;
    int zhuangpeixiantimer;
    int guntongtimer;
    QList<int> zhuangpeitimer;

    QMap<int,AGV*> m_agvId1;

    QMap<int,int>idzhandian;
    QMap<int,int>idzhuangtai;
    QMap<int,int>downlayer;
    QMap<int,int>uplayer;
    QMap<int,int>idzhanyong;

    QMap<int,QList<Dis_point>>AgvKuwei;


   QMap<int,bool>agvState;
    QMap<int,bool>lock_kuwei;//表示被占用or有任务的库位


    int fenjian;
    int chuku;
    int RuKuid;
    int Chukuid;


public:
    void add_store(QString st1, QString st2);
    void remove_store(QString st);
    void quhuoA();
    void quhuoB();
    void songhuoA();
    void songhuoB();
    void guidaoAGV();
    void liaoxiangup();
    void liaoxiangdown();
    void zhuangpeixian();
    void jiaruhuojia();
    void Delay_MSec_Suspend(unsigned int msec);
    void Delay_MSec(unsigned int msec);

    static bool CompareDistance(const Dis_point&a1,const Dis_point&a2)//从小到大
    {
       if(a1.distance<=a2.distance)
          return true;
       else
           return false;
   }

 private:
    QTableView *m_tableMission;
    //QStandardItemModel *m_tableModel;
    QSqlTableModel *m_sqlTableModel;
    QPushButton *m_btnCreate;
    QPushButton *m_btnEdit;
    QPushButton *m_btnHang;
    QPushButton *m_btnDelete;
    QPushButton* m_btnxunhuan;//循环4.3
    QPushButton* m_btnjinzhi;//禁止循环4.3

    QPushButton *m_btnTest;//测试AGV通信帧
    QPushButton *m_btnTestStop;//终止测试模式

    QList<Mission> m_missionList;

    QMap<int,int> idhuojia;


    TopoView *m_topoView;
    QStringList m_callPointList;
    QString m_databasePath;
    // 对话框
    QDialog *m_missionDlg;
    QDialog*m_testDlg;
    QComboBox *m_comboAGV;
    QComboBox *m_comboAGVType;
    QComboBox *m_comboStart;
    QComboBox *m_comboEnd;
    QComboBox *m_comboPriority;
    QLineEdit *m_lineStartLayer;
    QLineEdit *m_lineEndLayer;

    // WMS 接口
    WMSInterface *m_wms;
   QSqlQuery m_query;

    // 独立测试对话框
   QPushButton *QianJinZhenchang;
   QPushButton *YouGuai;
   QPushButton *ZuoGuai;
   QPushButton *HouTuiZhenchang;
   QPushButton *HouTuiJiansu;
   QPushButton *QianJinJiansu;
   QPushButton * Tingche;//禁止循环4.3
   QPushButton * Chushihua;//禁止循环4.3
   QTextEdit *RecvCommand;
   QTextEdit *SendCommand;
   QComboBox *TestAGV;
   QComboBox *Targetnode;
   QLineEdit*nodeDistance;

private:
    Mission recordToMission(QSqlRecord record);
    void showAllRecord();

    void timerEvent(QTimerEvent*event);
    QMap<int,int> came_from;
    QMap<int,double> cost_so_far;
signals:
    // 呼叫器接口
    void mission_accomplished(int start,int end);
    void MissionStateChanged();
    void mission_deleted(quint64 id);
    void wms_connected(bool);
    void MissionStateChanged(int start_nodeID, int end_nodeID);
    void huoliangxiugai(int,int);
public slots:
    void loadTopoView(TopoView *topoview);
    void addAGV(AGV* agv);
    void removeAGV(AGV* agv);
    void action_create();
    void action_edit();
    void action_hang();
    void action_delete();
    void action_xunhuan();
    void action_jinzhi();
    void action_Test();//独立测试窗口
    void action_Chushihua();//独立测试窗口
    void action_TestStop();//退出测试模式
    void action_QianjinZhenngchang();//独立测试窗口
    void action_QianjinJiansu();//独立测试窗口
    void action_HoutuiZhengchang();//独立测试窗口
    void action_HoutuiJiansu();//独立测试窗口
    void action_Zuoguai();//独立测试窗口
    void action_Youguai();//独立测试窗口
    void action_Tingchedengdai();//独立测试窗口
    void action_recvCommand(AGVCommand com,int Dongzuo,int Fangxiang,int agvid);
    void action_nodeDistance(int index);

    void new_request(QPair<int, int> info,int priority=PRIORITY_LOW);
    // WMS通信接口
    void new_request(quint64 id, QPair<int,int> startInfo, QPair<int,int> endInfo, int priority);
    void inquiry_state(quint64 ID);
    void stop_request(quint64 ID);
    void report_error(quint64 ID,char error_code);
    void mission_finished(quint64 ID);
    void MissionArrange(AGV*,bool);
    void MissionResetOrFin(int agvid,bool hasmission,quint64 missionid,bool reset);
    void jinzhiAGV()
    {
     AGV* agv = (AGV*)sender();
     int id=m_comboAGV->findData((agv->get_id()));
     if(id!=-1)
        m_comboAGV->removeItem(id);
    }//仍然需要测试
    void setMissionState(quint64 id, QString state, AGV *agv);//3.28 函数重载，目的是获得任务路径长度(换为slot）
};

#endif // MISSIONMANAGER_H
