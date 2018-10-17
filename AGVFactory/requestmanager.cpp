#include "requestmanager.h"
#include "global.h"
#include "ui_request.h"
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
int receive_length = -1;
RequestManager::RequestManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ui_request)
{
    ui->setupUi(this);
    setMaximumSize(500,460);
    setMinimumSize(500,460);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    app_stateChanged(APP_STOP);
    QFile file("呼叫器使用说明.txt");
    if(file.open(QIODevice::ReadOnly)){
        ui->label_instruction->setText(file.readAll());
    }
    else{
        QMessageBox::critical(0,"错误","未找到文件: \"呼叫器使用说明.txt\"");
    }
}

RequestManager::~RequestManager()
{
}

void RequestManager::load_topoView(TopoView *topoview)
{
    if(topoview)
        m_topoView = topoview;
}

void RequestManager::set_path(QString path)
{
    m_strPath = path;
    QSqlQuery query = getQuery();
    query.exec("create table if not exists tb_request (worker text ,button text,startNode int,endNode int,download int)");
    query.exec("select * from tb_request");
    m_mapReqID.clear();
    m_mapReqPriority.clear();
    while(query.next()){
        QSqlRecord record = query.record();
        int startNode = record.value("startNode").toInt();
        int endNode = record.value("endNode").toInt();
        QByteArray address = stringToAddr(record.value("worker").toString()+
                                          record.value("button").toString());
        m_mapReqID.insert(address,QPair<int,int>(startNode,endNode));
        m_mapReqPriority.insert(address,record.value("priority").toInt());
    }
}

void RequestManager::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::green);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(ui->label_A->pos(),30,30);
    painter.drawEllipse(ui->label_B->pos(),30,30);
    painter.drawEllipse(ui->label_C->pos(),30,30);
    painter.drawEllipse(ui->label_D->pos(),30,30);
    painter.setBrush(Qt::red);
    switch (m_currentAddr.second&BUTTON_MASK){
    case BUTTON_A:
        painter.drawEllipse(ui->label_A->pos(),30,30);break;
    case BUTTON_B:
        painter.drawEllipse(ui->label_B->pos(),30,30);break;
    case BUTTON_C:
        painter.drawEllipse(ui->label_C->pos(),30,30);break;
    case BUTTON_D:
        painter.drawEllipse(ui->label_D->pos(),30,30);break;
    }
    painter.setPen(Qt::blue);
}

bool RequestManager::is_frameValid(QByteArray buf)
{
    unsigned char bbb=0;
    int length = buf.length();
    for(int i=0;i<length-1;i++){
        bbb += (uchar)(buf[i]);
    }
    return ((uchar)(buf.at(length-1)))==255-bbb;
}

uchar RequestManager::create_validByte(QByteArray array)
{
    unsigned char bbb=0;
    int length = array.length();
    for(int i=0;i<length;i++){
        bbb += (uchar)(array[i]);
    }
    return 255-bbb;
}

QPair<QByteArray, char> RequestManager::resolve_address(QByteArray buf)
{
    QPair<QByteArray,uchar> address;
    for(int i=1;i<ADDRESS_LENGTH+1;i++)
        address.first.append(buf[i]);
    address.second = (buf.at(17));
    return address;
}

void RequestManager::new_request(QPair<QByteArray, uchar> addr)
{
    uchar button = addr.second&BUTTON_MASK;
    QByteArray combine_addr = addr.first;
    combine_addr.append(button);
    QByteArray worker_addr = addr.first;
    worker_addr.append(stringToAddr("00"));
    if((~button)&BUTTON_MASK){
        if(m_nApp==APP_RUN){
            if(m_mapReqID.count(combine_addr)>0){
                QPair<int,int> id = m_mapReqID.value(combine_addr);
                if(!m_mapMissionandAddr.count(make_nodes(id.first,id.second))){
                    emit request(id,m_mapReqPriority.value(combine_addr));
                    m_mapMissionandAddr.insert(make_nodes(id.first,id.second),addr);
                    response(addr,false);
                }
            }
        }
    }
}

void RequestManager::response(QPair<QByteArray, uchar> addr,bool is_finished)
{
    uchar button = addr.second & BUTTON_MASK;
    if((button&BUTTON_A_MASK)==0) light_enable(addr.first,BUTTON_LED_A,!is_finished);
    else if((button&BUTTON_B_MASK)==0) light_enable(addr.first,BUTTON_LED_B,!is_finished);
    else if((button&BUTTON_C_MASK)==0) light_enable(addr.first,BUTTON_LED_C,!is_finished);
    else if((button&BUTTON_D_MASK)==0) light_enable(addr.first,BUTTON_LED_D,!is_finished);
    else return;
}

void RequestManager::light_enable(QByteArray worker_addr,QByteArray light, bool on)
{
    QByteArray buf;
    buf.append(stringToAddr("7e00101700"));
    buf.append(worker_addr);
    buf.append(stringToAddr("fffe02"));
    buf.append(light);
    buf.append(on?BUTTON_LED_ON:BUTTON_LED_OFF);
    buf.append(255-_8BitsSum(buf.mid(3)));
    emit serial_write(buf);
}

void RequestManager::recvData(QPair<QByteArray, uchar> addr)
{
    m_currentAddr = addr;
    new_request(addr);
    update();
}

void RequestManager::app_stateChanged(int state)
{
    m_nApp = state;
    ui->btn_match->setEnabled(m_nApp==APP_PREPARE);
    ui->btn_reset->setEnabled(m_nApp==APP_PREPARE);
    if(m_nApp!=APP_PREPARE){
        ui->btn_match->setText("匹配");
    }
}

void RequestManager::request_accomplished(int start, int end)
{
    int key = make_nodes(start,end);
    if(m_mapMissionandAddr.count(key)){
        response(m_mapMissionandAddr.value(key),true);
        m_mapMissionandAddr.remove(key);
    }
}

void RequestManager::turnOffAllLight()
{
    QSqlQuery query = getQuery();
    query.exec("select * from tb_request");
    QByteArray worker;
    QByteArray last_worker;  // 防止重复关闭一个呼叫器上的LED
    while(query.next()){
        QSqlRecord record = query.record();
        worker = stringToAddr(record.value("worker").toString());
        if(last_worker!=worker){
            light_enable(worker,BUTTON_LED_A,false);
            light_enable(worker,BUTTON_LED_B,false);
            light_enable(worker,BUTTON_LED_C,false);
            light_enable(worker,BUTTON_LED_D,false);
        }
        last_worker = worker;
    }
}
