/********************************************************************************
** Form generated from reading UI file 'agvinformation.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AGVINFORMATION_H
#define UI_AGVINFORMATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QTreeView *treeView;
    QTreeView *treeView_2;
    QLabel *label;
    QLabel *label_2;
    QPushButton *add;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *remove;
    QPushButton *ok;
    QPushButton *pause;
    QPushButton *keepOn;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->setWindowModality(Qt::WindowModal);
        Form->setEnabled(true);
        Form->resize(558, 367);
        Form->setMouseTracking(true);
        Form->setAcceptDrops(false);
        treeView = new QTreeView(Form);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(10, 30, 241, 291));
        treeView->setAcceptDrops(true);
        treeView_2 = new QTreeView(Form);
        treeView_2->setObjectName(QStringLiteral("treeView_2"));
        treeView_2->setGeometry(QRect(300, 30, 241, 291));
        treeView_2->setMouseTracking(false);
        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(260, 100, 41, 16));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(260, 210, 41, 16));
        add = new QPushButton(Form);
        add->setObjectName(QStringLiteral("add"));
        add->setGeometry(QRect(10, 330, 71, 23));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 10, 54, 12));
        label_4 = new QLabel(Form);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(300, 10, 54, 12));
        remove = new QPushButton(Form);
        remove->setObjectName(QStringLiteral("remove"));
        remove->setGeometry(QRect(300, 330, 61, 23));
        ok = new QPushButton(Form);
        ok->setObjectName(QStringLiteral("ok"));
        ok->setGeometry(QRect(490, 330, 51, 23));
        pause = new QPushButton(Form);
        pause->setObjectName(QStringLiteral("pause"));
        pause->setGeometry(QRect(370, 330, 51, 23));
        keepOn = new QPushButton(Form);
        keepOn->setObjectName(QStringLiteral("keepOn"));
        keepOn->setGeometry(QRect(430, 330, 51, 23));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        label->setText(QApplication::translate("Form", "\345\212\240\345\205\245=>", 0));
        label_2->setText(QApplication::translate("Form", "\347\247\273\345\207\272<=", 0));
        add->setText(QApplication::translate("Form", "\345\212\240\345\205\245\347\263\273\347\273\237", 0));
        label_3->setText(QApplication::translate("Form", "\345\210\227\350\241\250\344\270\255\357\274\232", 0));
        label_4->setText(QApplication::translate("Form", "\347\263\273\347\273\237\344\270\255\357\274\232", 0));
        remove->setText(QApplication::translate("Form", "\347\247\273\345\207\272\347\263\273\347\273\237", 0));
        ok->setText(QApplication::translate("Form", "\347\241\256\345\256\232", 0));
        pause->setText(QApplication::translate("Form", "\346\232\202\345\201\234", 0));
        keepOn->setText(QApplication::translate("Form", "\347\273\247\347\273\255", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AGVINFORMATION_H
