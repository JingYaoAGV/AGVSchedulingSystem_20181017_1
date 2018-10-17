#include "wmsinterface.h"
#include "global.h"
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSettings>
#include <QDateTime>
#define WMS_FRAME_STORE 0x10
#define WMS_FRAME_FETCH 0x11
#define WMS_FRAME_TRANSFER_IN_DEPORT  0x12
#define WMS_FRAME_TRANSFER_IN_STATION 0x13
#define WMS_FRAME_INQUERY_STATE 0x14
#define WMS_FRAME_DELETE_MISSION 0x15
#define WMS_FRAME_REPORT_ERROR 0x20
#define WMS_FRAME_REPORT_STATE 0x21

#define WMS_STORE_START_NODE 110
#define WMS_FETCH_END_NODE 120

char _null_ = 0x00;
WMSInterface::WMSInterface(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    m_strServerIP = g_setting->value("/wms/Server_IP").toString();
    if(m_strServerIP.isEmpty()){
        m_strServerIP = "127.0.0.1";
        g_setting->setValue("/wms/Server_IP",m_strServerIP);
    }
    m_nServerPort = g_setting->value("/wms/Server_Port").toInt();
    if(m_nServerPort==0){
        m_nServerPort = 6666;
        g_setting->setValue("/wms/Server_Port",m_nServerPort);
    }
    m_nSendPeriod = g_setting->value("/wms/Comm_Period_ms").toInt();
    if(m_nSendPeriod==0){
        m_nSendPeriod = 50;
        g_setting->setValue("/wms/Comm_Period_ms",m_nSendPeriod);
    }
    m_nCommOverTime = g_setting->value("/wms/Comm_Overtime_s").toInt();
    if(m_nCommOverTime==0){
        m_nCommOverTime = 300;
        g_setting->setValue("/wms/Comm_Overtime_s",m_nCommOverTime);
    }
    m_bAutoConnect = g_setting->value("/wms/Comm_Auto_Connect").toBool();
    g_setting->setValue("/wms/Comm_Auto_Connect",m_bAutoConnect);

    m_bWMSOn = g_setting->value("/wms/ON").toBool();
    g_setting->setValue("/wms/ON",m_bWMSOn);
    if(__DEBUG__){
        m_file.setFileName(QString("log_wms_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
        if(!m_file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::warning(0,"错误","打开日志文件错误");
        }
    }
    if(m_bWMSOn){
        m_nHeartBeatCount = 65526;
        connect(m_socket,SIGNAL(readyRead()),this,SLOT(readData()));
        connect(m_socket,SIGNAL(connected()),this,SLOT(connected()));
        loadDatabase();
        m_nTimerID = startTimer(m_nSendPeriod); //启动一个计时器,并返回一个定时器标识符,或者返回0,如果它不能启动一个计时器。
    }
}

WMSInterface::~WMSInterface()
{
}

void WMSInterface::response_new_mission_received(quint64 id,char type,bool executable)
{
    if(m_socket==NULL || !m_bConnected) return;
    QByteArray buf;
    buf.append(0x50);
    buf.append(type);
    buf.append((id&0xff00000000000000)>>56);
    buf.append((id&0xff000000000000)>>48);
    buf.append((id&0xff0000000000)>>40);
    buf.append((id&0xff00000000)>>32);
    buf.append((id&0xff000000)>>24);
    buf.append((id&0xff0000)>>16);
    buf.append((id&0xff00)>>8);
    buf.append((id&0xff));
    buf.append(executable?0:1);
    buf.append(_8BitsSum(buf.mid(1)));
    buf.append(0x23);
    __log__(&m_file,QString("[response] new mission: id=%1 success=%2").arg(id).arg(executable));
    m_socket->write(buf);
}

void WMSInterface::response_delete(quint64 id,bool can_be_delete)
{
    if(m_socket==NULL || !m_bConnected) return;
    QByteArray buf;
    buf.append(0x50);
    buf.append(WMS_FRAME_DELETE_MISSION);
    buf.append((id&0xff00000000000000)>>56);
    buf.append((id&0xff000000000000)>>48);
    buf.append((id&0xff0000000000)>>40);
    buf.append((id&0xff00000000)>>32);
    buf.append((id&0xff000000)>>24);
    buf.append((id&0xff0000)>>16);
    buf.append((id&0xff00)>>8);
    buf.append((id&0xff));
    buf.append(can_be_delete?1:0);
    buf.append(_8BitsSum(buf.mid(1)));
    buf.append(0x23);
    __log__(&m_file,QString("[response] delete mission: id=%1 success=%2").arg(id).arg(can_be_delete));
    m_socket->write(buf);
}

void WMSInterface::response_state(quint64 id, int state)
{
    if(m_socket==NULL || !m_bConnected) return;
    QByteArray buf;
    buf.append(0x50);
    buf.append(WMS_FRAME_INQUERY_STATE);
    buf.append((id&0xff00000000000000)>>56);
    buf.append((id&0xff000000000000)>>48);
    buf.append((id&0xff0000000000)>>40);
    buf.append((id&0xff00000000)>>32);
    buf.append((id&0xff000000)>>24);
    buf.append((id&0xff0000)>>16);
    buf.append((id&0xff00)>>8);
    buf.append((id&0xff));
    buf.append(state&0xff);
    buf.append(_8BitsSum(buf.mid(1)));
    buf.append(0x23);
    __log__(&m_file,QString("[response] mission state: id=%1 state=%2").arg(id).arg(state));
    m_socket->write(buf);
}

void WMSInterface::report_state(quint64 id, int state)
{
    if(!m_bWMSOn) return;
    WMS_Message message;
    message.id = id;
    message.state = state;
    message.type=WMS_FRAME_REPORT_STATE;
    m_messageList.append(message);
}

void WMSInterface::report_error(quint64 id, int error_code)
{
    if(!m_bWMSOn) return;
    WMS_Message message;
    message.id = id;
    message.state = error_code;
    message.type=WMS_FRAME_REPORT_ERROR;
    m_messageList.append(message);
}

void WMSInterface::send_mission_state(quint64 id,int state)
{
    if(m_socket==NULL || !m_bConnected) return;
    QByteArray buf;
    buf.append(0x50);
    buf.append(WMS_FRAME_REPORT_STATE);
    buf.append((id&0xff00000000000000)>>56);
    buf.append((id&0xff000000000000)>>48);
    buf.append((id&0xff0000000000)>>40);
    buf.append((id&0xff00000000)>>32);
    buf.append((id&0xff000000)>>24);
    buf.append((id&0xff0000)>>16);
    buf.append((id&0xff00)>>8);
    buf.append((id&0xff));
    buf.append(state&0xff);
    buf.append(_8BitsSum(buf.mid(1)));
    buf.append(0x23);
    __log__(&m_file,QString("[send] mission state: id=%1 state=%2").arg(id).arg(state));
    m_socket->write(buf);

}

void WMSInterface::send_agv_error(quint64 id, int error_code)
{
    if(m_socket==NULL || !m_bConnected) return;
    QByteArray buf;
    buf.append(0x50);
    buf.append(WMS_FRAME_REPORT_ERROR);
    buf.append((id&0xff00000000000000)>>56);
    buf.append((id&0xff000000000000)>>48);
    buf.append((id&0xff0000000000)>>40);
    buf.append((id&0xff00000000)>>32);
    buf.append((id&0xff000000)>>24);
    buf.append((id&0xff0000)>>16);
    buf.append((id&0xff00)>>8);
    buf.append((id&0xff));
    buf.append(error_code&0xff);
    buf.append(_8BitsSum(buf.mid(1)));
    buf.append(0x23);
    __log__(&m_file,QString("[send] agv error: id=%1").arg(id));
    m_socket->write(buf);
}


void WMSInterface::loadDatabase()
{
    QSqlQuery query = getQuery();
    query.exec("create table if not exists tb_wmsdeport ( row int, col int, layer int, node int )");
    query.exec("select * from tb_wmsdeport");
    m_mapDeport.clear();
    while(query.next()){
        QSqlRecord record = query.record();
        m_mapDeport[record.value("col").toInt()*10000+record.value("row").toInt()*100+
                record.value("layer").toInt()] = record.value("node").toInt();
    }
    query.exec("create table if not exists tb_wmsstation ( station int, node int )");
    query.exec("select * from tb_wmsstation");
    m_mapStation.clear();
    while(query.next()){
        QSqlRecord record =query.record();
        m_mapStation[record.value("station").toInt()] = record.value("node").toInt();
    }
}

void WMSInterface::connectToHost()
{
    m_bConnected = false;
    m_nHeartBeatCount = 0;
    m_socket->abort(); //中止
    m_socket->bind(6336);  // 客户端固定端口号
    m_socket->connectToHost(m_strServerIP,m_nServerPort);
    m_socket->waitForConnected(30000);
    if(!m_bConnected){
        __log__(&m_file,QString("[proc] WMS Unconnected"));
        emit is_connected(false);
    }
}

void WMSInterface::timerEvent(QTimerEvent *)
{
    if(m_bConnected && !m_messageList.isEmpty()){
        WMS_Message message = m_messageList.first();
        if(message.type==WMS_FRAME_REPORT_STATE)
            send_mission_state(message.id,message.state);
        else if(message.type==WMS_FRAME_REPORT_ERROR)
            send_agv_error(message.id,message.state);
    }
    m_nHeartBeatCount++;
    if(m_bAutoConnect&&m_nHeartBeatCount*m_nSendPeriod/1000>m_nCommOverTime){
        __log__(&m_file,QString("[proc] WMS disconnected, trying to reconnect..."));
        connectToHost();//链接到主机
    }
}

void WMSInterface::readData()
{
    static int s_length = -1;
    static char s_type;
    if(s_length==-1){
        char startup;
        m_socket->read(&startup,1);
        if(startup==0x50)
            s_length=0;
        else
            s_length=-1;
    }
    if(s_length==0){
        if(m_socket->bytesAvailable()>0){
            m_socket->read(&s_type,1);
            if(s_type==WMS_FRAME_STORE) s_length = 19;
            else if(s_type==WMS_FRAME_FETCH) s_length = 19;
            else if(s_type==WMS_FRAME_TRANSFER_IN_DEPORT) s_length = 19;
            else if(s_type==WMS_FRAME_TRANSFER_IN_STATION) s_length = 19;
            else if(s_type==WMS_FRAME_INQUERY_STATE) s_length = 10;
            else if(s_type==WMS_FRAME_DELETE_MISSION) s_length = 10;
            else if(s_type==WMS_FRAME_REPORT_ERROR) s_length = 10;
            else if(s_type==WMS_FRAME_REPORT_STATE) s_length = 10;
            else s_length = -1;
        }
    }
    if(s_length>0){
        if(m_socket->bytesAvailable()>=s_length){
            m_nHeartBeatCount = 0;
            QByteArray buf = m_socket->read(s_length);
            buf.insert(0,s_type);s_length++;
            if(_8BitsSum(buf.mid(0,s_length-2))!=(uchar)buf.at(s_length-2)){
                s_length = -1;
                return;
            }
            quint64 id = 0;
            id |= ((quint64)buf.at(8) & 0x00000000000000ff);
            id |= (((quint64)buf.at(7)<<8) & 0x000000000000ff00);
            id |= (((quint64)buf.at(6)<<16) & 0x0000000000ff0000);
            id |= (((quint64)buf.at(5)<<24) & 0x00000000ff000000);
            id |= (((quint64)buf.at(4)<<32) & 0x000000ff00000000);
            id |= (((quint64)buf.at(3)<<40) & 0x0000ff0000000000);
            id |= (((quint64)buf.at(2)<<48) & 0x00ff000000000000);
            id |= (((quint64)buf.at(1)<<56) & 0xff00000000000000);

            switch(s_type){
            case WMS_FRAME_STORE:{
                int station = (uchar)buf.at(9);
                int deport = ((uchar)buf.at(14))*10000+((uchar)buf.at(15))*100+((uchar)buf.at(16));
                int priority = (uchar)(buf.at(17));
                if(m_mapDeport.count(deport) && m_mapStation.count(station)){
                    __log__(&m_file,QString("[recv] a valid store command: deport=%1").arg(deport));
                    int layer = deport%100;
                    if(layer==1) layer = 0;
                    else if(layer==2) layer = 2;
                    emit new_request(id,QPair<int,int>(m_mapStation.value(station),1),
                                     QPair<int,int>(m_mapDeport.value(deport),layer),priority);
                    response_new_mission_received(id,WMS_FRAME_STORE,true);

                }
                else{
                    __log__(&m_file,QString("[recv] a invalid store command: deport=%1").arg(deport));
                    response_new_mission_received(id,WMS_FRAME_STORE,false);
                }
                break;
            }
            case WMS_FRAME_FETCH:{
                int station = (uchar)buf.at(10);
                int deport = ((uchar)buf.at(11))*10000+((uchar)buf.at(12))*100+((uchar)buf.at(13));
                int priority = (uchar)(buf.at(17));
                if(m_mapDeport.count(deport) && m_mapStation.count(station)){
                    __log__(&m_file,QString("[recv] a valid fetch command: deport=%1").arg(deport));
                    int layer = deport%100;
                    if(layer==1) layer = 0;
                    else if(layer==2) layer = 2;
                    else{
                        response_new_mission_received(id,WMS_FRAME_FETCH,false);
                        __log__(&m_file,QString("[recv] a invalid fetch command: deport=%1").arg(deport));
                        break;
                    }
                    emit new_request(id,QPair<int,int>(m_mapDeport.value(deport),layer),
                                     QPair<int,int>(m_mapStation.value(station),1),priority);
                    response_new_mission_received(id,WMS_FRAME_FETCH,true);
                }
                else{
                    __log__(&m_file,QString("[recv] a invalid fetch command: deport=%1").arg(deport));
                    response_new_mission_received(id,WMS_FRAME_FETCH,false);
                }
                break;
            }
            case WMS_FRAME_TRANSFER_IN_DEPORT:{
                int deport_from = ((uchar)buf.at(11))*10000+((uchar)buf.at(12))*100+((uchar)buf.at(13));
                int deport_to = ((uchar)buf.at(14))*10000+((uchar)buf.at(15))*100+((uchar)buf.at(16));
                int priority = (uchar)(buf.at(17));
                if(m_mapDeport.count(deport_from) && m_mapDeport.count(deport_to)){
                    __log__(&m_file,QString("[recv] a valid transfer command: deport_start=%1 deport_end=%2").arg(deport_from).arg(deport_to));                    int layer_start = deport_from%100;
                    int layer_end = deport_to%100;
                    if(layer_start==1) layer_start = 0;
                    else if(layer_start==2) layer_start = 2;
                    if(layer_end==1) layer_end = 0;
                    else if(layer_end==2) layer_end = 2;
                    emit new_request(id,QPair<int,int>(m_mapDeport.value(deport_from),layer_start),
                                     QPair<int,int>(m_mapDeport.value(deport_to),layer_end),priority);
                    response_new_mission_received(id,WMS_FRAME_TRANSFER_IN_DEPORT,true);
                }
                else{
                    __log__(&m_file,QString("[recv] a invalid transfer command: deport_start=%1 deport_end=%2").arg(deport_from).arg(deport_to));
                    response_new_mission_received(id,WMS_FRAME_TRANSFER_IN_DEPORT,false);
                }
                break;
            }
            case WMS_FRAME_TRANSFER_IN_STATION:{
                int station_from = (uchar)buf.at(9);
                int station_to = (uchar)buf.at(10);
                int priority = (uchar)(buf.at(17));
                if(m_mapStation.count(station_from) && m_mapStation.count(station_to)){
                    __log__(&m_file,QString("[recv] a valid transfer command: station_start=%1 station_end=%2").arg(station_from).arg(station_to));
                    emit new_request(id,QPair<int,int>(m_mapStation.value(station_from),1),
                                     QPair<int,int>(m_mapStation.value(station_to),1),priority);
                    response_new_mission_received(id,WMS_FRAME_TRANSFER_IN_STATION,true);
                }
                else{
                    __log__(&m_file,QString("[recv] a invalid transfer command: station_start=%1 station_end=%2").arg(station_from).arg(station_to));
                    response_new_mission_received(id,WMS_FRAME_TRANSFER_IN_STATION,false);
                }
                break;
            }
            case WMS_FRAME_INQUERY_STATE:{
                __log__(&m_file,QString("[recv] a inquiry command: id=%1").arg(id));
                emit inquiry_state(id);
                break;
            }
            case WMS_FRAME_DELETE_MISSION:{
                __log__(&m_file,QString("[recv] a delete command: id=%1").arg(id));
                emit delete_request(id);
                break;
            }
            case WMS_FRAME_REPORT_ERROR:
            case WMS_FRAME_REPORT_STATE:
            {
                __log__(&m_file,QString("[recv] a response: id=%1").arg(id));
                for(int i=0;i<m_messageList.length();i++){
                    WMS_Message message= m_messageList.at(i);
                    if(message.id==id){
                        if(message.type==WMS_FRAME_REPORT_STATE && message.state==STATE_FINISHED){
                            emit mission_finished(id);
                        }
                        m_messageList.removeAt(i);
                        break;
                    }
                }
                break;
            }
            }
            s_length = -1;
        }
    }

}

void WMSInterface::connected()
{
    m_bConnected = true;
    m_nHeartBeatCount = 0;
    __log__(&m_file,"[init] WMS Is On Line");
    emit is_connected(true);
}

void WMSInterface::disconnected()
{
    m_bConnected = false;
    __log__(&m_file,"[init] WMS Is Off Line");
}

