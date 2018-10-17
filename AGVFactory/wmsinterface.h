#ifndef WMSINTERFACE_H
#define WMSINTERFACE_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include <QTimerEvent>
#include <QTextStream>
#include <QFile>
enum WMS_FRAME_REPORT_STATE {STATE_FINISHED=0x00,STATE_FETCHED=0x03};
enum WMS_FRAME_INQUIRY_STATE {STATE_WAIT=0x01,STATE_ERROR=0x02,STATE_FETCH=0x03,STATE_DELIVERY=0x04};
struct WMS_Message{
    quint64 id;
    char type;
    char state;
};

class WMSInterface : public QObject
{
    Q_OBJECT
public:
    //enum{WMS_STATE_WAIT=0,WMS_STATE_RUN=1,WMS_STATE_FINISH=2,WMS_STATE_ERROR=255};
public:
    explicit WMSInterface(QObject *parent = 0);
    ~WMSInterface();
    void response_new_mission_received(quint64 id, char type, bool executable);
    void response_delete(quint64 id, bool can_be_delete);
    void response_state(quint64 id, int state);
    void send_agv_error(quint64 id,int error_code);
    void report_state(quint64 id,int state);
    void report_error(quint64 id,int error_code);
    void send_mission_state(quint64 id, int state);  // 主动反馈任务完成状态
    void loadDatabase();
    void connectToHost();

private:
    void timerEvent(QTimerEvent*);

private:
    QTcpSocket* m_socket;
    QMap<int,int> m_mapDeport;
    QMap<int,int> m_mapStation;
    QList< WMS_Message > m_messageList;
    QString m_strServerIP;
    int m_nServerPort;
    int m_nSendPeriod;
    int m_nCommOverTime;
    bool m_bWMSOn;
    bool m_bAutoConnect;
    bool m_bConnected;
    int m_nTimerID;
    int m_nHeartBeatCount;
    QFile m_file;

signals:
    void new_request(quint64 id,QPair<int,int>,QPair<int,int>,int priority);
    void inquiry_state(quint64 id);
    void delete_request(quint64 id);
    void mission_finished(quint64 id);
    void is_connected(bool);
public slots:
    void readData();
    void connected();
    void disconnected();
};

#endif // WMSINTERFACE_H
