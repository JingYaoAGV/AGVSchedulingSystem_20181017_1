#include "tcpserver.h"
#include "threadhandle.h"
#include <QDebug>

TcpServer::TcpServer(QObject *parent,int numConnections) :
    QTcpServer(parent)
{
     tcpClient = new  QHash<int,TcpSocket *>;
     setMaxPendingConnections(numConnections);
}

TcpServer::~TcpServer()
{
    emit this->sentDisConnect(-1);
    delete tcpClient;
}

void TcpServer::setMaxPendingConnections(int numConnections)
{
    this->QTcpServer::setMaxPendingConnections(numConnections);//调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->maxConnections = numConnections;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    if (tcpClient->size() > maxPendingConnections())//继承重写此函数后，QTcpServer默认的判断最大连接数失效，自己实现
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();//发送完再断开0217
        return;
    }
    auto th = ThreadHandle::getClass().getThread();
    auto tcpTemp = new TcpSocket(socketDescriptor);
    QString ip =  tcpTemp->peerAddress().toString();
    qint16 port = tcpTemp->peerPort();
    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中

    connect(this,SIGNAL(sentData(int,QByteArray)),tcpTemp,SLOT(sentData(int,QByteArray)));
    connect(this,SIGNAL(sentData(QByteArray)),tcpTemp,SLOT(sentData(QByteArray)));
    connect(tcpTemp,SIGNAL(sockDisConnect(int,QString,quint16,QThread*)),this,SLOT(sockDisConnectSlot(int,QString,quint16,QThread*)));  //NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他

    connect(this,SIGNAL(sentDisConnect(int)),tcpTemp,SLOT(disConTcp(int))); //断开信号
    connect(tcpTemp,SIGNAL(connectformAgv(int,int)),this,SIGNAL(connectToSystem(int,int)));
    connect(tcpTemp,SIGNAL(fangzhenAgvINFO(int,char,QByteArray)),this,SIGNAL(fangzhenAGVxinxi(int,char,QByteArray)));
    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取

    emit connectClient(socketDescriptor,ip,port);
}
void TcpServer::receviedFormAGV(int id, QByteArray data)
{
   // qDebug()<<"recevied agv"<<id;
    //TcpSocket *socket= tcpClient->value(id);
   // socket->sentData(data);
    emit  sentData(id,data);

}
void TcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
    emit sockDisConnect(handle,ip,prot);
}


void TcpServer::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandle::getClass().clear();
    tcpClient->clear();
}
