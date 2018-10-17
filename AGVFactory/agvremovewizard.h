#ifndef AGVREMOVEWIZARD_H
#define AGVREMOVEWIZARD_H
#include<QWizard>
#include<QLabel>
#include<QLineEdit>
#include<QGroupBox>
#include <QVBoxLayout>
#include <QDebug>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTimer>
/*class IntroPage;
class userInfoMation;
class choseAGV;
class noMission;
*/
class AGVRemoveWizard:public QWizard
{
    Q_OBJECT
public:
    enum { Page_Intro, Page_userInfoMation, Page_choseAGV, Page_noMission,
           Page_Finish,Page_MissionArrange,Page_totheStart,Page_totheEnd,
          Page_MissionReset,Page_MissionFinish};

public:
    AGVRemoveWizard(bool hasMission,bool reach,int start,int end,int id,QList<int>ids,QWidget *parent=0);
   // bool validateCurrentPage();
private:
    int agv_id;
    QList<int> agv_all;
    bool AgvhasMission;
    bool Agvreach;
    int  missionStart;
    int  missionEnd;
    QMap<int,bool>agv_mission;
    /*IntroPage *into;
    userInfoMation *user;
    choseAGV *chose;
    noMission *no;
    */
public slots:
    void missionNext(bool,bool,bool);
signals:
    void missionstate(int ,bool,bool,bool);
};
class IntroPage:public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget *parent=0);
private:
    QLabel *label;
};
class userInfoMation:public QWizardPage
{
    Q_OBJECT
public:
    userInfoMation(QWidget *parent=0);
    bool validatePage();
  // virtual bool validateCurrentPage() ;
private:
     QLabel *UserNameLabel;
     QLabel *UserPasswordLabel;
     QLineEdit *UserNameLineEdit;
     QLineEdit *UserPasswordLineEdit;

};
class choseAGV:public QWizardPage
{
    Q_OBJECT
 public:
    choseAGV(int id,bool hasmission,bool reach,QList<int> ids,QWidget *parent=0);
    bool validatePage();
    int nextId() const;//百度下什么写法
private:
    QGroupBox *groupBox;
    QVBoxLayout *groupBoxLayout;
    QButtonGroup *groupButton;
    int agv_id;
    QList<int> agv_ids;
    bool agvreach;
    bool agvhasmission;
};
class noMission:public QWizardPage
{
   Q_OBJECT
public:
    noMission(QWidget *parent=0);
    int nextId() const;
    bool validatePage();
signals:
    void missionNext(bool ,bool,bool);//是否有任务，是否重新生成,是否完成

};
class MissionArrange:public QWizardPage
{
    Q_OBJECT
 public:
     MissionArrange(int start,int end,QWidget *parent=0);
     int nextId() const;
     bool validatePage();
private:
     QGroupBox *groupBox;
     QVBoxLayout *groupBoxLayout;
     //QButtonGroup *groupButton;
     QRadioButton *toTheStart;
     QRadioButton *toTheEnd;


};
class TotheStart:public QWizardPage
{
    Q_OBJECT
 public:
     TotheStart(QWidget *parent=0);
     int nextId() const;
public slots:
     //bool isComplete() const;
private:
    // QTimer *timerstart;
    // bool flag;
public slots:
    // void On_timeout();
};
class TotheEnd:public QWizardPage
{
    Q_OBJECT
 public:
     TotheEnd(QWidget *parent=0);
     int nextId() const;
private:
     QTimer *timerend;
};
class MissionReset:public QWizardPage
{
    Q_OBJECT
 public:
     MissionReset(QWidget *parent=0);
     int nextId() const;
     bool validatePage();
signals:
    void missionNext(bool ,bool,bool);//是否有任务，是否重新生成,是否完成
};
class MissionFinish:public QWizardPage
{
    Q_OBJECT
 public:
     MissionFinish(QWidget *parent=0);
     int nextId() const;
     bool validatePage();
signals:
    void missionNext(bool ,bool,bool);//是否有任务，是否重新生成,是否完成
};

class Finish:public QWizardPage
{
    Q_OBJECT
 public:
     Finish(QWidget *parent=0);
     int nextId() const;
};


#endif // AGVREMOVEWIZARD_H
