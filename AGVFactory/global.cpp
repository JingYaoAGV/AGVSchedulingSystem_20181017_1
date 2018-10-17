#include "global.h"
#include <QtMath>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QtMath>
#include <QTextStream>
#include <QDir>
bool __DEBUG__;
QTransform g_transform;
QTransform g_transformInverse;
QTransform g_scale;
QTransform g_scaleInverse;
QColor g_highLightColor = Qt::yellow;
QSqlDatabase db;

QSqlDatabase dbServer;
QDateTime g_runTime = QDateTime::currentDateTime();
QSettings *g_setting;

qreal m_sceneHigh;
QPointF m_sceneCoordinate;
qreal  m_sceneZoom;
void global_init(){
    g_transform.scale(1.0,1.0);
    g_transformInverse.scale(1.0,1.0);
    g_runTime = QDateTime::currentDateTime();
    qsrand(g_runTime.time().msec()+g_runTime.time().second()*1000);
    g_setting = new QSettings("AGVFactory.ini",QSettings::IniFormat);
    __DEBUG__ = g_setting->value("/debug/ON").toBool();
    g_setting->setValue("/debug/ON",__DEBUG__);
}

int sgn(qreal x)
{
    if(x<0)return -1;
    return 1;
}

void setGlobalTransform(QTransform trans){
    g_transform = trans;
    mapFromRealWorld(0);
}
void setGlobalInverseTransform(QTransform trans)
{
    g_transformInverse = trans;
}

QPointF mapToRealWorld(const QPointF point){
    //return g_transformInverse.map(point);

    qreal x=(point.x()-m_sceneCoordinate.x())/(m_sceneZoom);
    qreal y=(m_sceneHigh-point.y()-m_sceneCoordinate.y())/(m_sceneZoom);
    //qDebug()<<"x"<<m_sceneCoordinate.x()<<"ox"<<m_sceneZoom;
    return QPointF(x,y);
}
QPointF mapToRealWorld(int high,QPointF coordinatemove,qreal zoom, QPointF point)
{
   qreal x=(point.x()-coordinatemove.x())/(zoom);
   qreal y=(high-point.y()-coordinatemove.y())/(zoom);
   return QPointF(x,y);
}
QPointF mapFromRealWorld(QPointF point){
    //return g_transform.map(point);
    qreal x=point.x()*m_sceneZoom+m_sceneCoordinate.x();
    qreal y=m_sceneHigh-(point.y()*m_sceneZoom+m_sceneCoordinate.y());
    return QPointF(x,y);
}

qreal mapFromRealWorld(qreal radians)
{
    return qAtan2(qSin(radians)*g_scale.m22(),qCos(radians)*g_scale.m11());
}

qreal mapToRealWorld(qreal radians)
{
    return qAtan2(qSin(radians)/g_scale.m22(),qCos(radians)/g_scale.m11());
}

void setGlobalScale(QTransform scale)
{
    g_scale = scale;
    g_scaleInverse = g_scale.inverted();
}

QSize scaleFromRealWorld(QSize size)
{
    QPointF sizept = g_scale.map(QPointF(size.width(),size.height()));
    return QSize(sizept.x(),sizept.y());
}

QSize scaleToRealWorld(QSize size)
{
    QPointF sizept = g_scaleInverse.map(QPointF(size.width(),size.height()));
    return QSize(sizept.x(),sizept.y());
}
bool pointFuzzyCompare(QPointF p1, QPointF p2)
{
    return qAbs(p1.x()-p2.x())<1.0 && qAbs(p1.y()-p2.y())<1.0;
}

int penToInt(QPen pen)
{
    int result = 0;
    QColor clr = pen.color();
    result =(pen.width()&0x7f)<<24 | (clr.red()&0xff)<<16 | (clr.green()&0xff)<<8 | (clr.blue()&0xff);
    if(pen.isSolid())
        result |= 0x80000000;
    else
        result &= 0x7fffffff;
    return result;
}

int brushToInt(QBrush brush)
{
    int result = 0;
    QColor clr = brush.color();
    result = (clr.alpha()&0xff)<<24 | (clr.red()&0xff)<<16 | (clr.green()&0xff)<<8 | (clr.blue()&0xff);
    return result;
}

QBrush intToBrush(int value)
{
    QColor clr;
    clr.setAlpha((value>>24)&0xff);
    clr.setRed((value>>16)&0xff);
    clr.setGreen((value>>8)&0xff);
    clr.setBlue(value&0xff);
    QBrush brush = QBrush(clr);
    return brush;
}

QPen intToPen(int value)
{
    QPen pen;
    QColor clr;
    clr.setRed((value>>16)&0xff);
    clr.setGreen((value>>8)&0xff);
    clr.setBlue(value&0xff);
    pen.setColor(clr);
    pen.setWidth((value>>24)&0x7f);
    if(value&0x80000000)
        pen.setStyle(Qt::SolidLine);
    else
        pen.setStyle(Qt::DashLine);
    return pen;
}

void stringToAddr(uchar addr[],QString string)
{
    for(int i=0;i<string.length();i+=2){
        addr[i/2]=string.mid(i,2).toInt(0,16);
    }
}
QByteArray stringToAddr(QString string){
    QByteArray addr;

    for(int i=0;i<string.length();i+=2){
         bool ok;
        addr.append(string.mid(i,2).toInt(&ok,16));

    }
    return addr;
}

QString addrToString(QByteArray addr)
{
    QString string;
    for(int i=0;i<addr.length();i++){
        string+=QString("%1").arg((uchar)(addr.at(i)),2,16,QLatin1Char('0'));
    }
    return string;
}

QString addrToString(uchar addr[], int length)
{
    QString string;
    for(int i=0;i<length;i++){
        string+=QString("%1").arg(addr[i],2,16,QLatin1Char('0'));
    }
    return string;
}

char XNOR(char c1, char c2)
{
    return ((c1 & c2) | ((~c1) & (~c2)));
}

char XOR(char c1, char c2)
{
    return ((c1 & (~c2)) | ((~c1) & c2));
}

bool openDatabase(QString path)
{
    if(db.isOpen()) db.close();
    if(QSqlDatabase::contains("TopoMap")){
        db=QSqlDatabase::database("TopoMap");
    }
    else{
        db=QSqlDatabase::addDatabase("QSQLITE","TopoMap");
    }
    db.setDatabaseName(path);
    if(!db.open()){
        QMessageBox::critical(0,"Cannot open database","Unable to establish a database connection.",QMessageBox::Cancel);
        return false;
    }
    return true;
}

QSqlQuery getQuery()
{
    return QSqlQuery(db);
}

bool openSQLServer()
{
     if(dbServer.isOpen()) dbServer.close();
     if(QSqlDatabase::contains("AGV")){
         dbServer=QSqlDatabase::database("AGV");
     }
     else{
         dbServer=QSqlDatabase::addDatabase("QODBC","AGV");
     }
     QString m_strServerIP = g_setting->value("/sqlServer/Server_IP").toString();

     //dbServer=QSqlDatabase::addDatabase("QODBC");
     dbServer.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                "SERVER=%1;" //服务器名称
                                "DATABASE=%2;"//数据库名
                                "UID=%3;"           //登录名
                                "PWD=%4;"        //密码
                                ).arg(m_strServerIP)
                        .arg("AGV")
                        .arg("sa")
                        //.arg("123456")
                        .arg("d2413951Wjiang")

                        );
    if (!dbServer.open())
    {
        QMessageBox::critical(0, "Cannot open database",
                             "Unable connect to SQLServer", QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug()<<"ok";
        return  true;
    }
}
QSqlQuery getQueryServer()
{
    return QSqlQuery(dbServer);
}

int getNearstMatchedVer(TopoView *topo, QPointF position,int start)
{
    qreal min_dis = 65535;
    int id=0;
    if(getDistance(QPointF(0,0),position)/1000.0<1.50){  // 由于AGV初始化未完成时得到的坐标在0，0附近，因此不做拟合。
        return -1;
    }
    if(start>0){
        TopoVer *ver = topo->get_verByID(start);
        if(ver){
            qreal dis = getDistance(mapToRealWorld(ver->pos()),position)/1000.0;
            if(dis<=0.6) return start;
        }
    }
    foreach(TopoVer *ver,topo->m_verList.values()){
        qreal dis = getDistance(mapToRealWorld(ver->pos()),position)/1000.0;
        if(dis<min_dis){
            id = ver->get_id();
            min_dis = dis;
        }
    }
    if(min_dis>0.6)
        return -1;
    return id;
}

qreal getDistance(QPointF p1, QPointF p2)
{
    return qSqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
}


// 以下是Registry部分
int Registry::s_nTimeBase = 0;
Registry::Registry()
{
    s_nTimeBase = QDateTime::currentDateTime().secsTo(g_runTime);
}

Registry::~Registry()
{

}
/* 函数：Registry::addRegistry(QString name, int start, int duration)
 * 输入：QString name: AGV名
 *      int start: 开始时间点（相对于需要注册时的时间）
 *      int duration  : 时间片长度
 * 输出：bool ： 若注册成功，则返回true，若否，则返回false
 * 功能：向注册表中注册时间片
*/
bool Registry::addRegistry(QString name, int start, int duration)
{
    start = g_runTime.secsTo(QDateTime::currentDateTime())+start;
    int end = start+duration;
    int index = isRegistable(name,start,end);
    if(index<0) return false;
    // 如果待注册的时间片在index前或后为该AGV注册的时间片，则合并
    if(index>0 && m_twList.at(index-1).m_strName==name
            && m_twList.at(index-1).m_nTEnd>start){
        RegElement ele = m_twList.at(index-1);
        ele.m_nTStart = qMin(ele.m_nTStart,start);
        ele.m_nTEnd = qMax(ele.m_nTEnd,end);
        m_twList.replace(index-1,ele);
    }
    else if(index<m_twList.length() && m_twList.at(index).m_strName==name
            && m_twList.at(index).m_nTStart<end){
        RegElement ele = m_twList.at(index);
        ele.m_nTStart = qMin(ele.m_nTStart,start);
        ele.m_nTEnd = qMax(ele.m_nTEnd,end);
        m_twList.replace(index,ele);
    }
    else
        m_twList.insert(index,RegElement(name,start,end));
    return true;
}
/* 函数：Registry::isRegistable(QString name,int start, int end)
 * 输入：QString name: AGV名
 *      int start: 开始时间点
 *      int end  : 结束时间点
 * 输出：int ： 若可以注册，则返回注册的位置，若否，则返回-1
 * 功能：判断AGV是否可以在该时间表中注册
*/
int Registry::isRegistable(QString name,int start, int end)
{
    if(start>=end) return -1;
    if(m_twList.length()==0) return 0;  // 如果注册表为空，则可以注册
    int index = getIndex(start);
    if(index==m_twList.length()){
        if(m_twList.last().m_strName==name || m_twList.last().m_nTEnd<=start)
            return index;
        else return -1;
    }
    if(index==0){
        if(m_twList.last().m_strName==name || m_twList.first().m_nTStart>=end)
            return index;
        return -1; //否则不可注册
    }
    else{
        RegElement before = m_twList.at(index-1);
        RegElement after = m_twList.at(index);
        if(before.m_strName==name || before.m_nTEnd<start){
            if(after.m_strName==name || after.m_nTStart>end)
                return index;
        }
        return -1;
    }
}

void Registry::removeRegistry()
{

}

/* 函数：Registry::updateRegistry(int tRef)
 * 输入：参考时间点
 * 功能：清除注册表中所有结束时间在参考时间点之前的项
*/
void Registry::updateRegistry(int tRef)
{
    for(int i=0;i<m_twList.length();i++){
        if(m_twList.at(i).m_nTEnd<=tRef)
            m_twList.removeAt(i);
        else
            break;
    }
}

void Registry::showRegistry()
{
    foreach(RegElement ele,m_twList){
        qDebug()<<ele.m_strName<<ele.m_nTStart<<ele.m_nTEnd;
    }
}
/* 函数：private Registry::getIndex(int start)
 * 输入：开始时间点
 * 功能：二分法查找开始时间点对应的时间表中的位置
*/
int Registry::getIndex(int start)
{
    if(m_twList.length()==0)
        return 0;
    else if(m_twList.last().m_nTStart<start) return m_twList.length();
    int b = m_twList.length()-1;
    int a = 0;
    while(a<=b){
        int mid = (a+b)/2;
        if(start<m_twList.at(mid).m_nTStart){
            b = mid-1;
        }
        else if(start>m_twList.at(mid).m_nTStart){
            a = mid+1;
        }
        else return mid;
    }
    return a;
}

int bytesToInt32(QByteArray addr)
{
    int result = 0;
    int length = addr.length();
    if(length!=4) return 0;
    for(int i=0;i<4;i++){
        result |= (uchar)(addr.at(i))<<(3-i)*8;
    }
    return result;
}


int _8BitsSum(QByteArray buf)
{
    int result=0;
    for(int i=0;i<buf.length();i++){
        if(buf.at(i)<0){
            result += 256+(int)(buf.at(i));
        }
        else
            result += buf.at(i);
    }
    return result%256;
}









void __log__(QFile *log_file, QString text)
{
    if(__DEBUG__){
        QTextStream out(log_file);
        qDebug()<<text;
        out<<QDateTime::currentDateTime().toString("hh:mm::ss")<<" "<<text<<"\n";
    }
}
