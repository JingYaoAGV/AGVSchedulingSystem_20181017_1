#ifndef LINEITEM_H
#define LINEITEM_H



#include <QGraphicsLineItem>
#include <QPen>
#include <QGraphicsScene>
#include <QPointF>
class LineItem:QGraphicsLineItem
{

public:
   LineItem(QGraphicsScene *scene,QPointF start,QPointF end,QPen p=QPen());
};

#endif // LINEITEM_H
