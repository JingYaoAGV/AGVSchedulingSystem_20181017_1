#include "centralscene.h"
#include <QDebug>


void CentralScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}


CentralScene::CentralScene()
{

    //setBackgroundBrush(Qt::black);
}

CentralScene::~CentralScene()
{

}
