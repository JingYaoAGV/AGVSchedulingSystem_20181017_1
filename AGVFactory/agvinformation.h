    #ifndef AGVINFORMATION_H
#define AGVINFORMATION_H

#include <QWidget>
#include<QTableView>
#include<QPushButton>
#include <QLabel>
#include <QSqlTableModel>
#include <QTreeView>
#include <QStandardItemModel>
#include "agvremovewizard.h"
//以下是图标的宏设定
#ifndef TREE_ITEM_ICON_Project
#define TREE_ITEM_ICON_Project QStringLiteral("treeItem_Project")
#endif
#ifndef TREE_ITEM_ICON_folder
#define TREE_ITEM_ICON_folder QStringLiteral("treeItem_folder")
#endif
#ifndef TREE_ITEM_ICON_folderOriginal
#define TREE_ITEM_ICON_folderOriginal QStringLiteral("treeItem_folder-ansys")
#endif
#ifndef TREE_ITEM_ICON_folderAnsys
#define TREE_ITEM_ICON_folderAnsys QStringLiteral("treeItem_folder-ansys")
#endif
#ifndef TREE_ITEM_ICON_TdmsGroup
#define TREE_ITEM_ICON_TdmsGroup QStringLiteral("treeItem_group")
#endif
#ifndef TREE_ITEM_ICON_TdmsChannel
#define TREE_ITEM_ICON_TdmsChannel QStringLiteral("treeItem_channel")
#endif
#ifndef TREE_ITEM_ICON_DataItem
#define TREE_ITEM_ICON_DataItem QStringLiteral("treeItem_dataItem")
#endif
namespace Ui {
class Form;
}




class AgvInformation : public QWidget
{
    Q_OBJECT
public:
    explicit AgvInformation(QWidget *parent = 0);
public:
    void initTree();
    void initTree2();
    QList<int> agvInUse();
    void createTreeView();

public:
    void createTableWidget();
    void creatButton();
    void creatLayout();
    void open_database();
    void show_agvInda();

private:
     QTableView *m_tableAgvAnuse;
     QTreeView  *m_treeView;
     QTableView *m_tableAgvInuse;

     QPushButton *m_btnCreate;
     QPushButton *m_btnEdit;
     QPushButton *m_btnHang;
     QPushButton *m_btnDelete;

     QLabel  *m_addAgv;
     QLabel  *m_removeAgv;
     QSqlTableModel *m_agvTable;

signals:
     void AgvInformationChange();
     void addintoRun(int);
     void chushihua(int,int,int,int,int,int,int,int);//最后一个变量为角度
     void removeFromRun(int);
     void  get_agvmission(int);
     void  ToMissionmanager(int,bool,bool,bool);
     void  pauseAgv(int agvid,int pointid);
     void  keepMove(int);
public slots:
     void addIntoSystem();
     void removeFromSystem();
     void pause();
     void keepOn();
     void missionDecide(int,bool,bool,bool);
     void startWizard(bool hasMission,bool reach,int start,int end,int ,QList<int>);
     void insertIntoSystem();
     void  inSystem(int id);
     void   outSystem(int id);
     void  receiveMission(bool,bool,int,int);

public:
     QMap<int ,int> m_idAddress_1;//调度系统与不同车辆的ID
     QMap<int ,int> m_idAddress_2;//调度系统与不同车辆的ID
private:
     QMap<QString,QIcon> m_publicIconMap;///< 存放公共图标
     QMap<int ,QString> m_idName;
     QMap<int ,int> m_idStartNode;

     QMap<int ,QString> m_idType;
     QMap<int ,int> m_idChexing;
     QMap<int ,int> m_idAngle;
     QMap<int ,int > m_idChargeStation;
     QMap<int ,QStringList>m_idChargeLimit;
     QList<int> m_agvInuse;
     QList<int> m_agvall;
     QMap<QString,QList<int>>m_PStypeNode;


     QStandardItemModel* model;
     QStandardItem* itemProject;
     QStandardItem* itemProjectPS20 ;//需要修改，已经没用了
     QStandardItem* itemProjectPH20;
     QStandardItem* itemFolder;
     QStandardItem* itemgroup;
     QStandardItem* itemchannel;

     QStandardItemModel* model2;
     QStandardItem* itemProject2;
     QStandardItem* itemProject2PS20 ;
     QStandardItem* itemProject2PH20 ;
     QStandardItem* itemFolder2;
     QStandardItem* itemgroup2;
     QStandardItem* itemchannel2;
     /*
     QStandardItemModel* model3;
     QStandardItem* itemProject2PH20 ;
     QStandardItem* itemProject2PS20 ;
     QStandardItem* itemFolder2;
     QStandardItem* itemgroup2;
     QStandardItem* itemchannel2;
     */
     int deleteId;
     QModelIndex index2;
     QString nameparent;
private:
     Ui::Form *ui;
     AGVRemoveWizard *agvremoveWizard;


};

#endif // AGVINFORMATION_H
