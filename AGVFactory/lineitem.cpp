#include "lineitem.h"


LineItem::LineItem(QGraphicsScene *scene,QPointF start,QPointF end,QPen p)
{
    setLine(start.x(),start.y(),end.x(),end.y());
    setPen(p);
    scene->addItem(this);
}
