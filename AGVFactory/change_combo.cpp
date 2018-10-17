#include <QtGui>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSqlRecord>
#include <QLineEdit>
#include "change_combo.h"
#include "global.h"

ChangeComboDialog::ChangeComboDialog(QWidget *parent):
    QDialog(parent)
{
    store_combo = new QComboBox(this);
    layer_combo = new QComboBox(this);
    QSqlQuery query = getQuery();
    query.exec("select * from tb_storage");
    QVector<int> IDs;
    while(query.next()){
        QSqlRecord record = query.record();
        if(!IDs.contains(record.value("NodeID").toInt())){
            IDs.append(record.value("NodeID").toInt());
            store_combo->addItem(record.value("NodeID").toString());
        }
    }
//    for(int i = 0; i < 4; ++i){
//        store_combo->addItem(QString::number((i + 1) * 10));
//        store_combo->addItem(QString::number((i + 1) * 10 + 3));
//    }
    for( int i = 1; i < 5; ++i)
        layer_combo->addItem(QString::number(i));
    storage_lineEdit = new QLineEdit(this);
    label1 = new QLabel(tr("库位: "));
    label2 = new QLabel(tr("层级: "));
    label3 = new QLabel(tr("货物量: "));
    label1->setBuddy(store_combo);
    label2->setBuddy(layer_combo);
    label3->setBuddy(storage_lineEdit);
    okButton = new QPushButton(tr("&Ok"), this);
    cancelButton = new QPushButton(tr("&Cancel"),this);
    okButton->setDefault(true);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(okButton);
    downLayout->addWidget(cancelButton);
    QHBoxLayout *upLayout = new QHBoxLayout;
    upLayout->addWidget(label1);
    upLayout->addWidget(store_combo);
    upLayout->addWidget(label2);
    upLayout->addWidget(layer_combo);
    upLayout->addWidget(label3);
    upLayout->addWidget(storage_lineEdit);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(upLayout);
    mainLayout->addLayout(downLayout);
    setLayout(mainLayout);
}


