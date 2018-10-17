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
AGV::AGV(QGraphicsScene *scene, TopoView* topoview, qreal zoom,QPointF coordinatemove,int type,QString name, int id, QByteArray address,int startNode,int chargeFullLimit,int chargeUpLimit,int chargeDownLimit,int chexing,QPointF center, int rotate)
{

    Command1={0,0,0,false,0};
    Command2={0,0,0,false,0};

    nowmissionid=0;
      missionflag1=false;
      xiansudu=0;
     jiaosudu=0;
     dianliang=100;
      chushidian=startNode;
      agvchexing=chexing;

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
    setRotation(rotate);
    m_strID = name;
    m_stPower.m_nPercentage = 100;
    setSelected(true);
   // set_size(25,25);
    //setRect(-4,-20,8,12);
    if ((agvchexing==0)||(agvchexing==3)||(agvchexing==5)||(agvchexing==1)||(agvchexing==6)||(agvchexing==7))
    {
        QRectF r=QRectF(a_leftx*zoom,a_lefty*zoom,a_wight*zoom,a_hight*zoom);
       setRect(r);
    }
    else
    {
     QRectF r=QRectF(ps_leftx*zoom,-ps_lefty*zoom,ps_wight*zoom,ps_hight*zoom);
    setRect(r);
    qDebug()<<"r"<<r;
}


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
    m_nTopoPosID = -1;
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
    m_bIsPathOK = false;
    m_bIsEnableOK = false;
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
    m_zoom=zoom;
    m_coordinatemove=coordinatemove;
    m_shigh=scene->height();
   //LineUbiety();
    if(__DEBUG__){
        m_file.setFileName(QString("log_%1_%2.txt").arg(m_strID).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
        if(!m_file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::warning(0,"错误","打开日志文件错误");
            return;
        }
        __log__(&m_file,QString("[init] AGV: %1 Online").arg(m_nID));
        __log__(&m_file,QString("[init] Mac: %1").arg(addrToString(m_bytesAddress)));
        __log__(&m_file,QString("[init] Power param: FullLimit=%1,HighLimit=%2,LowLimit=%3").arg(m_nChargeFullLimit).arg(m_nChargeHighLimit).arg(m_nChargeLowLimit));
    }
    m_nTimerID = startTimer(TIMER_PERIOD);
    mission_state=tr("等待");
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
    Command1=com1;
    Command2=com2;

    if(com1.start==0)//0，0，0不可加入程序中
        return;
    if(com2.start==0)
        return;


    if(agvcommand.contains(1))
    {agvcommand[2]=com1;
        agvcommand[3]=com2;
    }
    else
    {
        agvcommand[1]=com1;
        agvcommand[2]=com2;

    }


    m_bIsPathOK=false;





}

int AGV::get_Commandnumber()//得到当前指令列表中，agvcommand指令数量
{ int number=0;
    for(int i=1;i<=3;i++)
    {
        if(agvcommand.contains(i))
            number++;
    }
    return number;
}

AGVCommand AGV::get_CommandInfo(int i)
{
    if(agvcommand.contains(i))
     return agvcommand[i];
    else
    {
        AGVCommand com={0,0,0,false,0};
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
    if((agvchexing==0)||(agvchexing==3)||(agvchexing==5)||(agvchexing==6)||(agvchexing==7))
    {

        //----此处需要重新进行界定，有时间要重新画

        // painter->drawRect(QRectF(QPointF(-4,-8),QPointF(-2,4)));//载物板
        /*
    QRectF rl=QRectF(ps_leftx*m_zoom,-(ps_lefty-ps_hight*0.5)*m_zoom,ps_wight*m_zoom/4,ps_hight*m_zoom/2);
    painter->drawRect(rl);
    */

        // painter->drawRect(QRectF(QPointF(2,-8),QPointF(4,4)));//载物板
        // QRectF rr=QRectF(a_leftx*m_zoom*0.7,a_lefty*m_zoom*0.7,a_wight*m_zoom*0.7,a_hight*m_zoom*0.7);
    //painter->drawRect(rr);
    //painter->drawLine(QPointF(ps_leftx*m_zoom,0),QPointF(ps_leftx*m_zoom+rl.width(),0));//导向轮
    //painter->drawLine(QPointF((ps_leftx+ps_wight)*m_zoom,0),QPointF((ps_leftx+ps_wight)*m_zoom+rr.width(),0));//导向轮

    /*painter->drawLine(QPointF(-4,0),QPointF(-2,0));//导向轮
    painter->drawLine(QPointF(2,0),QPointF(4,0));//导向轮
    */
    }
    else if((agvchexing==2)||(agvchexing==4))
    {
        painter->setPen(QPen(Qt::red, 0.2));
        QRectF rt=QRectF(ps_leftx*m_zoom,-(ps_lefty+ps_hight)*m_zoom,ps_wight*m_zoom,(ps_hight*0.5)*m_zoom);
        //painter->drawText(rt, Qt::AlignCenter,QString("%1").arg(cishu));
         painter->drawRect(rt);

       // painter->drawLine(QPointF(ps_leftx*m_zoom,-(ps_lefty+ps_hight*0.2)*m_zoom),QPointF((ps_leftx+ps_wight)*m_zoom,-(ps_lefty+ps_hight*0.2)*m_zoom));//导向轮
      //  painter->drawLine(QPointF((ps_leftx+ps_wight*0.3)*m_zoom,-(ps_lefty+ps_hight*0.2)*m_zoom),QPointF((ps_leftx+ps_wight*0.3)*m_zoom,(ps_lefty+ps_hight)*m_zoom));//导向轮
       // painter->drawLine(QPointF((ps_leftx+ps_wight*0.7)*m_zoom,-(ps_lefty+ps_hight*0.2)*m_zoom),QPointF((ps_leftx+ps_wight*0.7)*m_zoom,(ps_lefty+ps_hight)*m_zoom));//导向轮
        // painter->drawLine(QPointF(ps_leftx*m_zoom,(ps_lefty+ps_hight*0.7)*m_zoom),QPointF((ps_leftx+ps_wight)*m_zoom,(ps_lefty+ps_hight*0.7)*m_zoom));//导向轮


    }
    else if(agvchexing==1)
    {
        painter->drawLine(QPointF(ps_leftx*m_zoom,-(ps_lefty-ps_hight*0.2)*m_zoom),QPointF((ps_leftx+ps_wight)*m_zoom,-(ps_lefty-ps_hight*0.2)*m_zoom));//导向轮
        painter->drawLine(QPointF(ps_leftx*m_zoom,-(ps_lefty-ps_hight*0.3)*m_zoom),QPointF((ps_leftx+ps_wight)*m_zoom,-(ps_lefty-ps_hight*0.3)*m_zoom));//导向轮
        painter->drawLine(QPointF(ps_leftx*m_zoom,-(ps_lefty-ps_hight*0.6)*m_zoom),QPointF((ps_leftx+ps_wight)*m_zoom,-(ps_lefty-ps_hight*0.6)*m_zoom));//导向轮
         painter->drawLine(QPointF(ps_leftx*m_zoom,-(ps_lefty-ps_hight*0.7)*m_zoom),QPointF((ps_leftx+ps_wight)*m_zoom,-(ps_lefty-ps_hight*0.7)*m_zoom));//导向轮
    }
    painter->setPen(QPen(Qt::black, 1));
    QRectF rt=QRectF(ps_leftx*m_zoom*1.1,-(ps_lefty+ps_hight*0.5)*m_zoom*1.1,ps_wight*m_zoom*1.1,(ps_hight*0.5)*m_zoom*1.1);
    painter->drawText(rt, Qt::AlignCenter,QString("%1").arg(m_nID));
    painter->setPen(QPen(Qt::green, 0.2));
    painter->setBrush(Qt::black);
    painter->drawEllipse(QPoint(0,0),1,1);
    QFont font;
    font.setPointSize(4);
    painter->setFont(font);
    //painter->setPen(QPen(Qt::black, 0.2));
    //QRectF rt=QRectF(ps_leftx*m_zoom*1.2,-(ps_lefty+ps_hight*0.5)*m_zoom,ps_wight*m_zoom*1.2,(ps_hight*0.5)*m_zoom);
    //painter->drawText(rt, Qt::AlignCenter,QString("%1").arg(m_nID));

   // QRectF r=QRectF(this->pos().x(),this->pos().y(),840*0.0106,2100*0.0106);


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

 if(command1.start==0&&command1.end==0)
            return;
 if(!m_bIsPathOK)
    {
        qDebug()<<"AGV发送双指令"<<command1.start<<command1.end<<"--"<<command2.start<<command2.end<<endl;
        sendCommand(command1,command2);


    }
  else if(!m_bIsEnableOK){
      set_pathEnable(true);
     }
//    if(command1.number==0&&command2.number==0)
 //  {
 //   return;
  //  }






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
       mission_state=tr("回位");  //应该是任务整体完成了
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

    if(cnt%17==2){  // 1s 定时中断

        tcp_getPos();
       // tcp_getPower();//仿真中，实时获取车辆的电量信息


    }
    else if(cnt%17==7){
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



         set_path(Command1,Command2);
         //tcp_getmissionlength();//仿真中，实时获取车辆的任务长度，考虑此方面以后可以从调度平台获取。
    }
    else if(cnt%17==11){
        tcp_getState();
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

    __log__(&m_file,QString("[send] set path enable"));
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
    agvcommand.remove(1);
    if(agvcommand.contains(2))
       {
        agvcommand[1]=agvcommand[2];
        agvcommand.remove(2);
       }
    if(agvcommand.contains(3))
        {
        agvcommand[2]=agvcommand[3];
        agvcommand.remove(3);
        }

    //agvcommand.remove(number);//说明number的任务已经完成了，提醒 停车指令的时候，agv也要发送一下给上位机6.22
    //如果不存在怎么办？思考6.22
    /*
    if(agvcommand.contains(1)&&agvcommand.contains(2))
        {
        Command1=agvcommand.value(1);
        Command2=agvcommand.value(2);
        }
    else if(agvcommand.contains(1))
        {
        Command1=agvcommand.value(1);
        Command2.start=Command1.end;
        Command2.end=Command1.end;
        }
    else//说明当前无任务
    {
        Command1={0,0,0};
        Command2={0,0,0};
    }
    */
  //  Command1=agvcommand.value(number+2);
   // Command2=agvcommand.value(number+3);



}


void AGV::sendCommand(AGVCommand zhiling, AGVCommand zhiling1)
{


   QByteArray buff;
    buff.append(0x40);
    buff.append(m_nID);
    buff.append(0x10);
    buff.append(zhiling.start);
    buff.append(zhiling.end);
    buff.append(zhiling1.start);
    buff.append(zhiling1.end);
    buff.append(_8BitsSum(buff.mid(1)));//18
    buff.append(0x23);

    m_bIsEnableOK = false;
    m_bIsPathOK = false;

    //__log__(&m_file,QString("[send]agv=%1 set path command: mode=%2 start=%3 end=%4 ").arg(m_nID).arg(m_nMode).arg(start).arg(end));
   // emit serial_write(buf);
    emit tcp_write(buff);




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

void AGV::tcp_getPos() //6
{
    QByteArray buf = PROTOCAL_HEAD;
    buf.append(m_nID);
    buf.append(get_address());

    buf.append(PROTOCAL_MID);
    buf.append(0x40);
    buf.append(m_nID);
    buf.append(FRAME_GET_POS);
    buf.append(_null);
    buf.append(_null);
    buf.append(_null);
    buf.append(_null);
    buf.append(_8BitsSum(buf.mid(18)));//18
    buf.append(0x23);
    buf.append(0xff-_8BitsSum(buf.mid(3)));
    //__log__(&m_file,QString("[send] get pos command"));
    //emit serial_write(buf);
    QByteArray buff;
    buff.append(0x40);
    buff.append(m_nID);
    buff.append(FRAME_GET_POS);
    buff.append(_null);
    buff.append(_null);
    buff.append(_null);
    buff.append(_null);
    buff.append(_8BitsSum(buf.mid(1)));//18
    buff.append(0x23);

    emit tcp_write(buff);
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
        qDebug()<<"路径发送收到"<<endl;
         __log__(&m_file,QString("[recv] set Path:%1").arg(m_bIsPathOK));
    }
    else if(frame_type==FRAME_ENABLE_PATH || frame_type==FRAME_ENABLE_MULTIMODE_PATH)
    {
        m_bIsEnableOK=true;
        qDebug()<<"路径使能完成"<<endl;

        //__logqDebug()<<"路径使能完成"<<endl;__(&m_file,QString("[recv] enable Path:%1").arg(m_bIsEnableOK));
    }
    else if(frame_type==FRAME_GET_POS){
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
            emit state_change();
        }
        if(m_nTargetID==-1) m_nTargetID = m_nTopoPosID;
       // __log__(&m_file,QString("agv=:%1 [recv] pos:(%2,%3,%4)").arg(m_nID).arg(x).arg(y).arg(theta));
    }
    else if(frame_type==0x29)//查询任务通信指令
    {
        int nStart = (((uchar)data.at(0))<<8)+(uchar)(data.at(1));
        int nEnd = (((uchar)data.at(2))<<8)+(uchar)(data.at(3));


        if((nStart==nEnd)&&(nStart==0))//初始时刻，无任何任务，查任务
        {
                  if(mission_state!=tr("取货"))
                  emit state_change();
        }


       else if((nStart==nEnd)&&(nStart!=65535))//管制停车，或者到达目标点停车
         {
                 if(get_Commandnumber()==0)//已经没有指令执行了
                  emit state_change();
        }
//        else if(nStart!=65535)//无指令执行，可能下一个点恰好被管制，或者到达目标点停车
//      {
//         if(get_Commandnumber()==0)//已经没有指令执行
        //          emit state_change();

        //}
     }
    else if(frame_type==FRAME_GET_FINISH_NEW || frame_type==FRAME_GET_MULTIMODE_FINISH ){  // FRAME_GET_FINISH_NEW

        //-----------6.24新修改双指令模式------------
        int nStart = (((uchar)data.at(0))<<8)+(uchar)(data.at(1));
        int nEnd = (((uchar)data.at(2))<<8)+(uchar)(data.at(3));

        qDebug()<<"指令完成："<<nStart<<"--"<<nEnd<<endl;

        int start=0;
        int end=0;
        bool finish_flag;
        if(agvcommand.contains(1))
        {

             start=agvcommand[1].start;
             end=agvcommand[1].end;
             qDebug()<<"与agvcommand【1】配对："<<start<<"--"<<end<<endl;
        }

        finish_flag = (start==nStart && end==nEnd); //||(nStart==0 && nEnd==0)||((nStart==nEnd)&&(nStart!=655345))||(nStart!=nEnd) ;
        //__log__(&m_file,QString("agv=:%1 [recv] state= %2 %3 %4 %5").arg(m_nID).arg(nStart).arg(nEnd).arg(m_nCommandStart).arg(m_nCommandEnd));
        if((nStart==0 && nEnd==0) || m_nTopoPosID==-1){
            //m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld(pos()),m_nTopoPosID);
             m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld( pos()),m_nTopoPosID);
            __log__(&m_file,QString("[proc]agv=:%1, match:%2").arg(m_nID).arg(m_nTopoPosID));
            if(m_nTopoPosID==-1) return;
        }
        if(finish_flag){


                if(nStart==0)//需要改
                {
                    if(nStart==0&&nEnd==0)
                    {
                       m_nTargetID=m_nTopoPosID;
                    }
                    else
                    ForkState=2;
                    emit state_change();//开始的时候不断等调度任务

                    if(agvcommand.contains(1)&&agvcommand.contains(2))
                       {
                        //还有很多条指令
                        Command1=agvcommand[1];
                        Command2=agvcommand[2];
                        m_bIsPathOK = false;
                        }
                    else if(agvcommand.contains(1))
                       {
                        //只剩一下一条指令
                        Command1=agvcommand[1];
                        Command2.start=Command1.end;
                        Command2.end=Command1.end;
                        agvcommand[2]=Command2;
                         m_bIsPathOK = false;
                        }
                    else//说明当前无任务,无任务就不开标志位了
                    {

                       // Command1={nEnd,nEnd,0,false,0};
                        //Command2={nEnd,nEnd,0,false,0};
                        //agvcommand[1]=Command1;
                        //agvcommand[2]=Command2;

                    }


                }
                else
                {
                     qDebug()<<"AGV:"<<m_nID<<"已完成一条指令，再运行其他指令"<<endl;
                     if(agvcommand[1].start==nStart && agvcommand[1].end==nEnd)
                       {
                           qDebug()<<"AGV:"<<m_nID<<"完成路径，等待指令下达"<<endl;
                            CommandManager(1);//重新修改函数
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
                __log__(&m_file,QString("[proc] plan: currentID:%1").arg(m_nTopoPosID));


                emit state_moved(this,xiansudu,jiaosudu);
                qDebug()<<"查询agvcommand更新"<<endl;
                if(agvcommand.contains(1))
                    qDebug()<<"agvcommand[1]"<<agvcommand[1].start<<"--"<<agvcommand[1].end<<endl;
                if(agvcommand.contains(2))
                    qDebug()<<"agvcommand[2]"<<agvcommand[2].start<<"--"<<agvcommand[2].end<<endl;
                if(agvcommand.contains(3))
                    qDebug()<<"agvcommand[3]"<<agvcommand[3].start<<"--"<<agvcommand[3].end<<endl;



                m_nFinishTimerCnt = 0;


        }

        //----------------------------------------------------------------

        /*-------------------------------------------------------
        int nStart = (((uchar)data.at(0))<<8)+(uchar)(data.at(1));
        int nEnd = (((uchar)data.at(2))<<8)+(uchar)(data.at(3));
        bool finish_flag;
        finish_flag = (m_nCommandStart==nStart && m_nCommandEnd==nEnd) || (nStart==0 && nEnd==0)||(m_nCommandStart1==nStart && m_nCommandEnd1==nEnd) ;
        //__log__(&m_file,QString("agv=:%1 [recv] state= %2 %3 %4 %5").arg(m_nID).arg(nStart).arg(nEnd).arg(m_nCommandStart).arg(m_nCommandEnd));
        if((nStart==0 && nEnd==0) || m_nTopoPosID==-1){
            //m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld(pos()),m_nTopoPosID);
             m_nTopoPosID = getNearstMatchedVer(m_topoView,mapToRealWorld( pos()),m_nTopoPosID);
            __log__(&m_file,QString("[proc]agv=:%1, match:%2").arg(m_nID).arg(m_nTopoPosID));
            if(m_nTopoPosID==-1) return;
        }
        if(finish_flag){

            if(m_nFinishTimerCnt>30){

                if(nStart==0)//需要改
                {
                    if(nStart==0&&nEnd==0)
                    {
                       m_nTargetID=m_nTopoPosID;
                    }
                    else
                    ForkState=2;
                }
                else
                {
                    m_topoView->set_register(QString("Ver_%1").arg(nEnd),"",m_agvcount);
                    m_nTopoPosID = nEnd;
                    previousPoint=nStart;
                    //getMissionState();
                   // previousQueue.append(nEnd);
                }
                __log__(&m_file,QString("[proc] plan: currentID:%1").arg(m_nTopoPosID));

                qDebug()<<"AGV:"<<m_nID<<"完成路径，等待指令下达"<<endl;
                emit state_change();
                emit state_moved(this,xiansudu,jiaosudu);
                m_bIsPathOK = false;
                m_nFinishTimerCnt = 0;

            }
        }
        -------------------------------------------------------------------------------*/
    }

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

