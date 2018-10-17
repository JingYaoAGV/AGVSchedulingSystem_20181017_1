/*
 * @file test_creationclass.cpp
 */

/*****************************************************************************
**  $Id: test_creationclass.cpp 8865 2008-02-04 18:54:02Z andrew $
**
**  This is part of the dxflib library
**  Copyright (C) 2001 Andrew Mustun
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU Library General Public License as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Library General Public License for more details.
**
**  You should have received a copy of the GNU Library General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#include "test_creationclass.h"

#include <iostream>
#include <stdio.h>
#include <QDebug>

/**
 * Default constructor.
 */
Test_CreationClass::Test_CreationClass()
{
    BlockObj newblock;
    newblock.drawflag = true;
    newblock.name = "entities";
    myblock.push_back(newblock);//先插入一个空块，以供后面处理
}


/**
 * Sample implementation of the method which handles layers.
 */
void Test_CreationClass::addLayer(const DL_LayerData& data) {
   // printf("LAYER: %s flags: %d\n", data.name.c_str(), data.flags);
   // printAttributes();

}

/**
 * Sample implementation of the method which handles point entities.
 */
void Test_CreationClass::addPoint(const DL_PointData& data) {
   // printf("POINT    (%6.3f, %6.3f, %6.3f)\n",
   //        data.x, data.y, data.z);
   // printAttributes();


}

/**
 * Sample implementation of the method which handles line entities.
 */
void Test_CreationClass::addLine(const DL_LineData& data) {
    //printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)\n",
     //      data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);
   // printAttributes();
        DXFLine myline;
        myline.beginpoint = QPointF(data.x1, data.y1);
        myline.endpoint = QPointF(data.x2, data.y2);
        myblock[myblock.size() - 1].line.push_back(myline);

}

/**
 * Sample implementation of the method which handles arc entities.
 */
void Test_CreationClass::addArc(const DL_ArcData& data) {
   // printf("ARC      (%6.3f, %6.3f, %6.3f) %6.3f, %6.3f, %6.3f\n",
     //      data.cx, data.cy, data.cz,
     //      data.radius, data.angle1, data.angle2);
   // qDebug()<<"ARC"<<data.cx<<""<<data.cy<<""<<data.cx<<""<<data.radius<<""<<data.angle1<<""<<data.angle2;
  //  printAttributes();
        DXFArc myarc;
        myarc.centerpoint = QPointF(data.cx, data.cy);
        myarc.radius = data.radius;
        myarc.bangle = data.angle1;
        myarc.eangle = data.angle2;
        myblock[myblock.size() - 1].arc.push_back(myarc);

}

/**
 * Sample implementation of the method which handles circle entities.
 */
void Test_CreationClass::addCircle(const DL_CircleData& data) {
   // printf("CIRCLE   (%6.3f, %6.3f, %6.3f) %6.3f\n",
   //        data.cx, data.cy, data.cz,
    //       data.radius);
  //  printAttributes();
        DXFCircle mycircle;
        mycircle.centerpoint = QPointF(data.cx, data.cy);
        mycircle.radius = data.radius;
        myblock[myblock.size() - 1].circle.push_back(mycircle);

}


/**
 * Sample implementation of the method which handles polyline entities.
 */
void Test_CreationClass::addPolyline(const DL_PolylineData& data) {
   // printf("POLYLINE:%d \n",data.number);
   // printf("flags: %d\n", (int)data.flags);
   // printAttributes();
        DXFPolyLineEntities mypolylineentities;

        mypolylineentities.isclose = data.flags;//闭合标志位，1表示闭合，0表示非闭合
       // mypolylineentities.pNumber=data.number;
       // mypolylineentities.pM=data.m;
       // mypolylineentities.pN=data.n;
       // mypolylineentities.pEvevation=data.elevation;
        myblock[myblock.size() - 1].polylineentities.push_back(mypolylineentities);
}


/**
 * Sample implementation of the method which handles vertices.
 */
void Test_CreationClass::addVertex(const DL_VertexData& data) {
   // printf("VERTEX   (%6.3f, %6.3f, %6.3f) %6.3f\n",
   //        data.x, data.y, data.z,
    //       data.bulge);
   // printAttributes();
        QPointF myvertex=QPointF(data.x, data.y);
        myblock[myblock.size() - 1].polylineentities[myblock[myblock.size() - 1].polylineentities.size() - 1].vertex.push_back(myvertex);

}


void Test_CreationClass::add3dFace(const DL_3dFaceData& data) {
    printf("3DFACE\n");
    for (int i=0; i<4; i++) {
        printf("   corner %d: %6.3f %6.3f %6.3f\n",
            i, data.x[i], data.y[i], data.z[i]);
    }
    printAttributes();
}
void Test_CreationClass::addBlock(const DL_BlockData &data)
{
        myblock[myblock.size() - 1].name=data.name;
        myblock[myblock.size() - 1].drawflag = false;
}
void Test_CreationClass::endBlock()
{
        BlockObj newblock;
        newblock.drawflag = true;
        newblock.name = "entities";
        myblock.push_back(newblock);
}
void Test_CreationClass::addInsert(const DL_InsertData &data)
{
        vector<BlockObj> ::iterator itor;//容器迭代器
        itor = myblock.begin();
        while (itor != myblock.end())
        {
            if (!itor->name.compare(data.name))//插入块名称比较，相同则表示插入该块
            {
                itor->ipx = data.ipx;
                itor->ipy = data.ipy;
                itor->ipz = data.ipz;
                itor->sx = data.sx;
                itor->sy = data.sy;
                itor->sz = data.sz;
                itor->drawflag = true;//只有被插入的对象才进行绘制
                break;
            }
            itor++;
        }
}

void Test_CreationClass::printAttributes() {
    printf("  Attributes: Layer: %s, ", attributes.getLayer().c_str());
    printf(" Color: ");
    if (attributes.getColor()==256)	{
        printf("BYLAYER");
    } else if (attributes.getColor()==0) {
        printf("BYBLOCK");
    } else {
        printf("%d", attributes.getColor());
    }
    printf(" Width: ");
    if (attributes.getWidth()==-1) {
        printf("BYLAYER");
    } else if (attributes.getWidth()==-2) {
        printf("BYBLOCK");
    } else if (attributes.getWidth()==-3) {
        printf("DEFAULT");
    } else {
        printf("%d", attributes.getWidth());
    }
    printf(" Type: %s\n", attributes.getLinetype().c_str());
}
    
    

// EOF
