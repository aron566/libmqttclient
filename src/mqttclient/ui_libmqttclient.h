/********************************************************************************
** Form generated from reading UI file 'mqtt.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIBMQTTCLIENT_H
#define UI_LIBMQTTCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_libmqttclient
{
public:
    QGridLayout *gridLayout;
    QGroupBox *TOPICgroupBox;
    QLineEdit *SUBSCRIBElineEdit;
    QLabel *SUBSCRIBElabel;
    QLabel *PUBLISHlabel;
    QLineEdit *PUBLISHlineEdit;
    QPushButton *SUBSCRIBEpushButton;
    QPushButton *PUBLISHpushButton;
    QLabel *SUBSCRIBEQOSlabel;
    QLabel *PUBLISHQOSlabel;
    QSpinBox *SUBSCRIBEQOSspinBox;
    QSpinBox *PUBLISHQOSspinBox;
    QFrame *line;
    QPushButton *UNSUBSCRIBEpushButton;
    QPushButton *UNPUBLISHpushButton;
    QLabel *MQTTTITLElabel;
    QGroupBox *NETWOEKgroupBox;
    QLineEdit *HOSTNAMElineEdit;
    QLabel *HOSTNAMElabel;
    QLabel *HOSTIPlabel;
    QLineEdit *HOSTIPlineEdit;
    QLabel *HOSTPORTlabel;
    QLineEdit *HOSTPORTlineEdit;
    QLabel *USERNAMElabel;
    QLineEdit *USERNAMElineEdit;
    QLabel *KEYWORDlabel;
    QLineEdit *KEYWORDlineEdit;
    QPushButton *CONNECTpushButton;
    QLabel *CONNECTlabel;
    QLabel *CONNECTSTATElabel;
    QPushButton *CLEARpushButton;
    QPushButton *returnpushButton;
    QGroupBox *SUBSCRIBEgroupBox;
    QGridLayout *gridLayout_3;
    QListWidget *SUBSCRIBElistWidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QTextBrowser *textBrowser;
    QPushButton *SENDMSGpushButton;
    QPushButton *confirmpushButton;
    QCheckBox *checkBox;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QListWidget *PUBLISHlistWidget;
    QLineEdit *DEBUGlineEdit;

    int WINDOWWIDTH = 1280;
    int WINDOWHEIGHT = 720;
    void setWINDOWsize(int w ,int h)
    {
        WINDOWWIDTH = w;
        WINDOWHEIGHT = h;
    }
    void setupUi(QWidget *mqtt)
    {
        if (mqtt->objectName().isEmpty())
            mqtt->setObjectName(QString::fromUtf8("mqtt"));
        mqtt->resize(WINDOWWIDTH, WINDOWHEIGHT);
        mqtt->setStyleSheet(QString::fromUtf8("background-color: rgb(64, 65, 66);"));
        gridLayout = new QGridLayout(mqtt);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        TOPICgroupBox = new QGroupBox(mqtt);
        TOPICgroupBox->setObjectName(QString::fromUtf8("TOPICgroupBox"));

        int x0 = 0 ,y0 = 0;
        int width_min = 631*WINDOWWIDTH/1280;
        int weight_min = 271*WINDOWHEIGHT/720;
        TOPICgroupBox->setMinimumSize(QSize(width_min, weight_min));
        TOPICgroupBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 170, 0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        SUBSCRIBElineEdit = new QLineEdit(TOPICgroupBox);
        SUBSCRIBElineEdit->setObjectName(QString::fromUtf8("SUBSCRIBElineEdit"));

        x0 = 130*width_min/631;
        y0 = 30*weight_min/271;
        SUBSCRIBElineEdit->setGeometry(QRect(x0, y0, 391*width_min/631, 41*weight_min/271));
        SUBSCRIBElineEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        SUBSCRIBElabel = new QLabel(TOPICgroupBox);
        SUBSCRIBElabel->setObjectName(QString::fromUtf8("SUBSCRIBElabel"));

        x0 = 10*width_min/631;
        y0 = 35*weight_min/271;
        SUBSCRIBElabel->setGeometry(QRect(x0, y0, 111*width_min/631, 31*weight_min/271));

        PUBLISHlabel = new QLabel(TOPICgroupBox);
        PUBLISHlabel->setObjectName(QString::fromUtf8("PUBLISHlabel"));

        x0 = 10*width_min/631;
        y0 = 160*weight_min/271;
        PUBLISHlabel->setGeometry(QRect(x0, y0, 111*width_min/631, 31*weight_min/271));
        PUBLISHlabel->setStyleSheet(QString::fromUtf8("color: rgb(85, 170, 255);"));
        PUBLISHlineEdit = new QLineEdit(TOPICgroupBox);
        PUBLISHlineEdit->setObjectName(QString::fromUtf8("PUBLISHlineEdit"));

        x0 = 130*width_min/631;
        y0 = 160*weight_min/271;
        PUBLISHlineEdit->setGeometry(QRect(x0, y0, 391*width_min/631, 41*weight_min/271));
        PUBLISHlineEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        SUBSCRIBEpushButton = new QPushButton(TOPICgroupBox);
        SUBSCRIBEpushButton->setObjectName(QString::fromUtf8("SUBSCRIBEpushButton"));

        x0 = 530*width_min/631;
        y0 = 30*weight_min/271;
        SUBSCRIBEpushButton->setGeometry(QRect(x0, y0, 93*width_min/631, 41*weight_min/271));
        SUBSCRIBEpushButton->setMinimumSize(QSize(0, 41*weight_min/271));
        SUBSCRIBEpushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        PUBLISHpushButton = new QPushButton(TOPICgroupBox);
        PUBLISHpushButton->setObjectName(QString::fromUtf8("PUBLISHpushButton"));

        x0 = 530*width_min/631;
        y0 = 160*weight_min/271;
        PUBLISHpushButton->setGeometry(QRect(x0, y0, 93*width_min/631, 41*weight_min/271));
        PUBLISHpushButton->setMinimumSize(QSize(0, 41*weight_min/271));
        PUBLISHpushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        SUBSCRIBEQOSlabel = new QLabel(TOPICgroupBox);
        SUBSCRIBEQOSlabel->setObjectName(QString::fromUtf8("SUBSCRIBEQOSlabel"));

        x0 = 10*width_min/631;
        y0 = 80*weight_min/271;
        SUBSCRIBEQOSlabel->setGeometry(QRect(x0, y0, 111*width_min/631, 41*weight_min/271));
        PUBLISHQOSlabel = new QLabel(TOPICgroupBox);
        PUBLISHQOSlabel->setObjectName(QString::fromUtf8("PUBLISHQOSlabel"));

        x0 = 10*width_min/631;
        y0 = 210*weight_min/271;
        PUBLISHQOSlabel->setGeometry(QRect(x0, y0, 111*width_min/631, 41*weight_min/271));
        PUBLISHQOSlabel->setStyleSheet(QString::fromUtf8("color: rgb(85, 170, 255);"));
        SUBSCRIBEQOSspinBox = new QSpinBox(TOPICgroupBox);
        SUBSCRIBEQOSspinBox->setObjectName(QString::fromUtf8("SUBSCRIBEQOSspinBox"));

        x0 = 130*width_min/631;
        y0 = 80*weight_min/271;
        SUBSCRIBEQOSspinBox->setGeometry(QRect(x0, y0, 391*width_min/631, 41*weight_min/271));
        SUBSCRIBEQOSspinBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        SUBSCRIBEQOSspinBox->setMaximum(1);
        PUBLISHQOSspinBox = new QSpinBox(TOPICgroupBox);
        PUBLISHQOSspinBox->setObjectName(QString::fromUtf8("PUBLISHQOSspinBox"));

        x0 = 130*width_min/631;
        y0 = 210*weight_min/271;
        PUBLISHQOSspinBox->setGeometry(QRect(x0, y0, 391*width_min/631, 41*weight_min/271));
        PUBLISHQOSspinBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        PUBLISHQOSspinBox->setMaximum(0);
        line = new QFrame(TOPICgroupBox);
        line->setObjectName(QString::fromUtf8("line"));

        x0 = 10*width_min/631;
        y0 = 130*weight_min/271;
        line->setGeometry(QRect(x0, y0, 611*width_min/631, 16*weight_min/271));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        UNSUBSCRIBEpushButton = new QPushButton(TOPICgroupBox);
        UNSUBSCRIBEpushButton->setObjectName(QString::fromUtf8("UNSUBSCRIBEpushButton"));

        x0 = 530*width_min/631;
        y0 = 80*weight_min/271;
        UNSUBSCRIBEpushButton->setGeometry(QRect(x0, y0, 93*width_min/631, 41*weight_min/271));
        UNSUBSCRIBEpushButton->setMinimumSize(QSize(0, 41*weight_min/271));
        UNSUBSCRIBEpushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        UNPUBLISHpushButton = new QPushButton(TOPICgroupBox);
        UNPUBLISHpushButton->setObjectName(QString::fromUtf8("UNPUBLISHpushButton"));

        x0 = 530*width_min/631;
        y0 = 210*weight_min/271;
        UNPUBLISHpushButton->setGeometry(QRect(x0, y0, 93*width_min/631, 41*weight_min/271));
        UNPUBLISHpushButton->setMinimumSize(QSize(0, 41*weight_min/271));
        UNPUBLISHpushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        gridLayout->addWidget(TOPICgroupBox, 2, 0, 2, 1);

        MQTTTITLElabel = new QLabel(mqtt);
        MQTTTITLElabel->setObjectName(QString::fromUtf8("MQTTTITLElabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MQTTTITLElabel->sizePolicy().hasHeightForWidth());
        MQTTTITLElabel->setSizePolicy(sizePolicy);
        MQTTTITLElabel->setStyleSheet(QString::fromUtf8("font: 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(255, 255, 255);"));

        gridLayout->addWidget(MQTTTITLElabel, 0, 0, 1, 8);

        NETWOEKgroupBox = new QGroupBox(mqtt);
        NETWOEKgroupBox->setObjectName(QString::fromUtf8("NETWOEKgroupBox"));

        width_min = 631*WINDOWWIDTH/1280;
        weight_min = 381*WINDOWHEIGHT/720;
        NETWOEKgroupBox->setMinimumSize(QSize(width_min, weight_min));
        NETWOEKgroupBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        HOSTNAMElineEdit = new QLineEdit(NETWOEKgroupBox);
        HOSTNAMElineEdit->setObjectName(QString::fromUtf8("HOSTNAMElineEdit"));

        x0 = 130*width_min/631;
        y0 = 30*weight_min/381;
        HOSTNAMElineEdit->setGeometry(QRect(x0, y0, 491*width_min/631, 41*weight_min/381));
        HOSTNAMElabel = new QLabel(NETWOEKgroupBox);
        HOSTNAMElabel->setObjectName(QString::fromUtf8("HOSTNAMElabel"));

        x0 = 10*width_min/631;
        y0 = 35*weight_min/381;
        HOSTNAMElabel->setGeometry(QRect(x0, y0, 111*width_min/631, 31*weight_min/381));
        HOSTIPlabel = new QLabel(NETWOEKgroupBox);
        HOSTIPlabel->setObjectName(QString::fromUtf8("HOSTIPlabel"));

        x0 = 10*width_min/631;
        y0 = 95*weight_min/381;
        HOSTIPlabel->setGeometry(QRect(x0, y0, 111*width_min/631, 31*weight_min/381));
        HOSTIPlineEdit = new QLineEdit(NETWOEKgroupBox);
        HOSTIPlineEdit->setObjectName(QString::fromUtf8("HOSTIPlineEdit"));

        x0 = 130*width_min/631;
        y0 = 90*weight_min/381;
        HOSTIPlineEdit->setGeometry(QRect(x0, y0, 491*width_min/631, 41*weight_min/381));
        HOSTPORTlabel = new QLabel(NETWOEKgroupBox);
        HOSTPORTlabel->setObjectName(QString::fromUtf8("HOSTPORTlabel"));

        x0 = 10*width_min/631;
        y0 = 150*weight_min/381;
        HOSTPORTlabel->setGeometry(QRect(x0, y0, 111*width_min/631, 31*weight_min/381));
        HOSTPORTlineEdit = new QLineEdit(NETWOEKgroupBox);
        HOSTPORTlineEdit->setObjectName(QString::fromUtf8("HOSTPORTlineEdit"));

        x0 = 130*width_min/631;
        y0 = 145*weight_min/381;
        HOSTPORTlineEdit->setGeometry(QRect(x0, y0, 491*width_min/631, 41*weight_min/381));
        USERNAMElabel = new QLabel(NETWOEKgroupBox);
        USERNAMElabel->setObjectName(QString::fromUtf8("USERNAMElabel"));

        x0 = 10*width_min/631;
        y0 = 210*weight_min/381;
        USERNAMElabel->setGeometry(QRect(x0, y0, 111*width_min/631, 31*weight_min/381));
        USERNAMElineEdit = new QLineEdit(NETWOEKgroupBox);
        USERNAMElineEdit->setObjectName(QString::fromUtf8("USERNAMElineEdit"));

        x0 = 130*width_min/631;
        y0 = 205*weight_min/381;
        USERNAMElineEdit->setGeometry(QRect(x0, y0, 491*width_min/631, 41*weight_min/381));
        KEYWORDlabel = new QLabel(NETWOEKgroupBox);
        KEYWORDlabel->setObjectName(QString::fromUtf8("KEYWORDlabel"));

        x0 = 10*width_min/631;
        y0 = 270*weight_min/381;
        KEYWORDlabel->setGeometry(QRect(x0, y0, 111*width_min/631, 31*weight_min/381));
        KEYWORDlineEdit = new QLineEdit(NETWOEKgroupBox);
        KEYWORDlineEdit->setObjectName(QString::fromUtf8("KEYWORDlineEdit"));

        x0 = 130*width_min/631;
        y0 = 265*weight_min/381;
        KEYWORDlineEdit->setGeometry(QRect(x0, y0, 491*width_min/631, 41*weight_min/381));
        CONNECTpushButton = new QPushButton(NETWOEKgroupBox);
        CONNECTpushButton->setObjectName(QString::fromUtf8("CONNECTpushButton"));

        x0 = 530*width_min/631;
        y0 = 320*weight_min/381;
        CONNECTpushButton->setGeometry(QRect(x0, y0, 93*width_min/631, 50*weight_min/381));
        CONNECTpushButton->setMinimumSize(QSize(0, 50*weight_min/381));
        CONNECTpushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        CONNECTlabel = new QLabel(NETWOEKgroupBox);
        CONNECTlabel->setObjectName(QString::fromUtf8("CONNECTlabel"));

        x0 = 10*width_min/631;
        y0 = 330*weight_min/381;
        CONNECTlabel->setGeometry(QRect(x0, y0, 111*width_min/631, 31*weight_min/381));
        CONNECTSTATElabel = new QLabel(NETWOEKgroupBox);
        CONNECTSTATElabel->setObjectName(QString::fromUtf8("CONNECTSTATElabel"));

        x0 = 130*width_min/631;
        y0 = 330*weight_min/381;
        CONNECTSTATElabel->setGeometry(QRect(x0, y0, 72*width_min/631, 31*weight_min/381));

        gridLayout->addWidget(NETWOEKgroupBox, 1, 0, 1, 1);

        CLEARpushButton = new QPushButton(mqtt);
        CLEARpushButton->setObjectName(QString::fromUtf8("CLEARpushButton"));
        CLEARpushButton->setMinimumSize(QSize(0, 50*WINDOWHEIGHT/720));
        CLEARpushButton->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 255);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        gridLayout->addWidget(CLEARpushButton, 3, 1, 1, 1);

        returnpushButton = new QPushButton(mqtt);
        returnpushButton->setObjectName(QString::fromUtf8("returnpushButton"));

        returnpushButton->setMinimumSize(QSize(0, 50*WINDOWHEIGHT/720));
        returnpushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        gridLayout->addWidget(returnpushButton, 3, 6, 1, 1);

        SUBSCRIBEgroupBox = new QGroupBox(mqtt);
        SUBSCRIBEgroupBox->setObjectName(QString::fromUtf8("SUBSCRIBEgroupBox"));

        SUBSCRIBEgroupBox->setMinimumSize(QSize(306*WINDOWWIDTH/1280, 0));
        SUBSCRIBEgroupBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 170, 0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        gridLayout_3 = new QGridLayout(SUBSCRIBEgroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        SUBSCRIBElistWidget = new QListWidget(SUBSCRIBEgroupBox);
        SUBSCRIBElistWidget->setObjectName(QString::fromUtf8("SUBSCRIBElistWidget"));
        SUBSCRIBElistWidget->setMinimumSize(QSize(286*WINDOWWIDTH/1280, 0));

        gridLayout_3->addWidget(SUBSCRIBElistWidget, 0, 0, 1, 1);


        gridLayout->addWidget(SUBSCRIBEgroupBox, 1, 1, 1, 4);

        groupBox = new QGroupBox(mqtt);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 255);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        textBrowser = new QTextBrowser(groupBox);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(textBrowser, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox, 2, 1, 1, 7);

        SENDMSGpushButton = new QPushButton(mqtt);
        SENDMSGpushButton->setObjectName(QString::fromUtf8("SENDMSGpushButton"));
        SENDMSGpushButton->setMinimumSize(QSize(0, 50*WINDOWHEIGHT/720));
        SENDMSGpushButton->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 255);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        gridLayout->addWidget(SENDMSGpushButton, 3, 2, 1, 1);

        confirmpushButton = new QPushButton(mqtt);
        confirmpushButton->setObjectName(QString::fromUtf8("confirmpushButton"));
        confirmpushButton->setMinimumSize(QSize(0, 50*WINDOWHEIGHT/720));
        confirmpushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        gridLayout->addWidget(confirmpushButton, 3, 7, 1, 1);

        checkBox = new QCheckBox(mqtt);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setMinimumSize(QSize(0, 50*WINDOWHEIGHT/720));
        checkBox->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 255);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        gridLayout->addWidget(checkBox, 3, 3, 1, 1);

        groupBox_2 = new QGroupBox(mqtt);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(306*WINDOWWIDTH/1280, 0));
        groupBox_2->setStyleSheet(QString::fromUtf8("color: rgb(85, 170, 255);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        PUBLISHlistWidget = new QListWidget(groupBox_2);
        PUBLISHlistWidget->setObjectName(QString::fromUtf8("PUBLISHlistWidget"));
        PUBLISHlistWidget->setMinimumSize(QSize(286*WINDOWWIDTH/1280, 0));

        gridLayout_4->addWidget(PUBLISHlistWidget, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 1, 5, 1, 3);

        DEBUGlineEdit = new QLineEdit(mqtt);
        DEBUGlineEdit->setObjectName(QString::fromUtf8("DEBUGlineEdit"));
        DEBUGlineEdit->setMinimumSize(QSize(0, 50*WINDOWHEIGHT/720));
        DEBUGlineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);"));

        gridLayout->addWidget(DEBUGlineEdit, 3, 5, 1, 1);


        retranslateUi(mqtt);

        QMetaObject::connectSlotsByName(mqtt);
    } // setupUi

    void retranslateUi(QWidget *mqtt)
    {
        mqtt->setWindowTitle(QCoreApplication::translate("mqtt", "Form", nullptr));
        TOPICgroupBox->setTitle(QCoreApplication::translate("mqtt", "\344\270\273\351\242\230\345\217\202\346\225\260", nullptr));
        SUBSCRIBElineEdit->setPlaceholderText(QCoreApplication::translate("mqtt", "fill the topic,you can read msg from this topic.", nullptr));
        SUBSCRIBElabel->setText(QCoreApplication::translate("mqtt", "\350\256\242\351\230\205\344\270\273\351\242\230      \357\274\232", nullptr));
        PUBLISHlabel->setText(QCoreApplication::translate("mqtt", "\345\217\221\345\270\203\344\270\273\351\242\230      \357\274\232", nullptr));
        PUBLISHlineEdit->setPlaceholderText(QCoreApplication::translate("mqtt", "fill the topic,you can send this topic of msg.", nullptr));
        SUBSCRIBEpushButton->setText(QCoreApplication::translate("mqtt", "\350\256\242\351\230\205", nullptr));
        PUBLISHpushButton->setText(QCoreApplication::translate("mqtt", "\345\217\221\345\270\203", nullptr));
        SUBSCRIBEQOSlabel->setText(QCoreApplication::translate("mqtt", "\350\256\242\351\230\205Qos      \357\274\232", nullptr));
        PUBLISHQOSlabel->setText(QCoreApplication::translate("mqtt", "\345\217\221\345\270\203Qos      \357\274\232", nullptr));
        SUBSCRIBEQOSspinBox->setPrefix(QCoreApplication::translate("mqtt", "Qos", nullptr));
        PUBLISHQOSspinBox->setPrefix(QCoreApplication::translate("mqtt", "Qos", nullptr));
        UNSUBSCRIBEpushButton->setText(QCoreApplication::translate("mqtt", "\345\217\226\346\266\210\350\256\242\351\230\205", nullptr));
        UNPUBLISHpushButton->setText(QCoreApplication::translate("mqtt", "\345\217\226\346\266\210\345\217\221\345\270\203", nullptr));
        MQTTTITLElabel->setText(QCoreApplication::translate("mqtt", "<html><head/><body><p align=\"center\">MQTT\345\256\242\346\210\267\347\253\257\345\217\202\346\225\260\350\256\276\347\275\256</p></body></html>", nullptr));
        NETWOEKgroupBox->setTitle(QCoreApplication::translate("mqtt", "\347\275\221\347\273\234\350\277\236\346\216\245\345\217\202\346\225\260", nullptr));
        HOSTNAMElineEdit->setPlaceholderText(QCoreApplication::translate("mqtt", "broker.hivemq.com", nullptr));
        HOSTNAMElabel->setText(QCoreApplication::translate("mqtt", "\346\234\215\345\212\241\345\231\250\345\237\237\345\220\215  \357\274\232", nullptr));
        HOSTIPlabel->setText(QCoreApplication::translate("mqtt", "\346\234\215\345\212\241\345\231\250IP      \357\274\232", nullptr));
        HOSTIPlineEdit->setPlaceholderText(QCoreApplication::translate("mqtt", "192.168.10.62", nullptr));
        HOSTPORTlabel->setText(QCoreApplication::translate("mqtt", "\346\234\215\345\212\241\345\231\250PORT\357\274\232", nullptr));
        HOSTPORTlineEdit->setPlaceholderText(QCoreApplication::translate("mqtt", "1883", nullptr));
        USERNAMElabel->setText(QCoreApplication::translate("mqtt", "\347\224\250\346\210\267\345\220\215         \357\274\232", nullptr));
        USERNAMElineEdit->setPlaceholderText(QCoreApplication::translate("mqtt", "admin", nullptr));
        KEYWORDlabel->setText(QCoreApplication::translate("mqtt", "\345\257\206\347\240\201            \357\274\232", nullptr));
        KEYWORDlineEdit->setPlaceholderText(QCoreApplication::translate("mqtt", "123456", nullptr));
        CONNECTpushButton->setText(QCoreApplication::translate("mqtt", "\345\220\257\345\212\250\350\277\236\346\216\245", nullptr));
        CONNECTlabel->setText(QCoreApplication::translate("mqtt", "\350\277\236\346\216\245\347\212\266\346\200\201     \357\274\232", nullptr));
        CONNECTSTATElabel->setText(QCoreApplication::translate("mqtt", "\346\234\252\350\277\236\346\216\245", nullptr));
        CLEARpushButton->setText(QCoreApplication::translate("mqtt", "\346\270\205\351\231\244\346\266\210\346\201\257", nullptr));
        returnpushButton->setText(QCoreApplication::translate("mqtt", "\350\277\224\345\233\236", nullptr));
        SUBSCRIBEgroupBox->setTitle(QCoreApplication::translate("mqtt", "\345\267\262\350\256\242\351\230\205\345\210\227\350\241\250", nullptr));
        groupBox->setTitle(QCoreApplication::translate("mqtt", "\346\266\210\346\201\257\345\214\272", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("mqtt", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\345\276\256\350\275\257\351\233\205\351\273\221'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">mesage</p></body></html>", nullptr));
        SENDMSGpushButton->setText(QCoreApplication::translate("mqtt", "\345\217\221\345\270\203\346\266\210\346\201\257", nullptr));
        confirmpushButton->setText(QCoreApplication::translate("mqtt", "\347\241\256\350\256\244\344\277\256\346\224\271", nullptr));
        checkBox->setText(QCoreApplication::translate("mqtt", "\345\220\257\347\224\250\350\260\203\345\274\217", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("mqtt", "\345\267\262\345\217\221\345\270\203\345\210\227\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class libmqttclient: public Ui_libmqttclient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MQTT_H
