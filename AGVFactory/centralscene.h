#ifndef CENTRALSCENE_H
#define CENTRALSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
class CentralScene:public QGraphicsScene
{
public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
public:
    CentralScene();
    ~CentralScene();
};

#endif // CENTRALSCENE_H
