#include "sweptarea.h"
#include<QtMath>


#define pi 3.14

#define HhengXiang 1.0
#define pingfen 4  //圆弧平分
#define LineFen 10 //矩形内生成多少点
#define ps20_left_top QPoint(-420*HhengXiang,1700*HhengXiang)
#define ps20_right_top QPoint(420*HhengXiang,1700*HhengXiang)
#define ps20_left_bottom QPoint(-420*HhengXiang,-400*HhengXiang)
#define ps20_right_bottom QPoint(420*HhengXiang,-400*HhengXiang)

#define pt20_left_top QPoint(-415*HhengXiang,2000*HhengXiang)
#define pt20_right_top QPoint(415*HhengXiang,2000*HhengXiang)
#define pt20_left_bottom QPoint(-415*HhengXiang,-200*HhengXiang)
#define pt20_right_bottom QPoint(415*HhengXiang,-200*HhengXiang)


#define rukou 39

SweptArea::SweptArea()
{

}
void SweptArea::LoadTopoView(TopoView *topoview)
{

    bool multisystem=g_setting->value("/multiSystem/multi").toBool();
    if(multisystem==true)
    {   QList<int>pathA;
        QList<int>pathB;
        m_queryServer=getQueryServer();
       int CURRRENTAGVSYSTEM=g_setting->value("/multiSystem/system").toInt();
        m_queryServer.exec(QString("select * from tb_topover where system=%1").arg(CURRRENTAGVSYSTEM));
        while(m_queryServer.next()){
            QSqlRecord record = m_queryServer.record();
            int  point =record.value("NodeID").toInt();
            pathA.append(point);

        }
        m_queryServer.exec(QString("select * from tb_topover where system<>%1").arg(CURRRENTAGVSYSTEM));
        while(m_queryServer.next()){
            QSqlRecord record = m_queryServer.record();
            int  point =record.value("NodeID").toInt();
            pathB.append(point);

        }
        int na=pathA.length();
        int nb=pathB.length();
        QList<int>l;
        HashSet hashSet4(100);
        hashSet4.hashInterSection(&hashSet4,pathA,pathB,na,nb,l);

        //qDebug()<<"swepl"<<l;
        commonLine=l;
    }

    m_topoview=topoview;
    //QList<int>rukouids;
    foreach (TopoEdge *edge, m_topoview->m_edgeList.values()) {
       QList<QPointF>points=pointsInEdgeOfArea(edge);
       int id=edge->get_id();
       QList<int> edge_idd,edge_ver;
       edge_points.insert(id,points);
       edge_ver.append(edge->get_startID());
       edge_ver.append(edge->get_endID());
       if(!commonLine.isEmpty())
       {
         for (int i=0;i<commonLine.length();i++)
         {

            int  EndVer=edge->m_nEndID;

            if((EndVer==commonLine.at(i))&&(EndVer==rukou))
            {
              QList<int>edges=m_commonEndEdges.value(commonLine.at(i));
              if(!edges.contains(id))
              {
                  edges.append(id);
              }
              m_commonEndEdges[commonLine.at(i)]=edges;
            }
         }
       }
     //  int  EndVer=edge->m_nEndID;
      // int StartVer=edge->m_nStartID;
      // if(EndVer==rukou)
     //  {
     //    rukouids.append(id);
     //  }

      // TopoVer *startTopover=m_topoview->m_verList.value(QString("Ver_%1").arg(StartVer));
      // TopoVer *endTopover=m_topoview->m_verList.value(QString("Ver_%1").arg(EndVer));
      // int sita=startTopover->get_sita();
      // int endsita=endTopover->get_sita();
      // QPointF startPos=startTopover->get_center();
        /*if(sita!=endsita)
        {
            QList<int> huichedian;
            foreach (TopoEdge *edgegongxin,  m_topoview->m_edgeList.values())
            {

                int id2=edgegongxin->get_id();
                if(id==id2)
                    continue;
                else
                {
                    int EndVer2=edgegongxin->m_nEndID;
                    int StartVer2=edgegongxin->m_nStartID;
                     TopoVer *startTopover2=m_topoview->m_verList.value(QString("Ver_%1").arg(StartVer2));
                    int sita2=startTopover2->get_sita();
                    QPointF startPos2=startTopover2->get_center();
                    if ((EndVer2==EndVer)&&(sita==(360-sita2))&&((qFabs(startPos.x()-startPos2.x())<100)||((qFabs(startPos.y()-startPos2.y())<100))))
                    {
                        huichedian.append(id2);
                    }


                }
            }
            if(! huichedian.isEmpty())
            huichepoints.insert(id,huichedian);
        }
        */
       for(int i=0;i<points.length();i++)
       {

           foreach (TopoVer *verxX, m_topoview->m_verList.values()) {
              int verid=verxX->get_id();
                  if(verid!=edge->get_startID()||verid!=edge->get_endID())
                  {
                      QList<QPointF>pointAroundVer=pointsAroundVer(verxX);
                      bool flag=isVerInArea(points.at(i),pointAroundVer);
                      if(flag==true)
                      {
                          if(!edge_ver.contains(verid))
                              edge_ver.append(verid);
                      }
                  }



           }
           foreach (TopoEdge *edgexX,  m_topoview->m_edgeList.values())
           {
               int id2=edgexX->get_id();
               if(id==id2)
                   continue;
               else
               {
                 QList<QPointF>ppoints=pointsInEdgeOfArea(edgexX);
                 bool falg=isVerInArea(points.at(i),ppoints);
                 if(falg==true)
                    {
                     if(!edge_idd.contains(id2))
                         edge_idd.append(id2);
                 }

               }
           }


       }
       collsionEdge.insert(id,edge_idd);
       collsionEdgeVer.insert(id,edge_ver);
       QList<int> pp;//节点
      foreach (TopoVer *ver, m_topoview->m_verList.values()) {

               QPointF ppp=mapToRealWorld(ver->pos());
               QPointF p=QPointF(ppp.x(),ppp.y());
               bool flag=isVerInArea(p,points);
               if(flag==true)
               {
                   pp.append(ver->get_id());

               }

       }

         verInArea.insert(edge->get_id(),pp);

    }
    QMap<int,QList<int>>::const_iterator i;
    for(i=m_commonEndEdges.constBegin();i!=m_commonEndEdges.constEnd();i++)
    {
        int verid=i.key();
        QList<int>edges=i.value();
        QList<int>edgex;
        for(int j=0;j<edges.length();j++)
        {
          edgex.append(collsionEdge.value(edges.at(j)));
        }
        int ll=edgex.length();
        for(int m=0;m<ll;m++)
        {
          for(int n=m+1;n<ll;n++)
          {
              if(edgex.at(m)==edgex.at(n))
              {
                 //edgex.at(m)=-1;
                 edgex.replace(m,-1);
              }
          }
        }
        QList<int> edgexx;
       for(int m=0;m<ll;m++)
       {
        if(edgex.at(m)!=-1)
        {
           edgexx.append(edgex.at(m));
        }
       }
       m_commonEdges.insert(verid,edgexx);
    }
  /* int length= m_commonEndEdges.size();
   for(int i=0;i<length;i++)
   {
     QList<int> edges= m_commonEndEdges
   }
   */
   /* QMap<int,QList<int>>::const_iterator i;   //会车
    for(i=huichepoints.constBegin();i!=huichepoints.constEnd();i++)
    {
          int edge1=i.key();
          QList<int> edgex=i.value();
         int edge2=edgex.first();
          QList<int>edges1=collsionEdge.value(edge1);
          edges1.append(edge1);
          QList<int>vers1=collsionEdgeVer.value(edge1);
          QList<int> points=edges1;
          for(int j=0;j<edges1.length();j++)
          {
              TopoEdge *edge=m_topoview->m_edgeList.value( QString("Edge_%1").arg(edges1.at(j)));
              int start=edge->get_startID();
              int end =edge->get_endID();
              int id=edge->get_id();
              if((vers1.contains(start)&&(vers1.contains(end))))
              {
              }
              else
              {
                   points.removeOne(id);
              }

          }
          edges1=points;
          QList<int>edges2=collsionEdge.value(edge2);
          edges2.append(edge2);
          QList<int>vers2=collsionEdgeVer.value(edge2);
          QList<int> points2=edges2;
          for(int j=0;j<edges2.length();j++)
          {
              TopoEdge *edge=m_topoview->m_edgeList.value( QString("Edge_%1").arg(edges2.at(j)));
              int start=edge->get_startID();
              int end =edge->get_endID();
              int id=edge->get_id();
              if((vers2.contains(start)&&(vers2.contains(end))))
              {
              }
              else
              {
                   points2.removeOne(id);
              }

          }
           edges2=points2;
           QList<int> zong=edges1;
          for(int j=0;j<edges2.length();j++)
          {
              if(!zong.contains(edges2.at(j)))
              zong.append(edges2.at(j));
          }
          for(int j=0;j<edges1.length();j++)
          {
              int id=edges1.at(j);

            huicheEdges.insert(id, zong) ;
          }

        // collsionEdge.insert(edge1,edges1);

         // qDebug()<<i.key()<<i.value();
    }
    */
    /* QList<int>::const_iterator k;
   for(k=rukouids.begin();k!=rukouids.end();k++)
   {
       int edge1=(*k);
       QList<int>edges1=collsionEdge.value(edge1);
       edges1.append(edge1);
       QList<int>vers1=collsionEdgeVer.value(edge1);
       QList<int> points=edges1;
       for(int j=0;j<edges1.length();j++)
       {
           TopoEdge *edge=m_topoview->m_edgeList.value( QString("Edge_%1").arg(edges1.at(j)));
           int start=edge->get_startID();
           int end =edge->get_endID();
           int id=edge->get_id();
           if((vers1.contains(start)||(vers1.contains(end))))
           {
           }
           else
           {
                points.removeOne(id);
           }

       }
       edges1=points;
       for(int j=0;j<edges1.length();j++)
       {
           int id=edges1.at(j);
           if(!rukouEdges.contains(id))
            rukouEdges.append(id);
        // rukouEdges.insert(id, edges1) ;
       }
   }
   */



}
bool SweptArea::isVerInArea(QPointF p, QList<QPointF> poly)
{

      qreal px=p.x();
      qreal py=p.y();
      bool  flag=false;
      int l=poly.length();
      int j=l-1;
      for(int i=0;i<l;i++)
      {

           qreal sx=poly.at(i).x();
           qreal sy=poly.at(i).y();
           qreal tx=poly.at(j).x();
           qreal ty=poly.at(j).y();
           // 点与多边形顶点重合
           if((sx == px && sy == py) || (tx == px && ty == py))
           {
                   return true;
           }
           // 判断线段两端点是否在射线两侧
             if((sy < py && ty >= py) || (sy >= py && ty < py))
             {
               qreal x = sx + (py - sy) * (tx - sx) / (ty - sy);
               // 点在多边形的边上
                  if(x == px) {
                    return true;
                  }
                  // 射线穿过多边形的边界
                  if(x >= px) {
                    flag = !flag;
                  }
             }

           j=i;

      }
      return flag;
}

QList<QPointF> SweptArea::pointsInEdgeOfArea(TopoEdge *edge)
{

    QList<QPointF>points;
    QPointF start,end,startPos,endPos;
    //start=QPointF(edge->m_ptStartPos.x(),-edge->m_ptStartPos.y());

    //end=QPointF(edge->m_ptEndPos.x(),-edge->m_ptEndPos.y());

    start=mapToRealWorld(edge->m_ptStartPos);
    startPos=QPointF(start.x(),start.y());
    end=mapToRealWorld(edge->m_ptEndPos);
    endPos=QPointF(end.x(),end.y());

    qreal startSita,endSita;
    startSita=m_topoview->m_verList.value(QString("Ver_%1").arg(edge->m_nStartID))->get_sita();
    endSita=m_topoview->m_verList.value(QString("Ver_%1").arg(edge->m_nEndID))->get_sita();

   //int type=edge->get_type();

     SwepArc(startSita,endSita,startPos,endPos,points);



   //输出点集
   //points.append();
   return points;

}
QList<QPointF> SweptArea::pointsAroundVer(TopoVer *ver)
{
    QList<QPointF>points;
    QPointF verpos=mapToRealWorld(QPointF(ver->get_x(),ver->get_y()));
    QPointF verRealPos=QPointF(verpos.x(),verpos.y());
    int sita=ver->get_sita();
    pointsAroundVer(verRealPos,sita,points);
    return points;
}
void SweptArea::pointsAroundVer(QPointF ver,int sita, QList<QPointF> &verpoints)
{
    qreal verx=ver.x();
    qreal very=ver.y();
    qreal rightTopX=ps20_right_top.x()*cos(sita*pi/180)-ps20_right_top.y()*sin(sita*pi/180)+verx;
    qreal rightTopY=ps20_right_top.x()*sin(sita*pi/180)+ps20_right_top.y()*cos(sita*pi/180)+very;
    qreal rightbotX=ps20_right_bottom.x()*cos(sita*pi/180)-ps20_right_bottom.y()*sin(sita*pi/180)+verx;
    qreal rightbotY=ps20_right_bottom.x()*sin(sita*pi/180)+ps20_right_bottom.y()*cos(sita*pi/180)+very;
    qreal leftbotX=ps20_left_bottom.x()*cos(sita*pi/180)-ps20_left_bottom.y()*sin(sita*pi/180)+verx;
    qreal leftbotY=ps20_left_bottom.x()*sin(sita*pi/180)+ps20_left_bottom.y()*cos(sita*pi/180)+very;
    qreal leftTopX=ps20_left_top.x()*cos(sita*pi/180)-ps20_left_top.y()*sin(sita*pi/180)+verx;
    qreal leftTopY=ps20_left_top.x()*sin(sita*pi/180)+ps20_left_top.y()*cos(sita*pi/180)+very;

    verpoints.append(QPointF(rightTopX,rightTopY));
    QList<QPointF> a=pointsinLine(QPointF(rightTopX,rightTopY),QPointF(rightbotX,rightbotY));
    verpoints.append(a);
    verpoints.append(QPointF(rightbotX,rightbotY));
    QList<QPointF> b=pointsinLine(QPointF(rightbotX,rightbotY),QPointF(leftbotX,leftbotY));
    verpoints.append(b);
    verpoints.append(QPointF(leftbotX,leftbotY));
    QList<QPointF> c=pointsinLine(QPointF(leftbotX,leftbotY),QPointF(leftTopX,leftTopY));
    verpoints.append(c);
    verpoints.append(QPointF(leftTopX,leftTopY));


}
void SweptArea::SwepArc(qreal sita, qreal sita2, QPointF p1, QPointF p2, QList<QPointF> &points)
{
    qreal x=p1.x();
    qreal y=p1.y();
    qreal x2=p2.x();
    qreal y2=p2.y();
    bool flag=false ;//叉子朝向
    if(sita==sita2)
    {//只考虑了正向行驶
        qreal rightTop2X=ps20_right_top.x()*cos(sita2*pi/180)-ps20_right_top.y()*sin(sita2*pi/180)+p2.x();
        qreal rightTop2Y=ps20_right_top.x()*sin(sita2*pi/180)+ps20_right_top.y()*cos(sita2*pi/180)+p2.y();
        qreal rightbot2X=ps20_right_bottom.x()*cos(sita2*pi/180)-ps20_right_bottom.y()*sin(sita2*pi/180)+p2.x();
        qreal rightbot2Y=ps20_right_bottom.x()*sin(sita2*pi/180)+ps20_right_bottom.y()*cos(sita2*pi/180)+p2.y();
        qreal leftbot2X=ps20_left_bottom.x()*cos(sita2*pi/180)-ps20_left_bottom.y()*sin(sita2*pi/180)+p2.x();
        qreal leftbot2Y=ps20_left_bottom.x()*sin(sita2*pi/180)+ps20_left_bottom.y()*cos(sita2*pi/180)+p2.y();
        qreal leftTop2X=ps20_left_top.x()*cos(sita2*pi/180)-ps20_left_top.y()*sin(sita2*pi/180)+p2.x();
        qreal leftTop2Y=ps20_left_top.x()*sin(sita2*pi/180)+ps20_left_top.y()*cos(sita2*pi/180)+p2.y();

        qreal rightTopX=ps20_right_top.x()*cos(sita*pi/180)-ps20_right_top.y()*sin(sita*pi/180)+p1.x();
        qreal rightTopY=ps20_right_top.x()*sin(sita*pi/180)+ps20_right_top.y()*cos(sita*pi/180)+p1.y();
        qreal rightbotX=ps20_right_bottom.x()*cos(sita*pi/180)-ps20_right_bottom.y()*sin(sita*pi/180)+p1.x();
        qreal rightbotY=ps20_right_bottom.x()*sin(sita*pi/180)+ps20_right_bottom.y()*cos(sita*pi/180)+p1.y();
        qreal leftbotX=ps20_left_bottom.x()*cos(sita*pi/180)-ps20_left_bottom.y()*sin(sita*pi/180)+p1.x();
        qreal leftbotY=ps20_left_bottom.x()*sin(sita*pi/180)+ps20_left_bottom.y()*cos(sita*pi/180)+p1.y();
        qreal leftTopX=ps20_left_top.x()*cos(sita*pi/180)-ps20_left_top.y()*sin(sita*pi/180)+p1.x();
        qreal leftTopY=ps20_left_top.x()*sin(sita*pi/180)+ps20_left_top.y()*cos(sita*pi/180)+p1.y();

        QPointF rightTop2=QPointF(rightTop2X,rightTop2Y);
        QPointF rightbot2=QPointF(rightbot2X,rightbot2Y);
        QPointF leftbot2=QPointF(leftbot2X,leftbot2Y);
        QPointF leftTop2=QPointF(leftTop2X,leftTop2Y);

        QPointF rightTop=QPointF(rightTopX,rightTopY);
        QPointF rightbot=QPointF(rightbotX,rightbotY);
        QPointF leftbot=QPointF(leftbotX,leftbotY);
        QPointF leftTop=QPointF(leftTopX,leftTopY);
        QList<QPointF> ppoints;
        ppoints.append(rightTop2);
        ppoints.append(rightbot2);
        ppoints.append(leftbot2);
        ppoints.append(leftTop2);
        ppoints.append(rightTop);
        ppoints.append(rightbot);
        ppoints.append(leftbot);
        ppoints.append(leftTop);

        int l=ppoints.length();
        qreal minx=ppoints.at(1).x();
        qreal miny=ppoints.at(1).y();
        qreal maxx=ppoints.at(1).x();
        qreal maxy=ppoints.at(1).y();

        for(int i=0;i<l;i++)
        {
           QPointF p=ppoints.at(i);
          if( p.x()<minx)
          {
             minx= p.x();
          }
          if(p.y()<miny)
          {
              miny=p.y();
          }
          if(p.x()>maxx)
          {
              maxx=p.x();
          }
          if(p.y()>maxy)
          {
              maxy=p.y();
          }
        }
        QList<qreal> XInEdge,YInEdge;
        qreal deltaX=(maxx-minx)/LineFen;
        qreal deltaY=(maxy-miny)/LineFen;
        for(int i=0;i<LineFen+1;i++)
        {
          XInEdge.append(minx+i*deltaX);
          YInEdge.append(miny+i*deltaY);
        }
        if(sita==0||sita==90||sita==180||sita==270)
        {
            for(int i=LineFen;i>=0;i--)
            {
               points.append(QPointF(maxx,YInEdge.at(i)));
            }
            for(int i=LineFen;i>=0;i--)
            {
               points.append(QPointF(XInEdge.at(i),miny));
            }
            for(int i=0;i<LineFen+1;i++)
            {
               points.append(QPointF(minx,YInEdge.at(i)));
            }
            for(int i=0;i<LineFen+1;i++)
            {
               points.append(QPointF(XInEdge.at(i),maxy));
            }
        }
        else
        {
        }
    }
    else if(sita==90||sita2==90)
    {
        qreal x0,y0,r;
          if(sita==90)
          {
              x0=p1.x();
              y0=(p2.x()-p1.x())*(p2.x()-p1.x())/(2*(p2.y()-p1.y()))+(p1.y()+p2.y())/2;
              r=qAbs(p1.y()-y0);
          }
          else if(sita2==90)
          {
              x0=p2.x();
              y0=(p1.x()-p2.x())*(p1.x()-p2.x())/(2*(p1.y()-p2.y()))+(p2.y()+p1.y())/2;
              r=qAbs(p2.y()-y0);
          }
          if(sita>0&&sita<180)
          {
              if(p1.y()>y0)
              {
                 flag=true;
              }
              else
                  flag=false;
          }
          else if(sita>180&&sita<360)
          {
              if(p1.y()<y0)
                  flag=true;
              else
                  flag=false;
          }
          else if(sita==0||sita==180)
          {
              if(sita2>180&&sita2<360)
              {
                  if(p2.y()<y0)
                  {
                      flag=true;
                  }
                  else if(p2.y()>y0)
                  {
                      flag=false;
                  }

              }
              else if(sita2>0&&sita2<180)
              {
                  if(p2.y()>y0)
                  {
                      flag=true;
                  }
                  else if(p2.y()<y0)
                  {
                      flag=false;
                  }
              }
          }
           QList<qreal> sitas;
           qreal deltaSita=(sita2-sita)/pingfen;
           for(int i=0;i<pingfen+1;i++)
           {
             sitas.append(sita+i*deltaSita);
           }
           QList<qreal>::iterator i;
           //QList<QPointF> ppoints;
           for(i=sitas.begin();i!=sitas.end();i++)
           {
               qreal xarc,yarc,theatArc;
               if(flag==true)
               {
                   qreal Q=*i;
                   xarc=r*cos((Q)*pi/180)+x0;
                   yarc=r*sin((Q)*pi/180)+y0;
                   //slope=-xarc/(sqrt(r*r-(xarc-x0)*(xarc-x0)));
                   //thetaArc=qAtan(slope);
                   theatArc=((*i))*pi/180;
               }
               else
               {
                   qreal Q;
                    Q=*i+180;
                   xarc=r*cos((Q)*pi/180)+x0;
                   yarc=r*sin((Q)*pi/180)+y0;
                   //slope=-xarc/(sqrt(r*r-(xarc-x0)*(xarc-x0)));
                   //thetaArc=qAtan(slope);
                   theatArc=(*i)*pi/180;
               }
               qreal leftTopX=ps20_left_top.x()*cos(theatArc)-ps20_left_top.y()*sin(theatArc)+xarc;
               qreal leftTopY=ps20_left_top.x()*sin(theatArc)+ps20_left_top.y()*cos(theatArc)+yarc;
               qreal rightTopX=ps20_right_top.x()*cos(theatArc)-ps20_right_top.y()*sin(theatArc)+xarc;
               qreal rightTopY=ps20_right_top.x()*sin(theatArc)+ps20_right_top.y()*cos(theatArc)+yarc;
               if(flag==true)
               {
                 points.append(QPointF(rightTopX,rightTopY));
               }
               else
               {
                 points.append(QPointF(leftTopX,leftTopY));
               }

           }
           if(flag==true)
           {
               if(sita<sita2)
               {
                   qreal lefttop2X=ps20_left_top.x()*cos(sita2*pi/180)-ps20_left_top.y()*sin(sita2*pi/180)+p2.x();
                   qreal lefttop2Y=ps20_left_top.x()*sin(sita2*pi/180)+ps20_left_top.y()*cos(sita2*pi/180)+p2.y();
                   qreal leftbot2X=ps20_left_bottom.x()*cos(sita2*pi/180)-ps20_left_bottom.y()*sin(sita2*pi/180)+p2.x();
                   qreal leftbot2Y=ps20_left_bottom.x()*sin(sita2*pi/180)+ps20_left_bottom.y()*cos(sita2*pi/180)+p2.y();
                   qreal leftbotX=ps20_left_bottom.x()*cos(sita*pi/180)-ps20_left_bottom.y()*sin(sita*pi/180)+p1.x();
                   qreal leftbotY=ps20_left_bottom.x()*sin(sita*pi/180)+ps20_left_bottom.y()*cos(sita*pi/180)+p1.y();
                   qreal rightbotX=ps20_right_bottom.x()*cos(sita*pi/180)-ps20_right_bottom.y()*sin(sita*pi/180)+p1.x();
                   qreal rightbotY=ps20_right_bottom.x()*sin(sita*pi/180)+ps20_right_bottom.y()*cos(sita*pi/180)+p1.y();
                   QPointF last=points.last();
                   QList<QPointF> l=pointsinLine(last,QPointF(lefttop2X,lefttop2Y));
                   points.append(l);
                   points.append(QPointF(lefttop2X,lefttop2Y));
                   QList<QPointF> a=pointsinLine(QPointF(lefttop2X,lefttop2Y),QPointF(leftbot2X,leftbot2Y));
                   points.append(a);
                   points.append(QPointF(leftbot2X,leftbot2Y));
                   QList<QPointF> b=pointsinLine(QPointF(leftbot2X,leftbot2Y),QPointF(leftbotX,leftbotY));
                   points.append(b);
                   points.append(QPointF(leftbotX,leftbotY));
                   QList<QPointF> c=pointsinLine(QPointF(leftbotX,leftbotY),QPointF(rightbotX,rightbotY));
                   points.append(c);
                   points.append(QPointF(rightbotX,rightbotY));
                   QPointF first=points.last();
                   QList<QPointF> f=pointsinLine(first,QPointF(rightbotX,rightbotY));
                   points.append(f);
               }
               else
               {

                   qreal rightbot2X=ps20_right_bottom.x()*cos(sita2*pi/180)-ps20_right_bottom.y()*sin(sita2*pi/180)+p2.x();
                   qreal rightbot2Y=ps20_right_bottom.x()*sin(sita2*pi/180)+ps20_right_bottom.y()*cos(sita2*pi/180)+p2.y();

                   qreal leftbot2X=ps20_left_bottom.x()*cos(sita2*pi/180)-ps20_left_bottom.y()*sin(sita2*pi/180)+p2.x();
                   qreal leftbot2Y=ps20_left_bottom.x()*sin(sita2*pi/180)+ps20_left_bottom.y()*cos(sita2*pi/180)+p2.y();


                   qreal leftbotX=ps20_left_bottom.x()*cos(sita*pi/180)-ps20_left_bottom.y()*sin(sita*pi/180)+p1.x();
                   qreal leftbotY=ps20_left_bottom.x()*sin(sita*pi/180)+ps20_left_bottom.y()*cos(sita*pi/180)+p1.y();

                   qreal lefttopX=ps20_left_top.x()*cos(sita*pi/180)-ps20_left_top.y()*sin(sita*pi/180)+p1.x();
                   qreal lefttopY=ps20_left_top.x()*sin(sita*pi/180)+ps20_left_top.y()*cos(sita*pi/180)+p1.y();

                   QPointF last=points.last();
                   QList<QPointF> l=pointsinLine(last,QPointF(rightbot2X,rightbot2Y));

                   points.append(l);
                   points.append(QPointF(rightbot2X,rightbot2Y));
                   QList<QPointF> a=pointsinLine(QPointF(rightbot2X,rightbot2Y),QPointF(leftbot2X,leftbot2Y));
                   points.append(a);
                   points.append(QPointF(leftbot2X,leftbot2Y));
                   QList<QPointF> b=pointsinLine(QPointF(leftbot2X,leftbot2Y),QPointF(leftbotX,leftbotY));
                   points.append(b);
                   points.append(QPointF(leftbotX,leftbotY));
                   QList<QPointF> c=pointsinLine(QPointF(leftbotX,leftbotY),QPointF(lefttopX,lefttopY));
                   points.append(c);
                   points.append(QPointF(lefttopX,lefttopY));
                   QPointF first=points.last();
                   QList<QPointF> f=pointsinLine(first,QPointF(lefttopX,lefttopY));
                   points.append(f);


               }

           }
           else
           {

               if(sita>sita2)
               {

                   qreal rightTop2X=ps20_right_top.x()*cos(sita2*pi/180)-ps20_right_top.y()*sin(sita2*pi/180)+p2.x();
                  qreal rightTop2Y=ps20_right_top.x()*sin(sita2*pi/180)+ps20_right_top.y()*cos(sita2*pi/180)+p2.y();

                  qreal rightbot2X=ps20_right_bottom.x()*cos(sita2*pi/180)-ps20_right_bottom.y()*sin(sita2*pi/180)+p2.x();
                  qreal rightbot2Y=ps20_right_bottom.x()*sin(sita2*pi/180)+ps20_right_bottom.y()*cos(sita2*pi/180)+p2.y();

                  qreal rightbotX=ps20_right_bottom.x()*cos(sita*pi/180)-ps20_right_bottom.y()*sin(sita*pi/180)+p1.x();
                  qreal rightbotY=ps20_right_bottom.x()*sin(sita*pi/180)+ps20_right_bottom.y()*cos(sita*pi/180)+p1.y();
                  qreal leftbotX=ps20_left_bottom.x()*cos(sita*pi/180)-ps20_left_bottom.y()*sin(sita*pi/180)+p1.x();
                  qreal leftbotY=ps20_left_bottom.x()*sin(sita*pi/180)+ps20_left_bottom.y()*cos(sita*pi/180)+p1.y();


                  QPointF last=points.last();
                  QList<QPointF> l=pointsinLine(last,QPointF(rightTop2X,rightTop2Y));
                  points.append(l);
                  points.append(QPointF(rightTop2X,rightTop2Y));
                  QList<QPointF> a=pointsinLine(QPointF(rightTop2X,rightTop2Y),QPointF(rightbot2X,rightbot2Y));
                  points.append(a);
                  points.append(QPointF(rightbot2X,rightbot2Y));
                  QList<QPointF> b=pointsinLine(QPointF(rightbot2X,rightbot2Y),QPointF(rightbotX,rightbotY));
                  points.append(b);
                  points.append(QPointF(rightbotX,rightbotY));
                  QList<QPointF> c=pointsinLine(QPointF(rightbotX,rightbotY),QPointF(leftbotX,leftbotY));
                  points.append(c);
                  points.append(QPointF(leftbotX,leftbotY));

                  QPointF first=points.last();
                  QList<QPointF> f=pointsinLine(first,QPointF(leftbotX,leftbotY));
                  points.append(f);
               }
               else
               {
                   qreal leftbot2X=ps20_left_bottom.x()*cos(sita2*pi/180)-ps20_left_bottom.y()*sin(sita2*pi/180)+p2.x();
                   qreal leftbot2Y=ps20_left_bottom.x()*sin(sita2*pi/180)+ps20_left_bottom.y()*cos(sita2*pi/180)+p2.y();

                   qreal rightbot2X=ps20_right_bottom.x()*cos(sita2*pi/180)-ps20_right_bottom.y()*sin(sita2*pi/180)+p2.x();
                   qreal rightbot2Y=ps20_right_bottom.x()*sin(sita2*pi/180)+ps20_right_bottom.y()*cos(sita2*pi/180)+p2.y();

                   qreal rightbotX=ps20_right_bottom.x()*cos(sita*pi/180)-ps20_right_bottom.y()*sin(sita*pi/180)+p1.x();
                   qreal rightbotY=ps20_right_bottom.x()*sin(sita*pi/180)+ps20_right_bottom.y()*cos(sita*pi/180)+p1.y();

                   qreal rightTopX=ps20_right_top.x()*cos(sita*pi/180)-ps20_right_top.y()*sin(sita*pi/180)+p1.x();
                   qreal rightTopY=ps20_right_top.x()*sin(sita*pi/180)+ps20_right_top.y()*cos(sita*pi/180)+p1.y();

                   QPointF last=points.last();
                   QList<QPointF> l=pointsinLine(last,QPointF(leftbot2X,leftbot2Y));
                   points.append(l);
                   points.append(QPointF(leftbot2X,leftbot2Y));
                   QList<QPointF> a=pointsinLine(QPointF(leftbot2X,leftbot2Y),QPointF(rightbot2X,rightbot2Y));
                   points.append(a);
                   points.append(QPointF(rightbot2X,rightbot2Y));
                   QList<QPointF> b=pointsinLine(QPointF(rightbot2X,rightbot2Y),QPointF(rightbotX,rightbotY));
                   points.append(b);
                   points.append(QPointF(rightbotX,rightbotY));
                   QList<QPointF> c=pointsinLine(QPointF(rightbotX,rightbotY),QPointF(rightTopX,rightTopY));
                   points.append(c);
                   points.append(QPointF(rightTopX,rightTopY));
                   QPointF first=points.last();
                   QList<QPointF> f=pointsinLine(first,QPointF(rightTopX,rightTopY));
                   points.append(f);

               }
           }


    }

    else
    {

       qreal arc =pi*sita/180;
       qreal arc2=pi*sita2/180;
       qreal k=qTan(arc);

       qreal k2=qTan(arc2);

       qreal b=y-k*x;
       qreal b2=y2-k2*x2;

       qreal x0=(b2-b)/(k-k2);
       qreal y0=k*x0+b;
       qreal r=getDistance(QPointF(x0,y0),QPointF(x,y));


       if(sita>0&&sita<180)
       {
           if(p1.y()>y0)
           {
              flag=true;
           }
           else
               flag=false;
       }
       else if(sita>180&&sita<360)
       {
           if(p1.y()<y0)
               flag=true;
           else
               flag=false;
       }
       else if(sita==0)
       {
           if(p1.x()>x0)
               flag=true;
           else
               flag=false;
       }
       else if(sita==180)
       {
           if(p1.x()<x0)
               flag=true;
           else
               flag=false;
       }

       QList<qreal> sitas;
       qreal deltaSita=(sita2-sita)/pingfen;
       for(int i=0;i<pingfen+1;i++)
       {
         sitas.append(sita+i*deltaSita);
       }
       QList<qreal>::iterator i;

       for(i=sitas.begin();i!=sitas.end();i++)
       {
           qreal xarc,yarc,theatArc;
           if(flag==true)
           {
               qreal Q=*i;
               xarc=r*cos((Q)*pi/180)+x0;
               yarc=r*sin((Q)*pi/180)+y0;
               //slope=-xarc/(sqrt(r*r-(xarc-x0)*(xarc-x0)));
               //thetaArc=qAtan(slope);
               theatArc=((*i))*pi/180;
           }
           else
           {
               qreal Q;
                Q=*i+180;
               xarc=r*cos((Q)*pi/180)+x0;
               yarc=r*sin((Q)*pi/180)+y0;
               //slope=-xarc/(sqrt(r*r-(xarc-x0)*(xarc-x0)));
               //thetaArc=qAtan(slope);
               theatArc=(*i)*pi/180;
           }
           qreal leftTopX=ps20_left_top.x()*cos(theatArc)-ps20_left_top.y()*sin(theatArc)+xarc;
           qreal leftTopY=ps20_left_top.x()*sin(theatArc)+ps20_left_top.y()*cos(theatArc)+yarc;
           qreal rightTopX=ps20_right_top.x()*cos(theatArc)-ps20_right_top.y()*sin(theatArc)+xarc;
           qreal rightTopY=ps20_right_top.x()*sin(theatArc)+ps20_right_top.y()*cos(theatArc)+yarc;
           if(flag==true)
           {
             points.append(QPointF(rightTopX,rightTopY));
           }
           else
           {
             points.append(QPointF(leftTopX,leftTopY));
           }

       }

       if(flag==true)
       {
           if(sita<sita2)
           {
               qreal lefttop2X=ps20_left_top.x()*cos(sita2*pi/180)-ps20_left_top.y()*sin(sita2*pi/180)+p2.x();
               qreal lefttop2Y=ps20_left_top.x()*sin(sita2*pi/180)+ps20_left_top.y()*cos(sita2*pi/180)+p2.y();
               qreal leftbot2X=ps20_left_bottom.x()*cos(sita2*pi/180)-ps20_left_bottom.y()*sin(sita2*pi/180)+p2.x();
               qreal leftbot2Y=ps20_left_bottom.x()*sin(sita2*pi/180)+ps20_left_bottom.y()*cos(sita2*pi/180)+p2.y();
               qreal leftbotX=ps20_left_bottom.x()*cos(sita*pi/180)-ps20_left_bottom.y()*sin(sita*pi/180)+p1.x();
               qreal leftbotY=ps20_left_bottom.x()*sin(sita*pi/180)+ps20_left_bottom.y()*cos(sita*pi/180)+p1.y();
               qreal rightbotX=ps20_right_bottom.x()*cos(sita*pi/180)-ps20_right_bottom.y()*sin(sita*pi/180)+p1.x();
               qreal rightbotY=ps20_right_bottom.x()*sin(sita*pi/180)+ps20_right_bottom.y()*cos(sita*pi/180)+p1.y();
               QPointF last=points.last();
               QList<QPointF> l=pointsinLine(last,QPointF(lefttop2X,lefttop2Y));
               points.append(l);
               points.append(QPointF(lefttop2X,lefttop2Y));
               QList<QPointF> a=pointsinLine(QPointF(lefttop2X,lefttop2Y),QPointF(leftbot2X,leftbot2Y));
               points.append(a);
               points.append(QPointF(leftbot2X,leftbot2Y));
               QList<QPointF> b=pointsinLine(QPointF(leftbot2X,leftbot2Y),QPointF(leftbotX,leftbotY));
               points.append(b);
               points.append(QPointF(leftbotX,leftbotY));
               QList<QPointF> c=pointsinLine(QPointF(leftbotX,leftbotY),QPointF(rightbotX,rightbotY));
               points.append(c);
               points.append(QPointF(rightbotX,rightbotY));
               QPointF first=points.last();
               QList<QPointF> f=pointsinLine(first,QPointF(rightbotX,rightbotY));
               points.append(f);
           }
           else
           {

               qreal rightbot2X=ps20_right_bottom.x()*cos(sita2*pi/180)-ps20_right_bottom.y()*sin(sita2*pi/180)+p2.x();
               qreal rightbot2Y=ps20_right_bottom.x()*sin(sita2*pi/180)+ps20_right_bottom.y()*cos(sita2*pi/180)+p2.y();

               qreal leftbot2X=ps20_left_bottom.x()*cos(sita2*pi/180)-ps20_left_bottom.y()*sin(sita2*pi/180)+p2.x();
               qreal leftbot2Y=ps20_left_bottom.x()*sin(sita2*pi/180)+ps20_left_bottom.y()*cos(sita2*pi/180)+p2.y();


               qreal leftbotX=ps20_left_bottom.x()*cos(sita*pi/180)-ps20_left_bottom.y()*sin(sita*pi/180)+p1.x();
               qreal leftbotY=ps20_left_bottom.x()*sin(sita*pi/180)+ps20_left_bottom.y()*cos(sita*pi/180)+p1.y();

               qreal lefttopX=ps20_left_top.x()*cos(sita*pi/180)-ps20_left_top.y()*sin(sita*pi/180)+p1.x();
               qreal lefttopY=ps20_left_top.x()*sin(sita*pi/180)+ps20_left_top.y()*cos(sita*pi/180)+p1.y();

               QPointF last=points.last();
               QList<QPointF> l=pointsinLine(last,QPointF(rightbot2X,rightbot2Y));

               points.append(l);
               points.append(QPointF(rightbot2X,rightbot2Y));
               QList<QPointF> a=pointsinLine(QPointF(rightbot2X,rightbot2Y),QPointF(leftbot2X,leftbot2Y));
               points.append(a);
               points.append(QPointF(leftbot2X,leftbot2Y));
               QList<QPointF> b=pointsinLine(QPointF(leftbot2X,leftbot2Y),QPointF(leftbotX,leftbotY));
               points.append(b);
               points.append(QPointF(leftbotX,leftbotY));
               QList<QPointF> c=pointsinLine(QPointF(leftbotX,leftbotY),QPointF(lefttopX,lefttopY));
               points.append(c);
               points.append(QPointF(lefttopX,lefttopY));
               QPointF first=points.last();
               QList<QPointF> f=pointsinLine(first,QPointF(lefttopX,lefttopY));
               points.append(f);


           }

       }
       else
       {

           if(sita>sita2)
           {

               qreal rightTop2X=ps20_right_top.x()*cos(sita2*pi/180)-ps20_right_top.y()*sin(sita2*pi/180)+p2.x();
              qreal rightTop2Y=ps20_right_top.x()*sin(sita2*pi/180)+ps20_right_top.y()*cos(sita2*pi/180)+p2.y();

              qreal rightbot2X=ps20_right_bottom.x()*cos(sita2*pi/180)-ps20_right_bottom.y()*sin(sita2*pi/180)+p2.x();
              qreal rightbot2Y=ps20_right_bottom.x()*sin(sita2*pi/180)+ps20_right_bottom.y()*cos(sita2*pi/180)+p2.y();

              qreal rightbotX=ps20_right_bottom.x()*cos(sita*pi/180)-ps20_right_bottom.y()*sin(sita*pi/180)+p1.x();
              qreal rightbotY=ps20_right_bottom.x()*sin(sita*pi/180)+ps20_right_bottom.y()*cos(sita*pi/180)+p1.y();
              qreal leftbotX=ps20_left_bottom.x()*cos(sita*pi/180)-ps20_left_bottom.y()*sin(sita*pi/180)+p1.x();
              qreal leftbotY=ps20_left_bottom.x()*sin(sita*pi/180)+ps20_left_bottom.y()*cos(sita*pi/180)+p1.y();


              QPointF last=points.last();
              QList<QPointF> l=pointsinLine(last,QPointF(rightTop2X,rightTop2Y));
              points.append(l);
              points.append(QPointF(rightTop2X,rightTop2Y));
              QList<QPointF> a=pointsinLine(QPointF(rightTop2X,rightTop2Y),QPointF(rightbot2X,rightbot2Y));
              points.append(a);
              points.append(QPointF(rightbot2X,rightbot2Y));
              QList<QPointF> b=pointsinLine(QPointF(rightbot2X,rightbot2Y),QPointF(rightbotX,rightbotY));
              points.append(b);
              points.append(QPointF(rightbotX,rightbotY));
              QList<QPointF> c=pointsinLine(QPointF(rightbotX,rightbotY),QPointF(leftbotX,leftbotY));
              points.append(c);
              points.append(QPointF(leftbotX,leftbotY));

              QPointF first=points.last();
              QList<QPointF> f=pointsinLine(first,QPointF(leftbotX,leftbotY));
              points.append(f);
           }
           else
           {
               qreal leftbot2X=ps20_left_bottom.x()*cos(sita2*pi/180)-ps20_left_bottom.y()*sin(sita2*pi/180)+p2.x();
               qreal leftbot2Y=ps20_left_bottom.x()*sin(sita2*pi/180)+ps20_left_bottom.y()*cos(sita2*pi/180)+p2.y();

               qreal rightbot2X=ps20_right_bottom.x()*cos(sita2*pi/180)-ps20_right_bottom.y()*sin(sita2*pi/180)+p2.x();
               qreal rightbot2Y=ps20_right_bottom.x()*sin(sita2*pi/180)+ps20_right_bottom.y()*cos(sita2*pi/180)+p2.y();

               qreal rightbotX=ps20_right_bottom.x()*cos(sita*pi/180)-ps20_right_bottom.y()*sin(sita*pi/180)+p1.x();
               qreal rightbotY=ps20_right_bottom.x()*sin(sita*pi/180)+ps20_right_bottom.y()*cos(sita*pi/180)+p1.y();

               qreal rightTopX=ps20_right_top.x()*cos(sita*pi/180)-ps20_right_top.y()*sin(sita*pi/180)+p1.x();
               qreal rightTopY=ps20_right_top.x()*sin(sita*pi/180)+ps20_right_top.y()*cos(sita*pi/180)+p1.y();

               QPointF last=points.last();
               QList<QPointF> l=pointsinLine(last,QPointF(leftbot2X,leftbot2Y));
               points.append(l);
               points.append(QPointF(leftbot2X,leftbot2Y));
               QList<QPointF> a=pointsinLine(QPointF(leftbot2X,leftbot2Y),QPointF(rightbot2X,rightbot2Y));
               points.append(a);
               points.append(QPointF(rightbot2X,rightbot2Y));
               QList<QPointF> b=pointsinLine(QPointF(rightbot2X,rightbot2Y),QPointF(rightbotX,rightbotY));
               points.append(b);
               points.append(QPointF(rightbotX,rightbotY));
               QList<QPointF> c=pointsinLine(QPointF(rightbotX,rightbotY),QPointF(rightTopX,rightTopY));
               points.append(c);
               points.append(QPointF(rightTopX,rightTopY));
               QPointF first=points.last();
               QList<QPointF> f=pointsinLine(first,QPointF(rightTopX,rightTopY));
               points.append(f);

           }
       }

    }
}
QList<QPointF> SweptArea::pointsinLine(QPointF a, QPointF b)
{
    QList<QPointF> pp;
    qreal x1=a.x();
    qreal x2=b.x();
    qreal y1=a.y();
    qreal y2=b.y();
    qreal deltaX=(x2-x1)/LineFen;
    qreal deltaY=(y2-y1)/LineFen;
    QList<qreal> x,y;
    for(int i=0;i<LineFen+1;i++)
    {
      x.append(x1+i*deltaX);
      y.append(y1+i*deltaY);
    }
    for(int i=1;i<LineFen;i++)
    {
       pp.append(QPointF(x.at(i),y.at(i)));
    }
    return pp;
}
