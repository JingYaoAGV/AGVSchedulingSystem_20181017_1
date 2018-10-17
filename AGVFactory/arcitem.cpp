#include "arcitem.h"

ArcItem::ArcItem(QGraphicsScene *scene,QPointF leftTop, QPointF rightButtom,qreal sAngel,qreal spAngle,QPen p)
{
       QPainterPath path;
       QRect rect=QRect(leftTop.x(),leftTop.y(),rightButtom.x()-leftTop.x(),rightButtom.y()-leftTop.y());
       path.arcMoveTo(rect,sAngel);
       path.arcTo(rect,sAngel,spAngle);

       setPath(path);
       setPen(p);
       scene->addItem(this);
}
