#include "agvinformation.h"
#include <QGridLayout>
#include "global.h"
#include <QSqlRecord>
#include <QSqlQuery>

#include "ui_agvinformation.h"
//extern QSqlDatabase db;
AgvInformation::AgvInformation(QWidget *parent) : QWidget(parent)
                   ,ui(new Ui::Form)
{

   /* setWindowTitle("AGV-信息加载栏");
    setAttribute(Qt::WA_QuitOnClose,false);
    setMaximumSize(780,300);
    setMinimumSize(780,300);
    */
    ui->setupUi(this);
    this->setWindowTitle("AGV加载栏");


    m_publicIconMap[TREE_ITEM_ICON_Project] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/Project.png"));
    m_publicIconMap[TREE_ITEM_ICON_folder] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/folder.png"));
    m_publicIconMap[TREE_ITEM_ICON_folderAnsys] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/folder-ansys.png"));
    m_publicIconMap[TREE_ITEM_ICON_TdmsGroup] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/group.png"));
    m_publicIconMap[TREE_ITEM_ICON_TdmsChannel] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/channel.png"));
    m_publicIconMap[TREE_ITEM_ICON_folderOriginal] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/folder_original.png"));
    m_publicIconMap[TREE_ITEM_ICON_DataItem] = QIcon(QStringLiteral(":/treeIcon/res_treeItemIcon/dataItem.png"));

    open_database();
    initTree();
    initTree2();
    connect(ui->add,SIGNAL(clicked(bool)),this,SLOT(addIntoSystem()));
   // connect(ui->remove,SIGNAL(clicked(bool)),this,SLOT(startWizard()));
    connect(ui->remove,SIGNAL(clicked(bool)),this,SLOT(removeFromSystem()));
    connect(ui->ok,SIGNAL(clicked(bool)),this,SLOT(hide()));
    connect(ui->pause,SIGNAL(clicked(bool)),this,SLOT(pause()));
    connect(ui->keepOn,SIGNAL(clicked(bool)),this,SLOT(keepOn()));
}
void AgvInformation::initTree()
{

    model = new QStandardItemModel(this);
  //  model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("AGVS")<<QStringLiteral("信息"));
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("AGVS"));
    itemProject = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_Project")],QStringLiteral("AGV"));
    model->appendRow(itemProject);
  //  model->setItem(model->indexFromItem(itemProject).row(),1,new QStandardItem(QStringLiteral("")));

    //ps20类型的车

    int l=m_idName.size();
    for(int i=0;i<l;i++)
    {
      int id=m_idName.keys().at(i);
      QString name=m_idName.value(id);
      int startNode=m_idStartNode.value(id);
      int address_1=m_idAddress_1.value(id);
      //QString Address_1=addrToString(address_1);
      int address_2=m_idAddress_2.value(id);
      //QString Address_2=addrToString(address_2);

      QString type=m_idType.value(id);
      int angle=m_idAngle.value(id);
      int chargeStation=m_idChargeStation.value(id);
      int chexing=m_idChexing.value(id);
      QStringList chargelimit=m_idChargeLimit.value(id);
      QString highcharge=chargelimit.at(0);
      QString middlecharge=chargelimit.at(1);
      QString lowcharge=chargelimit.at(2);
     int xiansudu=0;
     int jiaosudu=0;
     int a=0;
     int w=0;
          itemFolder = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_folder")],QStringLiteral("%1").arg(id));
          itemProject->appendRow(itemFolder);

          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("name"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(name));
          itemgroup->appendRow(itemchannel);
          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("startNode"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(startNode));


          itemgroup->appendRow(itemchannel);

          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("angle"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(angle));
          itemchannel->setFlags(Qt::ItemIsSelectable);//不可更改
          itemgroup->appendRow(itemchannel);


          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("agvtype"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(chexing));
          itemchannel->setFlags(Qt::ItemIsSelectable);//不可更改
          itemgroup->appendRow(itemchannel);

          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("最大线速度"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(xiansudu));
          itemgroup->appendRow(itemchannel);
          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("最大角速度"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(jiaosudu));
          itemgroup->appendRow(itemchannel);

          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("线加速度"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(a));
          itemgroup->appendRow(itemchannel);

          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("角加速度"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(w));
          itemgroup->appendRow(itemchannel);

          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("Address_1"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(address_1));
          itemgroup->appendRow(itemchannel);

          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("Address_2"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(address_2));
          itemgroup->appendRow(itemchannel);

          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("chargeStation"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(chargeStation));
          itemgroup->appendRow(itemchannel);
          itemgroup = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_group")],QStringLiteral("%1").arg("chargelimit"));
          itemFolder->appendRow(itemgroup);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(highcharge));
          itemgroup->appendRow(itemchannel);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(middlecharge));
          itemgroup->appendRow(itemchannel);
          itemchannel = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_channel")],QStringLiteral("%1").arg(lowcharge));
          itemgroup->appendRow(itemchannel);
   }

    ui->treeView->setModel(model);    
}
void AgvInformation::initTree2()
{
    model2 = new QStandardItemModel(this);
    model2->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("AGVS")<<QStringLiteral("信息"));
    itemProject2 = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_Project")],QStringLiteral("AGV"));
    model2->appendRow(itemProject2);
    ui->treeView_2->setModel(model2);

}
QList<int> AgvInformation::agvInUse()
{
    return m_agvInuse;
}
void AgvInformation::addIntoSystem()//选中agv并加入
{

    bool flag=false;
    QModelIndex index = ui->treeView->currentIndex();
    int id=index.data().toInt();




/*
    QModelIndex shuju = model->index(0,0,QModelIndex());
     QModelIndex shuju1 = model->index(id-1,0,shuju);
    QModelIndex shuju2=  model->index(1,0,shuju1);
    QModelIndex shuju3=  model->index(0,0,shuju2);

  int chushidian= shuju3.data().toInt();
    */
    //qDebug()<<"chushidian"<<chushidian;

    int l=itemProject2->rowCount();




    for(int i=0;i<l;i++)
    {
    QModelIndex indexx= itemProject2->child(i)->index();
    int idx=indexx.data().toInt();

        if (idx==id)
           { flag=true;

            }


    }
    QString nameparent= index.parent().data().toString();

    if(flag==false)
    {

        itemFolder = new QStandardItem(m_publicIconMap[QStringLiteral("treeItem_folder")],QStringLiteral("%1").arg(id));
        if(nameparent=="AGV")
        {

            QModelIndex shuju = model->index(0,0,QModelIndex());
             QModelIndex shuju1 = model->index(id-1,0,shuju);
            QModelIndex shuju2=  model->index(1,0,shuju1);
            QModelIndex shuju3=  model->index(0,0,shuju2);

            QModelIndex chexing1=  model->index(3,0,shuju1);//此处后续操作需要修改注释掉
            QModelIndex chexing2=  model->index(0,0,chexing1);

            QModelIndex angle1=  model->index(2,0,shuju1);
            QModelIndex angle2=  model->index(0,0,angle1);


            QModelIndex xiansudu1=  model->index(3,0,shuju1);
            QModelIndex xiansudu2=  model->index(0,0,xiansudu1);

            QModelIndex jiaosudu1=  model->index(4,0,shuju1);
            QModelIndex jiaosudu2=  model->index(0,0,jiaosudu1);

            QModelIndex a1=  model->index(5,0,shuju1);
            QModelIndex a2=  model->index(0,0,a1);

            QModelIndex w1=  model->index(6,0,shuju1);
            QModelIndex w2=  model->index(0,0,w1);

             int chexing= chexing2.data().toInt();
             int chushidian= shuju3.data().toInt();
             int xiansudu=xiansudu2.data().toInt();
             int jiaosudu=jiaosudu2.data().toInt();;
             int a=a2.data().toInt();
             int w=w2.data().toInt();
             int angle=angle2.data().toInt();

            itemProject2->appendRow(itemFolder);
            itemProject2->setChild(itemFolder->index().row(),1,new QStandardItem(QStringLiteral("离线")));
          //  emit addintoRun(id);

            emit chushihua(id,chushidian,chexing,xiansudu,jiaosudu,a,w,angle);

        }

    }

  //  qDebug()<<"nameparent"<<nameparent<<"id"<<id;
}
void AgvInformation::startWizard(bool hasMission,bool reach,int start,int end, int id,QList<int>ids)
{
    qDebug()<<"启动Wizard";
        agvremoveWizard=new AGVRemoveWizard(hasMission,reach,start,end,id,ids);

       agvremoveWizard->show();
       connect(agvremoveWizard,SIGNAL(missionstate(int,bool,bool,bool)),this,SLOT(missionDecide(int,bool,bool,bool)));
}
void AgvInformation::removeFromSystem()
{

    QModelIndex index = ui->treeView_2->currentIndex();
    //int id=index.child(index.row(),0).data().toInt();
     index2= index.sibling(index.row(),0);
    deleteId=index2.data().toInt();
    qDebug()<<"要删除的AGV为："<<deleteId<<endl;
    //int id=index.data().toInt();
     nameparent= index2.parent().data().toString();
    //int row =index2.row();
    emit get_agvmission(deleteId);


   //  qDebug()<<"row"<<row<<"nameparent"<<nameparent<<"id"<<deleteId;
}
void AgvInformation::pause()
{
     QModelIndex index = ui->treeView_2->currentIndex();
     QModelIndex index3=index.sibling(index.row(),0);
     int id=index3.data().toInt();
     if(id!=0)
     {
        emit pauseAgv(id,0);
         qDebug()<<"pause"<<""<<id;
     }

}
void AgvInformation::keepOn()
{

     QModelIndex index = ui->treeView_2->currentIndex();
     QModelIndex index3=index.sibling(index.row(),0);
     int id=index3.data().toInt();
     if(id!=0)
     {
       emit keepMove(id);
         qDebug()<<"keepMove"<<""<<id;
     }

}
void AgvInformation::missionDecide(int id, bool hasmission, bool reset, bool finish)
{


    int row =index2.row();
    if(nameparent=="PT20")
    {
        itemProject2->removeRow(row);
        emit removeFromRun(id);
        emit ToMissionmanager(id,hasmission,reset,finish);
    }
    else if(nameparent=="PS20")
    {
        itemProject2PS20->removeRow(row);
        emit removeFromRun(id);
        emit ToMissionmanager(id,hasmission,reset,finish);
    }
    else if(nameparent=="PH20")
    {
        itemProject2PH20->removeRow(row);
        emit removeFromRun(id);
        emit ToMissionmanager(id,hasmission,reset,finish);
    }
  /*  int l=itemProject2PT20->rowCount();
    int ll=itemProject2PS20->rowCount();

    for(int i=0;i<l;i++)
    {
      int row=  itemProject2PT20->child(i)->row();
       QModelIndex indexx= itemProject2PT20->child(i)->index();
       int idx=indexx.data().toInt();
        if (idx==id)
        {
            int row =indexx.row();
             itemProject2PT20->removeRow(row);

             emit removeFromRun(id);
             emit ToMissionmanager(id,hasmission,reset,finish);
        }
    }
    for(int i=0;i<ll;i++)
    {
    QModelIndex indexx= itemProject2PS20->child(i)->index();
    int idx=indexx.data().toInt();
        if (idx==id)
        {
             int row =indexx.row();
                itemProject2PS20->removeRow(row);
                emit removeFromRun(id);
                emit ToMissionmanager(id,hasmission,reset,finish);
        }
    }
    */
}
void AgvInformation::receiveMission(bool hasmission, bool reach, int start, int end)
{
      startWizard(hasmission,reach,start,end,deleteId,m_agvall);
}
void AgvInformation::insertIntoSystem()
{
    qDebug()<<"插入系统";
    int l=itemProject2->rowCount();
    int ll=itemProject2PS20->rowCount();
    for(int i=0;i<l;i++)
    {
     QModelIndex indexx= itemProject2->child(i)->index();
     int idx=indexx.data().toInt();
     m_agvInuse.append(idx);

    }

   emit AgvInformationChange();
}
void AgvInformation::inSystem(int id)
{
    qDebug()<<"错误测试：加入系统成功在线"<<id<<endl;
    int l=itemProject2->rowCount();

    for(int i=0;i<l;i++)
    {
     QModelIndex indexx= itemProject2->child(i)->index();

     int idx=indexx.data().toInt();
     if(idx==id)
     {
         itemProject2->setChild(indexx.row(),1,new QStandardItem(QStringLiteral("在线")));

         m_agvInuse.removeOne(id);
     }

   }
}

void AgvInformation::outSystem(int id)
{
    int l=itemProject2->rowCount();
    int ll=itemProject2PS20->rowCount();
    for(int i=0;i<l;i++)
    {
     QModelIndex indexx= itemProject2->child(i)->index();

     int idx=indexx.data().toInt();
     if(idx==id)
     {
         itemProject2->setChild(indexx.row(),1,new QStandardItem(QStringLiteral("离线")));

     }
     for(int i=0;i<ll;i++)
     {
      QModelIndex indexx= itemProject2PS20->child(i)->index();

      int idx=indexx.data().toInt();
      if(idx==id)
      {
          itemProject2PS20->setChild(indexx.row(),1,new QStandardItem(QStringLiteral("离线")));

      }


     }
    }
}
void AgvInformation::createTreeView()
{
   m_treeView=new QTreeView;
   m_treeView->setMinimumSize(350,200);

}
void AgvInformation::createTableWidget()
{
    m_tableAgvAnuse = new QTableView;
    m_tableAgvAnuse->setMaximumSize(352,200);
    m_tableAgvAnuse->setMinimumSize(352,200);
    int x = m_tableAgvAnuse->geometry().x();
    int y = m_tableAgvAnuse->geometry().y();
    m_tableAgvAnuse->setGeometry(x,y,352,200);
    m_tableAgvAnuse->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableAgvAnuse->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tableAgvInuse = new QTableView;
    m_tableAgvInuse->setMaximumSize(352,200);
    m_tableAgvInuse->setMinimumSize(352,200);
    int x1 = m_tableAgvInuse->geometry().x();
    int y1 = m_tableAgvInuse->geometry().y();
    m_tableAgvInuse->setGeometry(x1,y1,352,200);
    m_tableAgvInuse->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableAgvInuse->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void AgvInformation::creatButton()
{
    m_btnCreate = new QPushButton("新建",this);
    m_btnEdit = new QPushButton("编辑",this);
    m_btnHang = new QPushButton("执行",this);
    m_btnDelete = new QPushButton("删除",this);
    m_addAgv =new QLabel (this);
    m_addAgv->setText("加入=>");
    m_removeAgv=new QLabel("移除<=");
}
void AgvInformation::creatLayout()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(m_tableAgvAnuse,0,0,4,4);
    mainLayout->addWidget(m_tableAgvInuse,0,5,4,4);
    mainLayout->addWidget(m_addAgv,1,4);
    mainLayout->addWidget(m_removeAgv,2,4);
    mainLayout->addWidget(m_btnCreate,4,0);
    mainLayout->addWidget(m_btnEdit,4,1);
    mainLayout->addWidget(m_btnHang,4,2);
    mainLayout->addWidget(m_btnDelete,4,3);
    setLayout(mainLayout);
}
void AgvInformation::open_database()
{


    QSqlQuery query = getQuery();
    //query.exec("create table if not exists tb_wmsdeport ( row int, col int, layer int, node int )");
    query.exec("select * from tb_agv");
    while(query.next()){
         QSqlRecord record = query.record();
         QString name = record.value("name").toString();
         int id = record.value("id").toInt();
         //int id = record.value("id").toInt();
         int chexing=record.value("type").toInt();//以后车型统一用type
         qDebug()<<"车型为"<<chexing<<endl;
         int angle=record.value("startangle").toInt();
         int startNode = record.value("startNode").toByteArray().toInt(0,16);
         int  address_1 = record.value("address_1").toInt();
         int  address_2 = record.value("address_2").toInt();
         QString type = record.value("type").toString();
         int chargeSation=record.value("chargeStation").toInt();
         QStringList chargeLimit = record.value("chargeLimit").toString().split(",");
         m_agvall.append(id);
         m_idName.insert(id,name);
         m_idStartNode.insert(id,startNode);
         m_idAngle.insert(id,angle);
         m_idChexing.insert(id,chexing);
         m_idAddress_1.insert(id,address_1);
         m_idAddress_2.insert(id,address_2);
         m_idType.insert(id,type);
         m_idChargeStation.insert(id,chargeSation);
         m_idChargeLimit.insert(id,chargeLimit);

    }

   // m_agvTable = new QSqlTableModel(this,db);
    //m_agvTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
   // show_agvInda();

}
void AgvInformation::show_agvInda()
{
    m_agvTable->setTable("tb_agv");
    m_agvTable->setHeaderData(0,Qt::Horizontal,"名称");
    m_agvTable->setHeaderData(1,Qt::Horizontal,"ID");
    m_agvTable->setHeaderData(2,Qt::Horizontal,"休息");
    m_agvTable->setHeaderData(3,Qt::Horizontal,"地址");
    m_agvTable->setHeaderData(4,Qt::Horizontal,"类型");
    m_agvTable->setHeaderData(5,Qt::Horizontal,"充电");
    m_agvTable->setHeaderData(6,Qt::Horizontal,"界限");
    m_agvTable->select();
    m_tableAgvAnuse->setModel(m_agvTable);
    m_tableAgvAnuse->setColumnWidth(0,45);
    m_tableAgvAnuse->setColumnWidth(1,45);
    m_tableAgvAnuse->setColumnWidth(2,45);
    m_tableAgvAnuse->setColumnWidth(3,45);
    m_tableAgvAnuse->setColumnWidth(4,45);
    m_tableAgvAnuse->setColumnWidth(5,45);
    m_tableAgvAnuse->setColumnWidth(6,60);

}
