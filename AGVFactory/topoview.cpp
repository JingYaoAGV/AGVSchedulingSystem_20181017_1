#include "topoview.h"
#include "global.h"
#include <QtMath>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsItem>
#include <QSql>
#include <QList>
#include <QMap>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFile>
#include <QPainter>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

TopoView::TopoView(QGraphicsScene *scene)
{
    m_scene = scene;
    m_nVerCount = 0;
    m_nEdgeCount = 0;
}

TopoView::~TopoView()
{
}

TopoVer* TopoView::add_topoVer(QPointF center,int sita,int tunnel,int SLNumber,int id,int radius, QColor color, bool is_callPoint)
{
    int real_ID = m_verIDM.get_newID(id); // 由ID生成器生成唯一的ID
    QString ver_name = QString("Ver_%1").arg(real_ID);
    TopoVer *ver = new TopoVer(scene(),sita,real_ID,tunnel,center,radius);
    ver->SLNumber=SLNumber;
    ver->set_callPoint(is_callPoint);
    ver->set_brushColor(color);
    m_verList.insert(ver_name,ver);
    m_verIdTunnel.insert(id,tunnel);
    m_nVerCount++;
    if(is_callPoint==true)
    {
        m_kuwei.append(id);
        kuweipoint.append(ver);
    }
    connect(ver,SIGNAL(ver_dirty()),this,SIGNAL(set_dirty()));
    emit set_dirty();   // 通知主窗口拓扑图发生改变
    return ver;
}

TopoEdge* TopoView::add_topoEdge(TopoVer *start, TopoVer *end,int type,int id, QColor color, bool is_dashline)
{


    if(is_verExist(start) && is_verExist(end)){
        //加入线段QMAP
        int real_ID = id; // 由ID生成器生成唯一的ID
        QString edge_name = QString("Edge_%1").arg(real_ID);


        int start1 =start->m_nID;
        int end1=end->m_nID;
        quint32 startend=start1*10000+end1;



        TopoEdge *edge = new TopoEdge(scene(),type,m_edgeIDM.get_newID(id),start->get_id(),end->get_id(),start->pos(),end->pos(),m_verList[start->get_name()]->m_pEdgeHead);
        edge->set_penColor(color);
        edge->set_dash(is_dashline);
        //connect(start,SIGNAL(position_change(QPointF)),edge,SLOT(startPosChange(QPointF)));
       // connect(end,SIGNAL(position_change(QPointF)),edge,SLOT(endPosChange(QPointF)));


        edge->m_pNextEdge = start->m_pEdgeHead;
        start->m_pEdgeHead = edge;
        m_edgeList.insert(edge_name,edge);
        EdgeSql[startend]=edge;

        //if((edge->m_nStartID==1)&&(edge->m_nEndID==2))
          //  qDebug()<<"*路径规划测试："<<"error"<<edge->m_pNextEdge->m_nStartID<<endl;
       // m_nEdgeType.insert(edge,type);
       /* if(commonRoute==1)
        {
            m_commonRoute.append(id);
        }
        */
        m_nEdgeCount++;
        emit set_dirty();   // 通知主窗口拓扑图发生改变
        return edge;
    }
    else return 0;
}

bool TopoView::remove_topoVer(TopoVer *ver)
{
    if(!m_verList.contains(ver->get_name())){
        return false;
    }
    TopoEdge *p = m_verList[ver->get_name()]->m_pEdgeHead,*q;
    bool result = true;
    while(p!=NULL){
        q = p->m_pNextEdge;
        result &= remove_topoEdge(p);
        p = q;
    }
    foreach(TopoVer* start,m_verList.values()){
        if(start->get_name()!=ver->get_name())
            remove_topoEdge(start->get_name(),ver->get_name());  // 在remove_topoEdge函数中检查是否存在该边
    }
    scene()->removeItem(m_verList[ver->get_name()]);
    result &= m_verList.remove(ver->get_name());
    m_nVerCount--;
    emit set_dirty();   // 通知主窗口拓扑图发生改变
    return result;
}

bool TopoView::remove_topoEdge(QString start,QString end)
{
    if(!is_edgeExist(start,end)) return false;

    TopoEdge *p = m_verList[start]->m_pEdgeHead,*q;
    if(p==NULL) return false;
    if(p->m_strEndVerName==end){
        m_verList[start]->m_pEdgeHead = p->m_pNextEdge;
        p->deleteLater();
        --m_nEdgeCount;
        return true;
    }
    while(p->m_pNextEdge!=NULL && p->m_pNextEdge->m_strEndVerName!=end) p = p->m_pNextEdge;
    if(p->m_pNextEdge==NULL) return false;
    q = p->m_pNextEdge;p->m_pNextEdge = q->m_pNextEdge;q->deleteLater();
    --m_nEdgeCount;
    emit set_dirty();   // 通知主窗口拓扑图发生改变
    return true;
}

bool TopoView::remove_topoEdge(TopoEdge *edge)
{
    return remove_topoEdge(edge->m_strStartVerName,edge->m_strEndVerName);
}

void TopoView::set_register(QString verName, QString agvName,int count)
{
    if(m_verList.count(verName)){
        m_verList.value(verName)->set_register(agvName,count);
    }
}

bool TopoView::is_verExist(TopoVer *ver) const
{
    return m_verList.count(ver->get_name())>0;
}

bool TopoView::is_edgeExist(QString start,QString end) const
{
    if(!m_verList.contains(start))
        return false;

    TopoEdge *p = m_verList.value(start)->m_pEdgeHead;
    while(p!=NULL && p->m_strEndVerName!=end) p = p->m_pNextEdge;
    return p!=NULL;
}

void TopoView::show_nodeName(bool flag)
{
    foreach(TopoVer* ver,m_verList.values()){
        ver->show_label(flag);
    }
}

void TopoView::clear()
{
    foreach(TopoVer*ver, m_verList.values()){
        remove_topoVer(ver);
    }
    m_verList.clear();
    m_verIDM.clear_IDs();  // 清除ID生成器内容
    m_edgeIDM.clear_IDs();
    emit set_dirty();   // 通知主窗口拓扑图发生改变
}

void TopoView::DFS()
{
    if(!m_verList.isEmpty()){
        QMap<QString,int> dfn;
        QMap<QString,int> low;
        QMap<QString,bool> visited;
        QMap<QString,QString> parentNode;
        foreach(QString name,m_verList.keys()){
            visited[name] = false;
        }
        DFS(m_verList.firstKey(),visited,dfn,low,parentNode);
    }
}
//形成一个库位点对其他库位点的距离MAP,对每一个值的qlist进行排序
void TopoView::DistanceJudge()
{
    TopoVer* point;


   foreach( point, kuweipoint)
   {
       QList<Dis_point>D;
       foreach(TopoVer*point1, kuweipoint)
       {

           if(point1->m_nID!=point->m_nID)
           {
              qreal distance=getDistance(QPointF(point->realx,point->realy),QPointF(point1->realx,point1->realy));
              Dis_point dispoint;
              dispoint.distance=distance;
              dispoint.point=point1->m_nID;
              D.append(dispoint);

           }

       }
        qSort(D.begin(),D.end(),CompareDistance);
       kuwei_D[point->m_nID]=D;
       qDebug()<<"库位点:"<<point->m_nID<<point->realx<<point->realy<<endl;
       for(int i=0;i<D.count();i++)
       {
       qDebug()<<(D.at(i)).distance<<endl;
       }
   }




}


//---排序判断函数，排序一下试一试，确定是从大到小还是从小到大




void TopoView::DFS(QString u, QMap<QString,bool> &visited, QMap<QString, int> &dfn, QMap<QString, int> &low,QMap<QString,QString> &parentNode)
{
    static int counter = 0;
    int child = 0;
    TopoEdge *p = m_verList.value(u)->m_pEdgeHead;
    visited[u] = true;
    dfn[u] = low[u] = counter++;
    for(;p!=0;p=p->m_pNextEdge){
        QString v = p->m_strEndVerName;
        if(!visited[v]){
            child ++;
            parentNode[v] = u;
            DFS(v,visited,dfn,low,parentNode);
            low[u] = qMin(low[u],low[v]);
            if(parentNode[u]=="" && child>1 && m_articulationVerList.count(u)==0)
                m_articulationVerList.append(u);
            else if(parentNode[u]!="" && low[v]>=dfn[u] && m_articulationVerList.count(u)==0){
                m_articulationVerList.append(u);
            }
        }
        else if(v!=parentNode[u]){
            low[u] = qMin(low[u],dfn[v]);
        }
    }
}

void TopoView::createAdjMatrix()
{
    int **adjMatrix = new int*[m_nVerCount];
    for(int i=0;i<m_nVerCount;i++)
        adjMatrix[i] = new int[m_nVerCount];
    for(int i=0;i<m_nVerCount;i++)
        for(int j=0;j<m_nVerCount;j++)
            adjMatrix[i][j] = 1000000;
    for(int i=0;i<m_nVerCount;i++)
        adjMatrix[i][i] = 0;
    foreach(TopoVer* ver,m_verList.values()){
        TopoEdge* p = ver->m_pEdgeHead;
        int row = m_map.value(ver->get_id());
        while(p!=NULL){
            int weight = p->get_weight();
            int col = m_map.value(p->get_endID());
            adjMatrix[row][col] = weight;
            p = p->m_pNextEdge;
        }
    }
    QFile file("data.csv");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return;
    QTextStream out(&file);
    for(int i=0;i<m_nVerCount;i++){
        for(int j=0;j<m_nVerCount;j++){
            out<<adjMatrix[i][j];
            if(j!=m_nVerCount-1) out<<",";
        }
        out<<endl;
    }
    out.flush();
    file.close();


}

void TopoView::app_stateChanged(int state)
{
    bool flag = state==APP_STOP;
    foreach(TopoVer* ver,m_verList.values())
        ver->setFlag(QGraphicsItem::ItemIsMovable,flag);

}

bool TopoView::load_topoMap(qreal zoom,QPointF coordinate,qreal hight)
{
    clear();
    qreal maxX,maxY,minX,minY;
    maxX = maxY = minX = minY = 0;
    QSqlQuery query = getQuery();
    if(!query.exec("select * from tb_topover where x in (select max(x) from tb_topover)")) return false;
    while(query.next()){
        QSqlRecord record = query.record();
        maxX = record.value("x").toReal()*1000;
       qDebug()<<"maxX=:"<<minX<<endl;
    }

    if(!query.exec("select * from tb_topover where y in (select min(y) from tb_topover)")) return false;
    while(query.next()){
        QSqlRecord record = query.record();
        maxY = hight-record.value("y").toReal()*1000;
       qDebug()<<"maxY=:"<<maxY<<endl;
    }
    if(!query.exec("select * from tb_topover where x in (select min(x) from tb_topover)")) return false;
    while(query.next()){
        QSqlRecord record = query.record();
        minX = record.value("x").toReal()*1000;
       qDebug()<<"minX=:"<<minX<<endl;
    }
    if(!query.exec("select * from tb_topover where y in (select max(y) from tb_topover)")) return false;
    while(query.next()){
        QSqlRecord record = query.record();
        minY = hight-record.value("y").toReal()*1000;
    }
    if(qFuzzyCompare(maxX,0) && qFuzzyCompare(maxY,0)
            && qFuzzyCompare(minX,0) && qFuzzyCompare(minY,0)){
        return false;   // 无拓扑图信息
    }


    // 计算拓扑视图中四周留白区域
   /* maxX*=(100+sgn(maxX)*MARGIN_RIGHT)/100.0;
    minX*=(100-sgn(minX)*MARGIN_LEFT)/100.0;
    maxY*=(100+sgn(maxY)*MARGIN_BOTTOM)/100.0;
    minY*=(100-sgn(minY)*MARGIN_TOP)/100.0;
    */
   /* qreal scene_width = scene()->width();
    qreal scene_height = scene()->height();
    QTransform scale,translate;
    translate.translate(-minX,-minY);
    scale.scale(scene_width/(maxX-minX),scene_height/(maxY-minY));
    m_transform = translate*scale;
    setGlobalScale(scale);
    setGlobalTransform(m_transform);
    setGlobalInverseTransform(scale.inverted()*translate.inverted());
    */

    // QTransform scale,translate;
    // scale.scale(zoom,zoom);
    // translate.translate(coordinate.x(),-hight-coordinate.y());
     //setGlobalTransform(scale);
    // 将拓扑图中顶点添加到拓扑视图里
    if(!query.exec("select * from tb_topover")) return false;
    qreal posx,posy;
    qreal realx,realy;
    int id,width,nbrush;
    QBrush brush;
    bool type;
    int SLType;
    int SLNumber;
    int isInTunnel=-1;
    qreal sita;
    while(query.next()){
        QSqlRecord record = query.record();
        id = record.value("NodeID").toByteArray().toInt(0,16);
        //m_map.insert(id,s_nID++);  // 为了创建邻接矩阵
        realx= record.value("x").toReal();
        realy= record.value("y").toReal();
        posx = record.value("x").toReal()*1000*zoom+coordinate.x();
        posy = hight-(record.value("y").toReal()*1000*zoom+coordinate.y());
        width = record.value("width").toInt();
        if(width==0) width = 5;
        type = record.value("SLType").toBool();
        nbrush = record.value("brush").toInt();
        isInTunnel=record.value("isInTunnel").toInt();
        sita=record.value("sita").toInt();
        SLType=record.value("SLType").toInt();
        SLNumber=record.value("SLNumber").toInt();
        if(type==1)
        {
            PointDegree[id]=sita;
            SLNumber_RFID[SLNumber]=id;
        }
        else
        {

        }

        if(nbrush==0) brush = QBrush(Qt::blue);
        else brush = intToBrush(nbrush);
        add_topoVer(QPointF(posx,posy),sita,
                    isInTunnel,SLNumber,id,5,brush.color(),type);
        m_verList[QString("Ver_%1").arg(id)]->realx=realx;//折中办法，求取真实点坐标
        m_verList[QString("Ver_%1").arg(id)]->realy=realy;
        m_verList[QString("Ver_%1").arg(id)]->SLType=SLType;



    }

    // 从数据库中载入拓扑边
    if(!query.exec("select * from tb_topoedge"))return false;
    int pathID,startNodeID,endNodeID,angel;

    qreal weight,weight1;
    int edgetype=-1;
    while(query.next()){
        QSqlRecord record = query.record();
        pathID = record.value("pathID").toInt();
        startNodeID = record.value("startNodeID").toByteArray().toInt(0,16);
        endNodeID = record.value("endNodeID").toByteArray().toInt(0,16);
        weight1 = record.value("weight").toReal();
        edgetype=record.value("type").toInt();
        angel=record.value("angel").toInt();
        TopoVer *start=0,*end=0;
        foreach(TopoVer* ver,m_verList.values()){
            if(ver->get_id()==startNodeID)
                start = ver;
            else if(ver->get_id()==endNodeID)
                end = ver;
            if(start!=0 && end!=0){
                TopoEdge *edge = add_topoEdge(start,end,edgetype,pathID);
                weight = getDistance(mapToRealWorld(start->pos()),mapToRealWorld(end->pos()));
                //if(start->get_id()==10)
                // qDebug()<<"start"<<(start->pos())<<"xx"<<(mapToRealWorld(start->pos()));
                //qDebug()<<"weight"<<weight;
                edge->set_weight(weight);
                edge->realWeight=weight1;
                qDebug()<<"路径边长:"<<edge->realWeight<<endl;
                break;
            }
        }

        EdgeLength[startNodeID*10000+endNodeID]=weight1;//记录边的长度
        qDebug()<<"长度"<<startNodeID*10000+endNodeID<<"-"<<weight1;
         qDebug()<<"角度"<<angel;

        EdgeDegree[startNodeID*10000+endNodeID]=angel;//记录边的长度


    }

    //createAdjMatrix();
    return true;
}

bool TopoView::save_topoMap()
{
    QSqlQuery query = getQuery();
    query.exec("drop table tb_topover");
    query.exec("drop table tb_topoedge");
    query.exec("create table tb_topover (NodeID int primary key,"
               "x real ,y real,width int ,brush int, type int,isInTunnel int,sita int)");
    query.exec("create table tb_topoedge (pathID int primary key,"
               "startNodeID int ,endNodeID int,weight int,pen int, type int)");
    int count = 0;
    foreach(TopoVer* ver,m_verList.values()){
        count += 2;
        emit step_progressBar(count);
        QPointF real_pos = mapToRealWorld(ver->pos());
        query.exec(QString("insert into tb_topover values(%1,%2,%3,%4,%5,%6,%7,%8)")
                             .arg(ver->get_id()).arg(real_pos.x()/1000).arg(real_pos.y()/1000).arg(ver->get_width())
                             .arg(brushToInt(ver->brush())).arg(ver->is_callPoint()).arg(ver->get_tunnel()).arg(ver->get_sita()));
        TopoEdge* edge = ver->m_pEdgeHead;
        while(edge){
            emit step_progressBar(++count);
            query.exec(QString("insert into tb_topoedge values (%1,%2,%3,%4,%5,%6)")
                       .arg(edge->get_id()).arg(edge->get_startID())
                       .arg(edge->get_endID()).arg(edge->get_weight()/1000.0)
                       .arg(penToInt(edge->pen()))
                       .arg(edge->get_type()));
            edge = edge->m_pNextEdge;
        }
    }
    return true;
}

void TopoView::draw_path(QColor color, QList<int> path)
{
    foreach(TopoVer *ver,m_verList.values()){
        ver->set_highlight(false);
    }

    for(int i=0;i<path.length();i++){
        int n = path.at(i);
        TopoVer* ver = m_verList.value(QString("Ver_%1").arg(n));
        ver->set_highlight(true,color);
    }
}

TopoVer *TopoView::get_verByID(int id)
{
    QString name = QString("Ver_%1").arg(id);
    return m_verList.value(name,NULL);
}

 TopoEdge *TopoView::get_edgeByVer(int start, int end)
{
    const TopoVer* ver = get_verByID(start);
    if(ver){
        TopoEdge* edge = ver->m_pEdgeHead;
        while(edge){
            if(edge->m_nEndID==end)
                return edge;
            edge = edge->m_pNextEdge;
        }
    }
    return 0;
}
void TopoView::crossSegments(QMap<int, QList<int> > &collionSegments)
{
    QMap<QString,TopoEdge*>::const_iterator i;
    for(i=m_edgeList.constBegin();i!=m_edgeList.constEnd();i++)
    {
       TopoEdge* edge=i.value();
       int id=edge->get_id();

       QPointF point1=mapToRealWorld(get_verByID(edge->m_nStartID)->pos());
       QPointF point2=mapToRealWorld(get_verByID(edge->m_nEndID)->pos());
       QList<int> segments;
       foreach (TopoEdge *edge2, m_edgeList.values()) {
          int id2=edge2->get_id();
          if(id!=id2)
          {
              QPointF point3=mapToRealWorld(get_verByID(edge2->m_nStartID)->pos());
              QPointF point4=mapToRealWorld(get_verByID(edge2->m_nEndID)->pos());
              bool collison=segments_intersect(point1,point2,point3,point4);
              if(collison==true)
              {
                  segments.append(id2);
              }

          }
       }
       collionSegments.insert(id,segments);

    }
}
bool TopoView::segments_intersect(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    int d1, d2, d3, d4;
    d1 = direction(p3, p4, p1);
    d2 = direction(p3, p4, p2);
    d3 = direction(p1, p2, p3);
    d4 = direction(p1, p2, p4);
    if (((d1 < 0 && d2 > 0) || (d1 > 0 && d2 < 0)) && ((d3 < 0 && d4 > 0) || (d3 > 0 && d4 < 0)))
            return true;
    else
            return false;
}
int TopoView::direction(QPointF p0, QPointF p1, QPointF p2)
{
     return ((p2.x() - p0.x())*(p1.y() - p0.y()) - (p1.x() - p0.x())*(p2.y() - p0.y()));
}
QColor color_list1[7] = {Qt::red,Qt::yellow ,Qt::white,Qt::cyan,Qt::magenta,Qt::darkRed,Qt::lightGray};
QBrush TopoVer::m_brushDefault=QBrush(Qt::blue);
TopoVer::TopoVer(QGraphicsScene *scene,int sita,int id,int tunnel,QPointF center, int radius, int ver_type)
{
    scene->clearSelection();
    setBrush(m_brushDefault);
    setPos(center);
    setRect(-radius/2,-radius/2,radius,radius);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
   // setFlag(QGraphicsItem::ItemIsMovable);
    m_rZValue = 100;
    setZValue(m_rZValue);
    m_nVerType = ver_type;
    m_nID = id;
    m_x=center.x();
    m_y=center.y();
    isInTunnel=tunnel;
    m_nSita=sita;
    m_labelItem = new QGraphicsSimpleTextItem(QString("%1").arg(m_nID,4,16,QLatin1Char('0')));//空位补0
    m_labelItem->setPen(QPen(Qt::white,1));
    QFont label_font = m_labelItem->font();
    label_font.setPointSize(15);
    m_labelItem->setFont(label_font);
    m_labelItem->setPos(center.x(),center.y()-20);
    m_labelItem->setZValue(10000);
    m_labelItem->setVisible(false);
    m_pEdgeHead = NULL;
    m_strID = QString("Ver_%1").arg(id);
    setPen(Qt::NoPen);
    scene->addItem(this);
    scene->addItem(m_labelItem);
    setSelected(true);
    m_registry = new Registry;
    m_bIsHighlight = false;
}

TopoVer::~TopoVer()
{

}


void TopoVer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if(!m_strRegisterAGV.isEmpty()){
        setBrush(color_list1[(m_RegistryAgvCount)%7]);
    }

    else if(m_bIsHighlight){
        setBrush(m_clrHighlight);
    }
    else if(is_callPoint())
        setBrush(Qt::magenta);
    else
        setBrush(Qt::blue);
    QGraphicsEllipseItem::paint(painter,option,widget);
}

QVariant TopoVer::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change==ItemSelectedChange){
        if(isSelected()) setZValue(m_rZValue);
        else setZValue(1000);
    }
    return QGraphicsEllipseItem::itemChange(change,value);
}

void TopoVer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit position_change(pos());
    emit ver_dirty();
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void TopoVer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit position_change(pos());
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

QPoint TopoVer::get_center() const
{
    QPointF real_pos = mapToRealWorld(pos());
    return QPoint(real_pos.x(),real_pos.y());
}

void TopoVer::show_label(bool show)
{
    m_labelItem->setVisible(show);
}

void TopoVer::set_center(QPoint center)
{
    if(isSelected()){
       // center.setY(center.y());
        QPointF real_pos = mapFromRealWorld(center);
        setPos(real_pos);
    }
}

QPen TopoEdge::m_penDefault = QPen(Qt::green,2,Qt::SolidLine);
TopoEdge::TopoEdge(QGraphicsScene *scene,int type,int id, int start_id, int end_id, QPointF startpos, QPointF endpos, TopoEdge *next)
{
    scene->clearSelection();
    m_nWidth = 0;
    m_pNextEdge = next;
    //setFlag(QGraphicsItem::ItemIsFocusable);
    //setFlag(QGraphicsItem::ItemIsMovable,false);
   // setFlag(QGraphicsItem::ItemIsSelectable,true);
    m_nID = id;
    m_strID = QString("Edge_%1").arg(m_nID);
   // scene->addItem(this);
   // setSelected(true);
   // setPen(m_penDefault);
    m_ptStartPos = startpos;
    m_ptEndPos = endpos;
    m_nStartID = start_id;
    m_nEndID = end_id;
    m_strStartVerName = QString("Ver_%1").arg(start_id);
    m_strEndVerName = QString("Ver_%1").arg(end_id);
    //setLine(QLineF(m_ptStartPos,m_ptEndPos));
    m_rZValue = 99;
    //setZValue(m_rZValue);
    m_registry = new Registry;
    m_nType=type;    //没啥用
}

int TopoEdge::type() const
{
    return EDGE;
}

void TopoEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(isSelected()){
        emit property_change(get_property());
    }
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(pen());
    painter->drawLine(m_nWidth/4,0,m_nWidth/4-5,5);
    painter->drawLine(m_nWidth/4,0,m_nWidth/4-5,-5);
    painter->drawLine(0,0,m_nWidth,0);
}

QRectF TopoEdge::boundingRect() const
{
    int penwidth = pen().width();
    return QRectF(-penwidth/2,-penwidth/2-5,penwidth+m_nWidth,penwidth+10);
}

void TopoEdge::setLine(QLineF line)
{
    QPointF p2;
    setPos(line.p1());
    p2.setX(line.x2()-line.x1());
    p2.setY(line.y2()-line.y1());
    m_nWidth = sqrt((p2.x()*p2.x())+(p2.y()*p2.y()));
    setRotation(atan2(p2.y(),p2.x())*180/M_PI);
}

QMap<QString, qreal> TopoEdge::get_property()
{
    QMap<QString,qreal> prop;
    return prop;
}

QVariant TopoEdge::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change==ItemSelectedChange){
        if(isSelected()) setZValue(m_rZValue);
        else setZValue(1000);
    }
    return QGraphicsObject::itemChange(change,value);
}


void TopoEdge::set_dash(bool flag)
{
    if(isSelected()){
        QPen p = pen();
        if(flag)
            p.setStyle(Qt::DashLine);
        else
            p.setStyle(Qt::SolidLine);
        setPen(p);
        m_penDefault = p;
    }
}

