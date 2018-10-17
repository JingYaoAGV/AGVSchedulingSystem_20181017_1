#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlQuery>
#include "trafficmanager.h"
#include "missionmanager.h"
#include "agv.h"
#include "topoview.h"
#include "global.h"
#include <QSqlError>
#include <QSqlDriver>
#include <QDateTime>
//#include "hashtable.h"
#define crossLength 2200


TrafficManager::TrafficManager(MissionManager *missionManager, QWidget *parent):
    QWidget(parent)
{
    panduan=false;
    m_missionManager = missionManager;
    setWindowTitle("AGVFactory-交通管理器");
    setMaximumWidth(1300);
    setMinimumWidth(1300);
    m_bAutoLiftFork = g_setting->value("/traffic_manager/Auto_Lift_Fork").toBool();
    g_setting->setValue("/traffic_manager/Auto_Lift_Fork",m_bAutoLiftFork);
    m_bAutoDropFork = g_setting->value("/traffic_manager/Auto_Drop_Fork").toBool();
    g_setting->setValue("/traffic_manager/Auto_Drop_Fork",m_bAutoDropFork);
    IsReachStart=false;
    IsReachEnd=false;
    CURRRENTAGVSYSTEM=g_setting->value("/multiSystem/system").toInt();
    multiUsed=g_setting->value("/multiSystem/multi").toBool();
    shilian=false;
    qDebug()<<"CURRRENTAGVSYSTEM"<<CURRRENTAGVSYSTEM;
    if(__DEBUG__){
        m_file.setFileName(QString("log_traffic_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
        if(!m_file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::warning(0,"错误","打开日志文件错误");
        }
         __log__(&m_file,QString("[init] system: %1 Online").arg(CURRRENTAGVSYSTEM));
    }


}

TrafficManager::~TrafficManager()
{
}
void TrafficManager::updateGlobalPlanning()
{
    static Mission old_mission;

    AGV* agv = (AGV*)sender();
    if(!agv) return;

    int current_node = agv->get_topoPosID();
    int target_node = current_node;

    //if(agv->agvchexing==5)
    //{
    //guidaoglobalplanning(agv,old_mission);
    //return;
    //}


    QString agv_name = agv->get_name();
    // 可在任一点或者下一点停
    int  agvid  = agv->get_id();

    qDebug()<<"agv:"<<agv->get_id()<<"执行任务当前点："<<current_node<<"起始点"<<agv->get_chushidian()<<endl;
    qDebug()<<"判断位"<<agv->missionflag1<<endl;


     /*3.28改
    *  if((current_node==(agv->get_chushidian()))&&( agv->missionflag1==true))
    {
             agv->missionflag1=false;
             m_missionManager->setMissionState(old_mission,MISSION_STATE_FINISH,agv);


             qDebug()<<"任务ID"<<old_mission.m_nID<<endl;
             qDebug()<<"计算结束时间"<<endl;
    }
    */

    if(m_pausePoints.contains(agvid))
    {
        QList<int> points=m_pausePoints.value(agvid);
        int point=points.first();
        if ((point==current_node)||(point==0))
        {
            __log__(&m_file,QString("agvid: %1 pause").arg(agvid));
            return;
        }

    }
    Mission mission;
    mission.m_nStartNodeID = -1;
    mission.m_nEndNodeID = -1;


    if(!m_mapAGVandMission.contains(agv_name)){  // 当前agv无任务




         qDebug()<<"AGV"<<agvid<<"无任务"<<endl;

        if(m_missionManager->getNextMission(agv_name,mission,agv)){  // 存在新任务

            m_mapPath.remove(agv_name);//删除掉之前的路径，重新规划新路径
            agv_command.remove(agv_name);//删除掉之前的指令，重新规划新的指令
            qDebug()<<"AGV"<<agvid<<"有新任务"<<endl;
            m_mapAGVandMission[agv_name]=St_Mission(mission,agv->get_startNode()==mission.m_nStartNodeID, mission.m_nStartNodeID==mission.m_nEndNodeID);
           target_node = mission.m_nStartNodeID; //绕回来  避免撞车  试验场地特加
           agv->stateRecord(mission.m_bReachStartNode,mission.m_bReachEndNode);



           if(agv->get_Commandnumber()!=0)//领取了新任务之后，之前还有指令未完成,应该先把之前的任务完成后，再继续
           {
               return;
           }

        }
        else{
           target_node = agv->get_restNode();
           emit AGVstateChange(agv,false);

        }
    }
    else{  // 当前agv有正在执行的任务


        St_Mission st_mission = m_mapAGVandMission.value(agv_name);
        mission = st_mission.m_mission;
        if(mission.m_bReachStartNode==false){ //如果还没有到达起始点
          if(current_node!=mission.m_nStartNodeID){    //当前点不是任务起点
                target_node = mission.m_nStartNodeID;
            }
          else{ //当前点是任务起点
              if(!agv_command.contains(agv_name))
              {
               m_mapPath.remove(agv_name);//取货结束了，清空一段路径
                target_node=mission.m_nEndNodeID;  //没效果
                mission.m_bReachStartNode = true;

              //   m_missionManager->setMissionState(mission,MISSION_STATE_DAOKU);
                m_mapAGVandMission[agv_name] = St_Mission(mission,true,mission.m_nStartNodeID==mission.m_nEndNodeID);// 更新任务标记
                    // 如果当前点为任务起点，则要检测叉子是否在起点货架层高顶端
                agv->set_startNode(mission.m_nStartNodeID);
               }
              }
            agv->stateRecord(mission.m_bReachStartNode,mission.m_bReachEndNode);

        }
        else {  // 已经路过起点
               qDebug()<<"已经路过起点"<<endl;
               qDebug()<<"已经路过起点"<<endl;
               qDebug()<<"已经路过起点"<<endl;
            m_missionManager->setMissionState(mission,MISSION_STATE_DELIVERY);
            if(current_node!=mission.m_nEndNodeID){//当前点不是任务终点
                if(mission.m_bReachEndNode==false)
                target_node = mission.m_nEndNodeID;
                else
                 target_node = agv->get_restNode();

               }

            else{ // 当前点是目标终点
                // 如果当前点为任务终点，则要检测叉子是否在终点货架层高底端
                if(!agv_command.contains(agv_name))//当前点是终点，且动作都执行完毕

                {
                    m_mapPath.remove(agv_name);//送货结束了，清空一段路径
                agv->set_startNode(mission.m_nEndNodeID);
                target_node = agv->get_restNode();
                 mission.m_bReachEndNode=true;
                 m_mapAGVandMission[agv_name] = St_Mission(mission,true,mission.m_nStartNodeID==mission.m_nEndNodeID);// 更新任务标记


                 m_missionManager->setMissionState(mission,MISSION_STATE_BACK);//表示现在agv正在返回原先的点
                // agv->missionflag1=true;//完成了取货送货的任务

                  old_mission= mission;

                 emit MissionStateChanged(mission.m_nStartNodeID, mission.m_nEndNodeID, mission.m_nStartNodeLayer, mission.m_nEndNodeLayer);
                 m_mapAGVandMission.remove(agv_name);

                 emit AGVstateChange(agv,false);//标明此时AGV开始接其他任务

                 }
            }
            agv->stateRecord(mission.m_bReachStartNode,mission.m_bReachEndNode);
        }
    }

    //-------------------------------------




  if(target_node==current_node)//产生一个走过的路径的区域，考虑是否需要把这段代码整合在一起6.22
  {

    if(!m_oldPath.contains(agv_name))
   {
      QList<int>path;
      path.append(current_node);
      m_oldPath[agv_name]=path;

    }


    if(target_node==agv->get_restNode())
      {
          agv->stateRecord(false,true);
      }
      //return;
  }


  if(!m_oldPath.contains(agv_name))//3.30 生成旧路径MAP 考虑过滤掉暂停时的点。
      {
          QList<int>path1;
          path1.append(current_node);
          m_oldPath[agv_name]=path1;
          qDebug()<<"AGV:"<<agvid<<"走过的路径"<<path1<<endl;
      }
  else
    {
        QList<int> path1=m_oldPath[agv_name];
        if(path1.last()!=current_node)
        {
        path1.append(current_node);
        }
            m_oldPath[agv_name]=path1;
            qDebug()<<"AGV:"<<agvid<<"走过的路径"<<path1<<endl;
     }


  //-------------------------------------


/******** Dijkstra 搜索路径 **********/

/*
if(!m_oldPath.contains(agv_name))//3.30 生成旧路径MAP 考虑过滤掉暂停时的点。
    {
        QList<int>path1;
        path1.append(current_node);
        m_oldPath[agv_name]=path1;
    }
else
    {
    QList<int> path1=m_oldPath[agv_name];
    path1.append(current_node);
    m_oldPath[agv_name]=path1;

       }
       */

//-----只进行一次规划-------------

  QList<int> path;
  QList<AGVCommand>Command;
if(!m_mapPath.contains(agv_name))
{

    double cost;
    dijkstra_search(agv,current_node,target_node,path,cost);
    if(path.isEmpty()){
        agv->set_targetID(current_node,current_node);
        //shengchengquyu(agv,m_oldPath[agv_name],path);//3.31 检验区域生成的是否有问题
    }

  else{
        path.removeFirst();
        //shengchengquyu(agv,m_oldPath[agv_name],path);//3.31 检验区域生成的是否有问题
        m_mapPath[agv_name] = path;
        mapPath_change[agv_name]=path;
        CommandProduce(current_node,path,Command);
        agv_command[agv_name]=Command;


    }

}
else
{

    path=m_mapPath[agv_name];
    Command=agv_command[agv_name];
}
//---------------------------------------------

    if(!mapPath_change[agv_name].isEmpty())
        {
         if(mapPath_change[agv_name].first()==current_node)//车辆执行完一段路径后,目标点targetnode 指向下个点，以此来判断此时车辆是否等待
             {
                  mapPath_change[agv_name].removeFirst();
                 if(mapPath_change[agv_name].count()>0)
                 agv->set_targetID(mapPath_change[agv_name].first(),mapPath_change[agv_name].last());

             }
            qDebug()<<"车辆静态规划路径为"<<mapPath_change[agv_name]<<endl;


         }

     shengchengquyu(agv,m_oldPath[agv_name],mapPath_change[agv_name]);//生成一个锁定点的集合



//---------------------------------------------------------------------
 for(int i=0;i<Command.count();i++)
 {

     qDebug()<<"车辆指令集合"<<Command.at(i).start<<"--"<<Command.at(i).end<<endl;

}


//-----------------------------------------------------------------------------
    // shengchengquyu(agv,m_oldPath[agv_name],path);//3.31 检验区域生成的是否有问题
    bool flag_collosion=false;
    if(multiUsed==true)
{
  //TopoEdge *ed=m_topoView->get_edgeByVer(current_node,path.first());
 m_queryServer=getQueryServer();
 if(shilian==false)
 {
     m_queryServer.exec("select * from agv_position");
     if(m_queryServer.isActive()==true)
     {
       m_queryServer.prepare("update agv_position set current_node=?,target_node=? where id=?");
       m_queryServer.addBindValue(current_node);
       m_queryServer.addBindValue(path.first());
       m_queryServer.addBindValue(agv->get_id());
       m_queryServer.exec();
     }
     else
     {
             agv->set_targetID(current_node,current_node);
             shilian=true;
             QMessageBox msgBox;
             msgBox.setText(tr("unconnect to database."));
             msgBox.setInformativeText(tr("Do you want to retry connect?"));
             msgBox.setStandardButtons(QMessageBox::Retry
                                       | QMessageBox::Cancel);
             int ret = msgBox.exec();
             bool l;
             switch (ret) {
             case QMessageBox::Retry:
                l= openSQLServer();
                if(l==false)
                {
                    agv->set_targetID(current_node,current_node);
                    shilian=false;
                    return;
                }
                else
                {
                    shilian=false;
                }
                 break;
             case QMessageBox::Cancel:
                 break;
             }
     }
 }
 else if(shilian==true)
 {
     qDebug()<<"shilian";
     agv->set_targetID(current_node,current_node);
     return;
 }

}
    // qDebug()<<"flag_collosion"<<flag_collosion;

    if((multiUsed==true))
{
 flag_collosion=willCollsionmultiNew(agv,current_node,path.first());
}
    else{
       //flag_collosion=willCollsionNew(agv,current_node,path.first());
       flag_collosion=willCollsionchange(agv);
       flag_collosion=false;
        }

 //----------------------------------------------------------------
 if(flag_collosion==true)
 {

        qDebug()<<"AGV:"<<agv->get_id()<<"产生节点冲突"<<"current node:"<<current_node<<endl;
       agv->set_targetID(current_node,current_node);  // 若路径中下一个节点被其它AGV占用，则停留在该节点处
       if(multiUsed==true)
       {
           m_queryServer=getQueryServer();
           m_queryServer.prepare("update agv_position set target_node=? where id=?") ;
           m_queryServer.addBindValue(current_node);
           m_queryServer.addBindValue(agv->get_id());
           m_queryServer.exec();
       }

 }
     else{


     if(path.length()>1){
         m_topoView->set_register(QString("Ver_%1").arg(path.first()),agv->get_name(),agv->get_count());
     }
     bool can_multi_mode = false;

     if(!can_multi_mode){  // 不能多步运行
          //qDebug()<<"path"<<path<<endl;


         if(agv->get_Commandnumber()>=2)
             return;
         if(Command.isEmpty())//说明无可行路径或者当前点是目标点
          {
             agv_command.remove(agv_name);
             qDebug()<<"指令为空"<<endl;


            // AGVCommand com={current_node,current_node,0,false,0};//表示无可行路径或路径已经运行完成
             //agv->set_targetCommand(com,com);
            // agv->set_targetID(current_node,current_node);


          }

         else
          {



              AGVCommand com1={0,0,0,false,0};
              AGVCommand com2={0,0,0,false,0};

             //----------双指令发送------------------
             if(Command.size()>=2)
               {
                //agv_command.remove(agv_name);

                int i=willCollsion_Region(com1,com2,Command.at(0),Command.at(1),agv_name,agv);
                  if(i==0)
                   {
                        Command.removeFirst();
                        Command.removeFirst();
                   }
                  else if(i==1)
                    {
                         Command.removeFirst();
                     }

               // com1=Command.at(0);
                //com2=Command.at(1);
                //agv->set_targetCommand(Command.at(0),Command.at(1));
                //Command.removeFirst();
                //Command.removeFirst();
                   if(Command.isEmpty())//两条指令正好发完
                     agv_command.remove(agv_name);

                }
             else if(Command.size()<2&&Command.size()>=1)
                {



                  AGVCommand com={Command.at(0).end,Command.at(0).end,0,false,0};

                 // qDebug()<<"updateGlobalPlanning"<<Command.at(0).start<<"--"<<Command.at(0).end<<endl;
                 //com1=Command.at(0);
                 //com2=com;
                 //agv->set_targetCommand(Command.at(0),com);
                 int i=willCollsion_Region(com1,com2,Command.at(0),com,agv_name,agv);

                   if(i>=1)
                    {
                         Command.removeAt(0);
                        agv_command[agv_name]=Command;
                    }
                }
             else
                {


                 agv_command.remove(agv_name);   //表示当前路径已经发完6.25,此处可以在实际车体中，设置车辆等待
                 AGVCommand com=agv->get_CommandInfo(1);
                 com1.start=com.start;
                 com1.end=com1.start;
                 com2=com1;
                }



             if((com1.start!=0)&&(com2.start!=0))
                agv->set_targetCommand(com1,com2);
           //------------------------------------------------


            // agv->set_targetID(path.first(),path.last());//此为agv下一段指令的运行基本函数

            // path.removeFirst();

             m_mapPath[agv_name]=path;//将路径和指令回存到Mapz中
             agv_command[agv_name]=Command;

             //qDebug()<<"车辆静态规划路径为"<<path;
             agv->set_mode(AGV::SINGLE_STEP_MODE); //如果AGV不包含固定路由表
         }
     }
 }

//----------------------------------------------------------------------------



}


void TrafficManager::someInforDelate(int agvid)
{
if(m_agvidname.contains(agvid))
{
    QString name=m_agvidname.value(agvid);
    AGV *agv=m_idAGV.value(agvid);
    if(m_mapAGVandMission.contains(name))
    {
      m_mapAGVandMission.remove(name);

    }
    if(m_mapPath.keys().contains(name))
    {
        m_mapPath.remove(name);
    }
    if(m_agvList.contains(agv))
    {
        m_agvList.removeOne(agv);
    }
    if(m_idAGV.contains(agvid))
    {
        m_idAGV.remove(agvid);
    }
    if(m_pausePoints.contains(agvid))
    {
        m_pausePoints.remove(agvid);
    }

}
}
void TrafficManager::multisystemused()
{
 multiUsed=true;
 qDebug()<<"mmmm"<<multiUsed;
}
void TrafficManager::pauseAgv(int agvid,int pointId)
{
if(m_pausePoints.keys().contains(agvid))
{
  QList<int>pausePoints=m_pausePoints.value(agvid);
  if(!pausePoints.contains(pointId))
  {

      pausePoints.append(pointId);
      m_pausePoints[agvid]=pausePoints;
  }
  qDebug()<<"agvid"<<agvid<<"point"<<pointId;
}
else
{
   QList<int>pausePoints;
   pausePoints.append(pointId);
   m_pausePoints[agvid]=pausePoints;
   qDebug()<<"agvid2"<<agvid<<"point"<<pointId;
}

}
void TrafficManager::keepOn(int agvid)
{
 if(m_pausePoints.keys().contains(agvid))
 {
     QList<int>pausePoints=m_pausePoints.value(agvid);
     if(!pausePoints.isEmpty())
     {
       pausePoints.removeFirst();
       if (pausePoints.isEmpty())
           m_pausePoints.remove(agvid);
       else
           m_pausePoints[agvid]=pausePoints;
     }
     qDebug()<<pausePoints;
     qDebug()<<"agvid3"<<agvid;
 }
}
void TrafficManager::report_error(QString agvname,char error_code)
{
quint64 id = 0;
if(m_mapAGVandMission.count(agvname)){
    id = m_mapAGVandMission[agvname].m_mission.m_nID;
    m_missionManager->report_error(id,error_code);
}

}

void TrafficManager::mission_abort(quint64 id)
{
foreach(QString agvname,m_mapAGVandMission.keys()){
    St_Mission st_mission = m_mapAGVandMission.value(agvname);
    if(st_mission.m_mission.m_nID==id){
        m_mapAGVandMission.remove(agvname);
        break;
    }
}
}

bool TrafficManager::addAGV(AGV *agv)
{
 if(!m_agvList.contains(agv))
m_agvList.append(agv);
m_agvidname.insert(agv->get_id(),agv->get_name());
m_idAGV.insert(agv->get_id(),agv);
qDebug()<<"multiUsed"<<multiUsed;
if(multiUsed==true)
{
   int count=0;
   m_queryServer=getQueryServer();
   m_queryServer.exec(QString("select *from agv_position where id=%1").arg(agv->get_id()));
   while(m_queryServer.next())
   {
       count++;
   }
   m_queryServer.exec(QString("select *from agv_position where system=%1").arg(CURRRENTAGVSYSTEM));
   QList<int>ids;
   while(m_queryServer.next())
   {
      QSqlRecord record = m_queryServer.record();
     int id= record.value("id").toInt();
      if(!m_agvidname.keys().contains(id))
      {
          ids.append(id);
      }
   }
   for(int i=0;i<ids.length();i++)
   {
       m_queryServer.prepare("delete from tb_storage WHERE id = ? ");
       m_queryServer.addBindValue(ids.at(i));
       m_queryServer.exec();
   }
   if(count==0)
   {
       m_queryServer.prepare("INSERT INTO agv_position (id,name,system,current_node,target_node,usepoint) "
                              "VALUES (:id,:name,:system,:current_node,:target_node,:usepoint)");
        m_queryServer.bindValue(":id", agv->get_id());
        m_queryServer.bindValue(":name", agv->get_name());
        m_queryServer.bindValue(":system", CURRRENTAGVSYSTEM);
        m_queryServer.bindValue(":current_node", 0);
        m_queryServer.bindValue(":target_node", 0);
        m_queryServer.bindValue(":usepoint",0);
        m_queryServer.exec();
   }
   else
   {
       m_queryServer.prepare("update agv_position set usepoint=? where id=?") ;
       m_queryServer.addBindValue(0);
       m_queryServer.addBindValue(agv->get_id());
       m_queryServer.exec();
   }

}

connect(agv,SIGNAL(report_error(QString,char)),this,SLOT(report_error(QString,char)));
return true;
}

void TrafficManager::removeAGV(AGV *agv)
{
foreach(AGV *a,m_agvList){
    if(!a || ((agv) && a->get_id()==agv->get_id()))  // 如果列表中指针为空或者参数不为空且参数ID等于列表中指针ID
        m_agvList.removeOne(a);
}
}
void TrafficManager::loadTopoView(TopoView *topoview)
{
m_topoView=topoview;
m_verIdTunnel=topoview->m_verIdTunnel;
// m_commonRoute= m_topoView->get_commonRoute();

}
bool TrafficManager::dijkstra_search(AGV* /*agv*/,int startID, int endID, QList<int> &path,double &cost)
{
came_from.clear();
cost_so_far.clear();
path.clear();
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

void TrafficManager::show_path(AGV* agv)
{
m_topoView->draw_path(agv->get_penColor(),m_mapPath.value(agv->get_name()));
}

void TrafficManager::recontruct_path(int startID,int endID,QList<int> &path)
{
int current = endID;
path.push_front(current);
while(current!=startID){
    current = came_from[current];
    path.push_front(current);
}
}

bool TrafficManager::contain_paths(int startID, int endID)
{
for(int i=0;i<m_listPaths.length();i++){
    QPair<int,int> pair = m_listPaths.at(i);
    if(pair.first==startID && pair.second==endID)
        return true;
}
return false;
}

void TrafficManager::loadDatabase()
{
QSqlQuery query = getQuery();
query.exec("create table if not exists tb_solidroute ( startNode int, endNode int )");
query.exec("select * from tb_solidroute");
m_listPaths.clear();
while(query.next()){
    QSqlRecord record = query.record();
    m_listPaths.append(QPair<int,int>(record.value("startNode").toInt(),record.value("endNode").toInt()));
}
}
void TrafficManager::traffic_control_type1()//两车并行，拐弯会相撞  0216
{

}
void TrafficManager::swap_pri(int *a, int *b)
{
int temp = *a;
    *a = *b;
    *b = temp;
}
void TrafficManager::register_normal(AGV* agv,int current, int target)
{
 QString name=m_topoView->m_verList.value(QString("Ver_%1").arg(target))->get_register();
 {
     if (name=="")
     {
         //扫过的点
         QString m_strID=agv->get_name();
         TopoEdge *edge=m_topoView->m_verList.value(QString("Ver_%1").arg(current))->m_pEdgeHead;
         while(target!=edge->m_nEndID)
         {
             edge=edge->m_pNextEdge;
         }
         int edge_id=edge->get_id();
         QList<int>pointsInArea=m_TrafficVerInArea.value(edge_id);
         //QList<int>pointsTarget= aroundTargetPoints.value(target);
         QList<int>::const_iterator i;
         for(i=pointsInArea.constBegin();i!=pointsInArea.constEnd();i++)
         {
            QString agv_name=m_topoView->m_verList.value(QString("Ver_%1").arg(*i))->get_register();
            if(agv_name=="")
            {
            //   m_topoView->set_register(QString("Ver_%1").arg(*i),m_strID,,agv->get_count());
            }
         }
     }
 }

}
void TrafficManager::register_InTunnel(AGV* agv,int current, int target)
{

QString m_strID=agv->get_name();
//int tunnelTarget=IsTargetInTunnel(target);
  int tunnelTarget=m_verIdTunnel.value(target);
// int currentTarget=IsTargetInTunnel(current);
  int currentTarget=m_verIdTunnel.value(current);
if((tunnelTarget!=0)&&(currentTarget==0))
{
    QList<int> TunnelPoints=pointsInTunnel(tunnelTarget);
    QList<int>::const_iterator i;
    for(i=TunnelPoints.constBegin();i!=TunnelPoints.constEnd();i++)
    {
        QString agv_name=m_topoView->m_verList.value(QString("Ver_%1").arg(*i))->get_register();
        if(agv_name=="")
        {
          // m_topoView->set_register(QString("Ver_%1").arg(*i),m_strID,,agv->get_count());
        }
    }
}


}
void TrafficManager::register_InCrossroads(AGV* agv,int current, int target)
{
//扫过的点
QString m_strID=agv->get_name();
TopoEdge *edge=m_topoView->m_verList.value(QString("Ver_%1").arg(current))->m_pEdgeHead;
while(target!=edge->m_nEndID)
{
    edge=edge->m_pNextEdge;
}
int edge_id=edge->get_id();
QList<int>pointsInArea=m_TrafficVerInArea.value(edge_id);
//新占用交叉的点
 for(int i=0;i<pointsInArea.length();i++)
 {
     for(int j=0;j<pointsInArea.length();j++)
     {
         if(i==j)
         {}
         else
         {
             bool exitFlag=m_topoView->is_edgeExist(QString("Ver_%1").arg(pointsInArea.at(i)),QString("Ver_%1").arg(pointsInArea.at(j)));

             if(exitFlag==true)
             {
                 TopoEdge *edge5=m_topoView->get_edgeByVer(pointsInArea.at(i),pointsInArea.at(j));
                 int id=edge5->get_id();
                 bool currentInCollion=!m_collionSegments.value(id).isEmpty();//把变量调成单一的

                 if(currentInCollion==true)
                 {
                     QList<int>Collions= m_collionSegments.value(id);
                     QList<int>::const_iterator j;
                     for(j=Collions.constBegin();j!=Collions.constEnd();j++)
                     {
                        QString name1 = QString("Edge_%1").arg(*j);
                        TopoEdge *edgex=m_topoView->m_edgeList.value(name1,NULL);
                        int startx=edgex->m_nStartID;
                       // int endx=edgex->m_nEndID;
                        QString agv_name=m_topoView->m_verList.value(QString("Ver_%1").arg(startx))->get_register();
                        if(agv_name=="")
                        {
                         // m_topoView->set_register(QString("Ver_%1").arg(startx),m_strID,,agv->get_count());
                        }
                        QString agv_name2=m_topoView->m_verList.value(QString("Ver_%1").arg(startx))->get_register();
                        if(agv_name2=="")
                        {
                         // m_topoView->set_register(QString("Ver_%1").arg(endx),m_strID,,agv->get_count());
                        }


                     }

                 }
             }
         }
     }
   // TopoEdge *edge5=m_topoView->m_verList.value(QString("Ver_%1").arg(pointsInArea.at(i)))->m_pEdgeHead;


 }





}
int TrafficManager::IsTargetInTunnel(quint32 target)
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
QList<int> TrafficManager::pointsInTunnel(int tunnel)
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
void TrafficManager::setcrossPoint(QMap<int, QList<int> > cross)
{
m_collionSegments=cross;
}
void TrafficManager::EquPoInArea(QMap<int, QList<QPointF> > point)
{
m_equPointInArea=point;
}
void TrafficManager::collsionEdge(QMap<int, QList<int> > collsionEdge)
{
m_collionEdge=collsionEdge;
}
void TrafficManager::collsionEdgeVer(QMap<int, QList<int> > collsionEdgeVer)
{
m_collionEdgeVer=collsionEdgeVer;
}
void TrafficManager::huicheEdges(QMap<int, QList<int> > huicheedges)
{
m_huicheEdges=huicheedges;
}
void TrafficManager::rukouEdges(QList<int> RuKouedges)
{
rukouIDS=RuKouedges;
}
void TrafficManager::commonPoints(QList<int> points)
{
m_commPoints=points;
}
void TrafficManager::commonEdges(QMap<int,QList<int>> commonEdges)
{
m_commEdges=commonEdges;
}
bool TrafficManager::collsion(AGV *agv, int current, int target)
{
TopoEdge *edge=m_topoView->m_verList.value(QString("Ver_%1").arg(current))->m_pEdgeHead;
while(target!=edge->m_nEndID)
{
    edge=edge->m_pNextEdge;
}
int edge_id=edge->get_id();

QList<int>pointsInArea=m_TrafficVerInArea.value(edge_id);

bool flag_collosion=false;
for(int i=0;i<pointsInArea.length();i++)
{
   QString agv_name=m_topoView->m_verList.value(QString("Ver_%1").arg(pointsInArea.at(i)))->get_register();
   if(agv_name!=agv->get_name()&&agv_name!="")
     flag_collosion=true;  // 若路径中下一个节点被其它AGV占用，则停留在该节点处

}
return  flag_collosion;

}

bool TrafficManager::willCollsion(AGV* agv, int current, int target)
{
int agvid=agv->get_id();
bool falg=false;

int edge_id=-1;
bool exitFlagg=m_topoView->is_edgeExist(QString("Ver_%1").arg(current),QString("Ver_%1").arg(target));
if(exitFlagg==true)
{
    TopoEdge *edge=m_topoView->get_edgeByVer(current,target);
    edge_id=edge->get_id();
}
QList<int> edges=m_collionEdge.value(edge_id);
QList<int> vers=m_collionEdgeVer.value(edge_id);
QList<int> huicheEdg=m_huicheEdges.value(edge_id);
foreach (AGV* agvx, m_agvList) {
   int agvxid=agvx->get_id();
   if(agvid==agvxid)
       continue;
   else
   {
       int AGVcurrent=agvx->get_topoPosID();
       int AGVtarget=agvx->get_targetID();
       int edgex_id=-1;
       if((AGVcurrent!=AGVtarget)&&(AGVcurrent!=-1))
       {
           bool exitFlag=m_topoView->is_edgeExist(QString("Ver_%1").arg(AGVcurrent),QString("Ver_%1").arg(AGVtarget));
           if(exitFlag==true)
           {
               if(vers.contains(AGVcurrent))
                  falg=true;
               TopoEdge *edgex=m_topoView->get_edgeByVer(AGVcurrent,AGVtarget);
               edgex_id=edgex->get_id();
           }


       }
       else if((AGVcurrent==AGVtarget)&&(AGVcurrent!=-1)&&(AGVcurrent!=0)&&(AGVcurrent!=agvx->get_restNode()))
       {
              if(vers.contains(AGVcurrent))
                 falg=true;
       }
       if(edges.contains(edgex_id))
       {
         falg=true;
       }
       if(huicheEdg.contains(edgex_id))
       {
           falg=true;
       }
   }
}

QString  agvname=agv->get_name();
St_Mission stmissionx=m_mapAGVandMission.value(agvname);
Mission missionx=stmissionx.m_mission;

if ((agv->get_topoPosID()==missionx.m_nStartNodeID)||(agv->get_topoPosID()==missionx.m_nEndNodeID))
{
    //int tunnel=IsTargetInTunnel(agv->get_topoPosID());
      int tunnel=m_verIdTunnel.value(agv->get_topoPosID());
    QMap<int,QPair<int,int>> agvtunnelpri=AGVTunnelPri;
    int pr=agvtunnelpri.value(agv->get_id()).second;



     foreach (int agviddx,agvtunnelpri.keys()) {
         if(agviddx!=agv->get_id())
         {
             if(!agvtunnelpri.isEmpty())
             {
                 if(agvtunnelpri.value(agviddx).first==tunnel)
                 {
                     int p= agvtunnelpri.value(agviddx).second;
                          if(p>pr)
                               falg=true;
                 }
             }

         }



     }
}

return falg;
}

bool TrafficManager::willCollsionmulti(AGV* agv, int current, int target)
{
int agvid=agv->get_id();
bool falg=false;

int edge_id=-1;
bool exitFlagg=m_topoView->is_edgeExist(QString("Ver_%1").arg(current),QString("Ver_%1").arg(target));
if(exitFlagg==true)
{
    TopoEdge *edge=m_topoView->get_edgeByVer(current,target);
    edge_id=edge->get_id();
}
QList<int> edges=m_collionEdge.value(edge_id);
QList<int> vers=m_collionEdgeVer.value(edge_id);
 QList<int> huicheEdg=m_huicheEdges.value(edge_id);

 int AGVcurrent=-1;
 int AGVtarget =-1;
 int inRuKou=-1;
 int inRuKoux=-1;
 int edgex_id=-1;
 int  gengxin=0;
 m_queryServer=getQueryServer();
 m_queryServer.exec(QString("select * from agv_position where system<>%1").arg(CURRRENTAGVSYSTEM));

 while(m_queryServer.next()){
     QSqlRecord record = m_queryServer.record();
      AGVcurrent =record.value("current_node").toInt();
      AGVtarget  =record.value("target_node").toInt();
      inRuKoux    =record.value("inRuKou").toInt();
      if((AGVcurrent!=AGVtarget)&&(AGVcurrent!=-1))
      {
          bool exitFlag=m_topoView->is_edgeExist(QString("Ver_%1").arg(AGVcurrent),QString("Ver_%1").arg(AGVtarget));
          if(exitFlag==true)
          {
              TopoEdge *edgex=m_topoView->get_edgeByVer(AGVcurrent,AGVtarget);
              edgex_id=edgex->get_id();
              if(edges.contains(edgex_id))
              {
                  __log__(&m_file,QString("agv:%1,reason:%2").arg(agvid).arg(1));
                falg=true;
                break;
              }
          }
          else
          {

              if(vers.contains(AGVtarget)||vers.contains(AGVcurrent))
             {
                   __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(2));
                falg=true;
                break;
             }
              if((inRuKoux==1)&&(rukouIDS.contains(edge_id)))
              {
                  gengxin=1;
                   __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(3));
                falg=true;
                break;
              }
          }
      }
      if(vers.contains(AGVtarget)||vers.contains(AGVcurrent))
     {
           __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(2));

        falg=true;
        break;
     }

 }
 if(gengxin==1)
 {
     m_queryServer.prepare("update agv_position set inRuKou=? where id=?") ;
     m_queryServer.addBindValue(0);
     m_queryServer.addBindValue(agv->get_id());
     m_queryServer.exec();
 }
 m_queryServer.exec(QString("select * from agv_position where id=%1").arg(agvid));
  while(m_queryServer.next()){
       QSqlRecord record = m_queryServer.record();
       inRuKou    =record.value("inRuKou").toInt();
       if((inRuKou==0)&&(rukouIDS.contains(edge_id)))
       {
           __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(9));
        falg=true;
       }

  }



 //本系统中的agv
 foreach (AGV* agvx, m_agvList) {
    int agvxid=agvx->get_id();
    if(agvid==agvxid)
        continue;
    else
    {
      AGVcurrent=agvx->get_topoPosID();
      AGVtarget=agvx->get_targetID();
      if((AGVcurrent!=AGVtarget)&&(AGVcurrent!=-1))
      {
          bool exitFlag=m_topoView->is_edgeExist(QString("Ver_%1").arg(AGVcurrent),QString("Ver_%1").arg(AGVtarget));
          if(exitFlag==true)
          {
              if(vers.contains(AGVcurrent))
              {
                   __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(5));
                  falg=true;
              }

              TopoEdge *edgex=m_topoView->get_edgeByVer(AGVcurrent,AGVtarget);
              edgex_id=edgex->get_id();
          }

      }
      else if((AGVcurrent==AGVtarget)&&(AGVcurrent!=-1)&&(AGVcurrent!=0)&&(AGVcurrent!=agvx->get_restNode()))
      {
             if(vers.contains(AGVcurrent))
             {
                 __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(6));
                falg=true;
             }
      }
      if(edges.contains(edgex_id))
      {
           __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(7));
        falg=true;
      }
      if(huicheEdg.contains(edgex_id))
      {
           __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(8));
          falg=true;
      }

    }

    //巷道中
    QString  agvname=agv->get_name();
    St_Mission stmissionx=m_mapAGVandMission.value(agvname);
    Mission missionx=stmissionx.m_mission;

    if ((agv->get_topoPosID()==missionx.m_nStartNodeID)||(agv->get_topoPosID()==missionx.m_nEndNodeID))
    {
        //int tunnel=IsTargetInTunnel(agv->get_topoPosID());
          int tunnel=m_verIdTunnel.value(agv->get_topoPosID());
        QMap<int,QPair<int,int>> agvtunnelpri=AGVTunnelPri;
        int pr=agvtunnelpri.value(agv->get_id()).second;
         foreach (int agviddx,agvtunnelpri.keys()) {
             if(agviddx!=agv->get_id())
             {
                 if(!agvtunnelpri.isEmpty())
                 {
                     if(agvtunnelpri.value(agviddx).first==tunnel)
                     {
                         int p= agvtunnelpri.value(agviddx).second;
                              if(p>pr)
                              {
                                   falg=true;
                                    __log__(&m_file,QString("agv: %1 ,reason:% 2").arg(agvid).arg(9));
                              }
                     }
                 }

             }
         }
    }
 }
 return falg;
}
bool TrafficManager::willCollsionNew(AGV *agv, int current, int target)
{
   QString agvName=agv->get_name();
   int     agvid=  agv->get_id();
   QList<int> path= m_mapPath.value(agvName);
   bool falg=false;

   int edge_id=-1;
   bool exitFlagg=m_topoView->is_edgeExist(QString("Ver_%1").arg(current),QString("Ver_%1").arg(target));
   if(exitFlagg==true)
   {
       TopoEdge *edge=m_topoView->get_edgeByVer(current,target);
       edge_id=edge->get_id();
   }
   QList<int> edges=m_collionEdge.value(edge_id);
   QList<int> vers=m_collionEdgeVer.value(edge_id);




   int AGVcurrent=-1;
   int AGVtarget =-1;
 foreach (AGV* agvx, m_agvList) {

     int agvxid=agvx->get_id();
     QString agvxName=agvx->get_name();
     if(agvid==agvxid)
         continue;
     else
     {
         AGVcurrent=agvx->get_topoPosID();
         AGVtarget=agvx->get_targetID();
          int edgex_id=-1;
         if((AGVcurrent!=AGVtarget)&&(AGVcurrent!=-1))
         {
             bool exitFlag=m_topoView->is_edgeExist(QString("Ver_%1").arg(AGVcurrent),QString("Ver_%1").arg(AGVtarget));
             if(exitFlag==true)
             {
                 if(vers.contains(AGVcurrent))
                 {
                      __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(5));
                     falg=true;
                     return falg;
                 }
                 TopoEdge *edgex=m_topoView->get_edgeByVer(AGVcurrent,AGVtarget);
                 edgex_id=edgex->get_id();
                 if(edges.contains(edgex_id))
                 {
                   falg=true;
                   return falg;
                 }
             }
         }
         else if((AGVcurrent==AGVtarget)&&(AGVcurrent!=-1)&&(AGVcurrent!=0)&&(AGVcurrent!=agvx->get_restNode()))
         {
                if(vers.contains(AGVcurrent))
                {
                    __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(6));
                   falg=true;
                   return falg;
                }
         }
         if(falg==false)
         {
             QList<int> pathx= m_mapPath.value(agvxName);
             HashSet hashSet3(100);
             QList<int>l;
             int na=path.length();
             int nb=pathx.length();
             hashSet3.hashInterSection(&hashSet3,path,pathx,na,nb,l);
             //hashInterSection(&hashSet3,path,pathx,na,nb,l);
             int length=l.length();
             if(length==0)
             {
                // 路线无重合s

             }
             else if(length==1)
             {
                 //重合路线只有一个点
                 //0812最后运动过程中，会有一个点是重合点（第二天去沈阳出差）
                 int fpo=l.first();


                 int  de  =path.indexOf(fpo);
                 int dex=pathx.indexOf(fpo);



                 double lengP=0,lengpx=0;

                 lengP=m_topoView->get_edgeByVer(current,path.at(0))->get_weight();
                 if(agvx->get_topoPosID()==agvx->get_targetID())
                 {
                   lengpx=m_topoView->get_edgeByVer(agvx->get_topoPosID(),pathx.at(0))->get_weight();
                 }

                 for(int i=0;i<de;i++)
                 {
                  lengP=lengP+m_topoView->get_edgeByVer(path.at(i),path.at(i+1))->get_weight();
                 }

                 for(int i=0;i<dex;i++)
                 {
                  lengpx=lengpx+m_topoView->get_edgeByVer(pathx.at(i),pathx.at(i+1))->get_weight();
                 }
                  //qDebug()<<"length___"<<lengP<<"length___x"<<lengpx;
                   __log__(&m_file,QString("0length: %1 ,0lengthx:%2,agv:%3,agvx:%4").arg(lengP).arg(lengpx).arg(agvid).arg(agvxid));
                 if(lengP>=lengpx)
                 {
                       falg=true;
                        __log__(&m_file,QString("0agv: %1 ,0reason:%2").arg(agvid).arg(8));
                        m_mapPath.remove(agvName);
                       return falg;


                 }

             }
             else
             {
                 int in1r1=path.indexOf(l.at(0));
                 int in1r2=path.indexOf(l.at(1));
                 int in2r1=pathx.indexOf(l.at(0));
                 int in2r2=pathx.indexOf(l.at(1));

                 int d1=in1r1-in1r2;
                 int d2=in2r1-in2r2;
                 if((d1>0&&d2>0)||(d1<0&&d2<0))
                 {
                     // 路线方向相同
                       __log__(&m_file,QString("same route"));
                       TopoEdge *edgeC=m_topoView->get_edgeByVer(current,path.at(0));
                       int lenwight=edgeC->get_weight();
                       int lenwightx=-1;
                       if(agvx->get_topoPosID()==agvx->get_targetID())
                       {
                           TopoEdge *edgecx=m_topoView->get_edgeByVer(agvx->get_topoPosID(),pathx.at(0));
                           lenwightx=edgecx->get_weight();
                       }
                     int num=path.indexOf(l.first());
                     int numx=pathx.indexOf(l.first());

                     for(int i=0;i<num;i++)
                     {
                         TopoEdge *edge=m_topoView->get_edgeByVer(path.at(i),path.at(i+1));
                         lenwight=lenwight+ edge->get_weight();
                     }
                     for(int i=0;i<numx;i++)
                     {
                         TopoEdge *edge=m_topoView->get_edgeByVer(pathx.at(i),pathx.at(i+1));
                         lenwightx=lenwightx+edge->get_weight();
                     }

                      int nex=lenwight-edgeC->get_weight();
                       __log__(&m_file,QString("agv: %1 ,lenwigth:%2,nex:%3,lenwigthx:%4").arg(agvid).arg(lenwight).arg(nex).arg(lenwightx));
                      if(((lenwight>crossLength)&&(nex<crossLength))||(path.first()==l.last())||(lenwight<crossLength))
                      {
                         if(lenwight>lenwightx)
                         {
                             falg=true;
                              __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(7));
                              //m_mapPath.remove(agvName);
                             return falg;
                         }

                      }
                 }
                 else
                 {
                   //路线方向相反
                    int la= l.last();
                    int fr= l.first();

                  int pf=  path.indexOf(la);

                  int pxf= pathx.indexOf(fr);
                  int pxl =pathx.indexOf(la);
                  __log__(&m_file,QString("pf: %1 ,pxf:%2,pxl:%3,la:%4,fr:%5").arg(pf).arg(pxf).arg(pxl).arg(la).arg(fr));
                  double lengP=0,lengpx=0;

                  lengP=m_topoView->get_edgeByVer(current,path.at(0))->get_weight();
                  if(agvx->get_topoPosID()==agvx->get_targetID())
                  {
                    lengpx=m_topoView->get_edgeByVer(agvx->get_topoPosID(),pathx.at(0))->get_weight();
                  }

                  for(int i=0;i<pf;i++)
                  {
                   lengP=lengP+m_topoView->get_edgeByVer(path.at(i),path.at(i+1))->get_weight();
                  }

                  for(int i=0;i<pxf;i++)
                  {
                   lengpx=lengpx+m_topoView->get_edgeByVer(pathx.at(i),pathx.at(i+1))->get_weight();
                  }
                   qDebug()<<"length"<<lengP<<"lengthx"<<lengpx;
                    __log__(&m_file,QString("length: %1 ,lengthx:%2,agv:%3,agvx:%4").arg(lengP).arg(lengpx).arg(agvid).arg(agvxid));
                  if(lengP>=lengpx)
                  {
                     int id= m_topoView->get_edgeByVer(path.at(pf),path.at(pf+1))->get_id();
                     int idl=m_topoView->get_edgeByVer(pathx.at(pxl),pathx.at(pxl+1))->get_id();
                      //QList<int>edgeNext=m_collionEdge.value(idl);
                     __log__(&m_file,QString("id: %1 ,idl:%2").arg(id).arg(idl));
                      if((edges.contains(id))||(edges.contains(idl)))
                      {
                        falg=true;
                         __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(8));
                         //m_mapPath.remove(agvName);
                        return falg;
                      }

                  }
                 }
             }
         }

     }
 }

 return falg;
}
bool TrafficManager::willCollsionmultiNew(AGV *agv, int current, int target)
{
QString agvName=agv->get_name();
int     agvid=  agv->get_id();
QList<int> path= m_mapPath.value(agvName);
bool falg=false;

TopoEdge *edge=m_topoView->get_edgeByVer(current,target);
int edge_id=edge->get_id();
QList<int> edges=m_collionEdge.value(edge_id);
QList<int> vers=m_collionEdgeVer.value(edge_id);

int AGVcurrent=-1;
int AGVtarget =-1;
int edgex_id=-1;
int usepoint=-1;
QList<int>lPointS= m_commPoints;

HashSet hashSet5(100);
QList<int> pathAndComm;
int na=lPointS.length();
int nb=path.length();
hashSet5.hashInterSection(&hashSet5,lPointS,path,na,nb,pathAndComm);
int pAcL=pathAndComm.length();
if((pAcL>0))
{
    int  indexFr=path.indexOf(pathAndComm.first());
    TopoEdge *edgeF=m_topoView->get_edgeByVer(current,path.at(0));
    int lenwight=edgeF->get_weight();
    for (int i=0;i<(indexFr-1);i++)
    {
        TopoEdge *edge=m_topoView->get_edgeByVer(path.at(i),path.at(i+1));
        lenwight= lenwight+edge->get_weight();
    }
    int nextlength=lenwight-(edgeF->get_weight());
    if(((lenwight>crossLength)&&(nextlength<crossLength))||(path.first()==pathAndComm.first())||(lenwight<crossLength))
    {
        m_queryServer=getQueryServer();
        m_queryServer.exec(QString("select * from agv_position where system<>%1").arg(CURRRENTAGVSYSTEM));
        while(m_queryServer.next()){
        QSqlRecord record = m_queryServer.record();
        AGVcurrent =record.value("current_node").toInt();
        AGVtarget  =record.value("target_node").toInt();
        usepoint=record.value("usepoint").toInt();
        if((AGVcurrent!=AGVtarget)&&(AGVcurrent!=-1))
        {
            bool exitFlag=m_topoView->is_edgeExist(QString("Ver_%1").arg(AGVcurrent),QString("Ver_%1").arg(AGVtarget));
            if(exitFlag==true)
            {
                TopoEdge *edgex=m_topoView->get_edgeByVer(AGVcurrent,AGVtarget);
                edgex_id=edgex->get_id();
                if(edges.contains(edgex_id))
                {
                    __log__(&m_file,QString("agv:%1,reason:%2").arg(agvid).arg(1));
                  falg=true;
                  break;
                }
            }
            else
            {

                if( vers.contains(AGVtarget)&&vers.contains(AGVcurrent))
               {
                     __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(2));
                  falg=true;
                  break;
               }
            }
        }
        if(vers.contains(AGVtarget)||vers.contains(AGVcurrent))
       {
             __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(20));

          falg=true;
          break;
       }
       if(usepoint==pathAndComm.first())
       {
           __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(15));
          falg=true;
          return falg;
       }
      /* else if(usepoint!=pathAndComm.first())
       {

       }
       */

    }
        //没有return
        m_queryServer=getQueryServer();
        m_queryServer.exec(QString("select * from agv_position where usepoint=%1").arg(pathAndComm.first()));
        int count=0;
        while(m_queryServer.next()){
            count++;
        }
        if(count==0)
        {
            m_queryServer.prepare("update agv_position set usepoint=? where id=?") ;
            m_queryServer.addBindValue(pathAndComm.first());
            m_queryServer.addBindValue(agv->get_id());
            m_queryServer.exec();
        }
    }
}
 else if(pAcL==0)
 {
    QMap<int,QList<int>>::const_iterator i;
    bool baoHan=false;
    int  usep;
    for(i=m_commEdges.constBegin();i!=m_commEdges.constEnd();i++)
    {
        if(i.value().contains(edge_id))
        {
            baoHan=true;
            usep=i.key();
            break;
        }
    }
    if(baoHan==true)
    {

        m_queryServer=getQueryServer();
         m_queryServer.exec(QString("select * from agv_position where system<>%1").arg(CURRRENTAGVSYSTEM));
         while(m_queryServer.next()){
         QSqlRecord record = m_queryServer.record();

         usepoint=record.value("usepoint").toInt();

         if(usepoint==usep)
         {
             __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(16));
            falg=true;
            return falg;
         }
         }
        m_queryServer.prepare("update agv_position set usepoint=? where id=?") ;
        m_queryServer.addBindValue(usep);
        m_queryServer.addBindValue(agv->get_id());
        m_queryServer.exec();
    }
    else
    {
     m_queryServer=getQueryServer();
     m_queryServer.prepare("update agv_position set usepoint=? where id=?") ;
     m_queryServer.addBindValue(0);
     m_queryServer.addBindValue(agv->get_id());
     m_queryServer.exec();
    }
 }
//本系统中
foreach (AGV* agvx, m_agvList) {

  int agvxid=agvx->get_id();
  QString agvxName=agvx->get_name();
  if(agvid==agvxid)
      continue;
  else
  {
      AGVcurrent=agvx->get_topoPosID();
      AGVtarget=agvx->get_targetID();
       int edgex_id=-1;
      if((AGVcurrent!=AGVtarget)&&(AGVcurrent!=-1))
      {
          bool exitFlag=m_topoView->is_edgeExist(QString("Ver_%1").arg(AGVcurrent),QString("Ver_%1").arg(AGVtarget));
          if(exitFlag==true)
          {
              if(vers.contains(AGVcurrent))
              {
                   __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(5));
                  falg=true;
                  return falg;
              }
              TopoEdge *edgex=m_topoView->get_edgeByVer(AGVcurrent,AGVtarget);
              edgex_id=edgex->get_id();
              if(edges.contains(edgex_id))
              {
                falg=true;
                return falg;
              }
          }
      }
      else if((AGVcurrent==AGVtarget)&&(AGVcurrent!=-1)&&(AGVcurrent!=0)&&(AGVcurrent!=agvx->get_restNode()))
      {
             if(vers.contains(AGVcurrent))
             {
                 __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(6));
                falg=true;
                return falg;
             }
      }
      if(falg==false)
      {
          QList<int> pathx= m_mapPath.value(agvxName);
          HashSet hashSet3(100);
          QList<int>l;
          int na=path.length();
          int nb=pathx.length();
          hashSet3.hashInterSection(&hashSet3,path,pathx,na,nb,l);
          int length=l.length();
          if(length==0)
          {
             // 路线无重合s

          }
          else if(length==1)
          {
              //重合路线只有一个点
              //0812最后运动过程中，会有一个点是重合点（第二天去沈阳出差）
              int fpo=l.first();
              int  de  =path.indexOf(fpo);
              int dex=pathx.indexOf(fpo);
              double lengP=0,lengpx=0;
              lengP=m_topoView->get_edgeByVer(current,path.at(0))->get_weight();
              if(agvx->get_topoPosID()==agvx->get_targetID())
              {
                lengpx=m_topoView->get_edgeByVer(agvx->get_topoPosID(),pathx.at(0))->get_weight();
              }

              for(int i=0;i<de;i++)
              {
               lengP=lengP+m_topoView->get_edgeByVer(path.at(i),path.at(i+1))->get_weight();
              }

              for(int i=0;i<dex;i++)
              {
               lengpx=lengpx+m_topoView->get_edgeByVer(pathx.at(i),pathx.at(i+1))->get_weight();
              }
               //qDebug()<<"length___"<<lengP<<"length___x"<<lengpx;
                __log__(&m_file,QString("0length: %1 ,0lengthx:%2,agv:%3,agvx:%4").arg(lengP).arg(lengpx).arg(agvid).arg(agvxid));
              if(lengP>=lengpx)
              {
                    falg=true;
                     __log__(&m_file,QString("0agv: %1 ,0reason:%2").arg(agvid).arg(8));
                     m_mapPath.remove(agvName);
                    return falg;
              }

          }
          else
          {
              int in1r1=path.indexOf(l.at(0));
              int in1r2=path.indexOf(l.at(1));
              int in2r1=pathx.indexOf(l.at(0));
              int in2r2=pathx.indexOf(l.at(1));

              int d1=in1r1-in1r2;
              int d2=in2r1-in2r2;
              if((d1>0&&d2>0)||(d1<0&&d2<0))
              {
                // 路线方向相同
                  __log__(&m_file,QString("same route"));
                  TopoEdge *edgeC=m_topoView->get_edgeByVer(current,path.at(0));
                  int lenwight=edgeC->get_weight();
                  int lenwightx=-1;
                  if(agvx->get_topoPosID()==agvx->get_targetID())
                  {
                      TopoEdge *edgecx=m_topoView->get_edgeByVer(agvx->get_topoPosID(),pathx.at(0));
                      lenwightx=edgecx->get_weight();
                  }
                int num=path.indexOf(l.first());
                int numx=pathx.indexOf(l.first());
                 __log__(&m_file,QString("agv: %1 ,num:%2,numx:%3").arg(agvid).arg(num).arg(numx));
                for(int i=0;i<num;i++)
                {
                    TopoEdge *edge=m_topoView->get_edgeByVer(path.at(i),path.at(i+1));
                    lenwight=lenwight+ edge->get_weight();
                }
                for(int i=0;i<numx;i++)
                {
                    TopoEdge *edge=m_topoView->get_edgeByVer(pathx.at(i),pathx.at(i+1));
                    lenwightx=lenwightx+edge->get_weight();
                }

                 int nex=lenwight-edgeC->get_weight();
                  __log__(&m_file,QString("agv: %1 ,lenwigth:%2,nex:%3,lenwigthx:%4").arg(agvid).arg(lenwight).arg(nex).arg(lenwightx));
                 if(((lenwight>crossLength)&&(nex<crossLength))||(path.first()==l.last())||(lenwight<crossLength))
                 {
                    if(lenwight>lenwightx)
                    {
                        falg=true;
                         __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(7));
                         //m_mapPath.remove(agvName);
                        return falg;
                    }

                 }
              }
              else
              {
                //路线方向相反
                 int la= l.last();
                 int fr= l.first();

               int pf=  path.indexOf(la);

               int pxf= pathx.indexOf(fr);
               int pxl =pathx.indexOf(la);
               __log__(&m_file,QString("pf: %1 ,pxf:%2,pxl:%3,la:%4,fr:%5").arg(pf).arg(pxf).arg(pxl).arg(la).arg(fr));
               double lengP=0,lengpx=0;

               lengP=m_topoView->get_edgeByVer(current,path.at(0))->get_weight();
               if(agvx->get_topoPosID()==agvx->get_targetID())
               {
                 lengpx=m_topoView->get_edgeByVer(agvx->get_topoPosID(),pathx.at(0))->get_weight();
               }

               for(int i=0;i<pf;i++)
               {
                lengP=lengP+m_topoView->get_edgeByVer(path.at(i),path.at(i+1))->get_weight();
               }

               for(int i=0;i<pxf;i++)
               {
                lengpx=lengpx+m_topoView->get_edgeByVer(pathx.at(i),pathx.at(i+1))->get_weight();
               }
                qDebug()<<"length"<<lengP<<"lengthx"<<lengpx;
                 __log__(&m_file,QString("length: %1 ,lengthx:%2,agv:%3,agvx:%4").arg(lengP).arg(lengpx).arg(agvid).arg(agvxid));
               if(lengP>=lengpx)
               {
                  int id= m_topoView->get_edgeByVer(path.at(pf),path.at(pf+1))->get_id();
                  int idl=m_topoView->get_edgeByVer(pathx.at(pxl),pathx.at(pxl+1))->get_id();
                   //QList<int>edgeNext=m_collionEdge.value(idl);
                  __log__(&m_file,QString("id: %1 ,idl:%2").arg(id).arg(idl));
                   if((edges.contains(id))||(edges.contains(idl)))
                   {
                     falg=true;
                      __log__(&m_file,QString("agv: %1 ,reason:%2").arg(agvid).arg(8));
                      //m_mapPath.remove(agvName);
                     return falg;
                   }
               }
              }
          }
      }
  }
}
return falg;
}


//生成一个包含（当前点，将要走的二三点的独占区域）

void TrafficManager::shengchengquyu(AGV*agv,QList<int>pathluguo,QList<int>pathguihua)
{

    //-------------解锁走过的点--------
   if(m_chongtuPath.contains(agv->get_name()))
   {
       QList<int>chongtupath1=m_chongtuPath[agv->get_name()];

     for(int i=0;i<chongtupath1.count();i++)
     {

         int point=chongtupath1[i];
         if((lockpoint[point]==agv->get_name())&&(lockpoint.contains(point)))
             lockpoint[point]="";

    }
   }

    //--------------------

    QList<int>chongtupath;



 chongtupath.append(agv->get_topoPosID());
 if(pathguihua.count()>=2)
     {
     chongtupath.append(pathguihua.value(0));
     chongtupath.append(pathguihua.value(1));
    }
 else if(pathguihua.count()==1)
     {
     chongtupath.append(pathguihua.value(0));
     }
  m_chongtuPath[agv->get_name()]=chongtupath;
  qDebug()<<"AGV:"<<agv->get_id()<<"管控区域为："<<chongtupath<<endl;



  //------锁定某些将要走的点--------

  for(int i=0;i<chongtupath.count();i++)
  {
      int point=chongtupath[i];
      if((lockpoint[point]!=agv->get_name())||(!lockpoint.contains(point)))
          lockpoint[point]=agv->get_name();

  }


//---------------------------



  QHashIterator<int, QString> i(lockpoint);

    qDebug() <<"锁定点为："<<endl;
      while (i.hasNext())
      {
          i.next();
       qDebug() <<i.key()<<"--"<< i.value()<<endl;
      }



}

//新的计算冲突的函数3.31---形成一个3到4个点的冲突区域，但是还没设置太好，缺少对点lock的机制
bool TrafficManager:: willCollsionchange(AGV*agv)
{

    int agvid=agv->get_id();
    QString agvname=agv->get_name();
   QList<int> path1= m_chongtuPath[agvname];
    QList<int> path;
   int num=path1.count();

   if((num==3)||(num==4))//基于冲突区域判断，path有多种情况
   {
       path.append(path1.value(num-1));
        path.append(path1.value(num-2));
   }
   else if (num==2)
   {
       path.append(path1.value(num-1));
   }

    foreach(AGV*agvx,m_agvList)
    { int agvxid=agvx->get_id();
        QString agvName=agvx->get_name();

        if(agvid==agvxid)
            continue;

        if (m_chongtuPath.contains(agvName)){
            QList<int>pathx=m_chongtuPath[agvName];
         foreach(int point1,pathx)
         {
           foreach(int point,path1)
            {if ((point1==point)&&(point!=agv->get_topoPosID()))
                {  if(path1.indexOf(point)<=pathx.indexOf(point1))
                        continue;
                  else
                   {return true;//表明发生冲突
                   qDebug()<<"agv:"<<agvid<<"与"<<agvxid<<"冲突"<<point<<endl;}
                 }
               else
                   continue;
            }
        }


    }
  }
      return false;

}

void TrafficManager::guidaoglobalplanning(AGV*agv,Mission old_mission)
{

    int current_node = agv->get_topoPosID();
    int target_node = current_node;
    int  agvid  = agv->get_id();

    QString agv_name = agv->get_name();



    if(m_pausePoints.contains(agvid))
    {
        QList<int> points=m_pausePoints.value(agvid);
        int point=points.first();
        if ((point==current_node)||(point==0))
        {
            __log__(&m_file,QString("agvid: %1 pause").arg(agvid));
            return;
        }

    }
    Mission mission;
    mission.m_nStartNodeID = -1;
    mission.m_nEndNodeID = -1;


    if(!m_mapAGVandMission.contains(agv_name)){  // 当前agv无任务

        qDebug()<<"AGV"<<agvid<<"无任务"<<endl;

        if(m_missionManager->getNextMission(agv_name,mission,agv)){  // 存在新任务
            qDebug()<<"AGV"<<agvid<<"有新任务"<<endl;
            m_mapAGVandMission[agv_name]=St_Mission(mission,agv->get_startNode()==mission.m_nStartNodeID, mission.m_nStartNodeID==mission.m_nEndNodeID);
           target_node = mission.m_nStartNodeID; //绕回来  避免撞车  试验场地特加
           agv->stateRecord(mission.m_bReachStartNode,mission.m_bReachEndNode);

        }
        else{
           target_node = agv->get_restNode();
           emit AGVstateChange(agv,false);

        }
    }
    else{  // 当前agv有正在执行的任务


        St_Mission st_mission = m_mapAGVandMission.value(agv_name);
        mission = st_mission.m_mission;
        if(mission.m_bReachStartNode==false){ //如果还没有到达起始点
          if(current_node!=mission.m_nStartNodeID){    //当前点不是任务起点
                target_node = mission.m_nStartNodeID;
            }
          else{ //当前点是任务起点
                target_node=mission.m_nEndNodeID;  //没效果
                mission.m_bReachStartNode = true;
              //   m_missionManager->setMissionState(mission,MISSION_STATE_DAOKU);
                m_mapAGVandMission[agv_name] = St_Mission(mission,true,mission.m_nStartNodeID==mission.m_nEndNodeID);// 更新任务标记
                    // 如果当前点为任务起点，则要检测叉子是否在起点货架层高顶端
                agv->set_startNode(mission.m_nStartNodeID);
              }
            agv->stateRecord(mission.m_bReachStartNode,mission.m_bReachEndNode);
        }
        else {  // 已经路过起点
            m_missionManager->setMissionState(mission,MISSION_STATE_DELIVERY);
            if(current_node!=mission.m_nEndNodeID){//当前点不是任务终点
                if(mission.m_bReachEndNode==false)
                target_node = mission.m_nEndNodeID;
                else
                 target_node = agv->get_restNode();

               }

            else{ // 当前点是目标终点
                // 如果当前点为任务终点，则要检测叉子是否在终点货架层高底端
                agv->set_startNode(mission.m_nEndNodeID);
                target_node = agv->get_restNode();
                 mission.m_bReachEndNode=true;
                 m_mapAGVandMission[agv_name] = St_Mission(mission,true,mission.m_nStartNodeID==mission.m_nEndNodeID);// 更新任务标记

                qDebug()<<"任务状态变成返回"<<endl;
                 m_missionManager->setMissionState(mission,MISSION_STATE_BACK);//表示现在agv正在返回原先的点
                // agv->missionflag1=true;//完成了取货送货的任务

                  old_mission= mission;
                 emit MissionStateChanged(mission.m_nStartNodeID, mission.m_nEndNodeID, mission.m_nStartNodeLayer, mission.m_nEndNodeLayer);
                 m_mapAGVandMission.remove(agv_name);
                 emit AGVstateChange(agv,false);
            }
            agv->stateRecord(mission.m_bReachStartNode,mission.m_bReachEndNode);
        }
    }





     bool flag_collosion=false;
    if(multiUsed==true)
    {
         //TopoEdge *ed=m_topoView->get_edgeByVer(current_node,path.first());
        m_queryServer=getQueryServer();
        if(shilian==false)
        {
            m_queryServer.exec("select * from agv_position");
            if(m_queryServer.isActive()==true)
            {
              m_queryServer.prepare("update agv_position set current_node=?,target_node=? where id=?");
              m_queryServer.addBindValue(current_node);
              m_queryServer.addBindValue(target_node);
              m_queryServer.addBindValue(agv->get_id());
              m_queryServer.exec();
            }
            else
            {
                    agv->set_targetID(current_node,current_node);
                    shilian=true;
                    QMessageBox msgBox;
                    msgBox.setText(tr("unconnect to database."));
                    msgBox.setInformativeText(tr("Do you want to retry connect?"));
                    msgBox.setStandardButtons(QMessageBox::Retry
                                              | QMessageBox::Cancel);
                    int ret = msgBox.exec();
                    bool l;
                    switch (ret) {
                    case QMessageBox::Retry:
                       l= openSQLServer();
                       if(l==false)
                       {
                           agv->set_targetID(current_node,current_node);
                           shilian=false;
                           return;
                       }
                       else
                       {
                           shilian=false;
                       }
                        break;
                    case QMessageBox::Cancel:
                        break;
                    }
            }
        }
        else if(shilian==true)
        {
            qDebug()<<"shilian";
            agv->set_targetID(current_node,current_node);
            return;
        }

    }

   // qDebug()<<"flag_collosion"<<flag_collosion;

    if((multiUsed==true))
    {

    }
    else{

         flag_collosion=false;
    }
     if(flag_collosion==true)
        {

               qDebug()<<"AGV:"<<agv->get_id()<<"产生节点冲突"<<"current node:"<<current_node<<endl;
              agv->set_targetID(current_node,current_node);  // 若路径中下一个节点被其它AGV占用，则停留在该节点处
              if(multiUsed==true)
              {
                  m_queryServer=getQueryServer();
                  m_queryServer.prepare("update agv_position set target_node=? where id=?") ;
                  m_queryServer.addBindValue(current_node);
                  m_queryServer.addBindValue(agv->get_id());
                  m_queryServer.exec();
              }

        }
        else{

            bool can_multi_mode = false;

            if(!can_multi_mode){  // 不能多步运行
                  agv->set_targetID(target_node,target_node);
                qDebug()<<"轨道AGV路线:"<<current_node<<"--"<<target_node<<endl;

                agv->set_mode(AGV::SINGLE_STEP_MODE); //如果AGV不包含固定路由表
            }
        }


  /*
 void TrafficManager::timerEvent(QTimerEvent*event)
 {
     int id=event->timerid();
     if(timeridagv.contains(id))
     {
         AGV*agv=timeridagv[id];
         bool qumission=agv->qumission
         bool songmission=
         liaoxiangdown();//每次定时器需要先更新料箱的信息，避免误操作


*/




}

void TrafficManager::CommandProduce(int current_node,QList<int> path,QList<AGVCommand>&Command)
{
    AGVCommand agvcommand;
    for(int i=0;i<=path.size()-1;i++)
    {
        if(i==0)
        {
            agvcommand.start=current_node;
            agvcommand.end=path.at(0);
            agvcommand.angel=0;
        }
        else
       {
            agvcommand.start=path.at(i-1);
            agvcommand.end=path.at(i);
            agvcommand.angel=0;//目前测试期，不考虑旋转的情况6.22
        }
       Command.append(agvcommand);
    }

}

void TrafficManager::CommandProcess()
{

}




//-----未考虑角度问题，简单粗暴-------
int TrafficManager::willCollsion_Region(AGVCommand &com1, AGVCommand &com2, AGVCommand Command1, AGVCommand Command2,QString agvname,AGV*agv)
{
   int start1=Command1.start;
   int end1=Command1.end;
   int start2=Command1.start;
   int end2=Command1.end;

    if(lockpoint.contains(start1)&&(lockpoint[start1]!=agvname))
    {
        if(agv->get_Commandnumber()>0)
        {
            AGVCommand command=agv->get_CommandInfo(1);
            com1.start=command.start;
            com1.end=com1.start;
            com2=com1;
            //不发送指令;
            return 2;
        }

    }
    else if((lockpoint.contains(end1))&&(lockpoint[end1]!=agvname))
    {
            com1.start=Command1.start;
            com1.end=Command1.start;

            com2=com1;

            return 2;


    }
    else if((lockpoint.contains(start2))&&(lockpoint[start2]!=agvname))
    {

        com1.start=Command1.start;
        com1.end=Command1.end;

        com2.start=com1.end;
        com2.end=com1.end;

        return 1;
    }
    else if((lockpoint.contains(end2))&&(lockpoint[end2]!=agvname))
     {
        com1.start=Command1.start;
        com1.end=Command1.end;
        com1.end=Command1.start;
        com2.start=Command2.start;
        com2.end=Command2.start;
        return 1;

    }
    else
    {
        com1=Command1;
        com2=Command2;
        return 0;
    }





}
