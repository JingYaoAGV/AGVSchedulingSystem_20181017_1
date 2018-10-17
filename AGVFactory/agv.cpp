#include "agv.h"
#include <QGraphicsScene>
#include <QDataStream>
#include "mainwindow.h"
#include "missionmanager.h"
#include "global.h"
#include <QtMath>
#include <QMessageBox>
#include <QTimerEvent>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QDebug>
#include <QtDebug>
#include <QString>


//#define PROTOCAL_HEAD stringToAddr("7e00171001")
#define PROTOCAL_HEAD stringToAddr("7e001710")
#define PROTOCAL_MID stringToAddr("fffe0000")
#define FUNCTION_PLATE_ARRIVE_ENABLE_MASK 0x08
#define FUNCTION_AUTO_LIFT_FORK_MASK 0x01

#define agvPT20_01    0x0013A20040E56ECE
#define agvPS20_01    0x0013A20041532210
#define  ThHoDis      3.00
#define  ThHoDisx      1.00

#define FRAME_GET_CHARGE 0x75


#define MISSION_STATE_WAIT "等待"
#define MISSION_STATE_BACK "返回"
#define MISSION_STATE_PICK_UP "取货"
#define MISSION_STATE_DELIVERY "放货"
#define MISSION_STATE_FINISH "完成"
#define MISSION_STATE_ABORT "终止"
#define MISSION_STATE_SUSPEND "挂起"
#define MISSION_STATE_ERROR "异常"

int _null = 0x00;
QColor color_list[7] = {Qt::red,Qt::yellow,Qt::white,Qt::cyan,Qt::magenta,Qt::darkRed,Qt::lightGray};
int agv_count = 0;
int s_forkHeight[6] = {1,10,85,94,112,120};
AGV::AGV(QGraphicsScene *scene, TopoView* topoview, qreal zoom,QPointF coordinatemove,int type,QString name, int id, QByteArray address,int startNode,int chargeFullLimit,int chargeUpLimit,int chargeDownLimit,int chexing,int rotate,QPointF center )
{
    CurrentMissionID=0;

    iconzoom=0.1;
    m_zoom=zoom;
    StateWait=false;

    //--------测试版直接赋值-------
    agvAngel=rotate;
    m_nTopoPosID=-1;
    //------------------------


    AGVChushihua=false;//表示接收到真实AGV的初始化命令
    TestModel=false;//最初的形式，testmodel是开启的

    Chushihua=false;
    Trafficmanager=true;//表征当前的交通规划完成

    CommandNumber=0;

    //Command1={0,0,0,false,0};
    //Command2={0,0,0,false,0};

    nowmissionid=0;
    missionflag1=false;
    xiansudu=0;
    jiaosudu=0;
    dianliang=100;
    chushidian=startNode;//初始点应该由车辆自己发送
    agvchexing=chexing;//面向仿真修改的，
    AGVType=0;//与实际车辆通信的
    AngleChushi=rotate;

    setPos(center);
    scene->clearSelection();
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    m_nPenStyle = Qt::SolidLine;
    m_bytesTargetFunction.append(_null);
    m_bytesTargetFunction.append(_null);
    m_bytesTargetFunction.append(_null);
    m_bytesTargetFunction.append(_null);

    m_bytesCurrentFunction = m_bytesTargetFunction;

    ForkState=0;


    QPen p = pen();
    p.setWidth(1);
    m_agvcount=agv_count++;
    p.setColor(Qt::red);
    scene->addItem(this);
    setPen(p);

    setRotation(rotate);//测试
    //setRotation(360-90);

    m_strID = name;
    m_stPower.m_nPercentage = 100;
    setSelected(true);
   // set_size(25,25);
    //setRect(-4,-20,8,12);
    QRectF r=QRectF(a_leftx*iconzoom,a_lefty*iconzoom,a_wight*iconzoom,a_hight*iconzoom);
    setRect(r);

   // set_size(25,25);
    //setRect(-4,-20,8,12);
    // QRectF r=QRectF(ps_leftx*zoom,-ps_lefty*zoom,ps_hight*zoom,ps_hight*zoom);
   // setRect(r);
    //qDebug()<<"r"<<r;
    m_nType = type;
    m_topoView = topoview;
    m_nID = id;
    m_bytesAddress = address;
    m_nRestNode = startNode;


    // m_nTopoPosID = -1;//为了测试

    m_nTargetID = -1;
    m_nTargetID_PT20=startNode;
    m_nStartID = startNode;
    priority0=10;
    priority1=20;
    priority2=30;
    m_nStartLayer=0;
    m_nEndLayer=0;
    m_nChargeFullLimit = chargeFullLimit;
    m_nChargeHighLimit = chargeUpLimit;
    m_nChargeLowLimit = chargeDownLimit;
    m_nTimerID = 0;
    m_nTimerCnt = 0;
    m_nCommandStart = 0;//902;//0;
    m_nCommandEnd = 0;//902;//0;
    m_nFinishTimerCnt = 0;
    m_nCurrentForkLayer = 0;
    lastForkLayer=0;
    m_nTargetForkLayer = 0;
    m_bIsPathOK = true;
    m_bIsEnableOK = true;
//    m_bIsPowerOK = false;  // 获取车电量信息置为false，在第一个通信周期即获取电量信息
 //   m_bIsErrorOK = false;  // 获取车辆错误码置为false，在第一个通信周期即获取车辆错误码
    m_bNeedToCharge = false;
    m_bCanCharge = false;

    set_plateArriveDetectEnable(false);
    set_missionExecutable(AGV_MISSION_EXECUTABLE_YES);
    set_mode(AGV::SINGLE_STEP_MODE);
    m_nForkPos=-1;
    m_nDestinationID=-1;
    previousPoint=-1;

    m_coordinatemove=coordinatemove;
    m_shigh=scene->height();
   //LineUbiety();
    if(__DEBUG__){
        m_file.setFileName(QString("log_%1_%2.txt").arg(m_strID).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
        if(!m_file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::warning(0,"错误","打开日志文件错误");
            return;
        }
        __log__(&m_file,QString("[init] AGV: %1 在线").arg(m_nID));
        __log__(&m_file,QString("[init] Mac: %1").arg(addrToString(m_bytesAddress)));
        __log__(&m_file,QString("[init] Power param: FullLimit=%1,HighLimit=%2,LowLimit=%3").arg(m_nChargeFullLimit).arg(m_nChargeHighLimit).arg(m_nChargeLowLimit));
    }
    m_nTimerID = startTimer(TIMER_PERIOD);
    mission_state=tr("等待");
    Command_state=tr("");


 //----------------------
 //   if((m_nTopoPosID!=-1)&&(TestModel==false))
 //  {

 //       Chushihua=true;

  //      emit state_change();

 //   }

 //-------------------




}

AGV::~AGV()
{
    if(__DEBUG__)
        m_file.close();
    if(m_nTimerID!=0) killTimer(m_nTimerID);
   // qDebug()<<"agv注销"<<m_nID;
}

void AGV::set_targetCommand(AGVCommand com1, AGVCommand com2)
{

    //-------仅用于显示，无其他用处-------------
     switch(com1.Fangxiang)
    {
    case 0x01 :
        com1.fangxiang="前进";
        break;
    case 0x02 :
        com1.fangxiang="后退";
        break;
    case 0x04 :
        com1.fangxiang="左拐";

        break;
    case 0x08 :
        com1.fangxiang="右拐";
        break;
    case 0x20 :
        com1.fangxiang="停止";
        break;
    }
    //-----------------------------
     switch(com1.Dongzuo)
    {
        case 0x01 :
            com1.dongzuo="正常行驶";
            break;
        case 0x02 :
            com1.dongzuo="减速行驶";
            break;
        case 0x04 :
            com1.dongzuo="原地旋转";
            break;
        case 0x07 :
            com1.dongzuo="等待";
            break;
        case 0x08 :
            com1.dongzuo="收货";
            break;
        case 0x09:
            com1.dongzuo="放货";
            break;

    }

     switch(com2.Fangxiang)
   {
   case 0x01 :
       com2.fangxiang="前进";
       break;
   case 0x02 :
       com2.fangxiang="后退";
       break;
   case 0x04 :
       com2.fangxiang="左拐";

       break;
   case 0x08 :
       com2.fangxiang="右拐";
       break;
   case 0x20 :
       com2.fangxiang="停止";
       break;
   }
   //-----------------------------
     switch(com2.Dongzuo)
   {
       case 0x01 :
           com2.dongzuo="正常行驶";
           break;
       case 0x02 :
           com2.dongzuo="减速行驶";
           break;
       case 0x04 :
           com2.dongzuo="原地旋转";
           break;
       case 0x07 :
           com2.dongzuo="等待";
           break;
       case 0x08 :
           com2.dongzuo="收货";
           break;
       case 0x09:
           com2.dongzuo="放货";
           break;

   }


   //-------仅用于显示，无其他用处------------

    if((com1.start==com1.end)&&(com1.Dongzuo==0x07)&&(com1.Fangxiang==0x20)&&(com2.end==com2.start)&&(com2.start==0))//过滤掉大段的等待通信帧，不发送
    {
      StateWait=true;
      return;

    }

    if((com1.start==Command1.start)&&(com1.end==Command1.end)&&(com1.Dongzuo==Command1.Dongzuo)&&(com1.Fangxiang==Command1.Fangxiang))//过滤掉重复的发送相同动作的通信帧
    {
        if((com2.start==Command2.start)&&(com2.end==Command2.end)&&(com2.Dongzuo==Command2.Dongzuo)&&(com2.Fangxiang==Command2.Fangxiang))

            return;

    }

    if((com1.start==Command2.start)&&(Command2.end==com1.end)&&(com1.Dongzuo==Command2.Dongzuo)&&(com1.Fangxiang==Command2.Fangxiang)&&(com1.MissionNumber==Command2.MissionNumber))
    {
        qDebug()<<"测试"<<endl;
        Command1=Command2;
    }
    else
    {
        Command1=com1;
        CommandNumber++;
        Command1.number=CommandNumber;
        agvcommand[Command1.number]=Command1;

    }
    if(com2.start!=0)
    {
        Command2=com2;
        CommandNumber++;
        Command2.number=CommandNumber;
        agvcommand[Command2.number]=Command2;


    }
    else
    {
        Command2=com2;

    }


    if(CommandNumber==254)
        CommandNumber=0;


    m_bIsPathOK=false;
    m_bIsEnableOK=false;




}

int AGV::get_Commandnumber()//得到当前指令列表中，agvcommand指令数量
{
    return  agvcommand.size();
}

AGVCommand AGV::get_CommandInfo(int i)
{
    if(agvcommand.contains(i))
     return agvcommand[i];
    else
    {
        AGVCommand com;//{0,0,0,false,0};
        return com;
    }
}

int AGV::type() const
{
    return m_nType;

}

void AGV::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setPen(pen());

        painter->setPen(QPen(Qt::blue));
       // painter->drawLine(QPointF(a_leftx*m_zoom,(a_lefty+a_hight*0.2)*m_zoom),QPointF((a_leftx+a_wight)*m_zoom,(a_lefty+a_hight*0.2)*m_zoom));//导向轮
        //painter->drawLine(QPointF(a_leftx*m_zoom,(a_lefty+a_hight*0.3)*m_zoom),QPointF((a_leftx+a_wight)*m_zoom,(a_lefty+a_hight*0.3)*m_zoom));//导向轮
        //painter->drawLine(QPointF(a_leftx*m_zoom,-(a_lefty-a_hight*0.6)*m_zoom),QPointF((a_leftx+a_wight)*m_zoom,-(a_lefty-a_hight*0.6)*m_zoom));//导向轮
        //painter->drawLine(QPointF(a_leftx*m_zoom,-(a_lefty-a_hight*0.7)*m_zoom),QPointF((a_leftx+a_wight)*m_zoom,-(a_lefty-a_hight*0.7)*m_zoom));//导向轮
        QRectF leftcircle=QRectF((a_leftx-30)*iconzoom,(a_lefty+20)*iconzoom,30*iconzoom,80*iconzoom);
        QRectF rightcircle=QRectF((a_leftx+a_wight)*iconzoom,(a_lefty+20)*iconzoom,30*iconzoom,80*iconzoom);
        QRectF leftcircle1=QRectF((a_leftx-30)*iconzoom,(a_lefty+a_hight*0.65+20)*iconzoom,30*iconzoom,80*iconzoom);
        QRectF rightcircle1=QRectF((a_leftx+a_wight)*iconzoom,(a_lefty+a_hight*0.65+20)*iconzoom,30*iconzoom,80*iconzoom);
        QRectF agvhead=QRectF(a_leftx*iconzoom,a_lefty*iconzoom,a_wight*iconzoom,a_hight*0.2*iconzoom);
        painter->fillRect(agvhead,QBrush(Qt::red));
        painter->setPen(QPen(Qt::red));
        painter->drawRect(leftcircle);
        painter->drawRect(rightcircle);
        painter->drawRect(leftcircle1);
        painter->drawRect(rightcircle1);
      painter->setPen(QPen(Qt::black,30));
      QRectF rt=QRectF(a_leftx*iconzoom*1.1,(a_lefty-a_hight*0.5)*iconzoom*1.1,a_wight*iconzoom*1.1,(a_hight*0.5)*iconzoom*1.1);

      QFont font;
      font.setPointSize(16);
      painter->setFont(font);

      painter->drawText(rt, Qt::AlignCenter,QString("%1").arg(m_nID));
      painter->setPen(QPen(Qt::green, 5));
      painter->setBrush(Qt::black);
      painter->drawEllipse(QPoint(0,0),1,1);


    QGraphicsRectItem::paint(painter,option,widget);
}
QRectF AGV::boundingRect() const
{


    QRectF r=QRectF(ps_leftx*m_zoom*3.2,-(ps_lefty+ps_hight*0.5)*m_zoom*3.2,ps_wight*m_zoom*3.2,(ps_hight+ps_hight*0.5)*m_zoom*3.2);



       // QRectF r=QRectF(ps_leftx*m_zoom*1.2,-(ps_lefty+ps_hight*0.5)*m_zoom,ps_wight*m_zoom*1.2,(ps_hight+ps_hight*0.5)*m_zoom);


    return r;
}
void AGV::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(isSelected()){
        QMenu menu;
        QAction *action_showError = menu.addAction("获取错误信息");
        connect(action_showError,SIGNAL(triggered()),this,SLOT(show_errorInfo()));
        menu.exec(event->screenPos());
    }
}

void AGV::set_forkUp()
{
    sendCommand(0,1);

}
void AGV::set_forkDown()
{
    sendCommand(1,0);
}

void AGV::set_plateArriveDetectEnable(bool flag)
{
    char byte2 = m_bytesTargetFunction.at(1);
    if(flag) byte2 |= FUNCTION_PLATE_ARRIVE_ENABLE_MASK;
    else byte2 &=  ~FUNCTION_PLATE_ARRIVE_ENABLE_MASK;
    m_bytesTargetFunction[1] = byte2;
}

void AGV::set_autoLiftFork(bool flag, int target_layer)
{
    char byte2 = m_bytesTargetFunction.at(1);
    char byte1;  // 叉腿高8位
    char byte0;  // 叉腿低8位
    __log__(&m_file,QString("[proc] target layer=%1").arg(target_layer));
    if(flag)
        byte2 |= FUNCTION_AUTO_LIFT_FORK_MASK;//置一   在路径跟随时将叉腿高度调整到功能字节1和功能字节0对应的高度值(cm)
    else
        byte2 &= ~FUNCTION_AUTO_LIFT_FORK_MASK;//清0
    byte1 = (s_forkHeight[target_layer]&0xff00)>>8;//保留高八位，右移八位
    byte0 = (s_forkHeight[target_layer]&0xff);//保留低八位
    m_nCurrentForkLayer = target_layer;
    m_nTargetForkLayer = target_layer;
    m_bytesTargetFunction[2] = byte1;
    m_bytesTargetFunction[3] = byte0;
    m_bytesTargetFunction[1] = byte2;
}

void AGV::set_restNode(int node)
{
    m_nRestNode = node;
    m_nTopoPosID = node;
    m_nDestinationID = node;
    m_nTargetID = node;
    m_nTargetID_PT20=node;
}

int AGV::layerToHeight(int layer)
{
    return s_forkHeight[layer];
}

QPoint AGV::get_center() const
{
    QPointF real_pos = mapToRealWorld(pos());
    return QPoint(real_pos.x(),real_pos.y());
}

void AGV::set_path(AGVCommand command1, AGVCommand command2)
{
   // 发送指令
 if((!m_bIsPathOK)&&(command1.start!=0))
    {

     //---------打印每次下达的指令信息-------

      qDebug()<<"AGV:"<<m_nID<<"发送双指令1:"<<Command1.start<<"-"<<Command1.end<<"Number："<<Command1.number<<endl;

      switch(Command1.Fangxiang)
      {
      case 0x01 :
          Command1.fangxiang="前进";
          qDebug()<<"前进"<<endl;
          break;
      case 0x02 :
          Command1.fangxiang="后退";
          qDebug()<<"后退"<<endl;
          break;
      case 0x04 :
          Command1.fangxiang="左拐";
          qDebug()<<"左拐"<<endl;
          break;
      case 0x08 :
          Command1.fangxiang="右拐";
          qDebug()<<"右拐"<<endl;
          break;
      case 0x20 :
          Command1.fangxiang="停止";
          qDebug()<<"停止"<<endl;
          break;
      }
      //-----------------------------
     switch(Command1.Dongzuo)
      {
          case 0x01 :
              Command1.dongzuo="正常行驶";
              qDebug()<<"正常行驶"<<endl;
              break;
          case 0x02 :
              Command1.dongzuo="减速行驶";
              qDebug()<<"减速行驶"<<endl;
              break;
          case 0x04 :
              Command1.dongzuo="原地旋转";
              qDebug()<<"原地旋转"<<endl;
              break;
          case 0x07 :
              Command1.dongzuo="等待";
              qDebug()<<"等待"<<endl;
              break;
          case 0x08 :
              Command1.dongzuo="收货";
              qDebug()<<"收货"<<endl;
              break;
          case 0x09:
              Command1.dongzuo="放货";
              qDebug()<<"放货"<<endl;
              break;

      }

     qDebug()<<"AGV:"<<m_nID<<"发送双指令2:"<<Command2.start<<"-"<<Command2.end<<"Number："<<Command2.number<<endl;

     switch(Command2.Fangxiang)
     {
     case 0x01 :
         Command2.fangxiang="前进";
         qDebug()<<"前进"<<endl;
         break;
     case 0x02 :
         Command2.fangxiang="后退";
         qDebug()<<"后退"<<endl;
         break;
     case 0x04 :
         Command2.fangxiang="左拐";
         qDebug()<<"左拐"<<endl;
         break;
     case 0x08 :
         Command2.fangxiang="右拐";
         qDebug()<<"右拐"<<endl;
         break;
     case 0x20 :
         Command2.fangxiang="停止";
         qDebug()<<"停止"<<endl;
         break;
     }
     //-----------------------------
    switch(Command2.Dongzuo)
     {
         case 0x01 :
             Command2.dongzuo="正常行驶";
             qDebug()<<"正常行驶"<<endl;
             break;
         case 0x02 :
             Command2.dongzuo="减速行驶";
             qDebug()<<"减速行驶"<<endl;
             break;
         case 0x04 :
             Command2.dongzuo="原地旋转";
             qDebug()<<"原地旋转"<<endl;
             break;
         case 0x07 :
             Command2.dongzuo="等待";
             qDebug()<<"等待"<<endl;
             break;

     }

     //---------打印每次下达的指令信息-------
    CommandRecord(Command1,Command2,1);//打印发送指令到日志
    sendCommand(Command1,Command2);


    }



}

void AGV::set_path(int start, int end)
{
   // qDebug()<<"start"<<start<<"end"<<end;
    if(start==end || start<0 || end<0) return;
    if(!m_bIsPathOK){
        sendCommand(start,end);
        m_nCommandStart = start;
        m_nCommandEnd = end;


    }


    else if(!m_bIsEnableOK){
         set_pathEnable(true);
         // 若开启托盘检测，则需要等待到下位机确认收到托盘检测才能使能路径

      /*  if(type()==PT20)
        {
             set_pathEnable(true);
        }


       else if(m_bytesCurrentFunction==m_bytesTargetFunction){  // 若开启托盘检测，则需要等待到下位机确认收到托盘检测才能使能路径
            set_pathEnable(true);

        }


*/




    }
}
void AGV::stateRecord(bool rs, bool re)
{
    if((rs==0)&&(re==0))
    {
       mission_state=tr("取货");
    }
    else if((rs==true)&&(re==false))
    {
       mission_state=tr("放货");
    }
    else if((rs==true)&&(re==true))
    {
       mission_state=tr("等待");  //应该是任务整体完成了
    }
    else if((rs==false)&&(re==true))
    {
       mission_state=tr("等待");
    }
}
void AGV::timerEvent(QTimerEvent *)
{


   //setPos(100+m_nTimerCnt,100+m_nTimerCnt);
   // setRotation(m_nTimerCnt);
   // emit agv_moved(this);

    static quint64 cnt = 0;

    if(cnt%17==3){  // ?s 定时中断 在通信的频率上可能有问题


        if((Chushihua==false)&&(TestModel==false)&&(m_nTopoPosID!=-1))//独立测试平台与其不同
        {
            qDebug()<<"触发循环找寻任务"<<endl;
            emit state_change();
            Chushihua=true;
        }


        //tcp_getPos();//先将此功能注释掉，观察后续（与仿真平台通信采用的，查询位姿，）
        set_path(Command1,Command2);
      // tcp_getPower();//仿真中，实时获取车辆的电量信息


    }
    else if(cnt%17==2)
    {
        if(StateWait==true)
        {
            StateWait=false;
            emit state_change();
        }

    }
    else if(cnt%17==7){

        
        if(commState==true)
        {
            commState=false;
        }
        else
        {
            //qDebug()<<"AGV:"<<m_nID<<"断线"<<endl;//是否需要添加信息框表示车辆断线？
            emit state_moved(this,xiansudu,jiaosudu);

        }
        // tcp_getState();
        /*if(ForkState==1)
        {
          //set_path(m_nStartLayer,m_nEndLayer);
        }
        else
        */
         //set_path(m_nTopoPosID,m_nTargetID);


        //qDebug()<<"pathflag"<<m_bIsPathOK<<endl;
        //qDebug()<<"enableflag"<<m_bIsEnableOK<<endl;
        //qDebug()<<Command1.start<<"--"<<Command1.end<<endl;
        //qDebug()<<Command2.start<<"--"<<Command2.end<<endl;




         //tcp_getmissionlength();//仿真中，实时获取车辆的任务长度，考虑此方面以后可以从调度平台获取。
    }
    else if(cnt%17==11){

    }
    else if(cnt%17==15){
        if(m_bytesCurrentFunction!=m_bytesTargetFunction){

        }
    }

     else if(cnt%17==0){
         if(cnt%680==0){  // 如果获取电量信息失败，则一直获取，否则每min获取一次
         }
         if(cnt%340==0 ){  // 每300个通信周期查询一次错误码，若无反馈，则一直获取




         }
     }

    cnt++;
    m_nTimerCnt++;
    m_nFinishTimerCnt++;
}

void AGV::set_pathEnable(bool enable)//修改这个
{
    QByteArray buf = PROTOCAL_HEAD;
    buf.append(m_nID);
    buf.append(get_address());

    buf.append(PROTOCAL_MID);
    int _0=0x00;
    buf.append(0x40);
    buf.append(m_nID);
    buf.append(m_nMode==SINGLE_STEP_MODE?FRAME_ENABLE_PATH:FRAME_ENABLE_MULTIMODE_PATH);
    buf.append(enable?0xff:0x00);
    buf.append(_0);
    buf.append(_0);
    buf.append(_0);
    buf.append(_8BitsSum(buf.mid(18)));  //18
    buf.append(0x23);
    buf.append(0xff-_8BitsSum(buf.mid(3)));
    m_bIsEnableOK=false;

   // __log__(&m_file,QString("[send] set path enable"));
   // emit serial_write(buf);

    QByteArray buff;
    buff.append(0x40);
    buff.append(m_nID);
    buff.append(0x11);
    buff.append(_0);
    buff.append(_0);
    buff.append(_0);
    buff.append(_0);
    buff.append(_8BitsSum(buf.mid(1)));//18
    buff.append(0x23);
    emit tcp_write(buff);
}


//agvcommand共有1 2 3个任务，并按此顺序执行
void AGV::CommandManager(int number)
{


}


void AGV::sendCommand(AGVCommand zhiling, AGVCommand zhiling1)
{


    if(zhiling1.start!=0)
   // __log__(&m_file,QString("[send]agv=%1 set path command: C1: start=%2 end=%3 C2: start=%3 end=%4  ").arg(m_nID).arg(zhiling.1).arg(start).arg(end));
  {



    QByteArray buff;
    buff.append(0x4A);//J
    buff.append(0x59);//Y
    buff.append(0x43);//C
    buff.append(0x12);//长度
    buff.append(0x02);//功能码
    buff.append(m_nID);//车号
    buff.append(zhiling.number);//状态
    buff.append(zhiling.end>>8);
    buff.append(zhiling.end&0xff);
    buff.append(zhiling.Fangxiang);//方向
    buff.append(zhiling.angel);//角度
    buff.append(zhiling.Dongzuo);//动作
    buff.append((uchar)(zhiling.length*10));//单位是0.1m
    buff.append(zhiling1.number);//状态
    buff.append(zhiling1.end>>8);
    buff.append(zhiling1.end&0xff);
    buff.append(zhiling1.Fangxiang);//方向
    buff.append(zhiling1.angel);//角度
    buff.append(zhiling1.Dongzuo);//动作
    buff.append((uchar)(zhiling1.length*10));//距离是0.1m的精度
    buff.append(_null);//保留
    buff.append(_8BitsSum(buff.mid(4,17)));//和校验
    buff.append(0x0D);// \r
    buff.append(0x0A);// \n
    emit tcp_write(buff);

  }

  else
 {
        QByteArray buff;
        buff.append(0x4A);//J
        buff.append(0x59);//Y
        buff.append(0x43);//C
        buff.append(0x12);//长度
        buff.append(0x02);//功能码
        buff.append(m_nID);//车号
        buff.append(zhiling.number);//状态
        buff.append(zhiling.end>>8);
        buff.append(zhiling.end&0xff);
        buff.append(zhiling.Fangxiang);//方向
        buff.append(zhiling.angel);//角度
        buff.append(zhiling.Dongzuo);//动作
        buff.append((uchar)(zhiling.length*10));//距离是0.1的精度
        buff.append(_null);//状态
        buff.append(0xff);
        buff.append(0xff);
        buff.append(0xff);//方向
        buff.append(0xff);//角度
        buff.append(0xff);//动作
        buff.append(0xff);//距离是0.1m的精度
        buff.append(_null);//保留
        buff.append(_8BitsSum(buff.mid(4,17)));//和校验
        buff.append(0x0D);// \r
        buff.append(0x0A);// \n
        emit tcp_write(buff);




    }


/*-------------
   QByteArray buff;
    buff.append(0x40);
    buff.append(m_nID);
    buff.append(0x10);
    buff.append(zhiling.start);
    buff.append(zhiling.end);
    buff.append(zhiling1.start);
    buff.append(zhiling1.end);
    buff.append(zhiling.number);
    buff.append(zhiling1.number);
    buff.append(_8BitsSum(buff.mid(1)));//18
    buff.append(0x23);

    //__log__(&m_file,QString("[send]agv=%1 set path command: mode=%2 start=%3 end=%4 ").arg(m_nID).arg(m_nMode).arg(start).arg(end));
   // emit serial_write(buf);
   ---------------------------------*/






}



void AGV::sendCommand(int start, int end)
{
    qDebug()<<"AGV"<<m_nID<<"发送下一段路径"<<endl;
    QByteArray buf = PROTOCAL_HEAD;
    buf.append(m_nID);
    buf.append(get_address());

    buf.append(PROTOCAL_MID);
    buf.append(0x40);
    buf.append(m_nID);
    buf.append(m_nMode==SINGLE_STEP_MODE?FRAME_SET_PATH:FRAME_SET_MULTIMODE_PATH);
    buf.append(start>>8);
    buf.append(start&0xff);
    buf.append(end>>8);
    buf.append(end&0xff);
    buf.append(_8BitsSum(buf.mid(18)));//18
    buf.append(0x23);
    buf.append(0xff-_8BitsSum(buf.mid(3)));
    //m_bIsEnableOK = false;
   // m_bIsPathOK = false;

    QByteArray buff;
    buff.append(0x40);
    buff.append(m_nID);
    buff.append(0x10);
    buff.append(start>>8);
    buff.append(start&0xff);
    buff.append(end>>8);
    buff.append(end&0xff);
    buff.append(_8BitsSum(buf.mid(1)));//18
    buff.append(0x23);

    m_bIsEnableOK = false;
    m_bIsPathOK = false;

    __log__(&m_file,QString("[send]agv=%1 set path command: mode=%2 start=%3 end=%4 ").arg(m_nID).arg(m_nMode).arg(start).arg(end));
   // emit serial_write(buf);
    emit tcp_write(buff);
}

void AGV::CommandFinish(AGVCommand com)
{
    __log__(&m_file,QString("[send]agv=1%,ComandFinish Answer: number:%1,").arg(com.number));
     QByteArray buff;
     buff.append(0x4A);//J
     buff.append(0x59);//Y
     buff.append(0x43);//C
     buff.append(0x05);//长度
     buff.append(0x03);//功能码
     buff.append(m_nID);//车号
     buff.append(com.number);//状态
     buff.append(_null);
     buff.append(_8BitsSum(buff.mid(4,4)));//和校验
     buff.append(0x0D);// \r
     buff.append(0x0A);// \n

    emit tcp_write(buff);

}

void AGV::CommandRecord(AGVCommand com1, AGVCommand com2, int i)
{
    if(i==1)
 {
     QString Fangxiang1;
     QString Dongzuo1;
     QString Fangxiang2;
     QString Dongzuo2;
     switch(com1.Fangxiang)
    {
    case 0x01 :
        Fangxiang1="前进";
        break;
    case 0x02 :
        Fangxiang1="后退";
        break;
    case 0x04 :
        Fangxiang1="左拐";
        break;
    case 0x08 :
        Fangxiang1= "右拐";
        break;
    case 0x20 :
        Fangxiang1= "停止";
        break;
    }
    //-----------------------------
     switch(com1.Dongzuo)
    {
        case 0x01 :
            Dongzuo1="正常行驶";
            break;
        case 0x02 :
            Dongzuo1="减速行驶";
            break;
        case 0x04 :
            Dongzuo1="原地旋转";
            break;
        case 0x07 :
           Dongzuo1= "等待";
            break;

    }


     switch(com2.Fangxiang)
   {
   case 0x01 :
       Fangxiang2="前进";
       break;
   case 0x02 :
       Fangxiang2= "后退";
       break;
   case 0x04 :
       Fangxiang2="左拐";
       break;
   case 0x08 :
       Fangxiang2="右拐";
       break;
   case 0x20 :
       Fangxiang2="停止";
       break;
   }
   //-----------------------------
    switch(com2.Dongzuo)
   {
       case 0x01 :
           Dongzuo2="正常行驶";
           break;
       case 0x02 :
           Dongzuo2="减速行驶";
           break;
       case 0x04 :
           Dongzuo2="原地旋转";
           break;
       case 0x07 :
           Dongzuo2="等待";
           break;
   }

  __log__(&m_file,QString("[send]agv=%1 send path command: C1 number=%2,start=%3 end=%4 %5 %6 length %7 angle %8").arg(m_nID).arg(com1.number).arg(com1.start,4,16,QLatin1Char('0')).arg(com1.end,4,16,QLatin1Char('0')).arg(Fangxiang1).arg(Dongzuo1).arg(com1.length).arg(com1.angel));
  __log__(&m_file,QString("[send]agv=%1 send path command: C2 number=%2,start=%3 end=%4 %5 %6 length %7 angle %8").arg(m_nID).arg(com2.number).arg(com2.start,4,16,QLatin1Char('0')).arg(com2.end,4,16,QLatin1Char('0')).arg(Fangxiang2).arg(Dongzuo2).arg(com2.length).arg(com2.angel));

}


    if(i==2)
 {
     QString Fangxiang1;
     QString Dongzuo1;

     switch(com1.Fangxiang)
    {
    case 0x01 :
        Fangxiang1="前进";
        break;
    case 0x02 :
        Fangxiang1="后退";
        break;
    case 0x04 :
        Fangxiang1="左拐";
        break;
    case 0x08 :
        Fangxiang1= "右拐";
        break;
    case 0x20 :
        Fangxiang1= "停止";
        break;
    }
    //-----------------------------
     switch(com1.Dongzuo)
    {
        case 0x01 :
            Dongzuo1="正常行驶";
            break;
        case 0x02 :
            Dongzuo1="减速行驶";
            break;
        case 0x04 :
            Dongzuo1="原地旋转";
            break;
        case 0x07 :
           Dongzuo1= "等待";
            break;

    }

  __log__(&m_file,QString("[recv]agv=%1 Path Command Finish: C1 number=%2 start=%3 end=%4 %5 %6  length %7 angle %8").arg(m_nID).arg(com1.number).arg(com1.start,4,16,QLatin1Char('0')).arg(com1.end,4,16,QLatin1Char('0')).arg(Fangxiang1).arg(Dongzuo1).arg(com1.length).arg(com1.angel));


}

}

void AGV::tcp_getPos() //6
{

    QByteArray buf;
    buf.append(0x4A);//J
    buf.append(0x59);//Y
    buf.append(0x43);//C
    buf.append(0x05);//长度
    buf.append(0x41);//功能码
    buf.append(m_nID);//车号
    buf.append(_8BitsSum(buf.mid(5,2)));//和校验
    buf.append(0x12);// \r
    buf.append(0x0E);// \n

    emit tcp_write(buf);
}

 void AGV:: tcp_getmissionlength()
 {
     QByteArray buff;
     buff.append(0x40);
     buff.append(m_nID);
     buff.append(0x52);
     buff.append(_null);
     buff.append(_null);
     buff.append(_null);
     buff.append(_null);
     buff.append(_8BitsSum(buff.mid(1)));//18
     buff.append(0x23);

     emit tcp_write(buff);
 }

void AGV::tcp_getState() //5
{
    QByteArray buf = PROTOCAL_HEAD;
    buf.append(m_nID);
    buf.append(get_address());

    buf.append(PROTOCAL_MID);
    buf.append(0x40);
    buf.append(m_nID);
    buf.append(FRAME_GET_BOTHMODE_FINISH);
    buf.append(_null);
    buf.append(_null);
    buf.append(_null);
    buf.append(_null);
    buf.append(_8BitsSum(buf.mid(18)));//18
    buf.append(0x23);
    buf.append(0xff-_8BitsSum(buf.mid(3)));
    //__log__(&m_file,QString("[send] get state command"));
    //emit serial_write(buf);

    QByteArray buff;
    buff.append(0x40);
    buff.append(m_nID);
    buff.append(0x29);
    buff.append(_null);
    buff.append(_null);
    buff.append(_null);
    buff.append(_null);
    buff.append(_8BitsSum(buf.mid(1)));//18
    buff.append(0x23);

    emit tcp_write(buff);
}

void AGV::tcp_getInventory()
{
    QByteArray buf = PROTOCAL_HEAD;
    buf.append(m_nID);
    buf.append(get_address());

    buf.append(PROTOCAL_MID);
    buf.append(0x40);
    buf.append(m_nID);
    buf.append(FRAME_GET_INVENTORY);
    buf.append(_null);
    buf.append(_null);
    buf.append(_null);
    buf.append(_null);
    buf.append(_8BitsSum(buf.mid(18)));//18
    buf.append(0x23);
    buf.append(0xff-_8BitsSum(buf.mid(3)));
   // emit serial_write(buf);

}

void AGV::tcp_setFunction() //ps20 叉子 抬升 下降
{
    QByteArray buf = PROTOCAL_HEAD;
    buf.append(m_nID);
    buf.append(get_address());

    buf.append(PROTOCAL_MID);
    buf.append(0x40);
    buf.append(m_nID);
    buf.append(FRAME_SET_FUNCTION);
    buf.append(m_bytesTargetFunction);
    buf.append(_8BitsSum(buf.mid(18)));//18
    buf.append(0x23);
    buf.append(0xff-_8BitsSum(buf.mid(3)));
    __log__(&m_file,QString("[send] set property command: value=%1").arg(addrToString(m_bytesTargetFunction)));
   // emit serial_write(buf);
}



void AGV::tcp_getErrorCode()
{
    QByteArray buf = PROTOCAL_HEAD;
    buf.append(m_nID);
    buf.append(get_address());

    buf.append(PROTOCAL_MID);
    buf.append(0x40);
    buf.append(m_nID);
    buf.append(FRAME_GET_ERROR);
    buf.append(_null);
    buf.append(_null);
    buf.append(_null);
    buf.append(_null);
    buf.append(_8BitsSum(buf.mid(18)));//18
    buf.append(0x23);
    buf.append(0xff-_8BitsSum(buf.mid(3)));
    __log__(&m_file,QString("[send] get Error cmd"));
   // emit serial_write(buf);
}

void AGV::tcp_getPower()//获取电量 也获取线速度，角速度
{
    QByteArray buff;
    buff.append(0x40);
    buff.append(m_nID);
    buff.append(0x75);
    buff.append(_null);
    buff.append(_null);
    buff.append(_null);
    buff.append(_null);
    buff.append(_8BitsSum(buff.mid(1)));//18
    buff.append(0x23);

    emit tcp_write(buff);
    __log__(&m_file,QString("[send] get power cmd"));
   // emit serial_write(buf);
}

bool AGV::errorCodeToString(QString &error_info)
{
    QString print = m_strID+":\n";
    bool error_flag = false;
    char error_code = -1;
    if(m_bytesErrorCode[0] & 0x01) {print+="\t检测到障碍物;\n";error_flag = true;error_code=5;}
    if(m_bytesErrorCode[0] & 0x02) {print+="\t导航功能异常;\n";error_flag = true;error_code=4;}
    if(m_bytesErrorCode[0] & 0x04) {print+="\t起点位置错误;\n";error_flag = true;error_code=1;}
    if(m_bytesErrorCode[0] & 0x08) {print+="\t舵轮位置异常;\n";error_flag = true;error_code=2;}
    if(m_bytesErrorCode[0] & 0x10) {print+="\t上位机通信超时;\n";error_flag = true;error_code=6;}
    if(m_bytesErrorCode[0] & 0x20) {print+="\t堆高车抬升功能异常;\n";error_flag = true;error_code=7;}
    if(m_bytesErrorCode[0] & 0x40) {print+="\t车轮堵转;\n";error_flag = true;error_code=3;}
    if(m_bytesErrorCode[0] & 0x80) {print+="\t导航功能接收定位数据超时;\n";error_flag = true;error_code=11;}

    if(m_bytesErrorCode[1] & 0x01) {print+="\t导航功能偏航告警;\n";error_flag = true;error_code=8;}
    if(m_bytesErrorCode[1] & 0x02) {print+="\t托盘检测位置异常告警;\n";error_flag = true;error_code=12;}
    if(m_bytesErrorCode[1] & 0x04) {print+="\t触边开关触发告警;\n";error_flag = true;error_code=9;}
    if(m_bytesErrorCode[1] & 0x08) {print+="\t电池状态异常;\n";error_flag = true;error_code=10;}
    if(error_flag){
        emit report_error(m_strID,error_code);
        error_info = print;
    }
    return error_flag;
}

void AGV::set_center(QPoint center)
{
    if(isSelected()){
       // center.setY(-center.y());
        QPointF real_pos = mapFromRealWorld(center);
        setPos(real_pos);
    }

}

void AGV::set_dashLine(bool lineType)
{
    if(isSelected()){
        QPen p = pen();
        if(lineType)
            m_nPenStyle = Qt::DashLine;
        else
            m_nPenStyle = Qt::SolidLine;
        p.setStyle(m_nPenStyle);
        setPen(p);
    }
}

/* void recvData(char frame_type,QByteArray data)
 * input: char frame_type: 帧类型
 *        QByteArray data: AGV通信协议中除去数据头0x40、AGV的ID、帧类型、校验和以及数据尾0x23的数据
 * fuction: 进一步解析AGV的返回信息
 */

void AGV::recvData(char frame_type,QByteArray data)
{
    if(frame_type==FRAME_SET_PATH || frame_type==FRAME_SET_MULTIMODE_PATH)
    {
        m_bIsPathOK=true;
        qDebug()<<"AGV"<<m_nID<<"路径发送收到"<<endl;
         __log__(&m_file,QString("[recv] set Path:%1").arg(m_bIsPathOK));
    }
    else if(frame_type==FRAME_ENABLE_PATH || frame_type==FRAME_ENABLE_MULTIMODE_PATH)
    {
        m_bIsEnableOK=true;
        qDebug()<<"AGV"<<m_nID<<"路径使能完成"<<endl;

        //__logqDebug()<<"路径使能完成"<<endl;__(&m_file,QString("[recv] enable Path:%1").arg(m_bIsEnableOK));
    }
    else if(frame_type==FRAME_GET_POS){

        qDebug()<<"实时位姿反馈"<<endl;
        qreal x,y,theta;
        x = bytesToInt32(data.mid(0,4));
        y = bytesToInt32(data.mid(4,4));
        theta = bytesToInt32(data.mid(8,4))/1000;

        //setPos(mapFromRealWorld(QPointF(x,y)));
        qreal px=x*m_zoom+m_coordinatemove.x();
        qreal py=m_shigh- (y*m_zoom+m_coordinatemove.y());
        setPos(px,py);
        //qDebug()<<"x"<<x<<"y"<<y<<"theta"<<theta<<"px"<<px<<"py"<<py;
       // setRotation(mapFromRealWorld((360-theta)*M_PI/180)*180/M_PI);

         setRotation(-(theta+90));


         //emit agv_moved(this);

        if(m_nTopoPosID == -1){
            //m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld(pos()));
              m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld(pos()));
            if(m_nTopoPosID==-1) return;
           // emit state_change();
        }
        if(m_nTargetID==-1) m_nTargetID = m_nTopoPosID;
       // __log__(&m_file,QString("agv=:%1 [recv] pos:(%2,%3,%4)").arg(m_nID).arg(x).arg(y).arg(theta));
    }

    else if(frame_type==0x29)//查询任务通信指令
    {
        int nStart = (((uchar)data.at(0))<<8)+(uchar)(data.at(1));
        int nEnd = (((uchar)data.at(2))<<8)+(uchar)(data.at(3));
     }
    //*******************************************************
    //*******************************************************
    else if(frame_type==FRAME_GET_FINISH_NEW || frame_type==FRAME_GET_MULTIMODE_FINISH ){  // FRAME_GET_FINISH_NEW

        //-----------6.24新修改双指令模式------------
        int nStart = (((uchar)data.at(0))<<8)+(uchar)(data.at(1));
        int nEnd = (((uchar)data.at(2))<<8)+(uchar)(data.at(3));
        int nNumber= (uchar)data.at(4);
        AGVCommand com;
        com.start=nStart;
        com.end=nEnd;
        com.number=nNumber;


        qDebug()<<"AGV"<<m_nID<<"指令完成："<<nStart<<"--"<<nEnd<<""<<nNumber<<endl;

        int start=0;
        int end=0;
        int number=0;
        bool finish_flag;
        bool flag1=false;
        if(agvcommand.contains(nNumber))
        {

             start=agvcommand[nNumber].start;
             end=agvcommand[nNumber].end;
             number=agvcommand[nNumber].number;
             qDebug()<<"AGV"<<m_nID<<"与agvcommand【1】配对："<<start<<"--"<<end<<number<<endl;
        }
        else
        {
            flag1=true;//说明此指令已经完成，任务完成反应未收到

        }


        bool flag2=(start==nStart) &&(end==nEnd)&&(number==nNumber);
        bool flag3=m_bIsPathOK&&m_bIsEnableOK;

        finish_flag =(flag2&&flag3)||(flag1);//指令匹配 or 停车，finishflag==1;

        qDebug()<<"AGV"<<m_nID<<flag1<<"-"<<flag2<<"-"<<Trafficmanager<<"匹配结果"<<finish_flag<<endl;

        if((nStart==0 && nEnd==0) || m_nTopoPosID==-1){
            return;
            //m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld(pos()),m_nTopoPosID);
             m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld( pos()),m_nTopoPosID);
            __log__(&m_file,QString("[proc]agv=:%1, match:%2").arg(m_nID).arg(m_nTopoPosID));
            if(m_nTopoPosID==-1) return;
        }
        if(finish_flag){


                if(flag1==true)//需要改
                {
                    qDebug()<<"AGV"<<m_nID<<"指令完成反应"<<com.start<<"-"<<com.end<<"-"<<com.number<<endl;
                    CommandFinish(com);//指令完成应答

                }
                else
                {
                     qDebug()<<"AGV:"<<m_nID<<"已完成一条指令，再运行其他指令"<<endl;
                     if((agvcommand[number].start==nStart) &&(agvcommand[number].end==nEnd)&&(number==nNumber)&&Trafficmanager)//保证不会出现指令发送覆盖，保证正确
                       {
                           Trafficmanager=false;//完成区域管制，并发送完下一段指令后，才会接收新的指令

                           qDebug()<<"AGV:"<<m_nID<<"匹配成功，完成路径，等待指令下达"<<endl;
                           ComFinish=agvcommand[number];
                           CommandFinish(agvcommand[number]);//指令完成应答
                           agvcommand.remove(nNumber);
                            m_topoView->set_register(QString("Ver_%1").arg(nEnd),"",m_agvcount);
                            m_nTopoPosID = nEnd;
                            previousPoint=nStart;

                           emit state_change();//在其中应该会更新agvcommand函数

                        }
                     else if(nStart==nEnd)
                     {
                        //  emit state_change();//在其中应该会更新agvcommand函数
                     }

                     //-----------------------------------

                }

                emit state_moved(this,xiansudu,jiaosudu);
        }

        else
        {
            AGVCommand com;//{0,0,0,false,0};
            CommandFinish(com);//指令匹配失败，反应帧
        }


    }
    //*******************************************************
    //*******************************************************
    else if(frame_type==FRAME_SET_FUNCTION){
        if(m_bytesTargetFunction==data.mid(0,4)){
            m_bytesCurrentFunction = m_bytesTargetFunction;
        }
    }
    else if(frame_type==FRAME_GET_POWER){

        m_stPower.m_nVoltage = ((int)((uchar)(data.at(0))))*2;
        m_stPower.m_nPercentage = (uchar)(data.at(1));
        m_stPower.m_errorCode[1] = data.at(2);
        m_stPower.m_errorCode[0] = data.at(3);
        if(m_stPower.m_nPercentage<m_nChargeLowLimit) m_bNeedToCharge = true;
        else if(m_stPower.m_nPercentage>m_nChargeHighLimit) m_bNeedToCharge = false;
        if(m_stPower.m_nPercentage<=m_nChargeHighLimit) m_bCanCharge=true;
        else if(m_stPower.m_nPercentage>=m_nChargeFullLimit) m_bCanCharge=false;
        __log__(&m_file,QString("[recv] voltage=%1 percent=%2").arg(m_stPower.m_nVoltage).arg(m_stPower.m_nPercentage));
    }
    else if(frame_type==FRAME_GET_ERROR){
//        m_bIsErrorOK = true;
        m_bytesErrorCode[3] = data.at(0);
        m_bytesErrorCode[2] = data.at(1);
        m_bytesErrorCode[1] = data.at(2);
        m_bytesErrorCode[0] = data.at(3);
        QString error_info;
        errorCodeToString(error_info); // 该函数包含了如果检测到错误则向trafficManager报告错误功能

    }
    else if(frame_type==FRAME_GET_CHARGE){//获得线速度，角速度，电量

        dianliang = (uchar)(data.at(0));
        xiansudu = (uchar)(data.at(1));
        jiaosudu = (uchar)(data.at(2));


          if (dianliang<80)

         emit dianliangjinggao();




        //qDebug()<<"查询线速度角速度电量"<<endl;
         emit state_moved(this,xiansudu,jiaosudu);
}
    else if(frame_type==0X52)//agv 完成了当前的一个任务
    {
        int missionstate = (uchar)(data.at(0));
        if(missionstate==1)
        {
           qDebug()<<"AGV:"<<m_nID<<"任务完成："<<nowmissionid<<"发送任务完成信号"<<endl;
           int x = (uchar)(data.at(1));
            int y = (uchar)(data.at(2));
            int z = (uchar)(data.at(3));
            missionlength=100*x+10*y+z;
           emit missionwancheng(nowmissionid,MISSION_STATE_FINISH,this);
        }
    else if(frame_type==0X50)//agv 完成了当前的一个任务
        {

            }



    }


}

void AGV::recvData(char frame_type, QByteArray data, bool sign)
{

    if(((uchar)frame_type)==0xf2)
    {

        int number1= (uchar)data.at(6);
        int number2=(uchar)data.at(7);
        __log__(&m_file,QString("[recv]Communication_Send Answer: number_1:%1,number_2:%2").arg(number1).arg(number2));
        qDebug()<<"AGV"<<m_nID<<"路径发送反应"<<number1<<number2<<endl;
        if((Command1.number==number1)&&(Command2.number==number2)) //路径发送收到
        {     m_bIsPathOK=true;
            qDebug()<<"AGV"<<m_nID<<"路径发送收到"<<endl;
            __log__(&m_file,QString("[recv] agv=%1,Send Path Answer:number_1=%2,number_2=%3").arg(m_nID).arg(number1).arg(number2));

        }
    }
    else if(frame_type==FRAME_ENABLE_PATH || frame_type==FRAME_ENABLE_MULTIMODE_PATH)
    {
        m_bIsEnableOK=true;
        qDebug()<<"AGV"<<m_nID<<"路径使能完成"<<endl;

        //__logqDebug()<<"路径使能完成"<<endl;__(&m_file,QString("[recv] enable Path:%1").arg(m_bIsEnableOK));
    }
    else if(((uchar)frame_type)==0xf4){//实时反馈位姿
      //----------获取AGV当前运行的动作指令----------------
       int number=ComFinish.number;
       if(number<253)
           number=number+1;
       else
           number=1;
       if(agvcommand.contains(number))
      {
            ComDoing=agvcommand[number];//ComDoing 表示AGV当前正在执行的指令
            Command_state=QString("targetnode:%1 action:%2,direction:%3").arg(ComDoing.end,4,16,QLatin1Char('0')).arg(ComDoing.dongzuo).arg(ComDoing.fangxiang);
      }

      //-------------------------------------------------------
       bool up=false;
       bool back=false;
       if(ComDoing.Fangxiang==0x02)
          back=true;
       else
          up=true;
        //实时反馈位姿，主要是检测车辆是否有断线情况
        commState=true;//通信连通位
        qreal D,theta;
        D = (((uchar)data.at(13))<<8)+(uchar)(data.at(14));
        theta =(((uchar)data.at(15))<<8)+(uchar)(data.at(16));
        int node=(((uchar)data.at(11))<<8)+(uchar)(data.at(12));
        __log__(&m_file,QString("[recv]Communication_GetPos: node:%1,D:%2cm,theta:%3").arg(node).arg(D).arg(theta));
        __log__(&m_file,QString("[recv]Communication_GetPos: Detail：D_1 %1,D_2 %2,T_1 %3,T_2 %4").arg(data.at(13)).arg(data.at(14)).arg(data.at(15)).arg(data.at(16)));
        if(!m_topoView->m_verList.contains(QString("Ver_%1").arg(node)))
        {
            return;
        }

        int posx,posy;

        posx=(m_topoView->m_verList[QString("Ver_%1").arg(node)])->m_x;
        posy=(m_topoView->m_verList[QString("Ver_%1").arg(node)])->m_y;

        qDebug()<<"AGV:"<<m_nID<<"Get Pos:"<<node<<"-"<<D<<"-"<<theta<<endl;

        __log__(&m_file,QString("[get]agv=%1 get Pos Feedback: current_node=%2 Distance=%3 Angle=%4 CurrentCommand:4%").arg(m_nID).arg(node).arg(D).arg(theta).arg(ComDoing.number));
       if(up==true)
       {
            switch(agvAngel)
            {
                case 0:
                    posy=posy-D*10*m_zoom;
                    posx=posx;
                    break;
                case 90:
                    posy=posy;
                    posx=posx-D*10*m_zoom;
                    break;
                case 180:
                    posy=posy+D*10*m_zoom;
                    posx=posx;
                    break;
                case 270:
                    posy=posy;
                    posx=posx+D*10*m_zoom;
                    break;
            }

        }
       else if(back==true)
       {

           switch(agvAngel)
           {
               case 0:
                   posy=posy+D*10*m_zoom;
                   posx=posx;
                   break;
               case 90:
                   posy=posy;
                   posx=posx+D*10*m_zoom;
                   break;
               case 180:
                   posy=posy-D*10*m_zoom;
                   posx=posx;
                   break;
               case 270:
                   posy=posy;
                   posx=posx-D*10*m_zoom;
                   break;
           }


       }

        theta=AngleChushi+theta;//在地图中是顺时针的,范围是0-360


        setPos(posx,posy);
        setRotation(-theta);
        emit state_moved(this,xiansudu,jiaosudu);

        /*----------------------------
        //setPos(mapFromRealWorld(QPointF(x,y)));
        qreal px=x*m_zoom+m_coordinatemove.x();
        qreal py=m_shigh- (y*m_zoom+m_coordinatemove.y());
        setPos(px,py);
        setRotation(-(theta+90));


         //emit agv_moved(this);

        if(m_nTopoPosID == -1){
            //m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld(pos()));
              m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld(pos()));
            if(m_nTopoPosID==-1) return;
           // emit state_change();
        }
        if(m_nTargetID==-1) m_nTargetID = m_nTopoPosID;
       // __log__(&m_file,QString("agv=:%1 [recv] pos:(%2,%3,%4)").arg(m_nID).arg(x).arg(y).arg(theta))

      ----------------------------------------*/

    }
    else if(frame_type==FRAME_GET_POS){


        //----------获取AGV当前运行的动作指令----------------
        int number=ComFinish.number;
        if(number<253)
            number=number+1;
        else
            number=1;
        if(agvcommand.contains(number))
       {
             ComDoing=agvcommand[number];//ComDoing 表示AGV当前正在执行的指令
             Command_state=QString("targetnode:%1 action:%2,direction:%3").arg(ComDoing.end,4,16,QLatin1Char('0')).arg(ComDoing.dongzuo).arg(ComDoing.fangxiang);

        }
         qDebug()<<"测试**"<<m_nID<<number<<ComDoing.dongzuo<<ComDoing.fangxiang<<endl;
       //-------------------------------------------------------
        qreal x,y,theta;
        x = bytesToInt32(data.mid(6,4));
        y = bytesToInt32(data.mid(10,4));
        theta = bytesToInt32(data.mid(14,4))/1000;
        qreal px=x*m_zoom+m_coordinatemove.x();
        qreal py=m_shigh- (y*m_zoom+m_coordinatemove.y());
        setPos(px,py);
        qDebug()<<"角度:"<<theta<<endl;
        setRotation(theta);
        emit state_moved(this,xiansudu,jiaosudu);

    }

    else if(((uchar)frame_type)==0x29)//查询任务通信指令
    {
        int nStart = (((uchar)data.at(0))<<8)+(uchar)(data.at(1));
        int nEnd = (((uchar)data.at(2))<<8)+(uchar)(data.at(3));
     }
    //*******************************************************
    //*******************************************************
    else if(((uchar)frame_type)==0xf3){  // 完成指令

        //-----------6.24新修改双指令模式------------
        int nNumber= (uchar)data.at(6);
        bool finish_flag;
        bool flag1=false;
        bool flag2=false;
        bool flag3=m_bIsPathOK;
        __log__(&m_file,QString("[recv]Communication_ComandFinish: number:%1,").arg(nNumber));
        if(agvcommand.contains(nNumber))
        {

             flag2=true;
             int start=agvcommand[nNumber].start;
             int end=agvcommand[nNumber].end;
             int angel=agvcommand[nNumber].angel;
             int number=agvcommand[nNumber].number;
             float length=agvcommand[nNumber].length;
             qDebug()<<"AGV"<<m_nID<<"指令完成/匹配成功："<<start<<"--"<<end<<"number:"<<number<<"length:"<<length<<"angel:"<<angel<<endl;

        }
        else
        {
            flag1=true;//说明此指令已经完成，任务完成反应未收到
            qDebug()<<"AGV"<<m_nID<<"指令完成/错过："<<nNumber<<endl;

        }





        finish_flag =(flag2&&flag3)||(flag1);//指令匹配 or 停车，finishflag==1;

        if(finish_flag){


                if(flag1==true)//需要改
                {
                    AGVCommand com;
                    com.number=nNumber;
                    CommandFinish(com);//指令完成应答

                }
                else
                {

                      m_nTopoPosID =agvcommand[nNumber].end ;//更新当前拓扑点
                      previousPoint=agvcommand[nNumber].start;
                      ComFinish=agvcommand[nNumber];
                      bool Test=agvcommand[nNumber].test;
                      Trafficmanager=false;//完成区域管制，并发送完下一段指令后，才会接收新的指令

                      CommandRecord(agvcommand[nNumber],agvcommand[nNumber],2);
                      CommandFinish(agvcommand[nNumber]);//指令完成应答
                      m_topoView->set_register(QString("Ver_%1").arg(agvcommand[nNumber].end),"",m_agvcount);//更新节点颜色

                     
                      int posx,posy;


                      //-----更新车辆位姿，使车辆始终在0-360之间------------
                      switch(agvcommand[nNumber].Fangxiang)
                      {
                        case 0x04:
                            agvAngel=agvAngel+agvcommand[nNumber].angel;
                            break;
                        case 0x08:
                            agvAngel=agvAngel-agvcommand[nNumber].angel;
                            break;

                      }
                      if(agvAngel==360)
                          agvAngel=0;
                      else if(agvAngel==-90)
                          agvAngel=270;

                      //-----------------------------------------


                      posx=(m_topoView->m_verList[QString("Ver_%1").arg(m_nTopoPosID)])->m_x;
                      posy=(m_topoView->m_verList[QString("Ver_%1").arg(m_nTopoPosID)])->m_y;
                      setPos(posx,posy);//需要测试一下
                      setRotation(360-agvAngel);//顺时针旋转

                      agvcommand.remove(nNumber);

                     if(Test==true)//只有非独立测试模式下，才可以进行车辆测试
                        emit TestFeedback(ComFinish,ComFinish.Dongzuo,ComFinish.Fangxiang,m_nID);

                     if(TestModel==false)
                        emit state_change();//在其中应该会更新agvcommand函数

                     emit state_moved(this,xiansudu,jiaosudu);//更新AGV的线速度和角速度？

                     qDebug()<<"AGV"<<m_nID<<"角度:"<<agvAngel<<"当前点:"<<m_nTopoPosID<<endl;

                }


        }


    }
    //*******************************************************
    //*******************************************************
    else if(frame_type==FRAME_SET_FUNCTION){
        if(m_bytesTargetFunction==data.mid(0,4)){
            m_bytesCurrentFunction = m_bytesTargetFunction;
        }
    }
    else if(frame_type==FRAME_GET_POWER){

        m_stPower.m_nVoltage = ((int)((uchar)(data.at(0))))*2;
        m_stPower.m_nPercentage = (uchar)(data.at(1));
        m_stPower.m_errorCode[1] = data.at(2);
        m_stPower.m_errorCode[0] = data.at(3);
        if(m_stPower.m_nPercentage<m_nChargeLowLimit) m_bNeedToCharge = true;
        else if(m_stPower.m_nPercentage>m_nChargeHighLimit) m_bNeedToCharge = false;
        if(m_stPower.m_nPercentage<=m_nChargeHighLimit) m_bCanCharge=true;
        else if(m_stPower.m_nPercentage>=m_nChargeFullLimit) m_bCanCharge=false;
        __log__(&m_file,QString("[recv] voltage=%1 percent=%2").arg(m_stPower.m_nVoltage).arg(m_stPower.m_nPercentage));
    }
    else if(frame_type==FRAME_GET_ERROR){
//        m_bIsErrorOK = true;
        m_bytesErrorCode[3] = data.at(0);
        m_bytesErrorCode[2] = data.at(1);
        m_bytesErrorCode[1] = data.at(2);
        m_bytesErrorCode[0] = data.at(3);
        QString error_info;
        errorCodeToString(error_info); // 该函数包含了如果检测到错误则向trafficManager报告错误功能

    }
    else if(frame_type==FRAME_GET_CHARGE){//获得线速度，角速度，电量

        dianliang = (uchar)(data.at(0));
        xiansudu = (uchar)(data.at(1));
        jiaosudu = (uchar)(data.at(2));


          if (dianliang<80)

         emit dianliangjinggao();




        //qDebug()<<"查询线速度角速度电量"<<endl;
         emit state_moved(this,xiansudu,jiaosudu);
}
    else if(frame_type==0x52)//agv 完成了当前的一个任务
    {
        int missionstate = (uchar)(data.at(0));
        if(missionstate==1)
        {
           qDebug()<<"AGV:"<<m_nID<<"任务完成："<<nowmissionid<<"发送任务完成信号"<<endl;
           int x = (uchar)(data.at(1));
            int y = (uchar)(data.at(2));
            int z = (uchar)(data.at(3));
            missionlength=100*x+10*y+z;
           emit missionwancheng(nowmissionid,MISSION_STATE_FINISH,this);
        }
    else if(frame_type==0X50)//agv 完成了当前的一个任务
        {

            }



    }
    else if(((uchar)frame_type)==0xf1)//初始化命令
    {

         m_nTopoPosID=(((uchar)data.at(7))<<8)+(uchar)(data.at(8));//7.8组成当前的拓扑点。
         chushidian=m_nTopoPosID;//表示车辆初始点，实际车辆初始点通过获得
         AGVType=(uchar)data.at(9);
         if(m_topoView->m_verList.contains(QString("Ver_%1").arg(m_nTopoPosID)))
         {

         }
         else
         {

            emit AGVWarn(1);
            m_nTopoPosID=-1;
            return;//表示初始化错误
         }

        __log__(&m_file,QString("[recv]agv=%1 Initialization: InitialPoint=%2,AGVType=%3").arg(m_nID).arg(m_nTopoPosID,4,16,QLatin1Char('0')).arg(AGVType));
        //暂未定义状态
        qDebug()<<"agv:"<<m_nID<<"初始化"<<"当前点："<<m_nTopoPosID<<endl;
        QByteArray buff;
        buff.append(0x4A);//J
        buff.append(0x59);//Y
        buff.append(0x43);//C
        buff.append(0x07);//0X07
        buff.append(0x01);//功能码
        buff.append(data.at(5));//车号
        buff.append(data.at(6));//状态
        buff.append(data.at(7));
        buff.append(data.at(8));
        buff.append(data.at(9));
        buff.append(data.at(10));
        buff.append(_8BitsSum(buff.mid(4,7)));//和校验
        buff.append(0x0D);// \r
        buff.append(0x0A);// \n


        qreal posx=(m_topoView->m_verList[QString("Ver_%1").arg(m_nTopoPosID)])->m_x;
        qreal posy=(m_topoView->m_verList[QString("Ver_%1").arg(m_nTopoPosID)])->m_y;
        setPos(posx,posy);
        setRotation(360-agvAngel);

        //emit tcp_write(buff);//按照这里的逻辑来说，初始化应答，应该是车辆持续发送上位机的。
        if(TestModel==true)
        {
            AGVCommand com;
            com.angel=agvAngel;
            com.start=m_nTopoPosID;
            com.Dongzuo=0x55;
            com.Fangxiang=0x55;
            emit TestFeedback(com,com.Dongzuo,com.Fangxiang,m_nID);
        }
        else
            emit tcp_write(buff);//非独立模式下，一切正常

       emit state_moved(this,xiansudu,jiaosudu);//更新AGV的线速度和角速度？


    }



}


void AGV::show_errorInfo()
{
 //   m_bIsErrorOK = false;
}

void AGV::changeForkState(int value)
{
    ForkState=value;
}
void AGV::setLastFork(int value)
{
    lastForkLayer=value;
}
Pre AGV::get_previous()
{
    //Pre p;
    if(previousQueue.length()>5)
        previousQueue.removeFirst();
    if(previousQueue.length()>=2)
    {


      prePoints.previous=previousQueue.at(previousQueue.length()-1);
      prePoints.previous_before=previousQueue.at(previousQueue.length()-2);
    }
   /* else
    {
         prePoints.previous=previousQueue.at(previousQueue.length()-1);//应该没有这种情况吧 0216
         prePoints.previous_before=-1;
    }
    */
    return prePoints;


}
int AGV::get_prePoint()
{
   return previousPoint;
}

int AGV::IsTargetInTunnel(quint32 target)
{
    int isInTunnel=0;
     QSqlQuery query = getQuery();
     query.exec(QString("select * from tb_topover where NodeID =%1").arg(target));
     while(query.next()){
         QSqlRecord record = query.record();
          isInTunnel=record.value("isInTunnel").toInt();
         //qDebug()<<"isInTunnel"<<isInTunnel;
     }
     return  isInTunnel;

}
QList<int> AGV::pointsInTunnel(int tunnel)
{
    QList<int> points;
    QSqlQuery query = getQuery();
    query.exec(QString("select * from tb_topover where isInTunnel =%1").arg(tunnel));
    while(query.next()){
        QSqlRecord record = query.record();
        int nodeID=record.value("NodeID").toInt();
        //qDebug()<<"isInTunnel"<<isInTunnel;
         points.append(nodeID);
    }
    return points;
}

