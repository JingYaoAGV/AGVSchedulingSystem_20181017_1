#ifndef ARCITEM_H
#define ARCITEM_H

#include <QGraphicsPathItem>
#include <QPen>
#include <QGraphicsScene>
#include <QPointF>
class ArcItem:public QGraphicsPathItem
{
public:
    ArcItem(QGraphicsScene *scene,QPointF leftTop, QPointF rightButtom,qreal sAngel,qreal spAngle,QPen p=QPen());
};
#endif // ARCITEM_H
