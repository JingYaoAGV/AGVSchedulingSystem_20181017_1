/********************************************************************************
** Form generated from reading UI file 'request.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REQUEST_H
#define UI_REQUEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ui_request
{
public:
    QPushButton *btn_reset;
    QLabel *label;
    QLabel *label_workerAddr;
    QLabel *label_2;
    QLabel *label_workerID;
    QLabel *label_3;
    QLabel *label_AID;
    QLabel *label_4;
    QLabel *label_BID;
    QLabel *label_5;
    QLabel *label_CID;
    QLabel *label_6;
    QLabel *label_DID;
    QLabel *label_instruction;
    QLabel *label_A;
    QLabel *label_B;
    QLabel *label_C;
    QLabel *label_D;
    QPushButton *btn_match;

    void setupUi(QWidget *Ui_request)
    {
        if (Ui_request->objectName().isEmpty())
            Ui_request->setObjectName(QStringLiteral("Ui_request"));
        Ui_request->resize(493, 454);
        btn_reset = new QPushButton(Ui_request);
        btn_reset->setObjectName(QStringLiteral("btn_reset"));
        btn_reset->setGeometry(QRect(390, 20, 71, 23));
        label = new QLabel(Ui_request);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 81, 16));
        label_workerAddr = new QLabel(Ui_request);
        label_workerAddr->setObjectName(QStringLiteral("label_workerAddr"));
        label_workerAddr->setGeometry(QRect(110, 20, 121, 21));
        label_2 = new QLabel(Ui_request);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(240, 20, 71, 16));
        label_workerID = new QLabel(Ui_request);
        label_workerID->setObjectName(QStringLiteral("label_workerID"));
        label_workerID->setGeometry(QRect(310, 20, 61, 20));
        label_3 = new QLabel(Ui_request);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(60, 130, 31, 16));
        label_AID = new QLabel(Ui_request);
        label_AID->setObjectName(QStringLiteral("label_AID"));
        label_AID->setGeometry(QRect(90, 130, 51, 21));
        label_4 = new QLabel(Ui_request);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(250, 130, 31, 16));
        label_BID = new QLabel(Ui_request);
        label_BID->setObjectName(QStringLiteral("label_BID"));
        label_BID->setGeometry(QRect(280, 120, 51, 31));
        label_5 = new QLabel(Ui_request);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(60, 250, 31, 16));
        label_CID = new QLabel(Ui_request);
        label_CID->setObjectName(QStringLiteral("label_CID"));
        label_CID->setGeometry(QRect(90, 250, 51, 21));
        label_6 = new QLabel(Ui_request);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(250, 250, 31, 16));
        label_DID = new QLabel(Ui_request);
        label_DID->setObjectName(QStringLiteral("label_DID"));
        label_DID->setGeometry(QRect(280, 240, 61, 31));
        label_instruction = new QLabel(Ui_request);
        label_instruction->setObjectName(QStringLiteral("label_instruction"));
        label_instruction->setGeometry(QRect(10, 290, 471, 141));
        label_A = new QLabel(Ui_request);
        label_A->setObjectName(QStringLiteral("label_A"));
        label_A->setGeometry(QRect(90, 90, 16, 16));
        label_B = new QLabel(Ui_request);
        label_B->setObjectName(QStringLiteral("label_B"));
        label_B->setGeometry(QRect(290, 90, 16, 16));
        label_C = new QLabel(Ui_request);
        label_C->setObjectName(QStringLiteral("label_C"));
        label_C->setGeometry(QRect(90, 210, 16, 16));
        label_D = new QLabel(Ui_request);
        label_D->setObjectName(QStringLiteral("label_D"));
        label_D->setGeometry(QRect(290, 210, 16, 16));
        btn_match = new QPushButton(Ui_request);
        btn_match->setObjectName(QStringLiteral("btn_match"));
        btn_match->setGeometry(QRect(390, 60, 71, 23));

        retranslateUi(Ui_request);

        QMetaObject::connectSlotsByName(Ui_request);
    } // setupUi

    void retranslateUi(QWidget *Ui_request)
    {
        Ui_request->setWindowTitle(QApplication::translate("Ui_request", "Form", 0));
        btn_reset->setText(QApplication::translate("Ui_request", "\351\207\215\347\275\256", 0));
        label->setText(QApplication::translate("Ui_request", "\345\221\274\345\217\253\345\231\250\345\234\260\345\235\200\357\274\232", 0));
        label_workerAddr->setText(QString());
        label_2->setText(QApplication::translate("Ui_request", "\345\267\245\344\275\215\347\202\271ID\357\274\232", 0));
        label_workerID->setText(QString());
        label_3->setText(QApplication::translate("Ui_request", "A ID\357\274\232", 0));
        label_AID->setText(QString());
        label_4->setText(QApplication::translate("Ui_request", "B ID\357\274\232", 0));
        label_BID->setText(QString());
        label_5->setText(QApplication::translate("Ui_request", "C ID\357\274\232", 0));
        label_CID->setText(QString());
        label_6->setText(QApplication::translate("Ui_request", "D ID\357\274\232", 0));
        label_DID->setText(QString());
        label_instruction->setText(QString());
        label_A->setText(QApplication::translate("Ui_request", "A", 0));
        label_B->setText(QApplication::translate("Ui_request", "B", 0));
        label_C->setText(QApplication::translate("Ui_request", "C", 0));
        label_D->setText(QApplication::translate("Ui_request", "D", 0));
        btn_match->setText(QApplication::translate("Ui_request", "\345\214\271\351\205\215", 0));
    } // retranslateUi

};

namespace Ui {
    class Ui_request: public Ui_Ui_request {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REQUEST_H
