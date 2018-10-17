#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include <QDebug>
TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readData);

    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            qDebug() << "disconnect" ;
            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    qDebug() << "new connect" ;




}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::sentData(int sd, QByteArray array)
{
    if(sd==this->socketDescriptor())
    {
        write(array);
    }
}
void TcpSocket::sentData(QByteArray data)
{
     // qDebug()<<"socketdata"<<data;
        write(data);

}

void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::readData()
{
/*-----------------------------------------------
//    datas.append(this->readAll());
   // auto data  = handleData(this->readAll(),this->peerAddress().toString(),this->peerPort());
    QByteArray array=this->readAll();
    if((array.at(0)==0x40)&&(array.at(array.length()-1)==0x23))
    {
                int id=array.at(1);
                char a=array.at(2);
               // qDebug()<<"tcpagvid"<<id;
          QByteArray  buf=array.mid(3,array.length()-4);
          emit fangzhenAgvINFO(id,a,buf);
    }
    else if((array.at(0)==0x10)&&(array.at(array.length()-1)==0x24))
    {
         // agv 连接上来了
        int id=array.at(1);
        emit connectformAgv(id,this->socketDescriptor());

    }

    //qDebug() << data;
   // this->write(data);
//    if (!watcher.isRunning())//放到异步线程中处理。
//    {
//        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
//    }

--------------------------------------------------*/

    QByteArray array=this->readAll();

 qDebug()<<array.size()<<endl;

    if(((uchar)array.at(4))!=0xF1)
    {
                int id=array.at(5);//车辆id
                char a=array.at(4);//功能码
               // qDebug()<<"tcpagvid"<<id;


          emit fangzhenAgvINFO(id,a,array);
          //emit connectformAgv(id,this->socketDescriptor());
    }
    else if(((uchar)array.at(4))==0xf1)
    {



         // agv 连接上来了
        int id=array.at(5);
        char a=array.at(4);//功能码


        qDebug()<<"收到初始化请求"<<id<<endl;
        emit connectformAgv(id,this->socketDescriptor());
        emit fangzhenAgvINFO(id,a,array);
      //  emit fangzhenAgvINFO(id,a,array);//此处应该注意emit的机制。



    }

}

QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{
    QTime time;
    time.start();

    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}
    data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
    return data;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
    }
}
