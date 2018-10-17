#ifndef AGV_H
#define AGV_H
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <QMap>
#include <QDebug>
#include <QFile>
#include "topoview.h"
#include "global.h"


enum AGY_TYPE{NONE_TYPE,PT20,PS20,PH20};


//AGV指令结构体
struct AGVCommand{
    int start;
    int end;
    int angel;
    bool state;//表示任务是否发送
    int number;//用于车辆初始化
};

struct AGV_POWER{
    int m_nVoltage;
    int m_nPercentage;
    char m_errorCode[2];
    AGV_POWER(){
        m_nVoltage = 0;
        m_nPercentage = 0;
        m_errorCode[0] = 0;
        m_errorCode[1] = 0;
    }
};
typedef struct{
    int previous=-1;
    int previous_before=-1;
}Pre;

#define ps_leftx -420*0.4
#define ps_lefty 1700*0.4
#define ps_wight 840*0.4
#define ps_hight 2100*0.4

#define a_leftx -800*0.4
#define a_lefty -800*0.4
#define a_wight 1600*0.4
#define a_hight 1600*0.4

#define b_leftx -800*0.4
#define b_lefty -800*0.4
#define b_wight 1600*0.4
#define b_hight 1600*0.4
class AGV : public QObject,public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QPoint _center READ get_center WRITE set_center)
    Q_PROPERTY(int _y READ y WRITE set_y)
    Q_PROPERTY(int _rotate READ rotation WRITE set_rotation)
    Q_PROPERTY(int _pen_width READ get_penWidth WRITE set_penWidth)
    Q_PROPERTY(QColor _pen_color READ get_penColor WRITE set_penColor)
    Q_PROPERTY(QColor _brush_color READ get_brushColor WRITE set_brushColor)
    Q_PROPERTY(QString _name READ get_name)
    Q_PROPERTY(int _pen_style READ get_penStyle)
    Q_PROPERTY(int _power READ get_powerPercent)
    Q_PROPERTY(QString _state READ ReadmissionState)
    Q_PROPERTY(int  _jiaosudu READ get_jiaosudu)
    Q_PROPERTY(int  _xiansudu READ get_xiansudu)
public:
    enum AGV_MISSION_EXECUTABLE{AGV_MISSION_EXECUTABLE_NO=0,AGV_MISSION_EXECUTABLE_UNKNOWN,AGV_MISSION_EXECUTABLE_YES};
    enum AGV_MODE{SINGLE_STEP_MODE = 100,MULTI_STEP_MODE};
public:

    AGV(QGraphicsScene *scene, TopoView* topoview,qreal zoom,QPointF coordinatemove, int type, QString name, int id, QByteArray address, int startNode,int chargeFullLimit, int chargeUpLimit,int chargeDownLimit, int chexing,QPointF center=QPointF(),int rotate=0);
    ~AGV();
    QColor get_penColor() const{return pen().color();}
    QColor get_brushColor() const{if(brush().style()==Qt::NoBrush) return Qt::white;else return brush().color();}
    QString get_name(){return m_strID;}
    QByteArray get_address(){return m_bytesAddress;}
    void set_topoPosID(int id){m_nTopoPosID = id;}
    void set_targetID(int target_id,int dest_id){m_nTargetID = target_id;m_nDestinationID=dest_id;}
    void set_targetCommand(AGVCommand com1,AGVCommand com2 );
    int get_Commandnumber();
    AGVCommand get_CommandInfo(int i);

    void set_targetID_layer(int startlayer,int endlayer){m_nStartLayer=startlayer;m_nEndLayer=endlayer;}
    void set_forkUp();
    void set_forkDown();
    void set_plateArriveDetectEnable(bool flag=false);
    void set_autoLiftFork(bool flag, int target_layer=1);
    void set_restNode(int node);
    void set_missionExecutable(int flag){m_nIsMissionExcutable = flag;}
    void set_mode(int mode){m_nMode = mode;}
    void set_forkLayer(int layer){m_nTargetForkLayer = layer;}
    void set_startNode(int node){m_nStartID = node;}
    void set_chargeLimit(int high,int low){m_nChargeHighLimit = high;m_nChargeLowLimit=low;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //int get_nowid(return getNearstMatchedVer(m_topoView,mapToRealWorld( pos()),m_nTopoPosID););
    int get_mode(){return m_nMode;}
    int get_missionExecutable() const{return m_nIsMissionExcutable;}
    int get_restNode(){return m_nRestNode;}
    int get_forkLayer(){return m_nCurrentForkLayer;}
    int get_forkPos(){return m_nForkPos;}
    void set_forkPos(int pos){m_nForkPos=pos;}
    int get_startNode(){return m_nStartID;}
    int layerToHeight(int layer);
    int get_state(){return m_nState;}
    int get_topoPosID(){return m_nTopoPosID;}
    int get_id(){return m_nID;}
    int get_targetID(){return m_nTargetID;}
    int get_powerPercent(){return dianliang;}//有修改
    int get_penWidth() const{return pen().width();}
    int get_penStyle() const{return pen().style();}
    int get_chargeHighLimit(){return m_nChargeHighLimit;}
    int get_chargeLowLimit(){return m_nChargeLowLimit;}
    int get_chargeFullLimit(){return m_nChargeFullLimit;}
    int get_count(){return m_agvcount;}
    int get_chushidian(){return chushidian;}
    int get_xiansudu(){return xiansudu;}
    int get_jiaosudu(){return jiaosudu;}
    QPoint get_center() const;
    int type() const;
    bool is_needToCharge(){return m_bNeedToCharge;}
    bool is_canCharge(){return m_bCanCharge && m_stPower.m_nPercentage < m_nChargeFullLimit;}
    void set_path(AGVCommand command1, AGVCommand command2);
    void set_path(int start,int end); //提出来  原来在private
    int missionlength;//统计一次任务的长度
    QString ReadmissionState(){  return mission_state; }
    bool    communicateState(){return  commState;}
    void    setCommState(bool state){commState=state;emit agv_moved(this);}
    void stateRecord(bool,bool);

    QMap<int,AGVCommand> agvcommand;//当前AGV执行的所有指令，以及每条指令编号6.22
public:
    void set_pathEnable(bool enable);
    void CommandManager(int number);
    void sendCommand(AGVCommand command1, AGVCommand command2);//双指令执行函数重映射
    void sendCommand(int start,int end);

    void timerEvent(QTimerEvent *);
    void tcp_getPos();
    void tcp_getmissionlength();//获取任务长度
    void tcp_getState();
    void tcp_getInventory();
    void tcp_setFunction();

    void tcp_getErrorCode();
    void tcp_getPower();
    bool errorCodeToString(QString &error_info);  //获取错误信息输出，无错误返回true
     QMap<int,QList<int>>m_AGVverInArea;
      int m_nTimerID;
      int dianliang;
      bool missionflag1;
      bool qumission;//代表一段延时时间后，取货任务完成
      bool songmission;//代表一段延时时间后，送货任务完成
      quint64 nowmissionid;
      int agvchexing;

      AGVCommand Command1;
      AGVCommand Command2;

private:
    Qt::PenStyle m_nPenStyle;
    int m_nType;
    int m_nState;
    int m_nTopoPosID;          // 当前点ID
    int m_nTargetID;           // 目标点ID
    int m_nStartLayer;
    int m_nEndLayer;
    int priority0;//巷道内
    int priority1;//普通
    int priority2;//十字路口
    int m_nTargetID_PT20;           // 目标点ID

    int m_nDestinationID;      // 路径终点ID
    int m_nStartID;
    int m_nID;                 // 车辆ID
    int m_nTimerCnt;
    int m_nRestNode;           // 车库ID
    int m_nCurrentForkLayer;
    int lastForkLayer;
    int m_nTargetForkLayer;
    int m_nChargeHighLimit;
    int m_nChargeLowLimit;
    int m_nChargeFullLimit;
    bool m_bNeedToCharge;
    AGV_POWER m_stPower;
    char m_bytesErrorCode[4];

    int chushidian;
    bool tingchedengdai;//目的是判断现在车辆是否处在停车等待状态

    QString m_strID;
    int m_agvcount;
    int m_nIsMissionExcutable;
    QMap<int,QString> s_mapDepotNode;

    QMap<int,QList<int>> collionSegments;
    QMap<int,QList<int>> collionSegmentWithout;
    QMap<QPair<int,int>,qreal> distance;



    QByteArray m_bytesAddress;


    int m_nForkPos;  //pt20需要
    // 日志记录变量
    QFile m_file;
    int m_nMode;
    // 通信部分变量

    int m_nFinishTimerCnt;
    bool m_bIsPathOK;
    bool m_bIsEnableOK;
 //   bool m_bIsPowerOK;
//    bool m_bIsErrorOK;
    bool m_bCanCharge;
    int m_nCommandStart;
    int m_nCommandEnd;

    int m_nCommandStart1;//双指令运行
    int m_nCommandEnd1;

 //    bool m_bIsFunctionOK;//pt20

    QByteArray m_bytesTargetFunction;
    QByteArray m_bytesCurrentFunction;
    TopoView *m_topoView;

//    quint64 carID_PT20;
 //   quint64 carID_PS20;
    QString mission_state;
    bool commState;
    int xiansudu;
    int jiaosudu;
signals:
    void agv_moved(QGraphicsItem*);
    void state_moved(QGraphicsItem*,int,int);
    void state_change();
    void state_change_PT20();
    void serial_write(QByteArray data);
    void tcp_write(QByteArray data);
    void mission_abort(QString agvname);
    void report_error(QString,char);

    void dianliangjinggao();//电量警告
    void  missionwancheng(quint64,QString,AGV*);
public slots:
    void set_x(qreal x){if(isSelected()) setPos(x,pos().y());}
    void set_y(qreal y){if(isSelected()) setPos(pos().x(),y);}
    void set_center(QPoint center);
    void set_rotation(int rotate){if(isSelected()) setRotation(rotate);}
    void set_dashLine(bool lineType);
    void set_size(int w,int h){if(isSelected()) setRect(-w/2,-h/2,w,h);}
    void set_penWidth(int w){if(isSelected()){QPen p = pen();p.setWidth(w+1);setPen(p);}}
    void set_penColor(QColor color){if(isSelected()){QPen p = pen();p.setColor(color);setPen(p);}}
    void set_brushColor(QColor color){if(isSelected()){if(color==Qt::white) setBrush(Qt::NoBrush); else setBrush(color);}}
    void set_name(QString name){if(isSelected()) m_strID = name;}
    void set_selected(){scene()->clearSelection();setSelected(true);}
    void recvData(char frame_type, QByteArray data);

    void show_errorInfo();
    void changeForkState(int value);
    int   get_forkState(){return ForkState;}
    void setLastFork(int value);
    int get_lastFork(){return lastForkLayer;}
    Pre  get_previous();
    void set_AGVpath(QList<int> path){AGVpath=path;}



private:
    int  ForkState;
    QList <int> AGVpath;

    QList<int> previousQueue;
    Pre prePoints;
    int previousPoint;
    qreal m_zoom;
    QPointF m_coordinatemove;
    qreal m_shigh;
    int cishu;
public:

    int IsTargetInTunnel(quint32 target);
    QList<int> pointsInTunnel(int tunnel);

   // bool segments_intersect (QPointF p1, QPointF p2, QPointF p3, QPointF p4);
   // int direction (QPointF p0, QPointF p1, QPointF p2);
   // void release(int current,int target);
  //  void release_normal(int current,int target);
   // void release_InTunnel(int current,int target);
   // void release_cross(int current,int target);
    int get_prePoint();

    QMap<int,QList<int>> aroundTargetPoints;
};

#endif // AGV_H
