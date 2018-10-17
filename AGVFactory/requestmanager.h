#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H
#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "topoview.h"
#include "global.h"

enum Frame_Type {Frame_Button=0x92};

namespace Ui {
class Ui_request;
}
class RequestManager : public QWidget
{
    Q_OBJECT
public:
    explicit RequestManager(QWidget *parent = 0);
    ~RequestManager();
    void load_topoView(TopoView* topoview);
    void set_path(QString path);
    void turnOffAllLight();
private:
    void paintEvent(QPaintEvent *);
    bool is_frameValid(QByteArray buf);
    QPair<QByteArray,char> resolve_address(QByteArray buf);
    void new_request(QPair<QByteArray,uchar> addr);
    void response(QPair<QByteArray, uchar> addr, bool is_finished=false);
    uchar create_validByte(QByteArray array);
    void light_enable(QByteArray worker_addr, QByteArray light, bool on=true);
    long make_nodes(int id1,int id2){return id1*1000+id2;}
private:
    Ui::Ui_request *ui;
    int m_nApp;   // 应用程序当前运行状态（可以改为全局变量）
    QMap<QByteArray, QPair<int,int> > m_mapReqID;  // 匹配的按钮和拓扑点
    QMap<QByteArray, int> m_mapReqPriority;
    TopoView *m_topoView; // 当前的拓扑图
    QPair<QByteArray,uchar> m_currentAddr;   // 最近一次按下的按钮
    QString m_strPath;  // 保存匹配结果的数据库文件路径
    QMap< int,QPair<QByteArray,uchar> >m_mapMissionandAddr;// 保存当前请求的按钮地址，以便当完成任务时关闭指示灯

signals:
    void request(QPair<int,int>,int);
    void serial_write(QByteArray data);
public slots:
    //void serialRead(QByteArray data);
    void recvData(QPair<QByteArray,uchar> addr);
    void app_stateChanged(int state);
    void request_accomplished(int start,int end);

};

#endif // REQUESTMANAGER_H
