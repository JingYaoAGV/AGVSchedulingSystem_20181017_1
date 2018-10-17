#ifndef SWEPTAREA_H
#define SWEPTAREA_H
#include "topoview.h"
#include "global.h"
#include <QVector>
#include <QSqlRecord>

class SweptArea
{
public:
    SweptArea();

    void LoadTopoView(TopoView* topoview);
    QList<QPointF> pointsInEdgeOfArea(TopoEdge *edge);
    QList<QPointF> pointsAroundVer(TopoVer *ver);
    void SwepArc(qreal sita,qreal sita2,QPointF p1,QPointF p2,QList<QPointF> &points);
    void pointsAroundVer(QPointF ver,int sita,QList<QPointF> &verpoints);
    bool isVerInArea(QPointF,QList<QPointF>);
private:
    TopoView *m_topoview;
   // QMap<TopoEdge*,int>edge_type;

 public:
    QMap<int,QList<QPointF>>edge_points;//扫过面积的关键点
    QMap<int,QList<int>>verInArea;//哪个点在面积内
    QMap<int,QList<QPointF>>EquPoInArea;//平均点在面积内
    QMap<int,QList<int>>collsionEdge;//哪个点在面积内
    QMap<int,QList<int>>collsionEdgeVer;//哪个点在面积内
    QList<QPointF>pointsinLine(QPointF,QPointF);
    QMap<int,QList<int>>huichepoints;
    QMap<int,QList<int>> huicheEdges;
    //QMap<int,QList<int>> rukouEdges;
     QList<int> rukouEdges;
   // QList<int> rukouids;
     QList<int> commonLine;
     QMap<int,QList<int>>m_commonEndEdges;
     QMap<int,QList<int>>m_commonEdges;
private:
     QSqlQuery m_queryServer;
};

#endif // SWEPTAREA_H
