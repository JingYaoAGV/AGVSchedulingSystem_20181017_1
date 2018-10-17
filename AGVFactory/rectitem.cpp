#include "rectitem.h"
#include "global.h"
#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlQuery>
RectItem::RectItem(QGraphicsScene *scene, qreal x, qreal y, qreal w, qreal h, int id, qreal sita, int div, int os, QPen p)
{
    //setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    
    p.setWidth(1);

    //p.setBrush(Qt::green);
    scene->addItem(this);
    leftTopx=x;
    leftTopy=y;
    wight=w;
    high=h;
    ID=id;
    Sita=sita;
    division = div;
    offset = os;
    m_name=QString("工位_%1").arg(id);
    //m_strID = QString("Ver_%1").arg(id);

    setRect(x,y,w,h);
    state=0;
}
QRectF RectItem::boundingRect() const
{
    QRectF r;
    if(wight>=0&&high>=0)
    {
        r=QRectF(leftTopx-1,leftTopy-10,wight+2,high+12);
        //qDebug()<<"1"<<ID;
    }
    else if(wight>=0&&high<0)
    {
        if(Sita==0)
      r=QRectF(leftTopx-1,leftTopy+high,wight+2,-high+12);
        else
         r=QRectF(leftTopx-12,leftTopy+high-2,wight+14,-high+4);
       // qDebug()<<"2"<<ID;
    }
   /* else if(wight<0&&high>=0)
    {
       r=QRectF(leftTopx-4,leftTopy-4,wight-6,high+6);
    }*/
    else if(wight<0&&high<0)
    {
        r=QRectF(leftTopx-4,leftTopy+high-4,-wight+6,-high+6);
    }
    return r;
}
void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, 0.2));
    painter->setBrush(QBrush(Qt::blue, Qt::DiagCrossPattern));
    if(Sita>=90&&Sita<270)
    {
      rotateAndPaintRect(painter,QRectF(leftTopx,leftTopy,wight,-10),Sita-180);
    }

    else
     {
        if(Sita==0)
        {
          rotateAndPaintRect(painter,QRectF(leftTopx,leftTopy,wight,10),Sita);
        }
        else
          rotateAndPaintRect(painter,QRectF(leftTopx,leftTopy,-10,high),Sita);
    }

   // painter->drawText(QRectF(leftTopx,leftTopy,wight,-10), Qt::AlignCenter,QString("%1").arg(ID));

    QSqlQuery query = getQuery();
    if(!query.exec("select * from tb_storage WHERE NodeID = ? and layer = ?"));
    query.addBindValue(ID);
    query.addBindValue(offset + 1);
    query.exec();
    while(query.next())
    {
        QSqlRecord record = query.record();
        if(record.value("storage").toInt() > 0) painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
        else painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
        if(record.value("Hidden").toBool()) painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter->drawRect(leftTopx + offset * wight / division, leftTopy, wight / division, high);
    }

    QGraphicsRectItem::paint(painter,option,widget);
}
void RectItem::rotateAndPaintRect(QPainter *painter, const QRectF &rect, int angle)
{
    QRectF rotatedRect(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());
        int cx = rect.x() + rect.width() / 2;
        int cy = rect.y() + rect.height() / 2;
        painter->save();
       // painter->drawText(rect, Qt::AlignCenter,QString("库位_%1").arg(ID));
        painter->translate(cx, cy);
        painter->rotate(angle);
        QFont font;
        //font.setPointSize(qAbs(rect.height()));
        font.setPointSize(5);

        font.setPointSize(10);
        painter->setFont(font);
        painter->drawText(rotatedRect, Qt::AlignCenter,QString("%1").arg(ID));
        painter->restore();
}

void RectItem::paint(bool empty, bool hidden)
{
    QPainter *p = new QPainter;
    if(!empty)  p->setBrush(QBrush(Qt::green, Qt::SolidPattern));
    else p->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    if(hidden) p->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    p->drawRect(leftTopx + offset * wight / division, leftTopy, wight / division, high);
}
