#ifndef GLOBAL
#define GLOBAL
#include <QTransform>
#include <QPoint>
#include <QColor>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QSettings>
#include <QFile>
#include "topoview.h"
#include  "hashset.h"
extern bool __DEBUG__;


//#define USE_PATH_TO_LIFT_FORK

#define IS_SIMULATION true
#define ADDRESS_LENGTH 8

#define BUTTON_MASK 0x99
#define BUTTON_A_MASK 0x80
#define BUTTON_B_MASK 0x10
#define BUTTON_C_MASK 0x01
#define BUTTON_D_MASK 0x08

#define BUTTON_A 0x98&BUTTON_MASK
#define BUTTON_B 0x89&BUTTON_MASK
#define BUTTON_C 0x91&BUTTON_MASK
#define BUTTON_D 0x19&BUTTON_MASK
#define BUTTON_RELEASE  BUTTON_MASK

#define BUTTON_LED_A stringToAddr("5032")
#define BUTTON_LED_B stringToAddr("4438")
#define BUTTON_LED_C stringToAddr("4431")
#define BUTTON_LED_D stringToAddr("4435")


#define BUTTON_LED_ON 0x05
#define BUTTON_LED_OFF 0x04

// AGV通信协议帧类型字节定义
#define FRAME_SET_PATH 0x10
#define FRAME_ENABLE_PATH 0x11
#define FRAME_GET_FINISH 0x12
#define FRAME_SET_FUNCTION 0x13
#define FRAME_SET_MULTIMODE_PATH 0x1e
#define FRAME_ENABLE_MULTIMODE_PATH 0x1f
#define FRAME_GET_MULTIMODE_FINISH 0x22
#define FRAME_GET_BOTHMODE_FINISH 0x29
#define FRAME_GET_FINISH_NEW 0x0f
#define FRAME_GET_POS 0x41
#define FRAME_GET_INVENTORY 0x32
#define FRAME_GET_ERROR 0x33
#define FRAME_GET_POWER 0x34

#define FRAME_LENGTH_PATH 6
#define FRAME_LENGTH_ENABLE_PATH 6
#define FRAME_LENGTH_FINISH 6
#define FRAME_LENGTH_POS 14
#define FRAME_LENGTH_INVENTORY 7

#define TIMER_PERIOD 50
enum SHAPE {NONE,AGVS,AGV_PT20,AGV_PS20,VER,EDGE,NOT_TO_USE,LINE,RECTANGLE,ARC,TEXT,ELLIPSE};  //NOT_TO_USE使用则死机，原因未知
enum APP_STATE{APP_STOP,APP_PREPARE,APP_RUN};
enum MISSIONSTAT{MWAIT,MPICK,MPUT,MFINISH};
class TopoView;
class QSettings;
extern QSettings *g_setting;
extern qreal m_sceneHigh;
extern QPointF m_sceneCoordinate;
extern qreal  m_sceneZoom;

void global_init();
void setGlobalTransform(QTransform trans);
void setGlobalInverseTransform(QTransform trans);
QPointF mapToRealWorld(const QPointF point);
qreal mapToRealWorld(qreal radians);
QPointF mapToRealWorld(int high,QPointF coordinatemove,qreal zoom, QPointF point);
QPointF mapFromRealWorld(QPointF point);
qreal mapFromRealWorld(qreal radians);
void setGlobalScale(QTransform scale);
QSize scaleFromRealWorld(QSize size);
QSize scaleToRealWorld(QSize size);
bool pointFuzzyCompare(QPointF p1,QPointF p2);
int penToInt(QPen pen);
int brushToInt(QBrush brush);
QString addrToString(uchar addr[],int length);
QString addrToString(QByteArray addr);
int bytesToInt32(QByteArray addr);
QByteArray stringToAddr(QString string);
void stringToAddr(uchar addr[], QString string);
qreal getDistance(QPointF p1,QPointF p2);
QPen intToPen(int value);
QBrush intToBrush(int value);
bool openDatabase(QString path);
bool openSQLServer();
QSqlQuery getQuery();
QSqlQuery getQueryServer();
int getNearstMatchedVer(TopoView *topo, QPointF position, int start=-1);
int sgn(qreal x);
int _8BitsSum(QByteArray buf);
char XNOR(char c1,char c2);
char XOR(char c1,char c2);
void __log__(QFile* log_file,QString text);
extern QSqlDatabase db;
extern QSqlDatabase dbServer;
extern QColor g_highLightColor;
extern QDateTime g_runTime;
template<class type>
class PriorityQueue{
public:
    struct PQElement{
        PQElement(){
            first=0;
            second=0;
        }
        PQElement(type _first,qreal _second){first = _first;second = _second;}
        type first;
        qreal second;
    } ;
    PriorityQueue(int capacity=100){
        array = new PQElement[capacity];
        //array =axx;
        maxSize = capacity;
        currentSize = 0;
    }
    PriorityQueue(const PQElement[],int size);
    ~PriorityQueue(){delete [] array;}
    bool isEmpty() const{return currentSize==0;}
    void enQueue(const PQElement &x);
    PQElement deQueue(){
        PQElement minItem;minItem = array[1];
        array[1] = array[currentSize--];
        percolateDown(1);
        return minItem;
    }
    PQElement getHead()const{return array[1];}
private:
    int currentSize;
    PQElement *array;
    int maxSize;
  //   PQElement axx[100];
    void doubleSpace();
    void percolateDown(int hole);
};

template<class type>
void PriorityQueue<type>::enQueue(const PQElement &x){
    if(currentSize==maxSize-1) doubleSpace();
    int hole = ++currentSize;
    for(;hole>1 && x.second<array[hole/2].second;hole/=2)
        array[hole] = array[hole/2];
    array[hole] = x;
}

template<class type>
void PriorityQueue<type>::percolateDown(int hole){
    int child;
    PQElement tmp = array[hole];
    for(;hole*2<=currentSize;hole = child){
        child = hole*2;
        if(child != currentSize && array[child+1].second<array[child].second)
            child++;
        if(array[child].second<tmp.second)
            array[hole] = array[child];
        else break;
    }
    array[hole] = tmp;
}

template<class type>
void PriorityQueue<type>::doubleSpace(){
    PQElement* tmp = array;
    maxSize *=2;
    array = new PQElement[maxSize];
    for(int i=0;i<=currentSize;i++) array[i] = tmp[1];
    delete []tmp;
}

// class Registry 时间预测算法的注册表
class Registry{
    struct RegElement{
        RegElement(QString name="",int t_start=0,int t_end=0){
            m_strName = name;m_nTStart = t_start; m_nTEnd = t_end;
        }
        QString m_strName;
        int m_nTStart;
        int m_nTEnd;
    };

public:
    Registry();
    ~Registry();
    bool addRegistry(QString name, int start, int duration);
    int isRegistable(QString name,int start,int end);
    void removeRegistry();
    void updateRegistry(int tRef);
    void showRegistry();
private:
    int getIndex(int start);
private:
    QList<RegElement> m_twList; // Time Window List
    static int s_nTimeBase;

};

#endif // GLOBAL

