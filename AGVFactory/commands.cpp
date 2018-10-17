#include "commands.h"

//#include "rectitem.h"
//#include "textitem.h"
//#include "ellipseitem.h"
#include "agv.h"
#include "global.h"

AddCommand::AddCommand(int addType, QPointF pos, QGraphicsScene *graphicsScene, TopoView *topoview, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = graphicsScene;
    m_topoView = topoview;
    switch(addType){
        /*case RECTANGLE:{
            item = new RectItem(graphicsScene,pos,2,2);
            break;
        }
        */
        /*case LINE:{
            QLineF linef = QLineF(pos,QPointF(pos.x()+2,pos.y()+2));
            item = new LineItem(graphicsScene,linef);
            break;
        }
        */
        /*case TEXT:{
            item = new TextItem(graphicsScene,"请输入文字内容",pos);
            break;
        }
        */
        /*case ELLIPSE:{
            item = new EllipseItem(graphicsScene,pos,2,2);
            break;
        }
        */
        case VER:{
            item = m_topoView->add_topoVer(pos,0,0);
            break;
        }
       case AGVS:{
        qDebug()<<"x";
            break;
        }
       case AGV_PS20:{
        qDebug()<<"y";
         break;
       }
        default: break;
    }
}

AddCommand::AddCommand(QGraphicsScene *graphicsScene, TopoView *topoview, TopoVer *start, TopoVer *end, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = graphicsScene;
    m_topoView = topoview;
    item = m_topoView->add_topoEdge(start,end,0);
}

AddCommand::~AddCommand()
{
    /*if(!item->scene())
        delete item;*/
}

void AddCommand::undo()
{
    if(item->type()==VER){
        m_topoView->remove_topoVer((TopoVer*)item);
    }
    else if(item->type()==EDGE){
        m_topoView->remove_topoEdge((TopoEdge*)item);
    }
    else myGraphicsScene->removeItem(item);
}

void AddCommand::redo()
{
    if(!item->scene()){
        if(item->type()==VER){
            TopoVer* ver = (TopoVer*)item;
            m_topoView->add_topoVer(ver->pos(),ver->get_tunnel(),ver->get_id(),ver->get_width());
        }
        else
            myGraphicsScene->addItem(item);
    }
}


DeleteCommand::DeleteCommand(QGraphicsScene *graphicsScene, QGraphicsItem *i,TopoView* topoview, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    myGraphicsScene = graphicsScene;
    m_topoView = topoview;
    i->setSelected(false);
    item = i;
}

void DeleteCommand::undo()
{
    if(item->type()==VER){
        TopoVer* ver = (TopoVer*)item;
        m_topoView->add_topoVer(ver->pos(),ver->get_tunnel(),ver->get_sita(),ver->get_id(),ver->get_width(),ver->get_brushColor(),ver->is_callPoint());
    }
    else myGraphicsScene->addItem(item);
}

void DeleteCommand::redo()
{
    if(item->type()==VER){
        TopoVer* ver = (TopoVer*)item;
        m_topoView->remove_topoVer(ver);
    }
    else myGraphicsScene->removeItem(item);
}


MoveCommand::MoveCommand(QGraphicsScene *graphicsScene,QGraphicsItem *i, const QPointF &oldPos, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    item = i;
    newPos = item->pos();
    myOldPos = oldPos;
    myGraphicsScene = graphicsScene;
}

void MoveCommand::undo()
{
    item->setPos(myOldPos);
    if(item->type()==VER)
        emit ((TopoVer*)item)->position_change(item->pos());
}

void MoveCommand::redo()
{
    item->setPos(newPos);
    if(item->type()==VER)
        emit ((TopoVer*)item)->position_change(item->pos());
}

bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    QGraphicsItem *i = moveCommand->item;
    if (item != i)
        return false;
    newPos = item->pos();
    return true;
}
