#include "missionmanager.h"
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlRecord>
#include <QLineEdit>
#include "global.h"
#include "wmsinterface.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QTextEdit>
#include <QtAlgorithms>
#include <QPointF>
QDateTime s_universeStart = QDateTime(QDate(1994,6,14),QTime(0,0));
MissionManager::MissionManager(QWidget *parent) : QWidget(parent)
{
 idzhanyong[5]=0;
 idzhanyong[6]=0;
 idzhanyong[11]=0;
 idzhanyong[12]=0;

 Aqu=false;
 Asong=false;
 Bqu=false;
 Bsong=false;

     Achaijie=0;
     Bchaijie=0;
    Azhuangpei=0;
    Bzhuangpei=0;
    //---------------------------------
    guidaostate=false;//轨道agv无任务
    downxiang=false;
    upxiang=false;

    Aquhuo=new MissionState();
    Bquhuo=new MissionState();
    Asonghuo=new MissionState();
    Bsonghuo=new MissionState();

   // --------------------------------------

    readgoods();
    setWindowTitle("AGV-任务管理器");
    setAttribute(Qt::WA_QuitOnClose,false);
    setMaximumSize(1400,560);
    setMinimumSize(1400,560);
    createTableWidget();
    createMissionDialog();
    createTestDialog();//创建独立测试窗口
    createButtons();
    createConnections();
    createLayout();
    open_database();
    m_wms = new WMSInterface;
    connect(m_wms,SIGNAL(is_connected(bool)),this,SIGNAL(wms_connected(bool)));
    connect(m_wms,SIGNAL(new_request(quint64,QPair<int,int>,QPair<int,int>,int)),this,SLOT(new_request(quint64,QPair<int,int>,QPair<int,int>,int)));
    connect(m_wms,SIGNAL(delete_request(quint64)),this,SLOT(stop_request(quint64)));
    connect(m_wms,SIGNAL(inquiry_state(quint64)),this,SLOT(inquiry_state(quint64)));
    connect(m_wms,SIGNAL(mission_finished(quint64)),this,SLOT(mission_finished(quint64)));
    bool bUIEnable = g_setting->value("/mission_manager/UI").toBool();
    g_setting->setValue("/mission_manager/UI",bUIEnable);
    m_btnCreate->setEnabled(bUIEnable);
    m_btnEdit->setEnabled(bUIEnable);

    xiangup=1;

    int id=startTimer(1000);


}

MissionManager::~MissionManager()
{

}

void MissionManager::createMissionDialog()
{
    m_missionDlg = new QDialog(this);
    m_comboAGV = new QComboBox(m_missionDlg);
    m_comboStart = new QComboBox(m_missionDlg);
    m_comboEnd = new QComboBox(m_missionDlg);
    m_comboPriority= new QComboBox(m_missionDlg);
    m_comboAGVType=new QComboBox(m_missionDlg);//根据车型下达调度任务

    m_lineStartLayer = new QLineEdit(m_missionDlg);
    m_lineEndLayer = new QLineEdit(m_missionDlg);
    m_lineStartLayer->setText("1");
    m_lineStartLayer->setDisabled(true);
    m_lineEndLayer->setText("1");
    m_lineEndLayer->setDisabled(true);



    //----------暂时禁用-----------------------
   // m_comboAGVType->addItem( "辊筒式",1);
    //m_comboAGVType->addItem("货叉式",2);
    //m_comboAGVType->addItem("顶升式",3);
    m_comboAGVType->setDisabled(true);
    m_comboAGV->setDisabled(true);
    m_comboPriority->setDisabled(true);
    QGridLayout *grid = new QGridLayout;
    int index = 0;
    grid->addWidget(new QLabel("A G V :"),index,0);
    grid->addWidget(m_comboAGV,index++,1);

    grid->addWidget(new QLabel("车型:"),index,0);
    grid->addWidget(m_comboAGVType,index++,1);

    grid->addWidget(new QLabel("任务起点工位:"),index,0);
    grid->addWidget(m_comboStart,index++,1);
    grid->addWidget(new QLabel("起点层级:"),index,0);
    grid->addWidget(m_lineStartLayer,index++,1);
    grid->addWidget(new QLabel("任务终点工位:"),index,0);
    grid->addWidget(m_comboEnd,index++,1);
    grid->addWidget(new QLabel("终点层级:"),index,0);
    grid->addWidget(m_lineEndLayer,index++,1);
    grid->addWidget(new QLabel("优先级"),index,0);
    grid->addWidget(m_comboPriority,index++,1);
    QDialogButtonBox *button_box = new QDialogButtonBox(m_missionDlg);
    button_box->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    grid->addWidget(button_box,index++,0,1,2);
    m_missionDlg->setLayout(grid);
    connect(button_box,SIGNAL(accepted()),m_missionDlg,SLOT(accept()));
    connect(button_box,SIGNAL(rejected()),m_missionDlg,SLOT(reject()));
}

void MissionManager::createTestDialog()
{
    m_testDlg = new QDialog();
    RecvCommand=new QTextEdit(m_testDlg);
    RecvCommand->setReadOnly(true);
    SendCommand=new QTextEdit(m_testDlg);
    SendCommand->setReadOnly(true);

    SendCommand->document()->setMaximumBlockCount(1000);
    RecvCommand->document()->setMaximumBlockCount(1000);

    TestAGV = new QComboBox(m_testDlg);
    Targetnode = new QComboBox(m_testDlg);//目标点距离
    nodeDistance=new QLineEdit(m_testDlg);
    nodeDistance->setReadOnly(true);


    QianJinZhenchang=new QPushButton("前进正常",m_testDlg);
    YouGuai=new QPushButton("右拐",m_testDlg);
    ZuoGuai=new QPushButton("左拐",m_testDlg);
    HouTuiZhenchang=new QPushButton("后退正常",m_testDlg);
    HouTuiJiansu=new QPushButton("后退减速",m_testDlg);
    QianJinJiansu=new QPushButton("前进减速",m_testDlg);
    Tingche=new QPushButton("停车等待",m_testDlg);
    Chushihua=new QPushButton("初始化",m_testDlg);

     QianJinZhenchang->setEnabled(false);
     YouGuai->setEnabled(false);
     ZuoGuai->setEnabled(false);
     HouTuiZhenchang->setEnabled(false);
     HouTuiJiansu->setEnabled(false);
     QianJinJiansu->setEnabled(false);
     Tingche->setEnabled(false);//禁止循环4.3
     Chushihua->setEnabled(true);//一开始初始化可以点击





    QVBoxLayout*mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("AGV:"));
    mainLayout->addWidget(TestAGV);
    mainLayout->addWidget(Chushihua);
    mainLayout->addWidget(new QLabel("目标RFID节点:"));
    mainLayout->addWidget(Targetnode);
    mainLayout->addWidget(new QLabel("节点距离:"));
    mainLayout->addWidget(nodeDistance);
    mainLayout->addWidget(QianJinZhenchang);
    mainLayout->addWidget(HouTuiZhenchang);
    mainLayout->addWidget(QianJinJiansu);
    mainLayout->addWidget(HouTuiJiansu);
    mainLayout->addWidget(ZuoGuai);
    mainLayout->addWidget(YouGuai);
    mainLayout->addWidget(Tingche);

    mainLayout->addWidget(new QLabel("发送动作:"));
    mainLayout->addWidget(SendCommand);
    mainLayout->addWidget(new QLabel("收到动作:"));
    mainLayout->addWidget(RecvCommand);

    m_testDlg->setLayout(mainLayout);

    Qt::WindowFlags flags=Qt::Dialog;

    flags |=Qt::WindowMinimizeButtonHint;

    m_testDlg->setWindowFlags(flags|Qt::WindowCloseButtonHint);

   connect(QianJinZhenchang,SIGNAL(clicked()),this,SLOT(action_QianjinZhenngchang()));
   connect(QianJinJiansu,SIGNAL(clicked()),this,SLOT(action_QianjinJiansu()));
   connect(HouTuiZhenchang,SIGNAL(clicked()),this,SLOT(action_HoutuiZhengchang()));
   connect(HouTuiJiansu,SIGNAL(clicked()),this,SLOT(action_HoutuiJiansu()));
   connect(ZuoGuai,SIGNAL(clicked()),this,SLOT(action_Zuoguai()));
   connect(YouGuai,SIGNAL(clicked()),this,SLOT(action_Youguai()));
   connect(Tingche,SIGNAL(clicked()),this,SLOT(action_Tingchedengdai()));
   connect(Chushihua,SIGNAL(clicked()),this,SLOT(action_Chushihua()));
   connect(Targetnode,SIGNAL(currentIndexChanged(int)),this,SLOT(action_nodeDistance(int)));
}

void MissionManager::createTableWidget()
{
    m_tableMission = new QTableView;
    m_tableMission->setMaximumSize(1300,390);
    m_tableMission->setMinimumSize(1300,390);
    int x = m_tableMission->geometry().x();
    int y = m_tableMission->geometry().y();
    m_tableMission->setGeometry(x,y,1200,390);
    m_tableMission->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableMission->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableMission->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);//tableview的滚动，方便用户查询
}

void MissionManager::createButtons()
{
    m_btnCreate = new QPushButton("新建",this);
    m_btnEdit = new QPushButton("编辑",this);
    m_btnHang = new QPushButton("执行",this);
    m_btnDelete = new QPushButton("删除",this);
    m_btnxunhuan=new QPushButton("开启循环",this);
    m_btnjinzhi=new QPushButton("停止循环",this);
    m_btnTest=new QPushButton("开启测试",this);
    m_btnTestStop=new QPushButton("退出测试",this);

    //----------------注释掉部分调度功能-----------
    m_btnTestStop->setEnabled(false);
    m_btnTest->setEnabled(false);
    m_btnxunhuan->setEnabled(false);
    m_btnjinzhi->setEnabled(false);
    //--------------------------------------------

}


void MissionManager::createLayout()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(m_tableMission,0,0,1,8);
    mainLayout->addWidget(m_btnCreate,1,0);
    mainLayout->addWidget(m_btnEdit,1,1);
    mainLayout->addWidget(m_btnHang,1,2);
    mainLayout->addWidget(m_btnDelete,1,3);
    mainLayout->addWidget(m_btnxunhuan,1,4);
    mainLayout->addWidget(m_btnjinzhi,1,5);
    mainLayout->addWidget(m_btnTest,1,6);
    mainLayout->addWidget(m_btnTestStop,1,7);
    setLayout(mainLayout);
}

void MissionManager::createConnections()
{
    connect(m_btnCreate,SIGNAL(clicked()),this,SLOT(action_create()));
    connect(m_btnEdit,SIGNAL(clicked()),this,SLOT(action_edit()));
    connect(m_btnHang,SIGNAL(clicked()),this,SLOT(action_hang()));
    connect(m_btnDelete,SIGNAL(clicked()),this,SLOT(action_delete()));
    connect(m_btnxunhuan,SIGNAL(clicked()),this,SLOT(action_xunhuan()));
    connect(m_btnjinzhi,SIGNAL(clicked()),this,SLOT(action_jinzhi()));
    connect(m_btnTestStop,SIGNAL(clicked()),this,SLOT(action_TestStop()));
    connect(m_btnTest,SIGNAL(clicked()),this,SLOT(action_Test()));

}

void MissionManager::setMissionState(Mission mission, QString state)
{
    setMissionState(mission.m_nID,state,mission);
}

void MissionManager::setMissionState(quint64 id, QString state,Mission mission)//4.11现在用的 着重于任务执行频率
{
      qDebug()<<"进入setMissionState1函数"<<endl;
    m_sqlTableModel->setFilter(QString("MissionID=%1").arg(id));
    m_sqlTableModel->select();
    if(m_sqlTableModel->rowCount()>0){
        QSqlRecord record = m_sqlTableModel->record(0);
        QString old_state = record.value("state").toString();
        int type=record.value("AGVType").toInt();

        int startnode=record.value("startNodeID").toInt();
         qDebug()<<"本条任务起始点："<<startnode<<endl;
        int endnode=record.value("endNodeID").toInt();


        record.setValue("state",state);
        if(state==MISSION_STATE_BACK){


          //-------------------为了当前的测试，暂时注释此部分---------------------
          //  m_comboStart->addItem(QString("Ver_%1").arg(startnode),startnode);//代表可以被选中
           // m_comboStart->addItem(QString("Ver_%1").arg(endnode),endnode);
           // m_comboEnd->addItem(QString("Ver_%1").arg(startnode),startnode);
           // m_comboEnd->addItem(QString("Ver_%1").arg(endnode),endnode);

            agvState.insert(record.value("AGV").toInt(),false);

             QDateTime startTime=mission.m_startTime;
             QDateTime endTime=QDateTime::currentDateTime();
             qint64 Time=startTime.secsTo(endTime);
             record.setValue("Time",Time);
            record.setValue("endTime",endTime.toString("yyyy-MM-dd hh:mm:ss"));
             //设置结束时间

          //暂时注释掉4.3
            //m_comboStart->addItem(QString("Ver_%1").arg(start),start);//代表可以被选中
            //m_comboStart->addItem(QString("Ver_%1").arg(end),end);
            //m_comboEnd->addItem(QString("Ver_%1").arg(start),start);
            //m_comboEnd->addItem(QString("Ver_%1").arg(end),end);
            if(m_wms!=NULL){
                m_wms->report_state(id,STATE_FINISHED);
            }
}
        else if((old_state!=MISSION_STATE_DELIVERY) && (state==MISSION_STATE_DELIVERY)){
        qDebug()<<"任务状态变为送货"<<startnode<<"--->"<<endnode<<endl;



        if(m_wms!=NULL){
                m_wms->report_state(id,STATE_DELIVERY);
            }
        }
        else if(state==MISSION_STATE_ERROR){
        }
        m_sqlTableModel->setRecord(0,record);
        m_sqlTableModel->submitAll();
    }
    showAllRecord();
}

void MissionManager::setMissionState(quint64 id, QString state)
{
     qDebug()<<"进入setMissionState2函数"<<endl;
    m_sqlTableModel->setFilter(QString("MissionID=%1").arg(id));
    m_sqlTableModel->select();
    if(m_sqlTableModel->rowCount()>0){
        QSqlRecord record = m_sqlTableModel->record(0);
        QString old_state = record.value("state").toString();
        int start=record.value("startNodeID").toInt();
        int end=record.value("endNodeID").toInt();
        record.setValue("state",state);
        if(state==MISSION_STATE_BACK){

            m_comboStart->addItem(QString("Ver_%1").arg(start),start);//代表可以被选中
            m_comboStart->addItem(QString("Ver_%1").arg(end),end);
            m_comboEnd->addItem(QString("Ver_%1").arg(start),start);
            m_comboEnd->addItem(QString("Ver_%1").arg(end),end);

             QDateTime endTime=QDateTime::currentDateTime();

            record.setValue("endTime",endTime.toString("yyyy-MM-dd hh:mm:ss"));
             //设置结束时间
            m_comboStart->addItem(QString("Ver_%1").arg(start),start);//代表可以被选中
            m_comboStart->addItem(QString("Ver_%1").arg(end),end);
            m_comboEnd->addItem(QString("Ver_%1").arg(start),start);
            m_comboEnd->addItem(QString("Ver_%1").arg(end),end);
            if(m_wms!=NULL){
                m_wms->report_state(id,STATE_FINISHED);
            }
        }
        else if(old_state!=MISSION_STATE_DELIVERY && state==MISSION_STATE_DELIVERY){
            if(m_wms!=NULL){
                m_wms->report_state(id,STATE_DELIVERY);
            }
        }
        else if(state==MISSION_STATE_ERROR){
        }
        m_sqlTableModel->setRecord(0,record);
        m_sqlTableModel->submitAll();
    }
    showAllRecord();
}



void MissionManager::setMissionState(quint64 id, QString state,AGV*agv)//函数重载，目的是获得路径长度,只有在任务完成的时候，才可以被调用
{

    qDebug()<<"进入setMissionState3函数"<<endl;
    m_sqlTableModel->setFilter(QString("MissionID=%1").arg(id));
    m_sqlTableModel->select();
    if(m_sqlTableModel->rowCount()>0){
          qDebug()<<"sqlTableModel里有记录"<<endl;
        QSqlRecord record = m_sqlTableModel->record(0);
        QString old_state = record.value("state").toString();
        int start=record.value("startNodeID").toInt();
        int end=record.value("endNodeID").toInt();
        record.setValue("state",state);
        if(state==MISSION_STATE_FINISH){
            agv->nowmissionid=0;
            qDebug()<<"任务ID:"<<id<<endl;
            qDebug()<<"agv:"<<agv->get_id()<<endl;
            qDebug()<<"已经计算时间"<<endl;
            QDateTime startTime=record.value("startTime").toDateTime();
            //QDateTime startTime=mission.m_startTime;
            QDateTime endTime=QDateTime::currentDateTime();
            qint64 Time=startTime.secsTo(endTime);
            record.setValue("Time",Time);
            record.setValue("missionlength",agv->missionlength);
            agv->missionlength=0;
           record.setValue("endTime",endTime.toString("yyyy-MM-dd hh:mm:ss"));
            //设置结束时间
           m_comboAGV->addItem( agv->get_name(),agv->get_id());
         //  m_comboStart->addItem(QString("Ver_%1").arg(start),start);//代表可以被选中
         //  m_comboStart->addItem(QString("Ver_%1").arg(end),end);
         //  m_comboEnd->addItem(QString("Ver_%1").arg(start),start);
         //  m_comboEnd->addItem(QString("Ver_%1").arg(end),end);
           if(m_wms!=NULL){
               m_wms->report_state(id,STATE_FINISHED);
           }
       }
        else if(old_state!=MISSION_STATE_DELIVERY && state==MISSION_STATE_DELIVERY){
            if(m_wms!=NULL){
                m_wms->report_state(id,STATE_DELIVERY);
            }
        }
        else if(state==MISSION_STATE_ERROR){
        }
        m_sqlTableModel->setRecord(0,record);
        m_sqlTableModel->submitAll();
    }
    showAllRecord();
}



void MissionManager::addMission(Mission mission)
{
    QSqlRecord record = m_sqlTableModel->record();
    record.setValue("MissionID",mission.m_nID);
    record.setValue("startTime",mission.m_startTime.toString("yyyy-MM-dd hh:mm:ss"));
    record.setValue("startNodeID",mission.m_nStartNodeID);
    record.setValue("endNodeID",mission.m_nEndNodeID);
    record.setValue("startNodeLayer",mission.m_nStartNodeLayer);
    record.setValue("endNodeLayer",mission.m_nEndNodeLayer);
    record.setValue("AGV",mission.m_strAGVID);
    record.setValue("priority",mission.m_nPriority);
    record.setValue("state",mission.m_strState);
    m_sqlTableModel->insertRecord(m_sqlTableModel->rowCount(),record);
    if(!m_sqlTableModel->submitAll()){
        QMessageBox::warning(0,"错误","新增任务失败");
    }
    showAllRecord();
}

void MissionManager::editMission(Mission mission)
{
    m_sqlTableModel->setFilter(QString("MissionID = %1").arg(mission.m_nID));
    m_sqlTableModel->select();
    if(m_sqlTableModel->rowCount()<1) return;
    QSqlRecord record = m_sqlTableModel->record(1);
    record.setValue("MissionID",mission.m_nID);
    record.setValue("startTime",mission.m_startTime.toString("yyyy-MM-dd hh:mm:ss"));
    record.setValue("startNodeID",mission.m_nStartNodeID);
    record.setValue("endNodeID",mission.m_nEndNodeID);
    record.setValue("startNodeLayer",mission.m_nStartNodeLayer);
    record.setValue("endNodeLayer",mission.m_nEndNodeLayer);
    record.setValue("AGV",mission.m_strAGVID);
    record.setValue("priority",mission.m_nPriority);
    record.setValue("state",mission.m_strState);
    m_sqlTableModel->setRecord(0,record);
    if(!m_sqlTableModel->submitAll()){
        QMessageBox::warning(0,"错误","新增任务失败");
    }
    showAllRecord();
}

Mission MissionManager::recordToMission(QSqlRecord record)
{
    Mission mission;
    mission.m_nID = record.value("MissionID").toULongLong();
    mission.m_nStartNodeID = record.value("startNodeID").toInt();
    mission.m_nStartNodeLayer = record.value("startNodeLayer").toInt();
    mission.m_nEndNodeID = record.value("endNodeID").toInt();
    mission.m_nEndNodeLayer = record.value("endNodeLayer").toInt();
    mission.m_strAGVID = record.value("AGV").toString();
    mission.m_strState = record.value("state").toString();
    mission.m_nPriority = record.value("priority").toInt();
    mission.m_startTime = record.value("startTime").toDateTime();
    mission.m_endTime = record.value("endTime").toDateTime();
    return mission;
}




void MissionManager::MissionToRecord(Mission mission,int chexing)
{


}


bool MissionManager::getNextMission(QString AGVName, Mission &mission, int startNotEqualTo)
{
    m_sqlTableModel->setFilter(QString("state='%1' and startNodeID<>%2 and AGV='%3'").arg(MISSION_STATE_WAIT).arg(startNotEqualTo).arg(AGVName));
    m_sqlTableModel->sort(6,Qt::DescendingOrder);
    m_sqlTableModel->select();
    if(m_sqlTableModel->rowCount()>0){
        QSqlRecord record = m_sqlTableModel->record(0);
        mission = recordToMission(record);
        record.setValue("state",MISSION_STATE_PICK_UP);
        m_sqlTableModel->setRecord(0,record);
        m_sqlTableModel->submitAll();
        showAllRecord();
        return true;
    }
    else{
        showAllRecord();
        return false;
    }
}


bool MissionManager::getNextMission(QString AGVName ,Mission &mission)
{
    m_sqlTableModel->setFilter(QString("state='%1' ").arg(MISSION_STATE_WAIT));
    m_sqlTableModel->sort(6,Qt::DescendingOrder);
    m_sqlTableModel->select();
    if(m_sqlTableModel->rowCount()>0){
        QSqlRecord record = m_sqlTableModel->record(0);
        mission = recordToMission(record);
        record.setValue("state",MISSION_STATE_PICK_UP);
        m_sqlTableModel->setRecord(0,record);
        m_sqlTableModel->submitAll();
        showAllRecord();
        AGV* agvi=m_agvNameMap.value(AGVName);
        m_agvState.insert(agvi,true);
        return true;
    }
    else{
        showAllRecord();
        return false;
    }
    return false;
}





bool MissionManager::getNextMission(QString AGVName, Mission &mission, AGV*agv)
{
    qDebug()<<"进入getnextmission函数"<<endl;
   m_sqlTableModel->setFilter(QString("state='%1' and AGVType='%2'").arg(MISSION_STATE_WAIT).arg(agv->AGVType));
    m_sqlTableModel->sort(6,Qt::DescendingOrder);//优先级排序
    m_sqlTableModel->select();
    if(m_sqlTableModel->rowCount()>0){

        QSqlRecord record = m_sqlTableModel->record(0);
        qDebug()<<"将任务分配给了特定AGV"<<endl;
        mission = recordToMission(record);
        agv->nowmissionid=mission.m_nID;
        record.setValue("state",MISSION_STATE_PICK_UP);
        record.setValue("AGV",QString::number(agv->get_id()));
        m_sqlTableModel->setRecord(0,record);
        m_sqlTableModel->submitAll();
        showAllRecord();
        agvState.insert(record.value("AGV").toInt(),true);
        return true;

    }
    else{
        showAllRecord();
        return false;
    }
}











void MissionManager::MissionResetOrFin(int agvid,bool hasmission,quint64 missionid, bool reset)
{
    if(hasmission==true)
    {
        m_sqlTableModel->setFilter(QString("MissionID=%1").arg(missionid));
        m_sqlTableModel->select();
        if(m_sqlTableModel->rowCount()>0){
            QSqlRecord record = m_sqlTableModel->record(0);
           int start= record.value("startNodeID").toInt();
           int end =record.value("endNodeID").toInt();

           m_comboStart->addItem(QString("Ver_%1").arg(start),start);
           m_comboStart->addItem(QString("Ver_%1").arg(end),end);
           m_comboEnd->addItem(QString("Ver_%1").arg(start),start);
           m_comboEnd->addItem(QString("Ver_%1").arg(end),end);
           if(reset==true)
           {

               record.setValue("AGV","");
               record.setValue("state",MISSION_STATE_WAIT);
           }
           else if(reset==false)
           {
            // QDateTime endTime = QDateTime::currentDateTime();
             record.setValue("state",MISSION_STATE_FINISH);
             //record.setValue("endTime",endTime.toString("yyyy-MM-dd hh:mm:ss"));

           }
           m_sqlTableModel->setRecord(0,record);

        }
        m_sqlTableModel->submitAll();
        showAllRecord();
    }


    AGV *agv=m_agvId.value(agvid);
    QString name=m_agvIdName.value(agvid);


    m_agvList.removeOne(agv);
    m_agvId.remove(agvid);
    m_agvState.remove(agv);
    m_agvNameMap.remove(name);
    m_agvIdName.remove(agvid);
   // m_agvList.append(agv);
   //  m_agvId.insert(agv->get_id(),agv);
   // m_agvState.insert(agv,false);
}
void MissionManager::MissionArrange(AGV *agv, bool flag)
{
    // m_agvState[agv]=flag;
    if(m_agvState.value(agv)!=flag)
    {
        m_agvState.insert(agv,flag);
    }
     m_sqlTableModel->setFilter(QString("state='%1'").arg(MISSION_STATE_WAIT));
     m_sqlTableModel->sort(6,Qt::DescendingOrder);
     m_sqlTableModel->select();
    if((m_sqlTableModel->rowCount()>0)&&(flag==false))
    {    
        for(int i=0;i<m_sqlTableModel->rowCount();i++)
        {
            QSqlRecord record = m_sqlTableModel->record(i);
            int startNode=record.value("startNodeID").toInt();
            int startNodeLayer=record.value("startNodeLayer").toInt();
            int endNodeLayer=record.value("endNodeLayer").toInt();
            if(m_agvState.keys(false).size()==0)
                break;
            AGV* agv_=NULL;
            int agv_id=-1;
            double mincost=0;
            if((startNodeLayer>1)||(endNodeLayer>1))
            {
                  int le=m_agvState.keys(false).count();
                  for(int i=0;i<le;i++)
                  {
                     if(m_agvState.keys(false).at(i)->type()==PS20)
                     {
                         agv_=m_agvState.keys(false).at(i);
                        int agv_Node=agv_->get_topoPosID();
                        //if(agv_Node==-1) continue;
                         QList<int> pathv;
                         //double mincost=0;
                         dijkstart(agv_Node,startNode,pathv,mincost);
                          agv_id=agv_->get_id();
                          break;
                     }
                  }
            }
            else
            {

                int le=m_agvState.keys(false).count();
                for(int i=0;i<le;i++)
                {
                   if(m_agvState.keys(false).at(i)->type()==PT20)
                   {
                       agv_=m_agvState.keys(false).at(i);
                      int agv_Node=agv_->get_topoPosID();
                       //if(agv_Node==-1) continue;
                       QList<int> pathv;
                       //double mincost=0;
                       dijkstart(agv_Node,startNode,pathv,mincost);
                        agv_id=agv_->get_id();
                        break;
                   }
                }
            }
            if(agv_id==-1) return;

               foreach (AGV* agvx, m_agvState.keys(false)) {
                         int agvxid=agvx->get_id();
                         int typex=agvx->type();
                         if(agvxid!=agv_id)
                         {
                             if((startNodeLayer>1)||(endNodeLayer>1))
                             {
                               if(typex==PS20)
                               {
                                   int agvNode=agvx->get_topoPosID();
                                   if(agvNode!=-1)
                                   {
                                       QList<int> path;
                                       double cost=0;
                                       dijkstart(agvNode,startNode,path,cost);
                                       if(cost<mincost)
                                       {
                                           mincost=cost;
                                           agv_=agvx;
                                       }
                                   }
                               }
                             }
                             else
                             {
                                if(typex==PT20)
                                {
                                    int agvNode=agvx->get_topoPosID();
                                    if(agvNode!=-1)
                                    {
                                        QList<int> path;
                                        double cost=0;
                                        dijkstart(agvNode,startNode,path,cost);
                                        if(cost<mincost)
                                        {
                                            mincost=cost;
                                            agv_=agvx;
                                        }
                                    }
                                }

                             }

                         }
               }
               //record.setValue("AGV",agv_->get_name());
               m_sqlTableModel->setRecord(i,record);

        }
    }
    m_sqlTableModel->submitAll();
    showAllRecord();

}

bool MissionManager::dijkstart(int startID, int endID, QList<int> &path,double &cost)
{
    came_from.clear();
    cost_so_far.clear();
    path.clear();
    cost=0;
    PriorityQueue<int> frontier;
    frontier.enQueue(PriorityQueue<int>::PQElement(startID,0));
    came_from[startID] = startID;
    cost_so_far[startID]=0;
    while(!frontier.isEmpty()){
        int current = frontier.deQueue().first;
        if(current==endID){
            cost=cost_so_far[endID];
            recontruct_path(startID,endID,path);
            return true;
        }
        TopoEdge *p = m_topoView->get_verByID(current)->m_pEdgeHead;
        while(p!=NULL){
            int next = p->m_nEndID;
            double new_cost = cost_so_far[current] + p->get_weight();  // 论文！
            if(!cost_so_far.count(next) || new_cost<cost_so_far[next]){
                cost_so_far[next] = new_cost;
                came_from[next] = current;
                frontier.enQueue(PriorityQueue<int>::PQElement(next,new_cost));
            }
            p = p->m_pNextEdge;
        }
    }
    return false;
}
void MissionManager::recontruct_path(int startID, int endID, QList<int> &path)
{
    int current = endID;
    path.push_front(current);
    while(current!=startID){
        current = came_from[current];
        path.push_front(current);
    }
}




int MissionManager::MissionDistribution(int startnode)
{

int endnode=22;//默认值
QMap <int,QList<Dis_point>>kuwei_D=m_topoView->kuwei_D;

QList<Dis_point>point=kuwei_D[startnode];

foreach(Dis_point point1,point)
{
    int ID=point1.point;

    if(lock_kuwei.contains(ID))
    {
    }
     else if(ID>=21&&ID<=31)  //21-31为分拣平台
      {
         endnode=ID;
         break;
      }



}


    return endnode;



}




int MissionManager::MissionDistribution(int startnode,bool flag)
{

int endnode=12;//默认值
QMap <int,QList<Dis_point>>kuwei_D=m_topoView->kuwei_D;

QList<Dis_point>point=kuwei_D[startnode];

foreach(Dis_point point1,point)
{
    int ID=point1.point;

    if(lock_kuwei.contains(ID))
    {
    }
     else if((ID>=2)&&(ID<=18)&&(ID%2==0))//2-18偶数为回库口
      {
         endnode=ID;
         break;
      }



}

    return endnode;

}

int MissionManager::MissionAllcoation(int startnode)
{
    int agvid=0;//默认2号车执行任务
    AGV_KuweiDis();//更新库位与agv的距离
    if(AgvKuwei.contains(startnode))
    {
       foreach(Dis_point agv,AgvKuwei[startnode])
       {
           if((agvState.contains(agv.point)&&(agvState[agv.point]==false))||(!agvState.contains(agv.point)))
              {
               agvid=agv.point;
               break;
              }
       }


    }

    if(agvid==0)
    {
        if(startnode<=18)
        return startnode/4+3;
        else if(startnode>18);
        return startnode/5;


     }
    else
        return agvid;
}

int MissionManager::AGV_KuweiDis()
{
   TopoVer* point;
   foreach(point, m_topoView->kuweipoint)
   {
       QList<Dis_point>D;
       foreach(AGV*agv, m_agvList)
       {
           int current_node=agv->get_topoPosID();
           TopoVer*current=m_topoView->m_verList[QString("Ver_%1").arg(current_node)];
           qreal distance=getDistance(QPointF(point->realx,point->realy),QPointF(current->realx,current->realy));
           Dis_point dispoint;
           dispoint.distance=distance;
           dispoint.point=agv->get_id();
           D.append(dispoint);



       }
      qSort(D.begin(),D.end(),CompareDistance);
       AgvKuwei[point->m_nID]=D;
       qDebug()<<"库位点与agv距离"<<point->m_nID<<point->realx<<point->realy<<endl;
       for(int i=0;i<D.count();i++)
       {
       qDebug()<<(D.at(i)).distance<<endl;
       }
   }











}

bool MissionManager::SearchNextMission(QString AGVName, Mission &mission, AGV *agv)
{

    qDebug()<<"进入SearchNextMission函数"<<endl;
   m_sqlTableModel->setFilter(QString("state='%1' and chexing='%2'").arg(MISSION_STATE_WAIT).arg(agv->agvchexing));
    m_sqlTableModel->sort(6,Qt::DescendingOrder);//优先级排序
    m_sqlTableModel->select();
    if(m_sqlTableModel->rowCount()>0)
    {
        return true;
    }
    else
    {
        return false;
    }


}


Mission MissionManager::createMission(int nstartID, int nEndID, QString strAGVID, MISSION_PRIORITY nPriority)
{
    Mission mission;
    mission.m_strState = MISSION_STATE_WAIT;
    mission.m_strAGVID = strAGVID;
    mission.m_nStartNodeID = nstartID;
    mission.m_nEndNodeID = nEndID;
    mission.m_nPriority = nPriority;
    return mission;
}

bool MissionManager::open_database()
{
    QDate dateTime = QDate::currentDate();
    m_databasePath = QString("mission_")+dateTime.toString("yyyyMMdd")+".db";
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE","log_mission");
    database.setDatabaseName(m_databasePath);
    if(!database.open()){
        QMessageBox::critical(0,"数据库错误","任务日志数据库打开错误");
        return false;
    }
    m_query = QSqlQuery(database);
    m_query.exec("create table if not exists tb_mission "
               "(MissionID int primary key,startNode int,startNodeID int,startNodeLayer int,endNode int,endNodeID int,"
               "endNodeLayer int,AGV text,priority int,startTime text,endTime text,Time text,state text,AGVType text)");
    m_sqlTableModel = new QSqlTableModel(this,database);
    m_sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    showAllRecord();
    return true;
}

void MissionManager::showAllRecord()
{
    m_sqlTableModel->setTable("tb_mission");
    m_sqlTableModel->setHeaderData(0,Qt::Horizontal,"ID");
    m_sqlTableModel->setHeaderData(1,Qt::Horizontal,"起始工位");
    m_sqlTableModel->setHeaderData(2,Qt::Horizontal,"起始RFID");
    m_sqlTableModel->setHeaderData(3,Qt::Horizontal,"起点层级");
    m_sqlTableModel->setHeaderData(4,Qt::Horizontal,"终止工位");
    m_sqlTableModel->setHeaderData(5,Qt::Horizontal,"终止RFID");
    m_sqlTableModel->setHeaderData(6,Qt::Horizontal,"终点层级");
    m_sqlTableModel->setHeaderData(7,Qt::Horizontal,"AGV");
    m_sqlTableModel->setHeaderData(8,Qt::Horizontal,"优先级");
    m_sqlTableModel->setHeaderData(9,Qt::Horizontal,"起始时间");
    m_sqlTableModel->setHeaderData(10,Qt::Horizontal,"终止时间");
    m_sqlTableModel->setHeaderData(11,Qt::Horizontal,"用时(S)");
    m_sqlTableModel->setHeaderData(12,Qt::Horizontal,"状态");
    m_sqlTableModel->setHeaderData(13,Qt::Horizontal,"车型");

    m_sqlTableModel->select();
    m_tableMission->setModel(m_sqlTableModel);
    m_tableMission->setColumnWidth(0,120);
    m_tableMission->setColumnWidth(1,60);
    m_tableMission->setColumnWidth(2,60);
    m_tableMission->setColumnWidth(3,60);
    m_tableMission->setColumnWidth(4,60);
    m_tableMission->setColumnWidth(5,60);
    m_tableMission->setColumnWidth(6,60);
    m_tableMission->setColumnWidth(7,60);
    m_tableMission->setColumnWidth(8,60);
    m_tableMission->setColumnWidth(9,140);
    m_tableMission->setColumnWidth(10,140);
    m_tableMission->setColumnWidth(11,60);
    m_tableMission->setColumnWidth(12,60);
    m_tableMission->setColumnWidth(13,60);



}

void MissionManager::loadTopoView(TopoView* topoview)
{
    m_topoView = topoview;
    m_comboStart->clear();
    m_comboEnd->clear();
    m_comboPriority->clear();
    m_comboAGV->clear();
    foreach(TopoVer* ver,m_topoView->m_verList.values()){
        if(ver->is_callPoint()){
            m_comboStart->addItem(QString("%1").arg(ver->SLNumber),ver->SLNumber);//SLNumber代表
            m_comboEnd->addItem(QString("%1").arg(ver->SLNumber),ver->SLNumber);

        }
         Targetnode->addItem(ver->get_name(),ver->get_id());

    }
    for(int i=0;i<5;i++){
        m_comboPriority->addItem(QString::number(i),i);
    }

   //TopoEdge *edge= m_topoView->get_edgeByVer(500,501);

   //double l=edge->get_weight();
   //qDebug()<<edge->get_id()<<"wight"<<l;
  /* QList<int> pathAndComm;
   pathAndComm.append(39);
   pathAndComm.append(29);
   pathAndComm.append(19);
   QList<int> path;
   //path.append(500);
   path.append(501);
   path.append(503);
   path.append(504);
   path.append(121);
   path.append(123);
   path.append(128);
   path.append(39);
   path.append(29);
   path.append(19);
   int pAcL=pathAndComm.length();
   if((pAcL>0))
   {
       int  indexFr=path.indexOf(pathAndComm.first());
       TopoEdge *edgeF=m_topoView->get_edgeByVer(500,path.at(0));
       int lenwight=edgeF->get_weight();
       for (int i=0;i<(indexFr-1);i++)
       {
           TopoEdge *edge=m_topoView->get_edgeByVer(path.at(i),path.at(i+1));
           lenwight= lenwight+edge->get_weight();
       }
       int nextlength=lenwight-(edgeF->get_weight());
       if(((lenwight>2200)&&(nextlength<2200))||(path.first()==pathAndComm.first())||(lenwight<2200))
       {
           qDebug()<<"6666"<<lenwight<<""<<nextlength;
       }
   }
   */
}

void MissionManager::addAGV(AGV * agv)
{
    foreach(AGV* a,m_agvList){    // 如果重名则返回，由于测试均在本机，故此处注释掉
        if(a->get_name()==agv->get_name())
            return;
    }
    if(!m_agvList.contains(agv))
    {
      m_agvList.append(agv);
       m_agvId1.insert(agv->get_id(),agv);
    }
    qDebug()<<"在combo中成功加入AGV"<<agv->get_name()<<endl;

    m_comboAGV->addItem(agv->get_name(),agv->get_id());//代表可以被选中


    TestAGV->addItem(agv->get_name(),agv->get_id());

    m_agvId.insert(agv->get_id(),agv);
    m_agvNameMap.insert(agv->get_name(),agv);
    m_agvIdName.insert(agv->get_id(),agv->get_name());
    m_agvState.insert(agv,false);
     m_agvState1.insert(agv,false);//4.5复制agv状态map
    //m_comboAGV->clear();
   /* if(m_agvList.count()>1) m_comboAGV->addItem("任意","");
    for(int i=0;i<m_agvList.length();i++){
        m_comboAGV->addItem(m_agvList.at(i)->get_name(),m_agvList.at(i)->get_name());
    }
    */
}

void MissionManager::removeAGV(AGV *agv)
{
    foreach(AGV* a,m_agvList){
        if(!a || ((agv) && a->get_id()==agv->get_id())){
            m_agvList.removeOne(a);
        }
    }
    m_comboAGV->clear();
    if(m_agvList.count()>1) m_comboAGV->addItem("任意","");
    for(int i=0;i<m_agvList.length();i++){
        m_comboAGV->addItem(m_agvList.at(i)->get_name(),m_agvList.at(i)->get_name());
    }
}

void MissionManager::action_create()
{
    m_missionDlg->setWindowTitle("新增任务");
    if(m_missionDlg->exec()){
        QSqlRecord record = m_sqlTableModel->record();
        QDateTime startTime = QDateTime::currentDateTime();
        uint nID = s_universeStart.msecsTo(startTime);
        record.setValue("MissionID",nID);
        record.setValue("startTime",startTime.toString("yyyy-MM-dd hh:mm:ss"));
        record.setValue("startNode",m_comboStart->currentData().toInt());
        record.setValue("startNodeID",m_topoView->SLNumber_RFID[m_comboStart->currentData().toInt()]);
        record.setValue("endNode",m_comboEnd->currentData().toInt());
        record.setValue("endNodeID",m_topoView->SLNumber_RFID[m_comboEnd->currentData().toInt()]);
        record.setValue("startNodeLayer",m_lineStartLayer->text().toInt());
        record.setValue("endNodeLayer",m_lineEndLayer->text().toInt());
       // record.setValue("AGV",m_comboAGV->currentData().toString());

        int id=m_topoView->SLNumber_RFID[m_comboEnd->currentData().toInt()];
        int SLType=m_topoView->m_verList[QString("Ver_%1").arg(id)]->SLType;
        int AGVType;
        switch(SLType)
        {
        case 1:
            AGVType=2;
            break;
        case 2:
            AGVType=1;
            break;
        }

        record.setValue("AGVType",QString("%1").arg(AGVType));//当前是按照车型下发任务


        qDebug()<<"新建任务加入的agv名字是"<<m_comboAGV->currentData().toString()<<endl;
        record.setValue("priority",m_comboPriority->currentData().toInt());
        record.setValue("state",MISSION_STATE_WAIT);

        m_sqlTableModel->insertRecord(m_sqlTableModel->rowCount(),record);
        if(!m_sqlTableModel->submitAll()){
            QMessageBox::warning(0,"错误","新增任务失败");
        }
        showAllRecord();

         //QString agv= m_comboAGV->currentData().toString();
         //int AGV=m_comboAGV->findData(agv);
        //  m_comboAGV->removeItem(AGV);

      /*---------------为保证正常，暂时注释掉此部分功能----------------------
        int start=m_comboStart->currentData().toInt();
        int end=m_comboEnd->currentData().toInt();
       int a= m_comboStart->findData(end);
       int b=m_comboEnd->findData(start);

        m_comboStart->removeItem(a);
        m_comboEnd->removeItem(b);
        int starts=m_comboStart->currentIndex();
        int ends=m_comboEnd->currentIndex();
        m_comboEnd->removeItem(ends);
       m_comboStart->removeItem(starts);

       --------------------------------------------*/

    }
    //MissionArrange();
}

void MissionManager::action_edit()
{
    int row = m_tableMission->currentIndex().row();
    m_missionDlg->setWindowTitle("编辑任务");
    if(row>-1){
        QSqlRecord record = m_sqlTableModel->record(row);
        if(record.value("state").toString()!=MISSION_STATE_WAIT){
            QMessageBox::warning(0,"错误","只有处于等待状态的任务才能被编辑");
            return;
        }
        m_comboStart->setCurrentText(record.value("startNodeID").toString());
        m_comboEnd->setCurrentText(record.value("endNodeID").toString());
        m_comboAGV->setCurrentText(record.value("AGV").toString());
        m_comboPriority->setCurrentText(record.value("priority").toString());
        m_lineStartLayer->setText(record.value("startNodeLayer").toString());
        m_lineEndLayer->setText(record.value("endNodeLayer").toString());
        if(m_missionDlg->exec()){
            record.setValue("startNodeID",m_comboStart->currentData().toInt());
            record.setValue("endNodeID",m_comboEnd->currentData().toInt());
            record.setValue("startNodeLayer",m_lineStartLayer->text().toInt());
            record.setValue("endNodeLayer",m_lineEndLayer->text().toInt());
            record.setValue("AGV",m_comboAGV->currentData().toString());
            record.setValue("priority",m_comboPriority->currentData().toInt());
            m_sqlTableModel->setRecord(row,record);
            if(!m_sqlTableModel->submitAll()){
                QMessageBox::warning(0,"错误","任务编辑失败");
            }
            showAllRecord();
        }
    }
}

void MissionManager::action_hang()
{
    int row = m_tableMission->currentIndex().row();
    if(row>-1){
        QSqlRecord record = m_sqlTableModel->record(row);
        int ret = QMessageBox::warning(0,"警告","该操作将设置任务为完成状态且向WMS报告该任务已完成，请人工确保该任务被正确执行\n"
                                               "确认强制改变任务为完成状态？",QMessageBox::Yes|QMessageBox::No);
        if(ret==QMessageBox::Yes){
            setMissionState(record.value("MissionID").toULongLong(),MISSION_STATE_FINISH);
            m_sqlTableModel->select();
        }
    }

    /*Mission mission;
    getNextMission("",mission,10);*/
}

void MissionManager::action_delete()
{
    int row = m_tableMission->currentIndex().row();
    if(row>-1){
        QString current_state = m_sqlTableModel->record(row).value("state").toString();
        int start=m_sqlTableModel->record(row).value("startNodeID").toInt();
        int  end=m_sqlTableModel->record(row).value("endNodeID").toInt();
        int l=m_comboStart->count();
        bool flag1=false,flag2=false,flag3=false,flag4=false;
        for(int i=0;i<l;i++)
        {
            if(m_comboStart->itemData(i)==start)
            {
               flag1=true;
            }
            if(m_comboStart->itemData(i)==end)
            {
               flag2=true;
            }
            if(m_comboEnd->itemData(i)==start)
            {
               flag3=true;
            }
            if(m_comboEnd->itemData(i)==end)
            {
               flag4=true;
            }
        }
        if(flag1==false) m_comboStart->addItem(QString("Ver_%1").arg(start),start);
        if(flag2==false) m_comboStart->addItem(QString("Ver_%1").arg(end),end);
        if(flag3==false) m_comboEnd->addItem(QString("Ver_%1").arg(start),start);
        if(flag4==false) m_comboEnd->addItem(QString("Ver_%1").arg(end),end);
       // m_comboStart->addItem(QString("Ver_%1").arg(end),end);
       // m_comboEnd->addItem(QString("Ver_%1").arg(start),start);
       // m_comboEnd->addItem(QString("Ver_%1").arg(end),end);
        if(current_state!=MISSION_STATE_PICK_UP && current_state!=MISSION_STATE_DELIVERY){
            m_sqlTableModel->removeRow(row);
            m_sqlTableModel->submitAll();
            m_sqlTableModel->select();

        }
        else{
            int ret = QMessageBox::warning(0,"危险","严禁在自动模式下删除正在执行的任务，请依次完成：\n"
                                                  "1.确认车辆已经处于手动运行状态并位于主干道上;"
                                                  "2.确认车辆货叉放至最底端并重启车辆;"
                                                  "3.确认正在运输的货物已经人工扫码至WMS系统;"
                                                  "4.删除任务",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
            if(ret==QMessageBox::Yes){
                int ret2 = QMessageBox::warning(0,"警告","该操作将删除正在执行的任务，可能有危险，请在人工监督下完成当前操作！\n"
                                                       "确认删除？",QMessageBox::Yes|QMessageBox::No);
                if(ret2==QMessageBox::Yes){
                emit mission_deleted(m_sqlTableModel->record(row).value("MissionID").toULongLong());
                    m_sqlTableModel->removeRow(row);
                    m_sqlTableModel->submitAll();
                    m_sqlTableModel->select();
                }
            }
        }
    }else{
        QMessageBox::warning(0,"删除失败","请先在任务列表中选择待删除的任务");
    }
}
void MissionManager:: action_xunhuan()
{
    Chukuid=startTimer(60000);
   RuKuid=startTimer(90000);


   // Chukuid=startTimer(12000);
    //RuKuid=startTimer(18000);
    /*
     xunhuantimer=startTimer(2000);
     zhuangpeixiantimer=startTimer(1000);
     guntongtimer=startTimer(1000);
     //huowuA=1;
     //huowuB=1;
     //huowuAB=0;
     */

}

void MissionManager::action_jinzhi()
{
    killTimer(xunhuantimer);
}

void MissionManager::action_Test()
{
    m_testDlg->show();
}

void MissionManager::action_Chushihua()
{
    AGV*agv;
    if(m_agvId1.contains(TestAGV->currentData().toInt()))
         agv=m_agvId1[TestAGV->currentData().toInt()];
    else
        return;



    int _null = 0x00;
    QByteArray buff;
    buff.append(0x4A);//J
    buff.append(0x59);//Y
    buff.append(0x43);//C
    buff.append(0x07);//0X07
    buff.append(0x01);//功能码
    buff.append(agv->get_id());//车号
    buff.append(_null);//状态暂时为0
    buff.append(agv->get_topoPosID()>>8);
    buff.append(agv->get_topoPosID());
    buff.append(_null);
    buff.append(_8BitsSum(buff.mid(4,6)));//和校验
    buff.append(0x0D);// \r
    buff.append(0x0A);// \n
    //qDebug()<<"测试取余"<<_8BitsSum(buff.mid(4,6))<<endl;
    SendCommand->append(QString("[%1],agv=%2,初始化发送").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agv->get_id()));
    //m_nTopoPosID=(((uchar)data.at(7))<<8)+(uchar)(data.at(8));//7.8组成当前的拓扑点。
    qDebug()<<"调通通信"<<endl;
    emit agv->tcp_write(buff);//初始化应答


  //  QianJinZhenchang->setEnabled(true);
  //  YouGuai->setEnabled(true);
  //  ZuoGuai->setEnabled(true);
 //   HouTuiZhenchang->setEnabled(true);
 //   HouTuiJiansu->setEnabled(true);
 //   QianJinJiansu->setEnabled(true);
 //   Tingche->setEnabled(true);
  //  Chushihua->setEnabled(true);

}

void MissionManager::action_TestStop()
{
    QList<AGV*>agvlist=m_agvId1.values();

    for(int i=0;i<agvlist.size();i++)
    {
        agvlist.at(i)->TestModel=false;
        emit agvlist.at(i)->state_change();

    }

    m_testDlg->close();
    QMessageBox::information(NULL, "提示", "独立测试功能已关闭", QMessageBox::Yes, QMessageBox::Yes);

}

void MissionManager::action_QianjinZhenngchang()
{

   bool direction= false;
   AGVCommand com,com1;
   AGV*agv;
   if(m_agvId1.contains(TestAGV->currentData().toInt()))
        agv=m_agvId1[TestAGV->currentData().toInt()];
   else
       return;

   int current_node=agv->get_topoPosID();
   TopoVer*current=m_topoView->m_verList[QString("Ver_%1").arg(current_node)];
   int target_node=Targetnode->currentData().toInt();
   TopoVer*target=m_topoView->m_verList[QString("Ver_%1").arg(target_node)];
   QPointF path=QPointF(target->realx-current->realx,target->realy-current->realy);
   float distance=sqrt((current->realx - target->realx)*(current->realx - target->realx) + (current->realy - target->realy)*(current->realy - target->realy));
   switch(agv->agvAngel)
   {
     case 0:
       if((path.y()>0)&&(path.x()==0))
           direction=true;
       break;
     case 90:
       if((path.x()<0)&&(path.y()==0))
           direction=true;
       break;
     case 180:
       if((path.y()<0)&&(path.x()==0))
           direction=true;
       break;
     case 270:
       if((path.x()>0)&&(path.y()==0))
           direction=true;
       break;

   }

   if(current_node==target_node)
   {
        QMessageBox::information(NULL, "提示", "目标节点设置错误", QMessageBox::Yes, QMessageBox::Yes);
   }


 if(direction==true)
   {

           com.start=agv->get_topoPosID();
           com.end=target_node;
           com.length=distance;
           com.angel=0;
           com.Fangxiang=0x01;
           com.Dongzuo=0x01;
           com.test=true;
           qDebug()<<"测试程序距离："<<(uint)distance<<endl;
           SendCommand->append(QString("[%1],agv=%2,start=%3,end=%4,length=%5,angle=%6,Direction=%7,Action=%8").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agv->get_id()).arg(com.start).arg(com.end).arg(com.length).arg(com.angel).arg("前进").arg("正常"));
           agv->set_targetCommand(com,com1);

   }
   else
   {
       QMessageBox::information(NULL, "提示", "目标节点设置错误", QMessageBox::Yes, QMessageBox::Yes);
       return;
   }


   QianJinZhenchang->setEnabled(false);
   YouGuai->setEnabled(false);
   ZuoGuai->setEnabled(false);
   HouTuiZhenchang->setEnabled(false);
   HouTuiJiansu->setEnabled(false);
   QianJinJiansu->setEnabled(false);
  // Tingche->setEnabled(false);
   Chushihua->setEnabled(false);
   qDebug()<<"测试程序111***:"<<com.start<<com.end<<endl;



}

void MissionManager::action_QianjinJiansu()
{

    bool direction=false;
    QPointF path;
    AGVCommand com,com1;
    AGV*agv;
    if(m_agvId1.contains(TestAGV->currentData().toInt()))
         agv=m_agvId1[TestAGV->currentData().toInt()];
    else
        return;

    int current_node=agv->get_topoPosID();
    TopoVer*current=m_topoView->m_verList[QString("Ver_%1").arg(current_node)];
    int target_node=Targetnode->currentData().toInt();
    TopoVer*target=m_topoView->m_verList[QString("Ver_%1").arg(target_node)];
    path=QPointF(target->realx-current->realx,target->realy-current->realy);
    float distance=sqrt((current->realx - target->realx)*(current->realx - target->realx) + (current->realy - target->realy)*(current->realy - target->realy));
    switch(agv->agvAngel)
    {
      case 0:
        if((path.y()>0)&&(path.x()==0))
            direction=true;
        break;
      case 90:
        if((path.x()<0)&&(path.y()==0))
            direction=true;
        break;
      case 180:
        if((path.y()<0)&&(path.x()==0))
            direction=true;
        break;
      case 270:
        if((path.x()>0)&&(path.y()==0))
            direction=true;
        break;

    }

    if(current_node==target_node)
    {
         QMessageBox::information(NULL, "提示", "目标节点设置错误", QMessageBox::Yes, QMessageBox::Yes);
    }


  if(direction==true)
    {

            com.start=agv->get_topoPosID();
            com.end=target_node;
            com.length=distance;
            com.angel=0;
            com.Fangxiang=0x01;
            com.Dongzuo=0x02;
            com.test=true;
            qDebug()<<"测试程序距离："<<(uint)distance<<endl;
            SendCommand->append(QString("[%1] agv=%2,start=%3,end=%4,length=%5,angle=%6,Direction=%7,Action=%8").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agv->get_id()).arg(com.start).arg(com.end).arg(com.length).arg(com.angel).arg("前进").arg("减速"));
            agv->set_targetCommand(com,com1);

    }
    else
    {
        QMessageBox::information(NULL, "提示", "目标节点设置错误", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }


    QianJinZhenchang->setEnabled(false);
    YouGuai->setEnabled(false);
    ZuoGuai->setEnabled(false);
    HouTuiZhenchang->setEnabled(false);
    HouTuiJiansu->setEnabled(false);
    QianJinJiansu->setEnabled(false);
   // Tingche->setEnabled(false);
    Chushihua->setEnabled(false);
    qDebug()<<"测试程序111***:"<<com.start<<com.end<<endl;











}

void MissionManager::action_HoutuiZhengchang()
{

    bool direction=false;
    AGVCommand com,com1;
    AGV*agv;
    if(m_agvId1.contains(TestAGV->currentData().toInt()))
         agv=m_agvId1[TestAGV->currentData().toInt()];
    else
        return;
    int current_node=agv->get_topoPosID();
    TopoVer*current=m_topoView->m_verList[QString("Ver_%1").arg(current_node)];
    int target_node=Targetnode->currentData().toInt();
    TopoVer*target=m_topoView->m_verList[QString("Ver_%1").arg(target_node)];
    QPointF path=QPointF(target->realx-current->realx,target->realy-current->realy);
    float distance=sqrt((current->realx - target->realx)*(current->realx - target->realx) + (current->realy - target->realy)*(current->realy - target->realy));
    switch(agv->agvAngel)
    {
      case 0:
        if((path.y()<0)&&(path.x()==0))
            direction=true;
        break;
      case 90:
        if((path.x()>0)&&(path.y()==0))
            direction=true;
        break;
      case 180:
        if((path.y()>0)&&(path.x()==0))
            direction=true;
        break;
      case 270:
        if((path.x()<0)&&(path.y()==0))
            direction=true;
        break;

    }

    if(current_node==target_node)
    {
         QMessageBox::information(NULL, "提示", "目标节点设置错误", QMessageBox::Yes, QMessageBox::Yes);
    }


  if(direction==true)
    {

            com.start=agv->get_topoPosID();
            com.end=target_node;
            com.length=distance;
            com.angel=0;
            com.Fangxiang=0x02;
            com.Dongzuo=0x01;
            com.test=true;
            qDebug()<<"测试程序距离："<<(uint)distance<<endl;
            SendCommand->append(QString("[%1] agv=%2,start=%3,end=%4,length=%5,angle=%6,Direction=%7,Action=%8").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agv->get_id()).arg(com.start).arg(com.end).arg(com.length).arg(com.angel).arg("后退").arg("正常"));
            agv->set_targetCommand(com,com1);

    }
    else
    {
        QMessageBox::information(NULL, "提示", "目标节点设置错误", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }


    QianJinZhenchang->setEnabled(false);
    YouGuai->setEnabled(false);
    ZuoGuai->setEnabled(false);
    HouTuiZhenchang->setEnabled(false);
    HouTuiJiansu->setEnabled(false);
    QianJinJiansu->setEnabled(false);
   // Tingche->setEnabled(false);
    Chushihua->setEnabled(false);
    qDebug()<<"测试程序111***:"<<com.start<<com.end<<endl;



}

void MissionManager::action_HoutuiJiansu()
{

    bool direction=false;
    AGVCommand com,com1;
    AGV*agv;
    if(m_agvId1.contains(TestAGV->currentData().toInt()))
         agv=m_agvId1[TestAGV->currentData().toInt()];
    else
        return;

    int current_node=agv->get_topoPosID();
    TopoVer*current=m_topoView->m_verList[QString("Ver_%1").arg(current_node)];
    int target_node=Targetnode->currentData().toInt();
    TopoVer*target=m_topoView->m_verList[QString("Ver_%1").arg(target_node)];
    QPointF path=QPointF(target->realx-current->realx,target->realy-current->realy);
    float distance=sqrt((current->realx - target->realx)*(current->realx - target->realx) + (current->realy - target->realy)*(current->realy - target->realy));
    switch(agv->agvAngel)
    {
      case 0:
        if((path.y()<0)&&(path.x()==0))
            direction=true;
        break;
      case 90:
        if((path.x()>0)&&(path.y()==0))
            direction=true;
        break;
      case 180:
        if((path.y()>0)&&(path.x()==0))
            direction=true;
        break;
      case 270:
        if((path.x()<0)&&(path.y()==0))
            direction=true;
        break;

    }

    if(current_node==target_node)
    {
         QMessageBox::information(NULL, "提示", "目标节点设置错误", QMessageBox::Yes, QMessageBox::Yes);
    }


  if(direction==true)
    {

            com.start=agv->get_topoPosID();
            com.end=target_node;
            com.length=distance;
            com.angel=0;
            com.Fangxiang=0x02;
            com.Dongzuo=0x02;
            com.test=true;
            qDebug()<<"测试程序距离："<<(uint)distance<<endl;
            SendCommand->append(QString("[%1]  agv=%2,start=%3,end=%4,length=%5,angle=%6,Direction=%7,Action=%8").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agv->get_id()).arg(com.start).arg(com.end).arg(com.length).arg(com.angel).arg("后退").arg("减速"));
            agv->set_targetCommand(com,com1);

    }
    else
    {
        QMessageBox::information(NULL, "提示", "目标节点设置错误", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }


    QianJinZhenchang->setEnabled(false);
    YouGuai->setEnabled(false);
    ZuoGuai->setEnabled(false);
    HouTuiZhenchang->setEnabled(false);
    HouTuiJiansu->setEnabled(false);
    QianJinJiansu->setEnabled(false);
   // Tingche->setEnabled(false);
    Chushihua->setEnabled(false);
    qDebug()<<"测试程序111***:"<<com.start<<com.end<<endl;






}

void MissionManager::action_Zuoguai()
{

    AGVCommand com,com1;
    AGV*agv;
    if(m_agvId1.contains(TestAGV->currentData().toInt()))
         agv=m_agvId1[TestAGV->currentData().toInt()];
    else
        return;

    com.start=agv->get_topoPosID();
    com.end=agv->get_topoPosID();
    com.Fangxiang=0x04;
    com.Dongzuo=0x04;
    com.angel=90;
    com.test=true;



    if((com.start==0)||(com.end==0))
    {
        return;
     }
    else
    {
        SendCommand->insertPlainText(QString("[%1] agv=%2,start=%3,end=%4,length=%5,angel=%6 左拐").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agv->get_id()).arg(com.start).arg(com.end).arg(com.length).arg(com.angel));
        agv->set_targetCommand(com,com1);
    }

    QianJinZhenchang->setEnabled(false);
    YouGuai->setEnabled(false);
    ZuoGuai->setEnabled(false);
    HouTuiZhenchang->setEnabled(false);
    HouTuiJiansu->setEnabled(false);
    QianJinJiansu->setEnabled(false);
   // Tingche->setEnabled(false);
    Chushihua->setEnabled(false);

}

void MissionManager::action_Youguai()
{

    AGVCommand com,com1;
    AGV*agv;
    if(m_agvId1.contains(TestAGV->currentData().toInt()))
         agv=m_agvId1[TestAGV->currentData().toInt()];
    else
        return;

    com.start=agv->get_topoPosID();
    com.end=agv->get_topoPosID();
    com.Fangxiang=0x08;
    com.Dongzuo=0x04;
    com.angel=90;
    com.test=true;

    if((com.start==0)||(com.end==0))
    {
        return;
     }
    else
    {
        SendCommand->append(QString("[%1] agv=%2,start=%3,end=%4,length=%5,angle=%6 右拐").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agv->get_id()).arg(com.start).arg(com.end).arg(com.length).arg(com.angel));
        agv->set_targetCommand(com,com1);
    }

    QianJinZhenchang->setEnabled(false);
    YouGuai->setEnabled(false);
    ZuoGuai->setEnabled(false);
    HouTuiZhenchang->setEnabled(false);
    HouTuiJiansu->setEnabled(false);
    QianJinJiansu->setEnabled(false);
    //Tingche->setEnabled(false);
    Chushihua->setEnabled(false);


}

void MissionManager::action_Tingchedengdai()
{

    AGVCommand com,com1;
    AGV*agv;
    if(m_agvId1.contains(TestAGV->currentData().toInt()))
         agv=m_agvId1[TestAGV->currentData().toInt()];
    else
        return;

    com.start=agv->get_topoPosID();
    com.end=agv->get_topoPosID();
    com.Fangxiang=0x20;
    com.Dongzuo=0x07;
    com.angel=0;
    com.test=true;

    QianJinZhenchang->setEnabled(false);
    YouGuai->setEnabled(false);
    ZuoGuai->setEnabled(false);
    HouTuiZhenchang->setEnabled(false);
    HouTuiJiansu->setEnabled(false);
    QianJinJiansu->setEnabled(false);
   // Tingche->setEnabled(false);
    Chushihua->setEnabled(false);

    SendCommand->append(QString("[%1] agv=%2,start=%3,end=%4,length=%5,angel=%6 停车等待").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agv->get_id()).arg(com.start).arg(com.end).arg(com.length).arg(com.angel));

    agv->set_targetCommand(com,com1);

}

void MissionManager::action_recvCommand(AGVCommand com,int Dongzuo,int Fangxiang,int agvid)
{
    static bool Chushihua=false;
    QString dongzuo;
    QString fangxiang;
    if((Dongzuo==0x01)&&(Fangxiang==0x01))
    {
        dongzuo="正常";
        fangxiang="前进";
    }
    else if((Dongzuo==0x01)&&(Fangxiang==0x02))
    {
        dongzuo="正常";
        fangxiang="后退";

    }
    else if((Dongzuo==0x02)&&(Fangxiang==0x01))
    {
        dongzuo="减速";
        fangxiang="前进";

    }
    else if((Dongzuo==0x02)&&(Fangxiang==0x02))
    {
        dongzuo="减速";
        fangxiang="后退";

    }
    else if((Dongzuo==0x04)&&(Fangxiang==0x04))
    {
        dongzuo="旋转";
        fangxiang="左拐";

    }
    else if((Dongzuo==0x04)&&(Fangxiang==0x08))
    {
        dongzuo="旋转";
        fangxiang="右拐";

    }
    else if((Dongzuo==0x07)&&(Fangxiang==0x20))
    {
        dongzuo="等待";
        fangxiang="停止";

    }
    else if((Dongzuo==0x55)&&(Fangxiang==0x55))
    {
       RecvCommand->append(QString("[%1] agv=%2,初始化完成,初始节点=%3，初始位姿=%4").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agvid).arg(com.start).arg(com.angel));
       //Chushihua->setEnabled(false);//这里考虑多台车辆的需求。
      if(Chushihua==false)
      {
            QianJinZhenchang->setEnabled(true);
            YouGuai->setEnabled(true);
            ZuoGuai->setEnabled(true);
            HouTuiZhenchang->setEnabled(true);
            HouTuiJiansu->setEnabled(true);
            QianJinJiansu->setEnabled(true);
            Tingche->setEnabled(true);
            Chushihua=true;
       }

       return;
    }

    RecvCommand->append(QString("[%1] agv=%2,start=%3,end=%4,length=%5,angle=%6,Direction=%7,Action=%8").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(agvid).arg(com.start).arg(com.end).arg(com.length).arg(com.angel).arg(fangxiang).arg(dongzuo));


    QianJinZhenchang->setEnabled(true);
    YouGuai->setEnabled(true);
    ZuoGuai->setEnabled(true);
    HouTuiZhenchang->setEnabled(true);
    HouTuiJiansu->setEnabled(true);
    QianJinJiansu->setEnabled(true);
    Tingche->setEnabled(true);


    //此处还有所不足
}

void MissionManager::action_nodeDistance(int index)
{
    AGV*agv;

    if(m_agvId1.contains(TestAGV->currentData().toInt()))
         agv=m_agvId1[TestAGV->currentData().toInt()];
    else
        return;

    int current_node=agv->get_topoPosID();
    TopoVer*current=m_topoView->m_verList[QString("Ver_%1").arg(current_node)];
    int target_node=Targetnode->currentData().toInt();
    TopoVer*target=m_topoView->m_verList[QString("Ver_%1").arg(target_node)];
    double distance=sqrt((current->realx - target->realx)*(current->realx - target->realx) + (current->realy - target->realy)*(current->realy - target->realy));
            //TwoPtDistance(QPointF(current->realx,current->realy), QPointF(target->realx,target->realy));
    nodeDistance->setText(QString("%1").arg(distance));
}


void MissionManager::timerEvent(QTimerEvent*event)
{

}

void MissionManager::new_request(QPair<int,int> info, int priority)
{
    Mission mission;
    mission.m_nStartNodeID = info.first;
    mission.m_nEndNodeID = info.second;
    mission.m_nPriority = priority;
    addMission(mission);
}

void MissionManager::add_store(QString st1, QString st2)
{
    m_comboStart->addItem(QString("Ver_%1_%2").arg(st1).arg(st2));
    m_comboStart->addItem(QString("Ver_%1_%2").arg(st1).arg(st2));
}

void MissionManager::remove_store(QString st)
{
    for(int i = 0; i < m_comboStart->count(); ++i)
    {
        m_comboStart->setCurrentIndex(i);
        if(st.toInt() == m_comboStart->currentText().toInt())
        {
            m_comboStart->removeItem(i);
            break;
        }
    }
}

// *-*-*-*-*-*-*-*-*-*-*-* 提供给WMS的接口 开始 *-*-*-*-*-*-*-*-*-*-*-*-
void MissionManager::new_request(quint64 id,QPair<int, int> startInfo, QPair<int, int> endInfo, int priority)
{
    m_query.exec(QString("select * from tb_mission where MissionID=%1").arg(id));
    while(m_query.next()){
        QSqlRecord record = m_query.record();
        if(record.value("MissionID").toULongLong()==id)
            return;
    }
    Mission mission;
    mission.m_nID = id;
    mission.m_nStartNodeID = startInfo.first;
    mission.m_nStartNodeLayer = startInfo.second;
    mission.m_nEndNodeID = endInfo.first;
    mission.m_nEndNodeLayer = endInfo.second;
    mission.m_nPriority = priority;
    addMission(mission);
}

void MissionManager::inquiry_state(quint64 ID)
{
    m_query.exec(QString("select * from tb_mission where MissionID=%1").arg(ID));
    int state = 2; // 默认异常状态
    if(m_query.next()){
        QSqlRecord record = m_query.record();
        QString str_state = record.value("state").toString();
        if(str_state==MISSION_STATE_PICK_UP)
            state = STATE_FETCH;
        else if(str_state==MISSION_STATE_DELIVERY)
            state =STATE_DELIVERY;
        else if(str_state==MISSION_STATE_WAIT)
            state = STATE_WAIT;
        else if(str_state==MISSION_STATE_ERROR)
            state = STATE_ERROR;
    }
    m_wms->response_state(ID,state);
}

void MissionManager::stop_request(quint64 ID)
{
    m_sqlTableModel->setFilter(QString("MissionID=%1").arg(ID));
    m_sqlTableModel->select();
    QString string = m_sqlTableModel->record(0).value("state").toString();
    if(string==MISSION_STATE_WAIT || string==MISSION_STATE_FINISH){
        m_sqlTableModel->removeRow(0);
        m_sqlTableModel->submitAll();
        m_wms->response_delete(ID,true);
    }
    else{
        m_wms->response_delete(ID,false);
    }
    showAllRecord();
}

void MissionManager::report_error(quint64 ID, char error_code)
{
    setMissionState(ID,MISSION_STATE_ERROR);
    m_wms->report_error(ID,error_code);
}

void MissionManager::mission_finished(quint64 ID)
{
    m_sqlTableModel->setFilter(QString("MissionID=%1").arg(ID));
    m_sqlTableModel->select();
    m_sqlTableModel->removeRow(0);
    m_sqlTableModel->submitAll();
    showAllRecord();
}
// *-*-*-*-*-*-*-*-*-*- 提供给WMS的接口 结束 -*-*-*-*-*-*-*-*-*-*-*-*-*
void MissionManager::readgoods()
{
    int id;
    int storage;
    QSqlQuery query = getQuery();
   if(!query.exec("select * from tb_storage")) return;
   while(query.next()){
       QSqlRecord record = query.record();

        id=record.value("NodeID").toInt();
        qDebug()<<"成功读取货量:"<<id<<endl;
        storage=record.value("storage").toInt();
        idhuoliang[id]=storage;


       }
}



//-------在此版本中已经删除--------------------------------------

//----------------------------------------------------
void MissionManager::Delay_MSec_Suspend(unsigned int msec)//阻塞延时
{

    QTime _Timer = QTime::currentTime();

    QTime _NowTimer;
    do{
              _NowTimer=QTime::currentTime();
    }while (_Timer.msecsTo(_NowTimer)<=msec);

}

void MissionManager::Delay_MSec(unsigned int msec)//非阻塞延时
{
  /*
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )

    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
*/
   QEventLoop loop;
   QTimer::singleShot(msec,&loop,SLOT(quit()));
   loop.exec();

}
