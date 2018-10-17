#include "mainwindow.h"
#include <QPixmap>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QDebug>
#include <QTransform>
#include <QGridLayout>
#include <QPalette>
#include <QStatusBar>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QApplication>
#include <QDesktopWidget>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlResult>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextCodec>
#include <QTcpServer>
#include <QTcpSocket>
#include <QGraphicsSimpleTextItem>
#include <QLatin1String>
#include <QVBoxLayout>
#include <QListWidget>
#include <QVector>
#include <QDesktopWidget>
#include <QTime>
//#include "rectitem.h"
//#include "ellipseitem.h"
//#include "lineitem.h"
//#include "textitem.h"
//#include "commands.h"
#include "global.h"
//#include "reminddialog.h"
#include "agv.h"
//#include "settingdlg.h"
// 属性栏头文件
#include "missionmanager.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include <QPen>

#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include "rectitem.h"

#define WINDOWTITLE_PREFIX QString("AGVFactory - ")
#define SETTING_PREFIX_REMIND QString("/remind/")
#define SETTING_PREFIX_PATH QString("/path/")

#define SETTING_SUFFIX_CURRENTPATH QString("currentPath")
#define SETTING_SUFFIX_HISTORYPATH QString("historyPath")
#define SETTING_SUFFIX_OPENFILEPATH QString("openFilePath")

#define FRAME_FROM_AGV 0x40
#define FRAME_FROM_REQUEST 0x7e

#define diaoxianchonglian  1
const QString MimeType="AGVFactory";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_progressBar = new QProgressBar;
    m_progressBarLabel = new QLabel;
    //m_settingDlg = new SettingDlg;
    m_trafficManager = NULL;
    m_missionManager = NULL;
    m_requestManager = NULL;
    m_sweptArea=NULL;
    m_agvinformation=NULL;
    serial = NULL;
    multi=g_setting->value("/multiSystem/multi").toBool();


    QString path = g_setting->value(SETTING_PREFIX_PATH+SETTING_SUFFIX_CURRENTPATH).toString();
    qDebug()<<"path测试"<<path;
    if(path!=""){
        setWindowTitle(WINDOWTITLE_PREFIX+path);
        openDatabase(path);
       //std::string S="E:/AGV System/jiemianxiugai/software/build-AGVFactory-Desktop_Qt_5_7_0_MinGW_32bit-Debug/duoche20170607.dxf";
        //cout<<S;
        //std::string S="a";
        QString p= path.remove(path.length()-3,3);
        qDebug()<<"p"<<p;
        QString ps=p.append(".dxf");
        std::string S=ps.toStdString();
        createSceneAndView(S,0);
        createActions();
        createDockWindow();
        createPropertyWindow();
        m_sceneHigh=m_centralScene->height();
        m_sceneCoordinate=coordinatemove;
        m_sceneZoom=zoom;
        m_topoView->load_topoMap(zoom,coordinatemove,m_centralScene->height());

        m_topoView->DistanceJudge();
        qDebug()<<"错误测试11"<<endl;
        drawDXF(m_centralScene,m_ncentralView,creationClass);
    }
    else
    {
    }
    statusBar()->addWidget(m_progressBar);
    statusBar()->addWidget(m_progressBarLabel);
    statusBar()->hide();
    reset_documentDirty();

    set_appState(APP_STOP);
    createConnections();


    m_trayIcon = new QSystemTrayIcon(this);
    connect(m_trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
    QIcon icon(":/action/images/trayIcon.ico");
    m_trayIconMenu = new QMenu;
    m_trayIconMenu->addAction(tr("退出"),qApp,SLOT(quit()));
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setIcon(icon);
    m_trayIcon->setToolTip("AGV");
    m_trayIcon->show();
    agvNumChange=false;


    ser.listen(QHostAddress::Any,6666);

    QObject::connect(&ser,SIGNAL(connectToSystem(int,int)),this,SLOT(addAGVasFangzhen(int,int)));
     connect(this,SIGNAL(writeToServer(int,QByteArray)),&ser,SLOT(receviedFormAGV(int,QByteArray)));

}

MainWindow::~MainWindow()
{
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QMainWindow::mousePressEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
   // if(m_bKeyLock) return;
    switch(event->key()){
    case 16777216:  //ESC
        updateToolbarUI();
        //m_centralView->prepareToDrawItem(NONE);
        //m_centralView->prepareToMethod(NONE);
        break;
    case 16777223:  // delete
        if (m_centralScene->selectedItems().isEmpty())
            break;
        foreach(QGraphicsItem* item,m_centralScene->selectedItems()){
            if(item->type()==EDGE){
                //m_topoView->remove_topoEdge((TopoEdge*)item);
            }
            else{
               // QUndoCommand *deleteCommand = new DeleteCommand(m_centralScene,item,m_centralView->m_topoView);
               // m_centralView->m_undoStack->push(deleteCommand);
            }
        }
        m_centralScene->clearSelection();
        break;
    }
}



void MainWindow::createSceneAndView(string f,int flag)
{

     int i=0;

     xleftborder=0;
     xrightborder=0;
     ytopborder=0;
     ybuttomborder=0;
    //std::string  q="E:/Qt_project/xianshiCAD/demo.dxf";
    creationClass = new Test_CreationClass();
    DL_Dxf* dxf = new DL_Dxf();
    if (!dxf->in(f, creationClass)) { // if file open failed
        std::cerr << f << " could not be opened.\n";
    }
    getimgdxfborder(creationClass);
   // qDebug()<<xleftborder<<""<<xrightborder<<""<<ytopborder<<""<<ybuttomborder;
    QRect rect = QApplication::desktop()->rect();
    qreal  w=rect.width();
    qreal  h=rect.height();


//------------------------求取缩放比例---------------------------------------
    QSqlQuery query = getQuery();
    if(!query.exec("select * from tb_topover")) return ;
    while(query.next())
    {

       QSqlRecord record = query.record();
       qreal realx= record.value("x").toReal();
       qreal realy= record.value("y").toReal();
       if(i==0)
       {
           xleftborder=xrightborder=realx*1000;
           ytopborder=ybuttomborder=realy*1000;

       }


       if(xrightborder<realx*1000)
          xrightborder =realx*1000;
       if(xleftborder>realx*1000)
           xleftborder=realx*1000;
       if(ybuttomborder<realy*1000)
           ybuttomborder=realy*1000;
       if(ytopborder>realy*1000)
           ytopborder=realy*1000;
       i++;
    }
//------------------------求取缩放比例---------------------------------------

   qDebug()<<"xrigthborder"<<xrightborder;
   qDebug()<<"xleftborder"<<xleftborder;
   qDebug()<<"ybuttomborder"<<ybuttomborder;
   qDebug()<<"ytopborder"<<ytopborder;
   
   qreal  ratioW=(xrightborder - xleftborder)*1.4/w;
   qreal  ratioH=(ybuttomborder - ytopborder)*1.4/h;
    if(ratioW>=ratioH)
    {
        zoom=1/ratioW;
    }
    else
    {
        zoom=1/ratioH;
    }
    qDebug()<<zoom;

    if(flag==0)
       m_centralScene = new CentralScene;
    //QRect rect = QApplication::desktop()->rect();

    //m_centralScene->setSceneRect(rect.x(),rect.y(),rect.width()-50,rect.bottom()-300);
    coordinatemove.setX(((xrightborder - xleftborder)*0.1- xleftborder)*zoom) ;
    coordinatemove.setY(((ybuttomborder - ytopborder)*0.1- ytopborder)*zoom)  ;

    qDebug()<<coordinatemove;

     QRectF rectX=QRectF(0,0,(xrightborder-xleftborder)*1.2*zoom,(ybuttomborder-ytopborder)*1.2*zoom);


     //QRectF rectX=QRectF(QPointF(xleftborder*zoom,ytopborder*zoom),QPointF(xrightborder*zoom,ybuttomborder*zoom));
    m_centralScene->setSceneRect(rectX);
    m_topoView = new TopoView(m_centralScene);
   // m_centralView = new CentralView(0,m_centralScene,m_topoView);
    m_ncentralView= new nCentralView(m_centralScene);

    setCentralWidget(m_ncentralView);
}

void MainWindow::createSceneAndView()
{

    xleftborder=0;
    xrightborder=0;
    ytopborder=0;
    ybuttomborder=0;


}
void MainWindow::getimgdxfborder(Test_CreationClass *creationClass)
{
    vector<BlockObj> ::iterator blockItor;//块容器迭代器
    blockItor = creationClass->myblock.begin();
    while (blockItor != creationClass->myblock.end())
    {
        if (blockItor->drawflag==false)
        {
            blockItor++;
            continue;
        }
        //直线
        vector<DXFLine> ::iterator lineItor;//块容器迭代器
        lineItor = blockItor->line.begin();
        while (lineItor != blockItor->line.end())
        {
            getWorldCoordinateborder(blockItor, lineItor->beginpoint);
            getWorldCoordinateborder(blockItor, lineItor->endpoint);
            lineItor++;
        }
        //圆
        vector<DXFCircle> ::iterator circleItor;//块容器迭代器
        circleItor = blockItor->circle.begin();
        while (circleItor != blockItor->circle.end())
        {
            getWorldCoordinateborder(blockItor, circleItor->centerpoint);
            //int radius = circleItor->radius*blockItor->sx * zoom;
            circleItor++;
        }
        //圆弧
        vector<DXFArc> ::iterator arcItor;//块容器迭代器
        arcItor = blockItor->arc.begin();
        while (arcItor != blockItor->arc.end())
        {
            getWorldCoordinateborder(blockItor, arcItor->centerpoint);
            //int radius = arcItor->radius*blockItor->sx * zoom;

            arcItor++;
        }
        //绘制多线实体
        vector<DXFPolyLineEntities> ::iterator polylineentitiesItor;//多线实体容器迭代器
        polylineentitiesItor = blockItor->polylineentities.begin();
        while (polylineentitiesItor != blockItor->polylineentities.end())
        {
            for (size_t i = 0; i < polylineentitiesItor->vertex.size(); i++)
            {
                int nextvertex = i + 1;
                if (nextvertex == polylineentitiesItor->vertex.size())
                {
                    if (polylineentitiesItor->isclose == true)//闭合则画终点到起点的线
                    {
                        nextvertex = 0;
                    }
                    else
                    {
                        break;
                    }
                }
                getWorldCoordinateborder(blockItor, polylineentitiesItor->vertex[i]);
                getWorldCoordinateborder(blockItor, polylineentitiesItor->vertex[nextvertex]);
            }
            polylineentitiesItor++;
        }
      blockItor++;
    }
}

void MainWindow::getimgdxfborder()
{
     QSqlQuery query = getQuery();
    if(!query.exec("select * from tb_topover")) return;
    qreal posx,posy;
    quint64 id=0;
    while(query.next()){
        QSqlRecord record = query.record();
        id = record.value("NodeID").toInt();
        //m_map.insert(id,s_nID++);  // 为了创建邻接矩阵
        posx= record.value("x").toReal();
        posy= record.value("y").toReal();

    }


}
void MainWindow::getWorldCoordinateborder(vector<BlockObj> ::iterator myblock, QPointF mypoint)
{
    QPointF worldcoordinatepoint;
    worldcoordinatepoint.setX((mypoint.x()*myblock->sy + myblock->ipx) );
    worldcoordinatepoint.setY((mypoint.y()*myblock->sy + myblock->ipy) );
    if (xleftborder>worldcoordinatepoint.x())
    {
        xleftborder = worldcoordinatepoint.x();
       // qDebug()<<xleftborder<<""<<mypoint.x()<<""<<myblock->sy<<""<<myblock->ipx;
    }
    if (xrightborder<worldcoordinatepoint.x())
    {
        xrightborder = worldcoordinatepoint.x();
    }
    if (ytopborder>worldcoordinatepoint.y())
    {
        ytopborder = worldcoordinatepoint.y();
    }
    if (ybuttomborder<worldcoordinatepoint.y())
    {
        ybuttomborder = worldcoordinatepoint.y();
    }
}
void MainWindow::drawDXF(QGraphicsScene *scene, QGraphicsView *, Test_CreationClass *creationClass)
{
    vector<BlockObj> ::iterator blockItor;//块容器迭代器
    blockItor = creationClass->myblock.begin();
    while (blockItor != creationClass->myblock.end())
    {
        if (blockItor->drawflag == false)
        {
            blockItor++;
            continue;
        }
        //绘制直线
        vector<DXFLine> ::iterator lineItor;//块容器迭代器
        lineItor = blockItor->line.begin();
        while (lineItor != blockItor->line.end())
        {
            QPointF	bpoint = setWorldCoordinate(scene->height(), blockItor, lineItor->beginpoint);
            QPointF epoint = setWorldCoordinate(scene->height(), blockItor, lineItor->endpoint);
            //scene->addLine(bpoint.x(),bpoint.y(),epoint.x(),epoint.y());
            new LineItem(scene,bpoint,epoint);
            lineItor++;
        }
        //绘制圆
        vector<DXFCircle> ::iterator circleItor;//块容器迭代器
        circleItor = blockItor->circle.begin();
        while (circleItor != blockItor->circle.end())
        {
            QPointF cpoint = setWorldCoordinate(scene->height(), blockItor, circleItor->centerpoint);
            int radius = circleItor->radius*blockItor->sx * zoom;

           scene->addEllipse(cpoint.x()-radius,cpoint.y()-radius,radius*2,radius*2);
            circleItor++;
        }
        //绘制圆弧
        vector<DXFArc> ::iterator arcItor;//块容器迭代器
        arcItor = blockItor->arc.begin();
        while (arcItor != blockItor->arc.end())
        {


            if ((arcItor->bangle)<(arcItor->eangle))//逆时针圆弧
            {
                QPointF cpoint = setWorldCoordinate(scene->height(), blockItor, arcItor->centerpoint);
                int radius =(((arcItor->radius)*(blockItor->sx))) * zoom;
                qreal startAngle=arcItor->bangle;
                qreal spanAngle=arcItor->eangle-arcItor->bangle;
                new ArcItem(scene,QPointF(cpoint.x()-radius,cpoint.y()-radius),QPointF(cpoint.x()+radius,cpoint.y()+radius),startAngle,spanAngle);
            }
            else//顺时针画圆弧
            {
                QPointF cpoint = setWorldCoordinate(scene->height(), blockItor, arcItor->centerpoint);
                int radius =(((arcItor->radius)*(blockItor->sx))) * zoom;


                qreal startAngle= -(360 - arcItor->bangle);
                qreal spanAngle=arcItor->eangle-startAngle;
                new ArcItem(scene,QPointF(cpoint.x()-radius,cpoint.y()-radius),QPointF(cpoint.x()+radius,cpoint.y()+radius),startAngle,spanAngle);
            }
            arcItor++;
        }
        //绘制多线实体
        vector<DXFPolyLineEntities> ::iterator polylineentitiesItor;//多线实体容器迭代器
        polylineentitiesItor = blockItor->polylineentities.begin();
        while (polylineentitiesItor != blockItor->polylineentities.end())
        {
            QList<QPointF> poly;
            bool flag;
            int idx;
                for (size_t i = 0; i < polylineentitiesItor->vertex.size(); i++)
               {
                   int nextvertex = i + 1;
                   if (nextvertex == polylineentitiesItor->vertex.size())
                   {
                       if (polylineentitiesItor->isclose == true)//闭合则画终点到起点的线
                       {
                           nextvertex = 0;
                       }
                       else
                       {
                           break;
                       }
                   }
                   QPointF bpoint = setWorldCoordinate(scene->height(), blockItor, polylineentitiesItor->vertex[i]);
                   QPointF epoint = setWorldCoordinate(scene->height(), blockItor, polylineentitiesItor->vertex[nextvertex]);
                   if(!poly.contains(bpoint))
                   poly.append(bpoint);
                   //scene->addLine(bpoint.x(),bpoint.y(),epoint.x(),epoint.y());
               }
                foreach (int id, m_topoView->m_kuwei)
                {
                    qreal x=m_topoView->m_verList.value(QString("Ver_%1").arg(id))->get_x();
                    qreal y=m_topoView->m_verList.value(QString("Ver_%1").arg(id))->get_y();
                    flag=isVerInArea(QPointF(x,y),poly);
                    if(flag==true)
                    {
                        idx=id;
                        break;
                    }
                }
                if(flag==true)
                {
                    qreal x=poly.at(0).x();
                    qreal y=poly.at(0).y();
                    qreal w=poly.at(2).x()-poly.at(0).x();
                    qreal h=poly.at(2).y()-poly.at(0).y();
                   //scene->addRect(x,y,w,h);
                   // qDebug()<<idx<<x<<y<<w<<h;
                    qreal sita= m_topoView->m_verList.value(QString("Ver_%1").arg(idx))->get_sita();
                    sita=0;//sita表示图中工位字体显示的方向
                    int SLNumber= m_topoView->m_verList.value(QString("Ver_%1").arg(idx))->SLNumber;
                    QSqlQuery query = getQuery();
                    for(int i = 0; i < 8; ++i){
                        for(int j = 0; j < 4; ++j){
                            rectitems.append(new RectItem(scene,x,y,w,h,SLNumber,sita,4,j));//地图中方框显示为工位号，非RFID
                        }
                    }
                    if(!query.prepare("select * from tb_storage WHERE NodeID = ?"));
                    query.addBindValue(idx);
                    query.exec();
                    while(query.next())
                    {
                        QSqlRecord record = query.record();
                        int number;
                        if(record.value("NodeID").toInt() % 10){
                            number = record.value("NodeID").toInt() / 10 + 1 + record.value("layer").toInt();
                        }else {
                            number = record.value("NodeID").toInt() / 10 * 2 - 1 + record.value("layer").toInt();
                        }
                        rectitems.replace(number, new RectItem(scene,x,y,w,h,idx,sita,4,record.value("layer").toInt() - 1));
                        //rectitems.append(new RectItem(scene,x,y,w,h,idx,sita,count,record.value("layer").toInt() - 1));
                    }
                }
                else
                {
                    qreal x=poly.at(0).x();
                    qreal y=poly.at(0).y();
                    qreal w=poly.at(2).x()-poly.at(0).x();
                    qreal h=poly.at(2).y()-poly.at(0).y();
                    //scene->addRect(x,y,w,h);
//                    qreal sita= m_topoView->m_verList.value(QString("Ver_%1").arg(idx))->get_sita();
//                    new RectItem(scene,x,y,w,h,idx,sita);
                }
            polylineentitiesItor++;
        }

        blockItor++;
    }
}
bool MainWindow::isVerInArea(QPointF p, QList<QPointF> poly)
{
    qreal px=p.x();
    qreal py=p.y();
    bool  flag=false;
    int l=poly.length();
    int j=l-1;
    for(int i=0;i<l;i++)
    {

         qreal sx=poly.at(i).x();
         qreal sy=poly.at(i).y();
         qreal tx=poly.at(j).x();
         qreal ty=poly.at(j).y();
         // 点与多边形顶点重合
         if((sx == px && sy == py) || (tx == px && ty == py))
         {
                 return true;
         }
         // 判断线段两端点是否在射线两侧
           if((sy < py && ty >= py) || (sy >= py && ty < py))
           {
             qreal x = sx + (py - sy) * (tx - sx) / (ty - sy);
             // 点在多边形的边上
                if(x == px) {
                  return true;
                }
                // 射线穿过多边形的边界
                if(x >= px) {
                  flag = !flag;
                }
           }

         j=i;

    }
    return flag;
}
QPointF MainWindow::setWorldCoordinate(int rows, vector<BlockObj>::iterator myblock, QPointF mypoint)
{
    QPointF worldcoordinatepoint;
    worldcoordinatepoint.setX(coordinatemove.x() + (mypoint.x()*myblock->sx + myblock->ipx) * zoom)  ;
    worldcoordinatepoint.setY(rows-((mypoint.y()*myblock->sy + myblock->ipy) * zoom+ coordinatemove.y())) ;
    return worldcoordinatepoint;
}
void MainWindow::createPropertyWindow()
{
    doubleManager = new QtDoublePropertyManager(this);
    stringManager = new QtStringPropertyManager(this);
    colorManager = new QtColorPropertyManager(this);
    fontManager = new QtFontPropertyManager(this);
    pointManager = new QtPointPropertyManager(this);
    sizeManager = new QtSizePropertyManager(this);
    boolManager = new QtBoolPropertyManager(this);
    intManager = new QtIntPropertyManager(this);


    doubleAgvInfor = new QtDoublePropertyManager(this);
    stringAgvInfor = new QtStringPropertyManager(this);
   // colorAgvInfor = new QtColorPropertyManager(this);
   // fontAgvInfor = new QtFontPropertyManager(this);
    pointAgvInfor = new QtPointPropertyManager(this);
    sizeAgvInfor = new QtSizePropertyManager(this);
    boolAgvInfor = new QtBoolPropertyManager(this);
    intAgvInfor = new QtIntPropertyManager(this);

    groupManager =new QtGroupPropertyManager(this);

    connect(doubleManager, SIGNAL(valueChanged(QtProperty *, double)),
                this, SLOT(valueChanged(QtProperty *, double)));
    connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
                this, SLOT(valueChanged(QtProperty *, const QString &)));
    connect(colorManager, SIGNAL(valueChanged(QtProperty *, const QColor &)),
                this, SLOT(valueChanged(QtProperty *, const QColor &)));
    connect(fontManager, SIGNAL(valueChanged(QtProperty *, const QFont &)),
                this, SLOT(valueChanged(QtProperty *, const QFont &)));
    connect(pointManager, SIGNAL(valueChanged(QtProperty *, const QPoint &)),
                this, SLOT(valueChanged(QtProperty *, const QPoint &)));
    connect(sizeManager, SIGNAL(valueChanged(QtProperty *, const QSize &)),
                this, SLOT(valueChanged(QtProperty *, const QSize &)));
    connect(boolManager,SIGNAL(valueChanged(QtProperty*,bool)),
            this,SLOT(valueChanged(QtProperty*,bool)));
    connect(intManager,SIGNAL(valueChanged(QtProperty*,int)),this,SLOT(valueChanged(QtProperty*,int)));

    QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
    QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
    QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);

    m_propertyWindow = new QDockWidget(this);
    m_propertyWindow->setWindowTitle("属性栏");
    //m_propertyWindow->setFixedHeight(m_centralScene->height()*0.2);
    //m_propertyWindow->setHidden(true);
    m_propertyWindow->setMaximumHeight(m_centralScene->height()*0.2);
    m_propertyWindow->setAutoFillBackground(true);
   // m_propertyWindow->setMinimumHeight(200);
    addDockWidget(Qt::RightDockWidgetArea, m_propertyWindow);



    propertyEditor = new QtTreePropertyBrowser(m_propertyWindow);
    propertyEditor->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
    propertyEditor->setFactoryForManager(stringManager, lineEditFactory);
    propertyEditor->setFactoryForManager(colorManager->subIntPropertyManager(), spinBoxFactory);
    propertyEditor->setFactoryForManager(fontManager->subIntPropertyManager(), spinBoxFactory);
    propertyEditor->setFactoryForManager(fontManager->subBoolPropertyManager(), checkBoxFactory);
    propertyEditor->setFactoryForManager(fontManager->subEnumPropertyManager(), comboBoxFactory);
    propertyEditor->setFactoryForManager(pointManager->subIntPropertyManager(), spinBoxFactory);
    propertyEditor->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
    propertyEditor->setFactoryForManager(boolManager,checkBoxFactory);
    propertyEditor->setFactoryForManager(intManager,spinBoxFactory);


    m_propertyWindow->setWidget(propertyEditor);

    m_propertyWindow2 = new QDockWidget(this);
    m_propertyWindow2->setWindowTitle("AGV信息栏");
    addDockWidget(Qt::RightDockWidgetArea, m_propertyWindow2);
    propertyEditor2 = new QtTreePropertyBrowser(m_propertyWindow2);
    propertyEditor2->setFactoryForManager(doubleAgvInfor, doubleSpinBoxFactory);
   // propertyEditor2->setFactoryForManager(stringManager, lineEditFactory);
   // propertyEditor2->setFactoryForManager(colorManager->subIntPropertyManager(), spinBoxFactory);
   // propertyEditor2->setFactoryForManager(fontManager->subIntPropertyManager(), spinBoxFactory);
   // propertyEditor2->setFactoryForManager(fontManager->subBoolPropertyManager(), checkBoxFactory);
  //  propertyEditor2->setFactoryForManager(fontManager->subEnumPropertyManager(), comboBoxFactory);
    propertyEditor2->setFactoryForManager(pointAgvInfor->subIntPropertyManager(), spinBoxFactory);
   // propertyEditor2->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
    propertyEditor2->setFactoryForManager(boolAgvInfor,checkBoxFactory);
    propertyEditor2->setFactoryForManager(intAgvInfor,spinBoxFactory);

   /* QtProperty *item1 = groupManager->addProperty("AGV_100");
    QtProperty *item11 = pointAgvInfor->addProperty("位置");
    item1->addSubProperty(item11);
    QtProperty *item12 = intAgvInfor->addProperty("电量");
    item1->addSubProperty(item12);
    QtProperty *item13 = boolAgvInfor->addProperty("通信");
    item1->addSubProperty(item13);
    propertyEditor2->addProperty(item1);
    */
    /*QtProperty *item2 = groupManager->addProperty("AGV_200");
    QtProperty *item21 = pointAgvInfor->addProperty("位置");
    item2->addSubProperty(item21);
    QtProperty *item22 = intAgvInfor->addProperty("电量");
    item2->addSubProperty(item22);
    QtProperty *item23 = boolAgvInfor->addProperty("通信");
    item2->addSubProperty(item23);

    propertyEditor2->addProperty(item2);
    */
    m_propertyWindow2->setWidget(propertyEditor2);
}

void MainWindow::createActions(){
    QMenu *menu_file = new QMenu("文件",this);
    QMenu *menu_edit = new QMenu("编辑",this);
   // QMenu *menu_tool = new QMenu("工具",this);
    QMenu *menu_run = new QMenu("运行",this);
    QMenu *menu_view = new QMenu("视图",this);

    QMenu *menu_window = new QMenu("窗口",this);
    QMenu *menu_agv  = new QMenu("AGV信息",this);
    QMenu *menu_help = new QMenu("帮助",this);
    QMenu *menu_storage = new QMenu(tr("库位管理"),this);

    //库位管理菜单
    action_add = new QAction(tr("添加"),this);
    action_add->setShortcut(tr("Ctrl+A"));
    action_remove = new QAction(tr("移除"),this);
    action_remove->setShortcut(tr("Ctrl+R"));
    action_enable = new QAction(tr("显示"),this);
    action_enable->setShortcut(tr("Ctrl+E"));
    action_disable = new QAction(tr("隐藏"),this);
    action_disable->setShortcut(tr("Ctrl+D"));
    action_adjust = new QAction(tr("修改"),this);
    action_adjust->setShortcut(tr("Ctrl+T"));
    menu_storage->addAction(action_add);
    menu_storage->addAction(action_remove);
    menu_storage->addAction(action_enable);
    menu_storage->addAction(action_disable);
    menu_storage->addAction(action_adjust);


    action_open = menu_file->addAction(QPixmap(":/action/images/open.ico"),"打开");
    action_open->setShortcut(QKeySequence::Open);
    action_save = menu_file->addAction(QPixmap(":/action/images/save.ico"),"保存");
    action_save->setShortcut(QKeySequence::Save);
    //action_export = menu_file->addAction(QPixmap(":/action/images/export.ico"),"导出");
    menu_file->addSeparator();
    QStringList list = get_recentOpenFileName();
    for(int i=0;i<list.length();i++){
        QAction *action = new QAction(list.at(i),this);
        action->setData(list.at(i));
        connect(action,SIGNAL(triggered()),this,SLOT(open_recentFile()));
        menu_file->addAction(action);
    }
    action_exit = menu_file->addAction(QPixmap(":/action/images/exit.ico"),"退出");
   // action_undo = m_centralView->m_undoStack->createUndoAction(this,"&Undo");
   // action_undo->setShortcut(QKeySequence::Undo);
    //action_undo->setIcon(QPixmap(":/action/images/undo.ico"));
  //  menu_edit->addAction(action_undo);
    menu_edit->addSeparator();
    menu_edit->addSeparator();
    action_rotate = menu_edit->addAction(QPixmap(":/action/images/rotation.ico"),"顺时针旋转");
    action_antiRotate=menu_edit->addAction(QPixmap(":/action/images/rotationAnti.ico"),"逆时针旋转");
    //action_translateUpDown = menu_edit->addAction(QPixmap(":/action/images/translation_updown.ico"),"上下平移");
    //action_translateLeftRight = menu_edit->addAction(QPixmap(":/action/images/translation_leftright.ico"),"左右平移");
    action_up=menu_edit->addAction(QPixmap(":/action/images/vup.ico"),"上移");
    action_down=menu_edit->addAction(QPixmap(":/action/images/vdown.ico"),"下移");
    action_left=menu_edit->addAction(QPixmap(":/action/images/vleft.ico"),"左移");
    action_right=menu_edit->addAction(QPixmap(":/action/images/vright.ico"),"右移");
    menu_edit->addSeparator();
    action_zoomin = menu_edit->addAction(QPixmap(":/action/images/zoomin.ico"),"放大");
    action_zoomin->setShortcut(QKeySequence("="));
    action_zoomout = menu_edit->addAction(QPixmap(":/action/images/zoomout.ico"),"缩小");
    action_zoomout->setShortcut(QKeySequence("-"));

    action_missionManager = menu_run->addAction(QPixmap(":/action/images/missionManager.ico"),"任务管理器");
    action_requestManager = menu_run->addAction(QPixmap(":/action/images/bell.ico"),"呼叫管理器");
    menu_run->addSeparator();
    action_multiSystem=menu_run->addAction("多系统");
    action_multiSystem->setCheckable(true);
    action_multiSystem->setChecked(multi);
    action_prepare = menu_run->addAction("准备");
    action_prepare->setShortcut(QKeySequence("F4"));
    action_run = menu_run->addAction(QPixmap(":/action/images/run.ico"),"运行");
    action_run->setShortcut(QKeySequence("F5"));
    action_stop = menu_run->addAction(QPixmap(":/action/images/stop.ico"),"停止");
    action_stop->setShortcut(QKeySequence("F6"));
    action_showNodeName = menu_view->addAction("显示节点RFID");
    action_showNodeName->setCheckable(true);
    menu_run->addSeparator();
    action_communicationSettings = menu_run->addAction(QPixmap(":/action/images/settings.ico"),"设置");
    action_toolbar = menu_window->addAction(QPixmap(":/action/images/toobar.ico"),"工具栏");
    action_toolbar->setCheckable(true);
    action_toolbar->setChecked(true);
    action_propertyWindow = menu_window->addAction(QPixmap(":/action/images/propertyWindow.ico"),"属性栏");
    action_propertyWindow->setCheckable(true);
    action_propertyWindow->setChecked(true);
    action_agvInformation=menu_agv->addAction(QPixmap(":/action/images/AGV.ico"),"AGV加载");
    menu_agvinfo=menu_agv->addMenu(tr("AGV信息"));
   // QAction *agv1=menu_agvinfo->addAction(QString("agv_%1").arg(100));
   // agv1->setCheckable(true);
    action_about = menu_help->addAction(QPixmap(":/action/images/about.ico"),"关于 AGVFactory");
    action_help = menu_help->addAction(QPixmap(":/action/images/help.ico"),"帮助");

    //spin_zoom = new QSpinBox;
    //spin_zoom->setRange(10,500);
   // spin_zoom->setValue(100);
    //spin_zoom->setSingleStep(10);
    m_centralMenuBar = menuBar();
    m_centralMenuBar->addMenu(menu_file);
    m_centralMenuBar->addMenu(menu_edit);
    //m_centralMenuBar->addMenu(menu_tool);
    m_centralMenuBar->addMenu(menu_run);
    m_centralMenuBar->addMenu(menu_agv);
    m_centralMenuBar->addMenu(menu_storage);
    m_centralMenuBar->addMenu(menu_view);

   // m_centralMenuBar->addMenu(menu_window);
    m_centralMenuBar->addMenu(menu_help);

    m_centralToolBar = addToolBar("工具栏");
    m_centralToolBar->addAction(action_open);
    m_centralToolBar->addAction(action_save);
   // m_centralToolBar->addAction(action_undo);
    m_centralToolBar->addSeparator();
    m_centralToolBar->addAction(action_rotate);
   // m_centralToolBar->addAction(action_antiRotate);
   // m_centralToolBar->addAction(action_translateUpDown);
   // m_centralToolBar->addAction(action_translateLeftRight);
   // m_centralToolBar->addAction(action_up);
  //  m_centralToolBar->addAction(action_down);
  //  m_centralToolBar->addAction(action_left);
   // m_centralToolBar->addAction(action_right);
    m_centralToolBar->addSeparator();
    m_centralToolBar->addAction(action_zoomin);
    //m_centralToolBar->addWidget(spin_zoom);
    //m_centralToolBar->addAction(action_zoomout);
    m_centralToolBar->addSeparator();

    m_centralToolBar->addAction(action_missionManager);
    m_centralToolBar->addAction(action_agvInformation);
}

void MainWindow::createConnections(){
    //connect(m_centralView,SIGNAL(itemMoving(QGraphicsItem*)),this,SLOT(itemMoved(QGraphicsItem*)));
    //connect(m_centralView,SIGNAL(itemDrawing(QGraphicsItem*)),this,SLOT(itemDrawn(QGraphicsItem*)));
    connect(m_centralScene,SIGNAL(selectionChanged()),this,SLOT(selectChange()));
    connect(action_propertyWindow,SIGNAL(triggered(bool)),m_propertyWindow,SLOT(setVisible(bool)));
   // connect(m_centralView,SIGNAL(set_keyLock(bool)),this,SLOT(setKeyLock(bool)));
    connect(m_propertyWindow,SIGNAL(visibilityChanged(bool)),action_propertyWindow,SLOT(setChecked(bool)));
    connect(action_toolbar,SIGNAL(triggered(bool)),m_centralToolBar,SLOT(setVisible(bool)));

    //connect(action_zoomin,SIGNAL(triggered()),spin_zoom,SLOT(stepUp()));
    connect(action_zoomin,SIGNAL(triggered(bool)),m_ncentralView,SLOT(zoomIn()));
    connect(action_zoomout,SIGNAL(triggered(bool)),m_ncentralView,SLOT(zoomOut()));
    connect(action_rotate,SIGNAL(triggered(bool)),m_ncentralView,SLOT(sratate()));
    connect(action_antiRotate,SIGNAL(triggered(bool)),m_ncentralView,SLOT(aratate()));
    connect(action_up,SIGNAL(triggered(bool)),m_ncentralView,SLOT(vup()));
    connect(action_down,SIGNAL(triggered(bool)),m_ncentralView,SLOT(vdown()));
    connect(action_left,SIGNAL(triggered(bool)),m_ncentralView,SLOT(vleft()));
    connect(action_right,SIGNAL(triggered(bool)),m_ncentralView,SLOT(vright()));
   // connect(action_zoomout,SIGNAL(triggered()),spin_zoom,SLOT(stepDown()));
   // connect(action_export,SIGNAL(triggered()),this,SLOT(export_to_png()));
    connect(action_save,SIGNAL(triggered()),this,SLOT(save()));
    connect(action_open,SIGNAL(triggered()),this,SLOT(load()));
    connect(action_exit,SIGNAL(triggered()),qApp,SLOT(quit()));
   // connect(spin_zoom,SIGNAL(valueChanged(int)),m_centralView,SLOT(zoom(int)));
    connect(action_missionManager,SIGNAL(triggered()),this,SLOT(open_missionManager()));
    connect(action_prepare,SIGNAL(triggered()),this,SLOT(prepare()));
    connect(action_run,SIGNAL(triggered()),this,SLOT(run()));
    connect(action_agvInformation,SIGNAL(triggered(bool)),this,SLOT(agv_information()));
    connect(action_stop,SIGNAL(triggered()),this,SLOT(stop()));
   // connect(m_centralView->m_undoStack,SIGNAL(indexChanged(int)),this,SLOT(set_documentDirty()));
    connect(m_topoView,SIGNAL(step_progressBar(int)),this,SLOT(set_progressStep(int)));
    connect(m_topoView,SIGNAL(set_dirty()),this,SLOT(set_topoViewDirty()));
   // connect(action_communicationSettings,SIGNAL(triggered()),this,SLOT(edit_settings()));
    connect(action_showNodeName,SIGNAL(triggered(bool)),this,SLOT(show_nodeName(bool)));
    connect(this,SIGNAL(update_progressBar(int)),this,SLOT(set_progressStep(int)));
   // connect(this,SIGNAL(app_stateChange(int)),m_centralView,SLOT(app_stateChanged(int)));

    //库位管理菜单按键触发信号连接
    connect(action_add, SIGNAL(triggered()), this, SLOT(call_dialog_add()));
    connect(action_remove, SIGNAL(triggered()), this, SLOT(call_dialog_remove()));
    connect(action_enable, SIGNAL(triggered()), this, SLOT(call_dialog_enable()));
    connect(action_disable, SIGNAL(triggered()), this, SLOT(call_dialog_disable()));
    connect(action_adjust, SIGNAL(triggered()), this, SLOT(call_dialog_adjust()));
    //connect(this, SIGNAL(MissionStatechanged()), this, SLOT(update_store()));
//    connect(m_trafficManager, SIGNAL(MissionStateChanged()), this, SLOT(update_store()));
//    connect(m_missionManager, SIGNAL(dialogcreated()), this, SLOT(update_store()));

}

void MainWindow::call_dialog_add()
{
    ccDialog = new ChangeComboDialog(this);
    connect(ccDialog, SIGNAL(accepted()), this, SLOT(add_store()));
    ccDialog->show();
    ccDialog->raise();
    ccDialog->activateWindow();
}
void MainWindow::call_dialog_remove()
{
    //if(!ccDialog)
        ccDialog = new ChangeComboDialog(this);
    connect(ccDialog, SIGNAL(accepted()), this, SLOT(remove_store()));
    ccDialog->show();
    ccDialog->raise();
    ccDialog->activateWindow();
}
void MainWindow::call_dialog_enable()
{
    //if(!ccDialog)
        ccDialog = new ChangeComboDialog(this);
    connect(ccDialog, SIGNAL(accepted()), this, SLOT(enable_store()));
    ccDialog->show();
    ccDialog->raise();
    ccDialog->activateWindow();
}
void MainWindow::call_dialog_disable()
{
    //if(!ccDialog)
        ccDialog = new ChangeComboDialog(this);
    connect(ccDialog, SIGNAL(accepted()), this, SLOT(disable_store()));
    ccDialog->show();
    ccDialog->raise();
    ccDialog->activateWindow();
}

void MainWindow::call_dialog_adjust()
{
    //if(!ccDialog)
        ccDialog = new ChangeComboDialog(this);
    connect(ccDialog, SIGNAL(accepted()), this, SLOT(adjust_store()));
    ccDialog->show();
    ccDialog->raise();
    ccDialog->activateWindow();
}

void MainWindow::update_store(int start_nodeID, int end_nodeID, int start_layer, int end_layer)
{
    QSqlQuery query = getQuery();
    int temp1,temp2;
    if(!query.exec("select * from tb_storage")) return;
    while(query.next())
    {
        QSqlRecord record = query.record();
        if(record.value("NodeID") == start_nodeID && record.value("layer") == start_layer) temp1 = record.value("storage").toInt() - 1;
        if(record.value("NodeID") == end_nodeID && record.value("layer") == end_layer) temp2 = record.value("storage").toInt() + 1;
    }
    query.prepare("update tb_storage set storage = ? where NodeID = ? and layer = ?");
    query.addBindValue(temp1);
    query.addBindValue(start_nodeID);
    query.addBindValue(start_layer);
    query.exec();
    query.prepare("update tb_storage set storage = ? where NodeID = ? and layer = ?");
    query.addBindValue(temp2);
    query.addBindValue(end_nodeID);
    query.addBindValue(end_layer);
    query.exec();
    readdb_update_dockwindow();
    update_topoview();
}

//更新线路图库位
void MainWindow::update_topoview()
{
    QSqlQuery query;
    query = getQuery();
    if(!query.exec("select * from tb_storage"));
    for(int i = 0; i < rectitems.size(); ++i){
        while(query.next())
        {
            QSqlRecord record = query.record();
            if(record.value("NodeID").toInt() == rectitems.at(i)->get_id())
            {
                rectitems.at(i)->paint(record.value("storage").toInt() <= 0, record.value("Hidden").toBool());
                //rectitems.at(i)->paint(record.value("storage").toInt() <= 0, false);
            }
        }
    }
}

//更新侧边栏显示
void MainWindow::readdb_update_dockwindow()
{
    topItems.clear();

    QSqlQuery query = getQuery();


   if(!query.exec("select * from tb_storage")) return;
   int id;
    QVector<int> IDs;
    QtVariantProperty *layer;
    QtVariantProperty *zhandian;
    QtVariantProperty *storage;
    //QtVariantProperty *count;

    if(!query.exec("select * from tb_huojia")) return; //tb_huojia
    while(query.next()){
        QSqlRecord record = query.record();
        if(!IDs.contains(record.value("id").toInt())){
            id=record.value("id").toInt();//4.9记录下来库位id
            IDs.append(record.value("id").toInt());
            topItems.append(store_variantmanager->addProperty(QtVariantPropertyManager::groupTypeId(), QString::number(record.value("id").toInt())));
        }
        layer = store_variantmanager->addProperty(QVariant::Int, tr("层级"));
        zhandian=store_variantmanager->addProperty(QVariant::Int, tr("站点"));
        zhandian->setValue(QString::number(record.value("zhandian").toInt()));
        layer->setValue(QString::number(record.value("layer").toInt()));
        storage = store_variantmanager->addProperty(QVariant::Int, tr("货类"));
        storage->setValue(QString::number(record.value("huolei").toInt()));
        idgoods[id]=storage;//4.9保存下来货量
//        count = store_variantmanager->addProperty(QVariant::Int, tr("记录数"));
//        count->setValue(count_size);
        for(int i = 0; i < topItems.size(); ++i){
            if(topItems.at(i)->propertyName() == record.value("id").toString())
            {
                topItems.at(i)->addSubProperty(layer);
                topItems.at(i)->addSubProperty(storage);
                topItems.at(i)->addSubProperty(zhandian);
                //topItems.at(i)->addSubProperty(count);
            }
        }
    }
    store_variantEditor->clear();
    for(int i = 0; i < topItems.size(); ++i) store_variantEditor->addProperty(topItems.at(i));
}


void MainWindow::add_store()
{
    bool LayerAdded = false;
    QSqlQuery query = getQuery();
    query.prepare("select * from tb_storage where NodeID = ?");
    query.addBindValue(ccDialog->Store_Value()->currentText().toInt());
    query.exec();
    while(query.next())
    {
        QSqlRecord record = query.record();
        if(record.value("layer").toInt() == ccDialog->Layer_Value()->currentText().toInt())
            LayerAdded = true;
    }
    if(!LayerAdded){
        query.prepare("INSERT INTO tb_storage (hidden, NodeID, layer, storage) ""VALUES (?, ?, ?, ?)");
        int temp;
        query.addBindValue(false);
        temp = ccDialog->Store_Value()->currentText().toInt();
        query.addBindValue(temp);
        temp = ccDialog->Layer_Value()->currentText().toInt();
        query.addBindValue(temp);
        temp = ccDialog->Goods_Value()->displayText().toInt();
        query.addBindValue(temp);
        query.exec();
        //m_missionManager->add_store(ccDialog->Store_Value()->currentText(), ccDialog->Layer_Value()->currentText());

        readdb_update_dockwindow();
        update_topoview();
    }
}

void MainWindow::remove_store()
{
    QSqlQuery query = getQuery();
    query.prepare("delete from tb_storage WHERE NodeID = ? AND layer = ?");
    int temp;
    temp = ccDialog->Store_Value()->currentText().toInt();
    query.addBindValue(temp);
    temp = ccDialog->Layer_Value()->currentText().toInt();
    query.addBindValue(temp);
    query.exec();
    readdb_update_dockwindow();
    update_topoview();
}

void MainWindow::enable_store()
{
    QSqlQuery query = getQuery();
    query.exec("select * from tb_storage");
    query.prepare("update tb_storage set hidden = ? WHERE NodeID = ? and layer = ?");
    int temp;
    temp = ccDialog->Store_Value()->currentText().toInt();
    query.addBindValue(false);
    query.addBindValue(temp);
    temp = ccDialog->Layer_Value()->currentText().toInt();
    query.addBindValue(temp);
    query.exec();
    readdb_update_dockwindow();
    update_topoview();
}

void MainWindow::disable_store()
{
    QSqlQuery query = getQuery();
    query.exec("select * from tb_storage");
    query.prepare("update tb_storage set hidden = ? WHERE NodeID = ? and layer = ?");
    int temp;
    temp = ccDialog->Store_Value()->currentText().toInt();
    query.addBindValue(true);
    query.addBindValue(temp);
    temp = ccDialog->Layer_Value()->currentText().toInt();
    query.addBindValue(temp);
    query.exec();
    readdb_update_dockwindow();
    update_topoview();
}

void MainWindow::adjust_store()
{
    QSqlQuery query = getQuery();
    query.prepare("update tb_storage set storage = ? WHERE NodeID = ? and layer = ?");
    int temp1,temp2,temp3;
    temp1 = ccDialog->Goods_Value()->displayText().toInt();
    temp2 = ccDialog->Store_Value()->currentText().toInt();
    temp3 = ccDialog->Layer_Value()->currentText().toInt();
    query.addBindValue(temp1);
    query.addBindValue(temp2);
    query.addBindValue(temp3);
    query.exec();
    readdb_update_dockwindow();
    update_topoview();
}

//库位信息侧边栏
void MainWindow::createDockWindow()//AGV属性栏“库位信息”的函数
{
    dw_storage = new QDockWidget(tr("库位信息"),this);
    dw_storage->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    store_variantmanager = new QtVariantPropertyManager(dw_storage);
    store_variantEditor = new QtTreePropertyBrowser(dw_storage);
    readdb_update_dockwindow();
    dw_storage->setWidget(store_variantEditor);
    addDockWidget(Qt::RightDockWidgetArea, dw_storage);
}

void MainWindow::updateToolbarUI(QAction *action)
{
    foreach(QAction *a ,m_centralToolBar->actions()){
            if(a->isCheckable() && a!=action)
                a->setChecked(false);
    }
    if(action)
        if(action->isChecked())
            action->setChecked(true);
}
bool MainWindow::loadAGVs()
{
    QSqlQuery query = getQuery();
    query.exec("create table if not exists tb_agv ( name text,id int,startNode int,address text,type text,chargeLimit text");
    query.exec("select * from tb_agv");
    m_mapIDtoAGV.clear();
    while(query.next()){
        QSqlRecord record = query.record();

        QString name = record.value("name").toString();
         int id = record.value("id").toInt();
         int startNode = record.value("startNode").toInt();
         QByteArray    address = stringToAddr(record.value("id").toString());
         QString str_type = record.value("type").toString();
        int chargeStation=record.value("chargeStation").toInt();
        QStringList chargeLimit = record.value("chargeLimit").toString().split(",");


       int type=0;
       if(str_type=="PT20")
        {
            type = PT20;
        }
        else if (str_type=="PS20")
        {
            type = PS20;
        }
       else if (str_type=="PH20")
       {
           type = PH20;
       }
        else    type = NONE_TYPE;
       AGV_INFORMATION agvinformation;
       agvinformation.name=name;
       agvinformation.startNode=startNode;
       agvinformation.address=address;
       agvinformation.type=type;
       agvinformation.chargeNode=chargeStation;
       agvinformation.chargeLimit=chargeLimit;
       m_agvINformation.insert(id,agvinformation);
       m_agv_count.insert(id,0);
       if(!m_agvList.contains(id))
       m_agvList.append(id);
    }
    return true;
}

QStringList MainWindow::get_recentOpenFileName()
{
    QStringList list;
    QString path;
    for(int i=1;i<5;i++){
        path = g_setting->value(SETTING_PREFIX_PATH+SETTING_SUFFIX_HISTORYPATH+QString::number(i)).toString();
        if(path=="") break;
        list.append(path);
    }
    return list;
}

void MainWindow::set_recentOpenFileName(QString currentPath)
{
    QStringList list = get_recentOpenFileName();
    if(list.contains(currentPath)){
        list.removeOne(currentPath);
        list.insert(0,currentPath);
    }
    else{
        if(list.length()>=5){
            list.removeLast();
            list.insert(0,currentPath);
        }
        else{
            list.insert(0,currentPath);
        }
    }
    for(int i=0;i<list.length();i++){
        g_setting->setValue(SETTING_PREFIX_PATH+SETTING_SUFFIX_HISTORYPATH+QString::number(i+1),list.at(i));
    }
}

void MainWindow::set_appState(int state)
{
    m_nAppState = state;
    action_run->setEnabled(m_nAppState==APP_PREPARE);
    action_multiSystem->setEnabled(m_nAppState==APP_STOP);
    action_prepare->setEnabled(m_nAppState==APP_STOP);
    action_stop->setEnabled(m_nAppState==APP_RUN || m_nAppState==APP_PREPARE);

    action_save->setEnabled(m_nAppState==APP_STOP);
    //action_export->setEnabled(m_nAppState==APP_STOP);
    action_open->setEnabled(m_nAppState==APP_STOP);
    action_communicationSettings->setEnabled(m_nAppState==APP_STOP);
    action_missionManager->setEnabled(m_nAppState!=APP_STOP);
    action_requestManager->setEnabled(m_nAppState!=APP_STOP);
    action_agvInformation->setEnabled(m_nAppState!=APP_STOP);
    emit app_stateChange(m_nAppState);
}



void MainWindow::selectChange()
{
    QList<QGraphicsItem* > items = m_centralScene->selectedItems();
    if(items.isEmpty()){
        m_currentItem = 0;
        return;
    }

    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
    while (itProp != propertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    propertyToId.clear();
    idToProperty.clear();

    m_currentItem = items.first();
    QtProperty *property;

    QObject *item = dynamic_cast<QObject*>(m_currentItem);
    if(!item)
    return;
    const QMetaObject *metaObject = item->metaObject();
    bool flag;

    property = stringManager->addProperty("工位名称");
    property->setEnabled(false);
    stringManager->setValue(property,item->property("_name").value<QString>());
    addProperty(property,QLatin1String("_name"));
    flag = metaObject->indexOfProperty("_Kuid")>-1;
    if(flag)
    {
        property = intManager->addProperty("工位号");
        property->setEnabled(false);
        intManager->setValue(property,item->property("_Kuid").value<int>());
        addProperty(property,QLatin1String("_Kuid"));


    }
    flag = metaObject->indexOfProperty("_state")>-1;
    if(flag)
    {
        property = intManager->addProperty("使用状态");
        property->setEnabled(false);
        intManager->setValue(property,item->property("_state").value<int>());
        addProperty(property,QLatin1String("_state"));


    }
    flag = metaObject->indexOfProperty("_center")>-1;
    if(flag){
        property = pointManager->addProperty("位置");
        pointManager->setValue(property,item->property("_center").value<QPoint>());
        addProperty(property,QLatin1String("_center"));
    }

   /* flag = metaObject->indexOfProperty("_text")>-1;
    if(flag){
        property = stringManager->addProperty("Text");
        stringManager->setValue(property,item->property("_text").value<QString>());
        addProperty(property,QLatin1String("_text"));
    }*/
    /*flag = metaObject->indexOfProperty("_font")>-1;
    if(flag){
        property = fontManager->addProperty("Font");
        fontManager->setValue(property,item->property("_font").value<QFont>());
        addProperty(property,QLatin1String("_font"));
    }*/
    flag = metaObject->indexOfProperty("_size")>-1;
    if(flag){
        property = sizeManager->addProperty("Size");
        sizeManager->setValue(property,item->property("_size").value<QSize>());
        addProperty(property,QLatin1String("_size"));
    }

    flag = metaObject->indexOfProperty("_ver_type")>-1;
    if(flag){
        property = boolManager->addProperty("库位点");
        boolManager->setValue(property,item->property("_ver_type").value<bool>());
        addProperty(property,QLatin1String("_ver_type"));


    }
    /*flag = metaObject->indexOfProperty("_rotate")>-1;
    if(flag){
        property = doubleManager->addProperty("角度");
        doubleManager->setValue(property,item->property("_rotate").value<double>());
        addProperty(property,QLatin1String("_rotate"));
    }
    */

    /*flag = metaObject->indexOfProperty("_power")>-1;
    if(flag){
        property = intManager->addProperty("剩余电量(%)");
        property->setEnabled(false);
        intManager->setValue(property,item->property("_power").value<int>());
        addProperty(property,QLatin1String("_power"));
    }*/
   // qDebug()<<"type"<<m_currentItem->type();
   /* if(m_currentItem->type()==AGV_PT20 || m_currentItem->type()==AGV_PS20)
        m_trafficManager->show_path((AGV*)m_currentItem);
   */
}
bool MainWindow::save()
{
    QString path = g_setting->value(SETTING_PREFIX_PATH+SETTING_SUFFIX_CURRENTPATH).toString();
    if(path=="" || path.right(3)!=".db"){
        path = QFileDialog::getSaveFileName(this,"保存","","*.db");
        if(path=="") return false; // 取消则返回
        if(path.right(3)!=".db") path+=".db";
    }
    if(m_bDocumentChangeFlag || m_bTopoViewChangeFlag){
        int total_items = m_centralScene->items().length();
        int total_item_count = m_bTopoViewChangeFlag?total_items:
                      total_items-m_topoView->get_edgeCount()-m_topoView->get_verCount();
        set_progressBarMax(total_item_count,"正在保存，请稍后...");
        openDatabase(path);

        m_topoView->save_topoMap();

        set_recentOpenFileName(path); // 更新setting中最近使用的文档
        g_setting->setValue(SETTING_PREFIX_PATH+SETTING_SUFFIX_CURRENTPATH,path);
        reset_documentDirty();
    }
    return true;
}

void MainWindow::load()
{ 
    if(m_bDocumentChangeFlag || m_bTopoViewChangeFlag){
        QMessageBox message(QMessageBox::Warning,"警告","当前文档已被修改，是否先保存当前工作？",
                             QMessageBox::Yes|QMessageBox::No);
        if(message.exec()==QMessageBox::Yes){
            if(save()==false) return; //如果取消了保存操作，则不能打开新的文档
        }
    }
    QString open_file_path = g_setting->value(SETTING_PREFIX_PATH+SETTING_SUFFIX_OPENFILEPATH).toString();
    QString path = QFileDialog::getOpenFileName(this,"打开",open_file_path,"*.db");
    if(path=="") return;
    m_topoView->clear();
    m_centralScene->clear();
    openDatabase(path);

//------------相当于重新执行一次开启界面的操作---------------
    qDebug()<<"path"<<path;
    QString p= path.remove(path.length()-3,3);
    qDebug()<<"p"<<p;
    QString ps=p.append(".dxf");
    std::string S=ps.toStdString();

    createSceneAndView(S,1);
    //createSceneAndView(S);
    //createActions();
   // createDockWindow();
    //createPropertyWindow();

    m_sceneHigh=m_centralScene->height();
    m_sceneCoordinate=coordinatemove;
    m_sceneZoom=zoom;
    m_topoView->load_topoMap(zoom,coordinatemove,m_centralScene->height());
    m_topoView->DistanceJudge();
    drawDXF(m_centralScene,m_ncentralView,creationClass);

//

    m_topoView->load_topoMap(zoom,coordinatemove,m_centralScene->height());
    m_topoView->show_nodeName(action_showNodeName->isChecked());
    path.append(".db");
    g_setting->setValue(SETTING_PREFIX_PATH+SETTING_SUFFIX_CURRENTPATH,path);
    g_setting->setValue(SETTING_PREFIX_PATH+SETTING_SUFFIX_OPENFILEPATH,path);

    set_recentOpenFileName(path);
    reset_documentDirty();
}

void MainWindow::open_recentFile()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(!action) return;
    QString path = action->data().toString();
    qDebug()<<"path***"<<path;
    if(path=="") return;
    m_topoView->clear();
    foreach(QGraphicsItem *item,m_centralScene->items())
        m_centralScene->removeItem(item);
    openDatabase(path);

  //------------相当于重新执行一次开启界面的操作---------------
    QString p= path.remove(path.length()-3,3); ;
    qDebug()<<"path"<<path;
    QString ps=p.append(".dxf");
    std::string S=ps.toStdString();
    createSceneAndView(S,1);
    m_sceneHigh=m_centralScene->height();
    m_sceneCoordinate=coordinatemove;
    m_sceneZoom=zoom;
    m_topoView->load_topoMap(zoom,coordinatemove,m_centralScene->height());
    m_topoView->DistanceJudge();
    drawDXF(m_centralScene,m_ncentralView,creationClass);

   //
    path.append('.db');
    m_topoView->load_topoMap(zoom,coordinatemove,m_centralScene->height());

    g_setting->setValue(SETTING_PREFIX_PATH+SETTING_SUFFIX_CURRENTPATH,path);
    setWindowTitle(WINDOWTITLE_PREFIX+path);
    reset_documentDirty();
}

void MainWindow::open_missionManager()
{
    if(m_missionManager->isHidden())
        m_missionManager->show();
    else
        m_missionManager->raise();

}

void MainWindow::prepare()  // 准备运行
{

    /* if(!open_serialPort())
       return;
        */

    if(action_multiSystem->isChecked())
    {
     bool r= openSQLServer();
     if(r==false) return;
      emit multiUsed();
    }
    QString path = g_setting->value(SETTING_PREFIX_PATH+SETTING_SUFFIX_CURRENTPATH).toString();
    /*if(!m_sweptArea)
    {
        m_sweptArea=new SweptArea;
        m_sweptArea->LoadTopoView(m_topoView);
        m_verInArea=m_sweptArea->verInArea;//马上注释掉
        //m_EquPointsInArea=m_sweptArea->EquPoInArea;
        m_edge_points=m_sweptArea->edge_points;
        m_collsionEdge=m_sweptArea->collsionEdge;
        m_collsionEdgeVer=m_sweptArea->collsionEdgeVer;
        m_huicheEdges  =m_sweptArea->huicheEdges;
        m_rukouEdges =m_sweptArea->rukouEdges;
        m_commonPoints=m_sweptArea->commonLine;
        m_commonEdges=m_sweptArea->m_commonEdges;
    }
    */
    if(!m_agvinformation)
    {
      m_agvinformation=new AgvInformation;
     // connect(m_agvinformation,SIGNAL(AgvInformationChange()),this,SLOT(AgvInformationChange()));
     // connect(m_agvinformation,SIGNAL(addintoRun(int)),this,SLOT(addAGVintoSystem(int)));

      connect(m_agvinformation,SIGNAL(chushihua(int,int,int,int,int,int,int,int)),this,SLOT(xuanzhongAGV(int,int,int,int,int,int,int,int)));

      // connect(m_agvinformation,SIGNAL(addintoRun(int,int)),this,SLOT(addAGVintoSystem(int,int)));

      connect(m_agvinformation,SIGNAL(removeFromRun(int)),this,SLOT(removeFromSystem(int)));
      connect(this,SIGNAL(IntoSySucc(int)),m_agvinformation,SLOT(inSystem(int)));
      connect(this,SIGNAL(diconnectFromSystem(int)),m_agvinformation,SLOT(outSystem(int)));
    }
    if(!m_missionManager){
        m_missionManager = new MissionManager;
        connect(m_missionManager,SIGNAL(wms_connected(bool)),this,SLOT(wms_state_chanaged(bool)));
        connect(m_missionManager,SIGNAL(huoliangxiugai(int,int)),this,SLOT(kuweixiugai(int,int)));
    }


    m_missionManager->loadTopoView(m_topoView);
    if(!m_trafficManager)
    {
        m_trafficManager = new TrafficManager(m_missionManager);
        connect(m_trafficManager, SIGNAL(MissionStateChanged(int,int, int, int)), this, SLOT(update_store(int, int, int, int)));
        connect(m_agvinformation ,SIGNAL(pauseAgv(int,int)),m_trafficManager,SLOT(pauseAgv(int,int)));
        connect(m_agvinformation, SIGNAL(keepMove(int)),m_trafficManager,SLOT(keepOn(int)));
    }
    m_trafficManager->loadTopoView(m_topoView);
    m_trafficManager->loadDatabase();

    m_trafficManager->m_TrafficVerInArea=m_verInArea;








    connect(m_missionManager,SIGNAL(mission_deleted(quint64)),m_trafficManager,SLOT(mission_abort(quint64)));
    connect(m_trafficManager,SIGNAL(AGVstateChange(AGV*,bool)),m_missionManager,SLOT(MissionArrange(AGV*,bool)));
    //agvinformation  连接
    connect(m_agvinformation,SIGNAL(get_agvmission(int)),this,SLOT(get_agvAndMission(int)));
    connect(this,SIGNAL(missionfromId(bool,bool,int,int)),m_agvinformation,SLOT(receiveMission(bool,bool,int,int)));
    connect(m_agvinformation,SIGNAL(ToMissionmanager(int,bool,bool,bool)),this,SLOT(whichMission(int,bool,bool,bool)));
    connect(this,SIGNAL(missionTowrite(int,bool,quint64,bool)),m_missionManager,SLOT(MissionResetOrFin(int,bool,quint64,bool)));
    connect(this,SIGNAL(agvinfor(int)),m_trafficManager,SLOT(someInforDelate(int)));
    connect(this,SIGNAL(multiUsed()),m_trafficManager,SLOT(multisystemused()));
    //,
    if(!m_requestManager){
        m_requestManager = new RequestManager;
        connect(action_requestManager,SIGNAL(triggered(bool)),m_requestManager,SLOT(setHidden(bool)));
        connect(this,SIGNAL(app_stateChange(int)),m_requestManager,SLOT(app_stateChanged(int)));
        connect(m_requestManager,SIGNAL(request(QPair<int,int>,int)),m_missionManager,SLOT(new_request(QPair<int,int>,int)));
        connect(m_requestManager,SIGNAL(serial_write(QByteArray)),this,SLOT(serial_write(QByteArray)));
        connect(m_missionManager,SIGNAL(mission_accomplished(int,int)),m_requestManager,SLOT(request_accomplished(int,int)));
    }
    m_requestManager->load_topoView(m_topoView);
    m_requestManager->set_path(path);
    loadAGVs();
    set_appState(APP_PREPARE);


    //  m_trafficManager->CommandProcess(1,6,180);//测试动作指令形成
    // QMessageBox::information(this, tr("测试"),tr("低电量，请尽快充电"), QMessageBox::Ok);

    QByteArray test;
    test.append(0x90);
    qDebug()<<"测试有符号数"<<(signed int)(test.at(0))<<endl;
    QList<int>path1;
    double cost=0;
    // m_trafficManager->dijkstra_Test(1,11,path1,cost);//测试路径规划生成
}

void MainWindow::agv_information()
{

   if(m_agvinformation->isHidden())
       m_agvinformation->show();
   else
       m_agvinformation->raise();
}
void MainWindow::run()
{
}
void MainWindow::stop()
{
    close_serialPort();
    set_appState(APP_STOP);
}
void MainWindow::set_documentDirty()
{
    if(m_bDocumentChangeFlag==false){
        QString path = g_setting->value(SETTING_PREFIX_PATH+SETTING_SUFFIX_CURRENTPATH).toString();
        setWindowTitle(WINDOWTITLE_PREFIX+path+"*");
    }
    m_bDocumentChangeFlag = true;
}

void MainWindow::reset_documentDirty()
{
    QString path = g_setting->value(SETTING_PREFIX_PATH+SETTING_SUFFIX_CURRENTPATH).toString();
    if(path=="") path = "untitled.db";
    m_bDocumentChangeFlag = false;
    m_bTopoViewChangeFlag = false;
    setWindowTitle(WINDOWTITLE_PREFIX+path);
}

void MainWindow::set_progressStep(int value)
{
    m_progressBar->setValue(value);
    if(value>=m_progressBar->maximum())
        statusBar()->hide();
}

void MainWindow::show_nodeName(bool flag)
{
    if(flag)
        action_showNodeName->setText("隐藏节点RFID");
    else
        action_showNodeName->setText("显示节点RFID");
    m_topoView->show_nodeName(flag);
}

void MainWindow::wms_state_chanaged(bool connected)
{
    if(connected){
        m_trayIcon->showMessage("AGV","WMS已连接");
        m_trayIcon->setToolTip("AGV(WMS已连接)");
    }
    else{
        m_trayIcon->showMessage("AGV","WMS已断开");
        m_trayIcon->setToolTip("AGV(WMS已断开");
    }
}

void MainWindow::set_progressBarMax(int max,QString text)
{
    m_progressBar->setRange(0,max);
    m_progressBar->setValue(0);
    m_progressBarLabel->setText(text);
    statusBar()->show();
}

//----------------------- 以下为属性栏函数定义 -------------------------
void MainWindow::valueChanged(QtProperty *property, double value)
{
    if(!propertyToId.contains(property))
        return;
    if(!m_currentItem)
        return;
    QString id = propertyToId[property];
    if(id==QLatin1String("_xpos")){
        m_currentItem->setX(value);
    }else if(id==QLatin1String("_ypos")){
        m_currentItem->setY(value);
    }else if(id==QLatin1String("_zpos")){
        m_currentItem->setZValue(value);
    }else{
        if(!propertyToId.contains(property))
            return;
        if(!m_currentItem)
            return;
        QObject *item = dynamic_cast<QObject*>(m_currentItem);
        if(!item) return;
        if(id==QLatin1String("_rotate"))
            item->setProperty("_rotate",value);
        else if(id==QLatin1String("_weight"))
            item->setProperty("_weight",value);
    }
    if(m_currentItem->type()==VER || m_currentItem->type()==EDGE)
        set_topoViewDirty();
    else
        set_documentDirty();
}

void MainWindow::valueChanged(QtProperty *property, const QString &value)
{
    if(!propertyToId.contains(property))
        return;
    if(!m_currentItem)
        return;
    QObject *item = dynamic_cast<QObject*>(m_currentItem);
    if(!item) return;
    item->setProperty("_text",value);
    set_documentDirty();
}

void MainWindow::valueChanged(QtProperty *property, const QColor &value)
{
    if(!propertyToId.contains(property))
        return;
    if(!m_currentItem)
        return;
    QObject *item = dynamic_cast<QObject*>(m_currentItem);
    if(!item) return;
    QString id = propertyToId[property];
    if(id==QLatin1String("_brush_color")){
        item->setProperty("_brush_color",value);
    }else if(id==QLatin1String("_pen_color")){
        item->setProperty("_pen_color",value);
    }
    if(m_currentItem->type()==VER || m_currentItem->type()==EDGE)
        set_topoViewDirty();
    else
        set_documentDirty();
}

void MainWindow::valueChanged(QtProperty *property, const QFont &value)
{
    if(!propertyToId.contains(property))
        return;
    if(!m_currentItem)
        return;
    QObject *item = dynamic_cast<QObject*>(m_currentItem);
    if(!item) return;
    QString id = propertyToId[property];
    if(id==QLatin1String("_font")){
        item->setProperty("_font",value);
    }
    set_documentDirty();
}

void MainWindow::valueChanged(QtProperty *property, const QPoint &value)
{
    if(!propertyToId.contains(property))
        return;
    if(!m_currentItem)
        return;
    QObject *item = dynamic_cast<QObject*>(m_currentItem);
    if(!item) return;
    QString id = propertyToId[property];
    if(id==QLatin1String("_center")){
        item->setProperty("_center",value);
    }
    if(m_currentItem->type()==VER){
        TopoVer *ver = ((TopoVer*)item);
        ver->position_change(ver->pos());
    }
    if(m_currentItem->type()==VER || m_currentItem->type()==EDGE)
        set_topoViewDirty();
    else
        set_documentDirty();
}

void MainWindow::valueChanged(QtProperty *property, const QSize &value)
{
    if(!propertyToId.contains(property))
        return;
    if(!m_currentItem)
        return;
    QObject *item = dynamic_cast<QObject*>(m_currentItem);
    if(!item) return;
    QString id = propertyToId[property];
    if(id==QLatin1String("_size")){
        item->setProperty("_size",value);
    }
    if(m_currentItem->type()==VER || m_currentItem->type()==EDGE)
        set_topoViewDirty();
    else
        set_documentDirty();
}

void MainWindow::valueChanged(QtProperty *property, const bool &value)
{
    qDebug()<<"进入此函数"<<endl;
    if(!propertyToId.contains(property))
        return;
    if(!m_currentItem)
        return;
    QObject *item = dynamic_cast<QObject*>(m_currentItem);
    if(!item) return;
    QString id = propertyToId[property];
    if(id==QLatin1String("_dash")){
        item->setProperty("_dash",value);
    }
    else if(id==QLatin1String("_ver_type")){
        item->setProperty("_ver_type",value);
    }
    if(m_currentItem->type()==VER || m_currentItem->type()==EDGE)
        set_topoViewDirty();
    else
        set_documentDirty();
}

void MainWindow::valueChanged(QtProperty *property, const int &value)
{
    if(!propertyToId.contains(property))
        return;
    if(!m_currentItem)
        return;
    QObject *item = dynamic_cast<QObject*>(m_currentItem);
    if(!item)
        return;
    QString id = propertyToId[property];
    if(id==QLatin1String("_pen_width")){
        item->setProperty("_pen_width",value);
    }
    if(m_currentItem->type()==VER || m_currentItem->type()==EDGE)
        set_topoViewDirty();
    else
        set_documentDirty();
}

void MainWindow::itemMoved(QGraphicsItem *item)
{
    // qDebug()<<"lalalala";
    if(item!=m_currentItem) return;
    QObject *i = dynamic_cast<QObject*>(item);
    pointManager->setValue(idToProperty[QLatin1String("_center")],i->property("_center").toPoint());
    doubleManager->setValue(idToProperty[QLatin1String("_rotate")],i->property("_rotate").toDouble());
    if(item->type()==PT20 || item->type()==PS20){
        intManager->setValue(idToProperty[QLatin1String("_power")],i->property("_power").toInt());
    }
}
void MainWindow::addProperty(QtProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    QtBrowserItem *item = propertyEditor->addProperty(property);
    if (idToExpanded.contains(id))
        propertyEditor->setExpanded(item, idToExpanded[id]);
}
//----------------------- 以上为属性栏函数定义 -------------------------------

//--------------------------- 串口通信函数定义 ------------------------------
bool MainWindow::open_serialPort()
{
    QString prefix = "/serial_com/";
    QString portname = g_setting->value(prefix+"Port").toString();
    if(portname.isEmpty()){
        portname = "COM2";
        g_setting->setValue(prefix+"Port",portname);
    }
    serial = new QSerialPort(portname);
    if(!serial->open(QIODevice::ReadWrite)){
        QMessageBox::warning(0,"错误","串口打开错误，计算机是否插入串口？");
        return false;
    }
    int nBaud = g_setting->value(prefix+"Baud").toInt();
    if(nBaud==0){
        nBaud = 57600;
        g_setting->setValue(prefix+"Baud",nBaud);
    }
    serial->setBaudRate(nBaud);  //波特率
    serial->setDataBits(QSerialPort::Data8); //数据位
    serial->setParity(QSerialPort::NoParity);    //无奇偶校验
    serial->setStopBits(QSerialPort::OneStop);   //无停止位
    serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
    connect(serial,SIGNAL(readyRead()),this,SLOT(serial_read()));
    return true;
}

void MainWindow::close_serialPort()
{
    if(serial && serial->isOpen()) serial->close();
    delete serial;
    serial = NULL;
}

void MainWindow::serial_write(QByteArray data)
{
    static bool s_warning = true;
    if(serial && serial->isOpen()){
        serial->write(data);
    }
    else{
        stop();
        if(s_warning){
            QMessageBox::critical(0,"串口错误","未检测到调度盒,请确认调度盒已连接并重启软件.");
            s_warning = false;
        }

    }
}

void MainWindow::serial_read()
{
    static char s_startup = -1;
    static char s_frame_type = 0;
    static int s_length = -1;
    if(s_length==-1){
        serial->read(&s_startup,1);
        if(s_startup==FRAME_FROM_REQUEST)
            s_length=0;
    }
    if(s_length==0){
        if(serial->bytesAvailable()>=3){
            char length[2];
            serial->read(length,2);
            serial->read(&s_frame_type,1);
            s_length = ((((uchar)length[0])<<8)&0xff00) | (((uchar)length[1])&0xff);
        }
    }
    if(s_length>14){
            readbuf = serial->readAll();
            if(readbuf.length()>=s_length)
            {
                readbuf.insert(0,s_frame_type);
                if((0xff-_8BitsSum(readbuf.mid(0,s_length)))==(uchar)(readbuf.at(s_length))){
                    QByteArray address;
                    address = readbuf.mid(1,8);
                    if(m_mapIDtoAGV.count(address)){
                        QByteArray extract_buf = readbuf.mid(12,s_length-12);
                        if(extract_buf.at(0)==0x40 && extract_buf.at(extract_buf.length()-1)==0x23){
                            m_mapIDtoAGV.value(address)->recvData(extract_buf.at(2),extract_buf.mid(3,extract_buf.length()-3));
                        }
                    }
                    else{
                        m_requestManager->recvData(QPair<QByteArray,uchar>(address,readbuf.at(readbuf.length()-2)));
                    }
                }
            }
            readbuf.clear();
            s_length = -1;
    }

    else if(s_length==7)
    {     
             readbuf = serial->readAll();
             if(readbuf.length()>=s_length)
             {
                 readbuf.insert(0,s_frame_type);
                 if((0xff-_8BitsSum(readbuf.mid(0,s_length)))==(uchar)(readbuf.at(s_length)))
                 {
                     if((readbuf.at(5)==0x25))
                     {
                         qDebug()<<"检测到掉线信息";
                        int s_frame_id=(int)readbuf.at(1);
                        if (s_frame_id<0)
                           s_frame_id=s_frame_id+256;
                        if(m_agv_count.keys().contains(s_frame_id))
                        {
                            int count =m_agv_count.value(s_frame_id);

                            count++;
                            m_agv_count.insert(s_frame_id,count);
                          //  m_agv_count[s_frame_id]=count;
                            if(count>=diaoxianchonglian)
                            {
                                //removeFromSystem(s_frame_id);
                                stopAgvSent(s_frame_id);

                            }
                        }


                     }
                 }
             }
            readbuf.clear();
            s_length = -1;
    }
    else
    {

         readbuf = serial->readAll();
         readbuf.clear();
          s_length = -1;
    }

}


//-------------------------- 以上为串口通信函数定义 ------------------------------
// -----------------------------  后台运行函数定义开始  ---------------------------
void MainWindow::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::Trigger:
        if(this->isHidden()){
            this->showMaximized();
        }
        else{
            this->hide();
            if(m_missionManager && m_missionManager->isVisible())
                m_missionManager->hide();
            m_trayIcon->showMessage(tr("AGV"),tr("单击重新回到主界面"));
        }
        break;
    default:
        break;
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    if((e->type()==QEvent::WindowStateChange)&&this->isMinimized()){
        this->hide();
        if(m_missionManager && m_missionManager->isVisible())
            m_missionManager->hide();
        m_trayIcon->showMessage(tr("AGV"),tr("单击重新回到主界面"));
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
   /*
    int r = QMessageBox::information(this, tr("提示"),

              tr("数值超出范围,是否保存修改?"),

            QObject::tr(("确认")) , QObject::tr(("取消")));

            if (r == QObject::tr(("确认")).toInt()) {


    qDebug()<<"谢谢"<<endl;

            }

    */

    if(isVisible()){
        this->hide();
        if(m_missionManager && m_missionManager->isVisible())
            m_missionManager->hide();
        m_trayIcon->showMessage(tr("AGV"),tr("单击重新回到主界面"));
        e->ignore();
    }
}
// ------------------------------  后台运行函数定义结束  ----------------------------

//----------------------仿真系统agv加入系统中---------

void MainWindow::addAGVasFangzhen(int id,int socketdescripte)
{
    m_agvidSocketScripte[id]=socketdescripte;
    qDebug()<<"加入仿真系统"<<id;

    /*此部分放入选中AGV里面
    QByteArray address=m_agvINformation.value(id).address;
    int type=m_agvINformation.value(id).type;
    QString name=m_agvINformation.value(id).name;
    int startNode=m_agvINformation.value(id).startNode;
    QStringList chargeLimit=m_agvINformation.value(id).chargeLimit;
    AGV *agv=new AGV(m_centralScene,m_topoView,zoom,coordinatemove,type,name,id,address,startNode,chargeLimit.first().toInt(),chargeLimit.at(1).toInt(),chargeLimit.at(2).toInt());
    m_mapIDtoAGV[address] =agv;
   // m_IDtoAGV[id] =agv;
    m_AGVtoID[agv]=id;
    m_trafficManager->addAGV(agv);
    m_missionManager->addAGV(agv);
    connect(agv,SIGNAL(state_change()),m_trafficManager,SLOT(updateGlobalPlanning()));
    connect(agv,SIGNAL(serial_write(QByteArray)),this,SLOT(serial_write(QByteArray)));
    connect(agv,SIGNAL(state_moved(QGraphicsItem*)),this,SLOT(showAgvInfo(QGraphicsItem*)));

    //connect(agv,SIGNAL(tcp_write(QByteArray )),&ser,SIGNAL(sentData(QByteArray)));
    connect(agv,SIGNAL(tcp_write(QByteArray)),this,SLOT(writeToTcp(QByteArray)));
    connect(&ser,SIGNAL(fangzhenAGVxinxi(int,char,QByteArray)),this,SLOT(receivFromTcp(int,char,QByteArray)));
    //connect(&ser,SIGNAL(fangzhenAGVxinxi(char,QByteArray)),agv,SLOT(recvData(char,QByteArray)));

    agv->m_AGVverInArea=m_verInArea;
*/
}
void MainWindow::writeToTcp(QByteArray array)
{
    int address_1=0;
    int address_2=0;
   /*-------------仿真
    if(array.length()>2)
    {
      int id=array.at(1);
      int socketdes=m_agvidSocketScripte.value(id);
      emit writeToServer(socketdes,array);

    }
    --------------*/

    if(array.length()>2)
    {
      int id=array.at(5);//车号id
      if((m_agvinformation->m_idAddress_1.contains(id))&&(m_agvinformation->m_idAddress_2.contains(id)))
      {
           address_1=m_agvinformation->m_idAddress_1[id];
           address_2=m_agvinformation->m_idAddress_2[id];
           array.prepend(address_1);//在原先通信帧基础上，加入了地址
           array.prepend(address_2);
           qDebug()<<"address测试***："<<address_1<<address_2<<endl;

      }
      int socketdes=m_agvidSocketScripte.value(id);
      emit writeToServer(socketdes,array);
    }
}
void MainWindow::receivFromTcp(int id, char frame_type, QByteArray data)
{
    /*考虑在收到信息后，再初始化一个agv实例，但是由于多线程原因，似乎并不能奏效
     if (frame_type==0x50)

     {
        int chexing=bytesToInt32(data.mid(0,4));
        // int qishidian=bytesToInt32(data.mid(4,4));
        QByteArray address=m_agvINformation.value(id).address;
        int type=m_agvINformation.value(id).type;
        QString name=m_agvINformation.value(id).name;
        int startNode=m_agvINformation.value(id).startNode;
        QStringList chargeLimit=m_agvINformation.value(id).chargeLimit;
        AGV *agv=new AGV(m_centralScene,m_topoView,zoom,coordinatemove,type,name,id,address,startNode,chargeLimit.first().toInt(),chargeLimit.at(1).toInt(),chargeLimit.at(2).toInt());
        m_mapIDtoAGV[address] =agv;
        // m_IDtoAGV[id] =agv;
        m_AGVtoID[agv]=id;
        m_trafficManager->addAGV(agv);
        m_missionManager->addAGV(agv);
        connect(agv,SIGNAL(state_change()),m_trafficManager,SLOT(updateGlobalPlanning()));
        connect(agv,SIGNAL(serial_write(QByteArray)),this,SLOT(serial_write(QByteArray)));
        connect(agv,SIGNAL(state_moved(QGraphicsItem*)),this,SLOT(showAgvInfo(QGraphicsItem*)));

        //connect(agv,SIGNAL(tcp_write(QByteArray )),&ser,SIGNAL(sentData(QByteArray)));
        connect(agv,SIGNAL(tcp_write(QByteArray)),this,SLOT(writeToTcp(QByteArray)));
      }

     */
    qDebug()<<"初始化消息检查"<<id<<(uchar)frame_type<<endl;
    if(m_IDtoAGV.contains(id)) //确保 AGV加入调度系统后，才会接收到相关数据信息
    {
       AGV *agv= m_AGVtoID.key(id);
       //agv->recvData(frame_type,data);
       agv->recvData(frame_type,data,true);
    }
}


//选中的AGV在这里向仿真平台发送初始化指令

void MainWindow::xuanzhongAGV(int id,int startnode,int chexing,int xiansudu,int jiaosudu,int a,int w,int angle)
{
     qDebug()<<"错误测试：加入系统成功在线3"<<id<<endl;
    //初始化需要发送初始化通信帧吗？
    /*--------------------------------------
     * -------------------------------------
    int _null=0x00;
    QByteArray buff;
    buff.append(0x40);
    buff.append(id);
    buff.append(0x50);
    buff.append(startnode);
    buff.append(chexing);
    buff.append(_null);
    buff.append(_null);
    buff.append(_8BitsSum(buff.mid(1)));//18
    buff.append(0x23);
    writeToTcp(buff);
    sleep(1000);
    qDebug()<<"初始化了"<<endl;
    QByteArray buf;
    buf.append(0x40);
    buf.append(id);
    buf.append(0x51);
    buf.append(xiansudu);
    buf.append(jiaosudu);
    buf.append(a);
    buf.append(w);
    buf.append(_8BitsSum(buff.mid(1)));//18
    buf.append(0x23);
    writeToTcp(buf);

    QByteArray address=m_agvINformation.value(id).address;
     if (m_mapIDtoAGV.contains(address))
     {

    QAction *infor=menu_agvinfo->addAction(QString("agv_%1").arg(id));
    infor->setCheckable(true);
    infor->setChecked(true);
    connect(infor,SIGNAL(triggered(bool)),this,SLOT(agvInforList(bool)));
    m_agvidAction.insert(id,infor);

    QtProperty *item2 = groupManager->addProperty(QString("AGV_%1").arg(id));
    QtProperty *item21 = pointAgvInfor->addProperty("位置");
    item2->addSubProperty(item21);
    QtProperty *item22 = doubleAgvInfor->addProperty("角度");
    item2->addSubProperty(item22);

    QtProperty *item23 = intAgvInfor->addProperty("电量");
    item2->addSubProperty(item23);

    QtProperty *item24 = boolAgvInfor->addProperty("通信");
     item24->setEnabled(false);

    item2->addSubProperty(item24);

    QtProperty *item25 = stringAgvInfor->addProperty("任务状态");
    item2->addSubProperty(item25);

    QtProperty *item26 = intAgvInfor->addProperty("线速度");
    item2->addSubProperty(item26);
    QtProperty *item27 = intAgvInfor->addProperty("角速度");
    item2->addSubProperty(item27);
    agvxianshi agvxs;
    agvxs.item1=item2;
    agvxs.item11=item21;
    agvxs.item12=item22;
    agvxs.item13=item23;
    agvxs.item14=item24;
    agvxs.item15=item25;
    agvxs.item16=item26;
    agvxs.item17=item27;
    idAgvXinshi.insert(id,agvxs);
    propertyEditor2->addProperty(item2);
    m_agvShowList.append(id);

}


  else
    //QByteArray address=m_agvINformation.value(id).address;
   // AGV* agv=m_mapIDtoAGV[address];

    {

    QByteArray address=m_agvINformation.value(id).address;
    int type=m_agvINformation.value(id).type;
    QString name=m_agvINformation.value(id).name;
    qDebug()<<"选中AGV名字是"<<name<<endl;
    int startNode=startnode;
            //m_agvINformation.value(id).startNode;
    QStringList chargeLimit=m_agvINformation.value(id).chargeLimit;
    AGV *agv=new AGV(m_centralScene,m_topoView,zoom,coordinatemove,type,name,id,address,startNode,chargeLimit.first().toInt(),chargeLimit.at(1).toInt(),chargeLimit.at(2).toInt(),chexing);
    m_mapIDtoAGV[address] =agv;
   // m_IDtoAGV[id] =agv;
    m_AGVtoID[agv]=id;
    m_IDtoAGV[id]=agv;
    m_trafficManager->addAGV(agv);
    m_missionManager->addAGV(agv);
    connect(agv,SIGNAL(state_change()),m_trafficManager,SLOT(updateGlobalPlanning()));
    //connect(agv,SIGNAL(missionwancheng(quint64,QString,AGV*)),m_missionManager,SLOT(setMissionState(quint64,QString,AGV*)));//3.28 任务完成，任务列表槽函数
//4.1为其他的原因将此页注释掉
     connect(agv,SIGNAL(dianliangjinggao()),this,SLOT(dianliangdi()));
     connect(agv,SIGNAL(dianliangjinggao()),m_missionManager,SLOT(jinzhiAGV()));//3.28 低电量，禁止执行任务

    connect(agv,SIGNAL(serial_write(QByteArray)),this,SLOT(serial_write(QByteArray)));
    connect(agv,SIGNAL(state_moved(QGraphicsItem*,int,int)),this,SLOT(showAgvInfo(QGraphicsItem*,int,int)));

    //connect(agv,SIGNAL(tcp_write(QByteArray )),&ser,SIGNAL(sentData(QByteArray)));
    connect(agv,SIGNAL(tcp_write(QByteArray)),this,SLOT(writeToTcp(QByteArray)));
    connect(&ser,SIGNAL(fangzhenAGVxinxi(int,char,QByteArray)),this,SLOT(receivFromTcp(int,char,QByteArray)));
    //connect(&ser,SIGNAL(fangzhenAGVxinxi(char,QByteArray)),agv,SLOT(recvData(char,QByteArray)));

    agv->m_AGVverInArea=m_verInArea;



    emit IntoSySucc(id);
     agv->setCommState(true);
     QAction *infor=menu_agvinfo->addAction(QString("agv_%1").arg(id));
     infor->setCheckable(true);
     infor->setChecked(true);
     connect(infor,SIGNAL(triggered(bool)),this,SLOT(agvInforList(bool)));
     m_agvidAction.insert(id,infor);

     QtProperty *item2 = groupManager->addProperty(QString("AGV_%1").arg(id));
     QtProperty *item21 = pointAgvInfor->addProperty("位置");
     item2->addSubProperty(item21);
     QtProperty *item22 = doubleAgvInfor->addProperty("角度");
     item2->addSubProperty(item22);

     QtProperty *item23 = intAgvInfor->addProperty("电量");
     item2->addSubProperty(item23);

     QtProperty *item24 = boolAgvInfor->addProperty("通信");
      item24->setEnabled(false);

     item2->addSubProperty(item24);

     QtProperty *item25 = stringAgvInfor->addProperty("任务状态");
     item2->addSubProperty(item25);

     QtProperty *item26 = intAgvInfor->addProperty("线速度");
     item2->addSubProperty(item26);
     QtProperty *item27 = intAgvInfor->addProperty("角速度");
     item2->addSubProperty(item27);
     agvxianshi agvxs;
     agvxs.item1=item2;
     agvxs.item11=item21;
     agvxs.item12=item22;
     agvxs.item13=item23;
     agvxs.item14=item24;
     agvxs.item15=item25;
     agvxs.item16=item26;
     agvxs.item17=item27;
     idAgvXinshi.insert(id,agvxs);
     propertyEditor2->addProperty(item2);
     m_agvShowList.append(id);

}
------------------------------------------------
---------------------------------------------*/


    QByteArray address=m_agvINformation.value(id).address;
     if (m_mapIDtoAGV.contains(address))
     {

    QAction *infor=menu_agvinfo->addAction(QString("agv_%1").arg(id));
    infor->setCheckable(true);
    infor->setChecked(true);
    connect(infor,SIGNAL(triggered(bool)),this,SLOT(agvInforList(bool)));
    m_agvidAction.insert(id,infor);

    QtProperty *item2 = groupManager->addProperty(QString("AGV_%1").arg(id));
    QtProperty *item21 = pointAgvInfor->addProperty("位置");
    item2->addSubProperty(item21);
    QtProperty *item22 = doubleAgvInfor->addProperty("角度");
    item2->addSubProperty(item22);

    QtProperty *item23 = intAgvInfor->addProperty("电量");
    item2->addSubProperty(item23);

    QtProperty *item24 = boolAgvInfor->addProperty("通信");
    item24->setEnabled(false);

    item2->addSubProperty(item24);

    QtProperty *item25 = stringAgvInfor->addProperty("任务状态");
    item2->addSubProperty(item25);

    QtProperty *item26 = stringAgvInfor->addProperty("动作指令");
    item2->addSubProperty(item26);
    QtProperty *item27 = intAgvInfor->addProperty("线速度");
    item2->addSubProperty(item27);
    QtProperty *item28 = intAgvInfor->addProperty("角速度");
    item2->addSubProperty(item28);

    agvxianshi agvxs;
    agvxs.item1=item2;
    agvxs.item11=item21;
    agvxs.item12=item22;
    agvxs.item13=item23;
    agvxs.item14=item24;
    agvxs.item15=item25;
    agvxs.item16=item26;
    agvxs.item17=item27;
    agvxs.item18=item28;
    idAgvXinshi.insert(id,agvxs);
    propertyEditor2->addProperty(item2);
    m_agvShowList.append(id);

}

  else

    {

    QByteArray address=m_agvINformation.value(id).address;
    int type=m_agvINformation.value(id).type;
    QString name=m_agvINformation.value(id).name;
    qDebug()<<"选中AGV名字是"<<name<<endl;
    int startNode=startnode;
            //m_agvINformation.value(id).startNode;
    QStringList chargeLimit=m_agvINformation.value(id).chargeLimit;
    AGV *agv=new AGV(m_centralScene,m_topoView,zoom,coordinatemove,type,name,id,address,startNode,chargeLimit.first().toInt(),chargeLimit.at(1).toInt(),chargeLimit.at(2).toInt(),chexing,angle);
    m_mapIDtoAGV[address] =agv;
   // m_IDtoAGV[id] =agv;
    m_AGVtoID[agv]=id;
    m_IDtoAGV[id]=agv;
    m_trafficManager->addAGV(agv);
    m_missionManager->addAGV(agv);
    connect(agv,SIGNAL(state_change()),m_trafficManager,SLOT(updateGlobalPlanning()));
    //connect(agv,SIGNAL(missionwancheng(quint64,QString,AGV*)),m_missionManager,SLOT(setMissionState(quint64,QString,AGV*)));//3.28 任务完成，任务列表槽函数
     connect(agv,SIGNAL(TestFeedback(AGVCommand,int,int,int)),m_missionManager,SLOT(action_recvCommand(AGVCommand,int,int,int)));//3.28 任务完成，任务列表槽函数
   //4.1为其他的原因将此页注释掉
    connect(agv,SIGNAL(dianliangjinggao()),this,SLOT(dianliangdi()));
    connect(agv,SIGNAL(dianliangjinggao()),m_missionManager,SLOT(jinzhiAGV()));//3.28 低电量，禁止执行任务

    connect(agv,SIGNAL(AGVWarn(int)),this,SLOT(Warning(int)));

    connect(agv,SIGNAL(serial_write(QByteArray)),this,SLOT(serial_write(QByteArray)));
    connect(agv,SIGNAL(state_moved(QGraphicsItem*,int,int)),this,SLOT(showAgvInfo(QGraphicsItem*,int,int)));

    //connect(agv,SIGNAL(tcp_write(QByteArray )),&ser,SIGNAL(sentData(QByteArray)));
    connect(agv,SIGNAL(tcp_write(QByteArray)),this,SLOT(writeToTcp(QByteArray)));
    connect(&ser,SIGNAL(fangzhenAGVxinxi(int,char,QByteArray)),this,SLOT(receivFromTcp(int,char,QByteArray)));
    //connect(&ser,SIGNAL(fangzhenAGVxinxi(char,QByteArray)),agv,SLOT(recvData(char,QByteArray)));

    agv->m_AGVverInArea=m_verInArea;



     emit IntoSySucc(id);//agvinformation中，显示在线
     qDebug()<<"错误测试：加入系统成功在线2"<<id<<endl;

     agv->setCommState(true);
     QAction *infor=menu_agvinfo->addAction(QString("agv_%1").arg(id));
     infor->setCheckable(true);
     infor->setChecked(true);
     connect(infor,SIGNAL(triggered(bool)),this,SLOT(agvInforList(bool)));
     m_agvidAction.insert(id,infor);

     QtProperty *item2 = groupManager->addProperty(QString("AGV_%1").arg(id));
     QtProperty *item21 = pointAgvInfor->addProperty("位置");
     item2->addSubProperty(item21);
     QtProperty *item22 = doubleAgvInfor->addProperty("角度");
     item2->addSubProperty(item22);

     QtProperty *item23 = intAgvInfor->addProperty("电量");
     item2->addSubProperty(item23);

     QtProperty *item24 = boolAgvInfor->addProperty("通信");
      item24->setEnabled(false);

     item2->addSubProperty(item24);

     QtProperty *item25 = stringAgvInfor->addProperty("任务状态");
     item2->addSubProperty(item25);

     QtProperty *item26 = stringAgvInfor->addProperty("动作指令");
     item2->addSubProperty(item26);
     QtProperty *item27 = intAgvInfor->addProperty("线速度");
     item2->addSubProperty(item27);
     QtProperty *item28 = intAgvInfor->addProperty("角速度");
     item2->addSubProperty(item28);

     agvxianshi agvxs;
     agvxs.item1=item2;
     agvxs.item11=item21;
     agvxs.item12=item22;
     agvxs.item13=item23;
     agvxs.item14=item24;
     agvxs.item15=item25;
     agvxs.item16=item26;
     agvxs.item17=item27;
     agvxs.item18=item28;
     idAgvXinshi.insert(id,agvxs);
     propertyEditor2->addProperty(item2);
     m_agvShowList.append(id);

}




}




void MainWindow::addAGVintoSystem(int id)
{
        qDebug()<<"加入系统";
        QByteArray address=m_agvINformation.value(id).address;
        int type=m_agvINformation.value(id).type;
        QString name=m_agvINformation.value(id).name;
        int startNode=m_agvINformation.value(id).startNode;
        QStringList chargeLimit=m_agvINformation.value(id).chargeLimit;
        AGV *agv=new AGV(m_centralScene,m_topoView,zoom,coordinatemove,type,name,id,address,startNode,chargeLimit.first().toInt(),chargeLimit.at(1).toInt(),chargeLimit.at(2).toInt(),0);
        m_mapIDtoAGV[address] =agv;
        m_IDtoAGV[id] =agv;
        m_AGVtoID[agv]=id;
        m_trafficManager->addAGV(agv);
        m_missionManager->addAGV(agv);
        connect(agv,SIGNAL(state_change()),m_trafficManager,SLOT(updateGlobalPlanning()));
        //connect(agv,SIGNAL(agv_moved(QGraphicsItem*)),this,SLOT(itemMoved(QGraphicsItem*)));
       // connect(agv,SIGNAL(agv_moved(QGraphicsItem*)),this,SLOT(showAgvInfo(QGraphicsItem*)));
        connect(agv,SIGNAL(serial_write(QByteArray)),this,SLOT(serial_write(QByteArray)));
        connect(agv,SIGNAL(state_moved(QGraphicsItem*)),this,SLOT(showAgvInfo(QGraphicsItem*)));

        agv->m_AGVverInArea=m_verInArea;

        emit IntoSySucc(id);
         agv->setCommState(true);
         QAction *infor=menu_agvinfo->addAction(QString("agv_%1").arg(id));
         infor->setCheckable(true);
         infor->setChecked(true);
         connect(infor,SIGNAL(triggered(bool)),this,SLOT(agvInforList(bool)));
         m_agvidAction.insert(id,infor);

         QtProperty *item2 = groupManager->addProperty(QString("AGV_%1").arg(id));
         QtProperty *item21 = pointAgvInfor->addProperty("位置");
         item2->addSubProperty(item21);
         QtProperty *item22 = doubleAgvInfor->addProperty("角度");
         item2->addSubProperty(item22);

         QtProperty *item23 = intAgvInfor->addProperty("电量");
         item2->addSubProperty(item23);

         QtProperty *item24 = boolAgvInfor->addProperty("通信");
          item24->setEnabled(false);

         item2->addSubProperty(item24);

         QtProperty *item25 = stringAgvInfor->addProperty("任务状态");
         item2->addSubProperty(item25);
         agvxianshi agvxs;
         agvxs.item1=item2;
         agvxs.item11=item21;
         agvxs.item12=item22;
         agvxs.item13=item23;
         agvxs.item14=item24;
         agvxs.item15=item25;
         idAgvXinshi.insert(id,agvxs);
         propertyEditor2->addProperty(item2);
         m_agvShowList.append(id);

}
void MainWindow::agvInforList(bool flag)
{
    QAction *action = qobject_cast<QAction*>(sender());
   QString agv_text= action->text();
   QString agv_id=agv_text.split("_").at(1);
   int  id=agv_id.toInt();
 // qDebug()<<"flag"<<flag<<action->text();
 //  qDebug()<<"id"<<id;
   agvxianshi agcxs=idAgvXinshi.value(id);
   QtProperty *item=agcxs.item1;

   if(flag==true)
   {
        if(!m_agvShowList.contains(id))
        {
         m_agvShowList.append(id);
         propertyEditor2->addProperty(item);
        }
   }
   else
   {
       if(m_agvShowList.contains(id))
       {
           qDebug()<<"remove";
         m_agvShowList.removeOne(id);
         propertyEditor2->removeProperty(item);
       }
   }
   agvNumChange=true;

}
void MainWindow::showAgvInfo(QGraphicsItem *item,int xiansudu,int jiaosudu)
{
    //qDebug()<<"idxxx";
    if(item==NULL) return;
   //写入数据
    //如果状态改变   重新生成信息栏 写入数据
    //否则 直接写入数据
    int id=m_AGVtoID.value((AGV*)item);
   // qDebug()<<"xxid"<<id;
    agvxianshi its=idAgvXinshi.value(id);
    QtProperty *item11=its.item11;
    QtProperty *item12=its.item12;
     QtProperty *item13=its.item13;
    //QtProperty *item12=its.item12;
   QtProperty *item14=its.item14;

   QtProperty *item15=its.item15;
   QtProperty *item16=its.item16;
   QtProperty *item17=its.item17;
   QtProperty *item18=its.item18;

    QObject *i = dynamic_cast<QObject*>(item);
    pointAgvInfor->setValue(item11,i->property("_center").toPoint());
    double angle= i->property("_rotate").toDouble();
    if(angle==0)
        angle=0;
    else if (angle>0)
        angle=360-angle;
    else
        angle=-(int)angle%360;

    doubleAgvInfor->setValue(item12,angle);//逆时针方向
    intAgvInfor->setValue(item13,i->property("_power").toInt());
    boolAgvInfor->setValue(item14,i->property("_commstata").toBool());
    stringAgvInfor->setValue(item15,i->property("_state").toString());
    stringAgvInfor->setValue(item16,i->property("_comstate").toString());


     intAgvInfor->setValue(item17,xiansudu);
     intAgvInfor->setValue(item18,jiaosudu);

    //pointManager->setValue(idToProperty[QLatin1String("_center")],i->property("_center").toPoint());
     //doubleManager->setValue(idToProperty[QLatin1String("_rotate")],i->property("_rotate").toDouble());
}

void MainWindow::Warning(int id)
{

    if(id==1)
    {
        static bool flag=false;
         AGV* agv = (AGV*)sender();

         if (flag==false)
         {
             flag=true;
        QMessageBox::information(this, tr("Warnning"),QString("%1").arg(agv->get_id())+"初始化节点错误", QMessageBox::Ok);
       }
    }


}


void MainWindow::stopAgvSent(int id)
{
     qDebug()<<"掉线"<<id;//串口掉线
      QMap<QByteArray,AGV*> m_addressAgv=m_mapIDtoAGV;
     foreach (AGV *agv, m_addressAgv.values()) {
          if( agv->get_id()==id)
          {
             agv->killTimer(agv->m_nTimerID);
             agv->m_nTimerID=0;
             m_agv_count[id]=0;
            emit diconnectFromSystem(id);
             agv->setCommState(false);
          }
     }
          m_mapIDtoAGV=m_addressAgv;

}


//从仿真系统中移除AGV
void MainWindow::removeFromSystem(int id)
{

    int _null=0x00;
    QByteArray buff;
    buff.append(0x40);
    buff.append(id);
    buff.append(0x55);
    buff.append(_null);
    buff.append(_null);
    buff.append(_null);
    buff.append(_null);
    buff.append(_8BitsSum(buff.mid(1)));//18
    buff.append(0x23);
    writeToTcp(buff);

     qDebug()<<"移除系统"<<id;
     QAction *agv_text=m_agvidAction.value(id);
     menu_agvinfo->removeAction(agv_text);
     m_agvidAction.remove(id);
     agvxianshi agcxs=idAgvXinshi.value(id);
     QtProperty *item=agcxs.item1;
     propertyEditor2->removeProperty(item);
     idAgvXinshi.remove(id);

     /*
     QMap<QByteArray,AGV*> m_addressAgv=m_mapIDtoAGV;
    foreach (AGV *agv, m_addressAgv.values()) {
         if( agv->get_id()==id)
         {
             QByteArray a=m_addressAgv.key(agv);
             m_addressAgv.remove(a);
             int target=agv->get_targetID();
             m_topoView->set_register(QString("Ver_%1").arg(target),"",agv->get_count());
             agv->~AGV();
         }
    }
            m_mapIDtoAGV=m_addressAgv;
      QAction *agv_text=m_agvidAction.value(id);
      menu_agvinfo->removeAction(agv_text);
      m_agvidAction.remove(id);
      agvxianshi agcxs=idAgvXinshi.value(id);
      QtProperty *item=agcxs.item1;
      propertyEditor2->removeProperty(item);
      idAgvXinshi.remove(id);

*/

}


void MainWindow::whichMission(int id, bool hasmission, bool reset, bool finish)
{
   // emit
    QMap<QString,St_Mission> m_agvStMission=m_trafficManager->m_mapAGVandMission;
    AGV_INFORMATION infor=m_agvINformation.value(id);
    QString agvname=infor.name;
    if(hasmission==true)
    {
         if(m_agvStMission.keys().contains(agvname))
         {
             Mission mission=m_agvStMission.value(agvname).m_mission;
             quint64 missionid=mission.m_nID;
             if((reset==true)&&(finish==false))
             emit missionTowrite(id,hasmission,missionid,true);
             else if((reset==false)&&(finish==true))
             emit missionTowrite(id,hasmission,missionid,false);
         }
    }
    else
    {
         emit missionTowrite(id,hasmission,0,false);
        qDebug()<<"无任务";
    }
    emit agvinfor(id);

}
void MainWindow::get_agvAndMission(int id)
{
   QMap<QString,St_Mission> m_agvStMission=m_trafficManager->m_mapAGVandMission;

   AGV_INFORMATION infor=m_agvINformation.value(id);
   QString agvname=infor.name;
   if(m_agvStMission.keys().contains(agvname))
   {
       Mission mission=m_agvStMission.value(agvname).m_mission;
       int start=mission.m_nStartNodeID;
       int end  =mission.m_nEndNodeID;
       bool reach=mission.m_bReachStartNode;
       emit missionfromId(true,reach,start,end);

   }
   else
   {
       emit missionfromId(false,0,0,0);
   }




}
void MainWindow::sleep(int msec)//自定义Qt延时函数,单位毫秒
{
    QDateTime last = QDateTime::currentDateTime();
    QDateTime now;
    while (1)
    {
        now = QDateTime::currentDateTime();
        if (last.msecsTo(now) >= msec)
        {
            break;
        }
    }
}
 void MainWindow:: dianliangdi()
 {
     static bool flag=false;
      AGV* agv = (AGV*)sender();

      if (flag==false)
      {
          flag=true;
     QMessageBox::information(this, tr("低电量警告"),QString("%1").arg(agv->get_name())+"低电量，请尽快充电", QMessageBox::Ok);
    }
 }


 //显示库位的变化
  void MainWindow::kuweixiugai(int id,int huoliang)
  {
      QtVariantProperty *storage=idgoods[id];//4.9保存下来货量
      storage->setValue(QString::number(huoliang));
  }

