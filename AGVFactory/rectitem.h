#ifndef RECTITEM_H
#define RECTITEM_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPen>
#include <QPainter>
#include "global.h"
class RectItem:public QObject,public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(int _Kuid READ get_id WRITE set_id)
    Q_PROPERTY(QString _name READ get_name)
    Q_PROPERTY(int _state READ get_state)
public:
    RectItem(QGraphicsScene *scene, qreal x, qreal y, qreal w, qreal h, int id, qreal sita,int div, int offset, QPen p=QPen());
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paint(bool empty, bool hidden);
    void rotateAndPaintRect(QPainter *painter, const QRectF &rect, int angle);
    int get_id(){return ID;}
    QString get_name() {return m_name;}
    void set_id(qreal id){ID=id;}
    int type() const{return RECTANGLE;}
    int get_state(){return state;}
    void set_div(int div) { division = div; }
    void set_offset(int os) { offset = os; }
private:
    int ID;
    int wight;
    int high;
    int leftTopx;
    int leftTopy;
    int division;
    int offset;
    qreal Sita;
    QString m_name;
    int state;
};

#endif // RECTITEM_H
