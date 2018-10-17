#ifndef TOPOVIEW_H
#define TOPOVIEW_H
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>
#include <QTransform>
#include <QDebug>
#define MARGIN_LEFT 10
#define MARGIN_RIGHT 10
#define MARGIN_TOP 10
#define MARGIN_BOTTOM 5
class TopoEdge;
class TopoVer;
class Registry;

struct PathEdge
{
    int start;
    int end;

};

struct Dis_point
 { int point;
    qreal distance;
 };
struct ID_Manager{
    int maxID;
    QVector<int> v_IDs;
    ID_Manager(){
        maxID = 0;
        v_IDs.clear();
    }
    int get_newID(int id=-1){
        if(id<0 || v_IDs.contains(id)){
            v_IDs.push_back(++maxID);
            return maxID;
        }
        else{
            if(id>maxID) maxID = id;
            v_IDs.push_back(id);
            return id;
        }
    }
    void clear_IDs(){
        maxID = 0;
       v_IDs.clear();
    }
};
// 拓扑节点
class TopoVer : public QObject, public QGraphicsEllipseItem{
    Q_OBJECT
    Q_PROPERTY(QPoint _center READ get_center WRITE set_center)
    Q_PROPERTY(int _width READ get_width WRITE set_width)
    Q_PROPERTY(QColor _brush_color READ get_brushColor WRITE set_brushColor)
    Q_PROPERTY(bool _ver_type READ is_callPoint WRITE set_callPoint)
    Q_PROPERTY(QString _name READ get_name)
public:
    TopoVer(QGraphicsScene *scene,int sita,int id,int tunnel,QPointF center=QPointF(), int radius=12,int ver_type=NORMAL);
    ~TopoVer();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int get_width()const{return rect().width();}
    QColor get_brushColor()const{if(brush().style()==Qt::NoBrush) return Qt::white;else return brush().color();}
    QString get_name()const{return m_strID;}
    int get_id()const{return m_nID;}
    int get_tunnel()const{return isInTunnel;}
    qreal get_sita()const{return m_nSita;}
    qreal get_x()   const{return m_x;}
    qreal get_y()   const{return m_y;}
    QString get_register(){return m_strRegisterAGV;}
    bool get_highlight(){return m_bIsHighlight;}
    QPoint get_center() const;
    bool is_callPoint(){return m_nVerType==CALLPOINT;}
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void set_register(QString agv,int count){ m_strRegisterAGV=agv;m_RegistryAgvCount=count;update(boundingRect());}
    void set_highlight(bool flag,QColor color=Qt::blue){m_bIsHighlight=flag;m_clrHighlight=color;update(boundingRect());}
    void show_label(bool show=true);
public:
    enum VER_TYPE{NORMAL,CALLPOINT};
    TopoEdge *m_pEdgeHead;
    Registry *m_registry;
    int m_nID;
    qreal realx;
    qreal realy;
    int m_x;
    int m_y;
    int SLType;
    int SLNumber;//工位号
    qreal m_nSita;
private:
    int m_nVerType;

    int isInTunnel;


    qreal m_rZValue;

    QString m_strID;
    QString m_strRegisterAGV;
    int     m_RegistryAgvCount;
    bool m_bIsHighlight;
    QColor m_clrHighlight;
    QGraphicsSimpleTextItem *m_labelItem;
    static QBrush m_brushDefault;
    QMap<QString,int>Ver_AGV_Pri;


signals:
    void property_change(QMap<QString,qreal>prop);  // 通知属性窗口更改属性编辑器内容
    void position_change(QPointF);    // 通知相邻的边更改其位置
    void address_change(QString);
    void ver_dirty();
public slots:
    void set_center(QPoint center);
    void set_x(qreal x){if(isSelected()){ setPos(x,pos().y());emit position_change(pos());emit ver_dirty();}}
    void set_y(qreal y){if(isSelected()){ setPos(pos().x(),y);emit position_change(pos());emit ver_dirty();}}
    void set_width(int width){if(isSelected()) {set_size(width,width);emit ver_dirty();}}
    void set_size(int w,int h){if(isSelected()) {setRect(-w/2,-h/2,w,h);emit ver_dirty();}}
    void set_brushColor(QColor color){if(isSelected()){if(color==Qt::white) setBrush(Qt::NoBrush); else setBrush(color);m_brushDefault=brush();emit ver_dirty();}}
    void set_vertype(int t){if(isSelected()) {m_nVerType=t;emit ver_dirty();}}
    void set_callPoint(bool flag){if(isSelected()){if(flag) {m_nVerType = CALLPOINT;setBrush(Qt::magenta);} else {m_nVerType = NORMAL;setBrush(Qt::blue);} scene()->update(boundingRect());emit ver_dirty();}}//
    void set_selected(){scene()->clearSelection();setSelected(true);}
};

// 拓扑边
class TopoEdge : public QGraphicsObject{
    Q_OBJECT
    Q_PROPERTY(int _pen_width READ get_penWidth WRITE set_penWidth)
    Q_PROPERTY(QColor _pen_color READ get_penColor WRITE set_penColor)
    Q_PROPERTY(QString _name READ get_name)
    Q_PROPERTY(bool _dash READ get_dash WRITE set_dash)
    Q_PROPERTY(qreal _weight READ get_weight WRITE set_weight)
    Q_PROPERTY(int _start_node READ get_startID)
    Q_PROPERTY(int _end_node READ get_endID)
public:
    TopoEdge(QGraphicsScene* scene,int type,int id, int start_id, int end_id, QPointF startpos, QPointF endpos, TopoEdge *next);
    ~TopoEdge(){}
    int type() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    QPen pen() const{return m_pen;}
    int get_penWidth() const{return pen().width();}
    bool get_dash() const{return pen().style()==Qt::DashLine;}
    QColor get_penColor() const{return pen().color();}
    void setPen(QPen p){m_pen = p;scene()->update();}//
    void setLine(QLineF line);
    int get_id() const{return m_nID;}
    int get_startID() const{return m_nStartID;}
    int get_endID() const{return m_nEndID;}
    int get_type()  const{return m_nType;}
    qreal get_weight() const{return m_nWeight;}
    QString get_name() const{return m_strID;}
    QMap<QString,qreal> get_property();
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
public:
    QString m_strStartVerName;
    QString m_strEndVerName;
    int m_nStartID;
    int m_nEndID;
    QPointF m_ptStartPos;
    QPointF m_ptEndPos;
    TopoEdge *m_pNextEdge;
    Registry *m_registry;
    qreal m_nWeight;
    int m_nID;
    qreal realWeight;
private:
    int m_nWidth;

    qreal m_rZValue;
    static QPen m_penDefault;
    QPen m_pen;
    QString m_strID;
    int m_nType;
signals:
    void property_change(QMap<QString,qreal>);
public slots:
    void set_penColor(QColor color){if(isSelected()){QPen p = pen();p.setColor(color);setPen(p);m_penDefault=p;}}
    void set_penWidth(int n_penWidth){if(isSelected()){QPen p = pen();p.setWidth(n_penWidth+1);setPen(p);m_penDefault=p;}}
    void set_dash(bool flag);
    void set_weight(qreal w){m_nWeight = w;}
    //void startPosChange(QPointF start);
    //void endPosChange(QPointF end);
    //void set_selected(){scene()->clearFocus();scene()->clearSelection();setFocus();setSelected(true);}
};


// 拓扑图视图
class TopoView : public QObject
{
    Q_OBJECT
public:
    TopoView(QGraphicsScene *scene);
    ~TopoView();
    TopoVer *add_topoVer(QPointF center,int sita,int tunnel,int SLNumber,int id=0,int radius=12,QColor color=Qt::blue,bool is_callPoint=false);//带有默认值参数必须放在后面
    TopoEdge *add_topoEdge(TopoVer* start,TopoVer* end,int type,int id=-1,QColor color=Qt::green,bool is_dashline=false);
    QGraphicsScene* scene(){return m_scene;}
    QTransform get_transform(){return m_transform;}
    bool remove_topoVer(TopoVer *ver);
    bool remove_topoEdge(QString start, QString end);
    bool remove_topoEdge(TopoEdge* edge);
    int get_verCount(){return m_nVerCount;}
    int get_edgeCount(){return m_nEdgeCount;}
    void set_register(QString verName,QString agvName,int agvcount);
    bool is_verExist(TopoVer *ver) const;
    bool is_edgeExist(QString start, QString end) const;
    void show_nodeName(bool flag=true);
    void clear();
    void DFS();
    void DistanceJudge();
    static bool CompareDistance(const Dis_point&a1,const Dis_point&a2)//从小到大
    {
       if(a1.distance<=a2.distance)
          return true;
       else
           return false;
   }


  //  QList<int> get_commonRoute(){return m_commonRoute;}

    // 数据库操作
public:
    bool load_topoMap(qreal zoom,QPointF coordinate,qreal hight);
    bool save_topoMap();
    void draw_path(QColor color,QList<int> path);
    TopoVer *get_verByID(int id);
    TopoEdge* get_edgeByVer(int start,int end);
    void crossSegments(QMap<int,QList<int>> &collionSegments);
    bool segments_intersect (QPointF p1, QPointF p2, QPointF p3, QPointF p4);
    int direction (QPointF p0, QPointF p1, QPointF p2);
private:
    void DFS(QString u, QMap<QString, bool> &visited, QMap<QString,int> &dfn, QMap<QString,int> &low, QMap<QString, QString> &parentNode);
    void createAdjMatrix();
   // QList<int> m_commonRoute;
public:
    QGraphicsScene *m_scene;
    QTransform m_transform;
    QMap<int,int> m_map;
    // 拓扑图操作
    QMap<QString,TopoVer*> m_verList;
    QMap<int,int> m_verIdTunnel;
    QMap<QString,TopoEdge*> m_edgeList;
    QMap<quint32,TopoEdge*>EdgeSql;
    QMap<TopoEdge*,int> m_nEdgeType;
    QMap <int,QList<Dis_point>>kuwei_D;

    QMap<quint32,float>EdgeLength;//记录边的长度
    QMap<quint32,int>EdgeDegree;//记录边的角度
    QMap<int,int>PointDegree;//库位的位姿
    QMap<int,int>SLNumber_RFID;







    QList<QString> m_articulationVerList;
    QList<int> m_kuwei;
    QList<TopoVer*> kuweipoint;
    int m_nVerCount;
    int m_nEdgeCount;
    ID_Manager m_verIDM;
    ID_Manager m_edgeIDM;
signals:
    void step_progressBar(int);
    void set_dirty();  // 通知主窗口拓扑图发生了变化

private slots:
    void app_stateChanged(int state);
};

#endif // TOPOVIEW_H
