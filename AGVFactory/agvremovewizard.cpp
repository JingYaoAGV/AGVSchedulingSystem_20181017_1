#include "agvremovewizard.h"

AGVRemoveWizard::AGVRemoveWizard(bool hasMission,bool reach,int start,int end,int id,QList<int>ids,QWidget *parent)
    :QWizard(parent)
{
       agv_id=id;
       agv_all=ids;
       AgvhasMission=hasMission;
       Agvreach=reach;
       missionStart=start;
       missionEnd=end;

       setWizardStyle(AeroStyle);
       setPage(Page_Intro,new IntroPage);
       setPage(Page_userInfoMation,new userInfoMation);
       setPage(Page_choseAGV,new choseAGV(agv_id,AgvhasMission,Agvreach,agv_all));
       noMission *no=new noMission;
       setPage(Page_noMission,no);
       setPage(Page_MissionArrange,new MissionArrange(missionStart,missionEnd));
       setPage(Page_Finish,new Finish);
       setPage(Page_totheStart,new TotheStart);
       setPage(Page_totheEnd,new TotheEnd);
       MissionReset *reset=new MissionReset;
       setPage(Page_MissionReset, reset);
       MissionFinish *finish=new MissionFinish;
       setPage(Page_MissionFinish,finish);
       //addPage(new IntroPage);
       //addPage(new userInfoMation);
      // addPage(new choseAGV(agv_id,agv_all));
      // addPage(new noMission);
     //  connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(indexChanged(int)));
       this->setPixmap(QWizard::BannerPixmap, QPixmap(":/wizard/wizardimages/banner.png"));
       this->setPixmap(QWizard::BackgroundPixmap, QPixmap(":/wizard/wizardimages/background.png"));
       connect(no,SIGNAL(missionNext(bool,bool,bool)),this,SLOT(missionNext(bool,bool,bool)));
       connect(reset,SIGNAL(missionNext(bool,bool,bool)),this,SLOT(missionNext(bool,bool,bool)));
       connect(finish,SIGNAL(missionNext(bool,bool,bool)),this,SLOT(missionNext(bool,bool,bool)));
       setWindowTitle(tr("AGV移出向导"));
}
void AGVRemoveWizard::missionNext(bool hasmission, bool reset, bool finish)
{
        emit missionstate(agv_id,hasmission,reset,finish);
}
/*bool AGVRemoveWizard::validateCurrentPage()
{
    qDebug()<< this->currentPage()->nextId();

   if(-1==this->currentPage()->nextId())
   {
       QString title=this->currentPage()->windowTitle();
      if(title==tr("用户信息"))
      {
         this->currentPage()->layout()->children().at(1)->
      }
   }
   return true;
}
*/
IntroPage::IntroPage(QWidget *parent)
    :QWizardPage(parent)
{


    setTitle(tr("提示"));
    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/wizard/wizardimages/run.ico"));

    label = new QLabel(tr("               当AGV掉线或故障时，方可将AGV移出系统"));

    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}
userInfoMation::userInfoMation(QWidget *parent)
    :QWizardPage(parent)
{
    setTitle(tr("用户信息"));
    setSubTitle(tr("请输入您的账号和密码"));
    UserNameLabel= new QLabel(tr("&用户名:"));
    UserNameLineEdit=new QLineEdit;
    UserNameLabel->setBuddy(UserNameLineEdit);
    UserPasswordLabel=new QLabel(tr("&密码："));
    UserPasswordLineEdit=new QLineEdit;
    UserPasswordLabel->setBuddy(UserPasswordLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(UserNameLabel, 0, 0);
    layout->addWidget(UserNameLineEdit, 0, 1);
    layout->addWidget(UserPasswordLabel, 1, 0);
    layout->addWidget(UserPasswordLineEdit, 1, 1);
    setLayout(layout);




   // QAbstractButton *bt = this->wizard()->button(QWizard::FinishButton);

}
bool userInfoMation::validatePage()
{
   // qDebug()<<nextId();
   QString name=UserNameLineEdit->text();
   QString  password=UserPasswordLineEdit->text();
    if(nextId()==2)//2  next -1 finsh  shunxu  mingbaile
    {

        if((name==tr("admin"))&&(password==tr("123456")))
            return true;
        else
            return false;
    }
    return false;
}
choseAGV::choseAGV(int id,bool hasmission,bool reach,QList<int>ids,QWidget *parent)
    :QWizardPage(parent)
{
     agv_id=id;
     agv_ids=ids;
     agvreach=reach;
     agvhasmission=hasmission;
     setTitle(tr("请选择要删除的车辆"));
     groupBox = new QGroupBox(tr("&选项"));
     groupBoxLayout = new QVBoxLayout;
      groupButton=new QButtonGroup;
     for(int i=0;i<ids.length();i++)
     {
         QRadioButton *RaioButton= new QRadioButton(QString("%1").arg(ids.at(i)));
         groupButton->addButton(RaioButton,i);
         groupBoxLayout->addWidget(RaioButton);
     }

     groupBox->setLayout(groupBoxLayout);

     QGridLayout *layout = new QGridLayout;
     layout->addWidget(groupBox, 0, 0, 1, 2);
     setLayout(layout);
}
bool choseAGV::validatePage()
{
    if(nextId()==3)
    {    if(agv_id!=0)
        {
            int l=agv_ids.indexOf(agv_id);
            QAbstractButton *bt3=groupButton->button(l);
           // QAbstractButton *bt1=qobject_cast<QAbstractButton *>(groupButton->children().at(i));
              if(bt3->isChecked()&&bt3->text()==QString("%1").arg(agv_id))
              {
                return true;
              }
              else
                 return false;
        }
    }
    return true;
}
int choseAGV::nextId()const
{

                 if(agvhasmission==true)
                 {
                   if(agvreach==true)
                       return AGVRemoveWizard::Page_MissionArrange;
                    else
                       return AGVRemoveWizard::Page_MissionReset;
                 }
                 else
                 {
                     return AGVRemoveWizard::Page_noMission;
                 }




          return AGVRemoveWizard::Page_noMission;

}
noMission::noMission(QWidget *parent)
    :QWizardPage(parent=0)
{
    setTitle(tr("该AGV无任务"));
    setSubTitle(tr("                           移出AGV成功!"));
}
bool noMission::validatePage()
{
    emit missionNext(false,false,false);
    return true;
}
int noMission::nextId() const
{
    return -1;
}
MissionArrange::MissionArrange(int start,int end,QWidget *parent)
    :QWizardPage(parent=0)
{

    setTitle(tr("该AGV有任务存在"));
    setSubTitle(tr("请选择要执行的操作"));
    groupBox = new QGroupBox(tr("&选项"));
    groupBoxLayout = new QVBoxLayout;
    //groupButton=new QButtonGroup;
    toTheStart= new QRadioButton(QString("将货物放回原处：%1").arg(start));//100表示任务起点
    toTheStart->setChecked(false);
    toTheEnd  =new  QRadioButton(QString("将货物运往终点：%1").arg(end));
    toTheEnd->setChecked(false);
   // groupButton->addButton(RaioButton,i);
    groupBoxLayout->addWidget(toTheStart);
    groupBoxLayout->addWidget(toTheEnd);

    groupBox->setLayout(groupBoxLayout);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(groupBox, 0, 0, 1, 2);
    setLayout(layout);


}
bool MissionArrange::validatePage()
{
    if(toTheStart->isChecked())
    {
      return true;
    }
    else if(toTheEnd->isChecked())
    {
        return true;
    }
    else
        return false;
}
int MissionArrange::nextId() const
{
    if(toTheStart->isChecked())
    return AGVRemoveWizard::Page_totheStart;
    else if(toTheEnd->isChecked())
        return AGVRemoveWizard::Page_totheEnd;
    else
        return AGVRemoveWizard::Page_Finish;
}
TotheStart::TotheStart(QWidget *parent)
     :QWizardPage(parent=0)
{
   // flag=false;
   // emit completeChanged();

    setTitle(tr("货物人工运回起点中..."));
    setSubTitle(tr("  请在货物已经运到位置后点击下一步"));

   // timerstart=new QTimer;
    //timerstart->setInterval(2000);
  //  timerstart->start(5000);
   // connect(timerstart,SIGNAL(timeout()),this,SLOT(On_timeout()));
   // connect(this,SIGNAL(completeChanged()),this,SLOT(isComplete()));
}
int TotheStart::nextId()const
{
   return AGVRemoveWizard::Page_MissionReset;
}

/*bool TotheStart::isComplete() const
{

     if(flag==true)
         return true;
     else return false;
}
void TotheStart::On_timeout()
{
    flag=true;
    timerstart->stop();
     emit completeChanged();
}
*/

TotheEnd::TotheEnd(QWidget *parent)
     :QWizardPage(parent=0)
{
    setTitle(tr("货物人工运往终点中..."));
    setSubTitle(tr("  请在货物已经运到位置后点击下一步"));

}
int TotheEnd::nextId()const
{
    return AGVRemoveWizard::Page_MissionFinish;
}
MissionReset::MissionReset(QWidget *parent)
     :QWizardPage(parent=0)
{
    setTitle(tr("AGV移出成功,任务已经重新生成"));
   // setSubTitle(tr("  请在货物已经运到位置后点击下一步"));

}
bool MissionReset::validatePage()
{
    emit missionNext(true,true,false);
    return true;
}
int MissionReset::nextId()const
{
    return -1;
}
MissionFinish::MissionFinish(QWidget *parent)
     :QWizardPage(parent=0)
{
    setTitle(tr("AGV移出成功,任务已经完成"));
   // setSubTitle(tr("  请在货物已经运到位置后点击下一步"));

}
bool MissionFinish::validatePage()
{
    emit missionNext(true,false,true);
    return true;
}
int MissionFinish::nextId()const
{
    return -1;
}
Finish::Finish(QWidget *parent)
     :QWizardPage(parent=0)
{
    setTitle(tr("完成"));

}
int Finish::nextId() const
{
    return -1;
}
