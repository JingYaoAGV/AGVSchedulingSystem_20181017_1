#ifndef TRAFFICMANAGER_H
#define TRAFFICMANAGER_H
#include <QObject>
#include <QMap>
#include <QWidget>
#include "missionmanager.h"
class TopoView;
class MissionManager;
class AGV;
class Mission;

struct St_Mission{
    Mission m_mission;
    bool m_bLastIsCurrent;  // 上一个任务终点是当前任务起点
    bool m_bCurrentIsNext;  // 本次任务起点是下一个任务终点
    St_Mission(Mission mission,bool last_is_current=false,bool current_is_next=false){
        m_mission = mission;
        m_bLastIsCurrent = last_is_current;
        m_bCurrentIsNext = current_is_next;
    }
    St_Mission(){}
};
struct AGVCommand_
{
    int start;
    int end;
    int angel;


};


class TrafficManager :public QWidget
{
    Q_OBJECT
public:
    TrafficManager(MissionManager* missionManager,QWidget *parent=0);
    ~TrafficManager();

    void loadTopoView(TopoView* topoview);

    bool dijkstra_search(AGV *agv, int startID, int endID, QList<int> &path,double &cost);
    void show_path(AGV *agv);
    bool addAGV(AGV* agv);
    void removeAGV(AGV* agv);
    void loadDatabase();
    bool panduan;
    void guidaoglobalplanning(AGV*agv,Mission old_mission);
    QMap<QString,QList<int>>mapPath_change;
    QMap<int,QList<AGVCommand_>>agvCommand;//指令集合

    void CommandProduce(int current_node,QList<int> path, QList<AGVCommand>&Command);//将AGV的path 生成一系列的Command;
    void CommandProcess();
    QMap<int,AGVCommand>Command;
    QMap<QString, QList<AGVCommand>>agv_command;
    QHash<int,QString>lockpoint;
    int willCollsion_Region(AGVCommand &com1,AGVCommand &com2,AGVCommand Command1,AGVCommand Command2,QString agvname,AGV*agv);

private:
    void recontruct_path(int startID, int endID, QList<int> &path);
    bool contain_paths(int startID,int endID);
private:
    MissionManager *m_missionManager;
    TopoView *m_topoView;
    QList<AGV*> m_agvList;
    QMap<int,QString>m_agvidname;
    QMap<int,AGV *>m_idAGV;
    QMap<int,AGV*>timeridagv;//保存建立的agv延时定时器




   // QList<int> m_commonRoute;

    QList< QPair<int,int> > m_listPaths;
    bool m_bAutoLiftFork;    //是否边走边抬叉
    bool m_bAutoDropFork;    //是否边走边放叉

    // Dijkstra 变量
    QMap<int,int> came_from;
    QMap<int,double> cost_so_far;
    QMap< QString,QList<int> > m_mapPath;
    QMap< QString,QList<int> > m_oldPath;
    QMap< QString,QList<int> > m_chongtuPath;
    QMap<int, QList<int> > m_collionSegments;
    QMap<int,QList<QPointF>>m_equPointInArea;
    QMap<int, QList<int> > m_collionEdge;
    QMap<int, QList<int> > m_collionEdgeVer;
    QMap<int, QList<int> > m_huicheEdges;
    QMap<int,QPair<int,int>>AGVTunnelPri;
   // QMap<int,QPair<int,qreal>>AGVTunnelCost;
    QMap<int,int> m_verIdTunnel;
    QList<int> rukouIDS;
    QList<int> m_commPoints;
    QMap<int,QList<int>> m_commEdges;
    QMap<int,QList<int>> m_pausePoints;
protected slots:
    void updateGlobalPlanning();

    void report_error(QString agvname, char error_code);
    void mission_abort(quint64 id);
    void someInforDelate(int agvid);
public slots:
    void multisystemused();
    void pauseAgv(int agvid,int pointID);
    void keepOn(int id);
private:
   bool IsReachEnd;
   bool IsReachStart;
   QSqlQuery m_queryServer;
   bool multiUsed;

  int CURRRENTAGVSYSTEM;
  bool shilian;

  QFile m_file;
public:
   void traffic_control_type1();
   void swap_pri(int *a,int *b);
   void shengchengquyu(AGV*,QList<int>,QList<int>);
signals:
   void AGVstateChange(AGV*,bool);
   void MissionStateChanged(int start_nodeID, int end_nodeID, int start_layer, int end_layer);
public:
   QMap<int,QList<int>>m_TrafficVerInArea;
   void register_normal(AGV*,int current, int target);
   void register_InTunnel(AGV*,int current, int target);
   void register_InCrossroads(AGV*,int current, int target);
   int IsTargetInTunnel(quint32 target);
   QList<int> pointsInTunnel(int tunnel);
   void setcrossPoint(QMap<int,QList<int>>cross);
   void EquPoInArea(QMap<int,QList<QPointF>>point);
   void collsionEdge(QMap<int,QList<int>>collsionEdge);
   void collsionEdgeVer(QMap<int,QList<int>>collsionEdgeVer);
   void  huicheEdges(QMap<int,QList<int>> huicheedges);
   void  rukouEdges(QList<int>RuKouedges);
   void  commonPoints(QList<int>points);
   void  commonEdges(QMap<int,QList<int>>commonEdges);
   bool  collsion(AGV*,int current,int target);
   bool  willCollsion(AGV*,int current,int target);
   bool  willCollsionmulti(AGV*, int current,int target);
   bool  willCollsionNew(AGV*,int current,int target);//3.30新的判断是否会冲突的函数

   bool willCollsionchange(AGV*);//新的计算冲突的函数3.31
   bool  willCollsionmultiNew(AGV *, int current, int target);
 //向导
   //bool hasMission(int id);
    QMap< QString,St_Mission > m_mapAGVandMission;

};
#endif // TRAFFICMANAGER_H
