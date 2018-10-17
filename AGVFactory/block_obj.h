#ifndef BLOCK_OBJ_H
#define BLOCK_OBJ_H

#include <vector>//包含头文件
#include <QPointF>
using namespace std;


//块对象类，存放DXF的块对线，包括世界坐标、缩放比例、线、圆等对象
//线对象
class DXFLine
{
public:
    QPointF beginpoint;
    QPointF endpoint;
};
//圆对象
class DXFCircle
{
public:
    QPointF centerpoint;
    double radius;
};
//多线实体对象
class DXFPolyLineEntities
{
public:
    vector<QPointF> vertex;//顶点
    bool isclose;//闭合标志位
  //  int pNumber;
  //  int pM;
  //  int pN;
  //  double pEvevation;
};
//圆弧对象对象
class DXFArc
{
public:
    QPointF centerpoint;
    double radius;
    double bangle;//起点角度
    double eangle;//终点角度
};

class BlockObj
{
public:
    BlockObj()
    {
        sx = 1;
        sy = 1;
        sz = 1;
        ipx = 0;
        ipy = 0;
        ipz = 0;
        line.clear();
    }
    ~BlockObj(){}
    //块名称
    string name;
    //缩放比例
    double sx;
    double sy;
    double sz;
    //用户坐标系相在世界坐标系中的位置
    double ipx;
    double ipy;
    double ipz;
    vector<DXFLine> line;
    vector<DXFCircle> circle;
    vector<DXFPolyLineEntities> polylineentities;
    vector<DXFArc> arc;
    bool drawflag;
};
#endif // BLOCK_OBJ_H
