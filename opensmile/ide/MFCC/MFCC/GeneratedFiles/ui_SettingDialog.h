/********************************************************************************
** Form generated from reading UI file 'SettingDialog.ui'
**
** Created: Sat Jun 27 06:16:36 2015
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGDIALOG_H
#define UI_SETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingDialog
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *framer;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *framesize;
    QLineEdit *lineedit_framesize;
    QHBoxLayout *horizontalLayout_2;
    QLabel *framestep;
    QLineEdit *lineedit_framestep;
    QHBoxLayout *horizontalLayout_6;
    QLabel *framemode;
    QLineEdit *lineedit_framemode;
    QHBoxLayout *horizontalLayout_5;
    QLabel *framecenterspecial;
    QLineEdit *lineedit_framecenterspecial;
    QPushButton *config;
    QWidget *Window;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QComboBox *winfunc;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QLineEdit *lineEdit_sigma;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_9;
    QLineEdit *lineEdit_alpha0;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_10;
    QLineEdit *lineEdit_alpha1;
    QWidget *melfreq;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_11;
    QLineEdit *lineEdit_nbands;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_12;
    QLineEdit *lineEdit_lofreq;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_13;
    QLineEdit *lineEdit_hifreq;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_14;
    QLineEdit *lineEdit_inverse;
    QWidget *Mfcc;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_15;
    QLineEdit *firstMfcc;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_16;
    QLineEdit *lastMfcc;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_17;
    QLineEdit *ceplifter;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_18;
    QLineEdit *htkcompatible;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SettingDialog)
    {
        if (SettingDialog->objectName().isEmpty())
            SettingDialog->setObjectName(QString::fromUtf8("SettingDialog"));
        SettingDialog->resize(800, 600);
        centralwidget = new QWidget(SettingDialog);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 50, 431, 371));
        framer = new QWidget();
        framer->setObjectName(QString::fromUtf8("framer"));
        verticalLayoutWidget = new QWidget(framer);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 10, 311, 321));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        framesize = new QLabel(verticalLayoutWidget);
        framesize->setObjectName(QString::fromUtf8("framesize"));

        horizontalLayout->addWidget(framesize);

        lineedit_framesize = new QLineEdit(verticalLayoutWidget);
        lineedit_framesize->setObjectName(QString::fromUtf8("lineedit_framesize"));

        horizontalLayout->addWidget(lineedit_framesize);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        framestep = new QLabel(verticalLayoutWidget);
        framestep->setObjectName(QString::fromUtf8("framestep"));

        horizontalLayout_2->addWidget(framestep);

        lineedit_framestep = new QLineEdit(verticalLayoutWidget);
        lineedit_framestep->setObjectName(QString::fromUtf8("lineedit_framestep"));

        horizontalLayout_2->addWidget(lineedit_framestep);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        framemode = new QLabel(verticalLayoutWidget);
        framemode->setObjectName(QString::fromUtf8("framemode"));

        horizontalLayout_6->addWidget(framemode);

        lineedit_framemode = new QLineEdit(verticalLayoutWidget);
        lineedit_framemode->setObjectName(QString::fromUtf8("lineedit_framemode"));

        horizontalLayout_6->addWidget(lineedit_framemode);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        framecenterspecial = new QLabel(verticalLayoutWidget);
        framecenterspecial->setObjectName(QString::fromUtf8("framecenterspecial"));

        horizontalLayout_5->addWidget(framecenterspecial);

        lineedit_framecenterspecial = new QLineEdit(verticalLayoutWidget);
        lineedit_framecenterspecial->setObjectName(QString::fromUtf8("lineedit_framecenterspecial"));

        horizontalLayout_5->addWidget(lineedit_framecenterspecial);


        verticalLayout->addLayout(horizontalLayout_5);

        config = new QPushButton(framer);
        config->setObjectName(QString::fromUtf8("config"));
        config->setGeometry(QRect(340, 300, 75, 23));
        tabWidget->addTab(framer, QString());
        Window = new QWidget();
        Window->setObjectName(QString::fromUtf8("Window"));
        verticalLayoutWidget_2 = new QWidget(Window);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(30, 20, 311, 321));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_7 = new QLabel(verticalLayoutWidget_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_7->addWidget(label_7);

        winfunc = new QComboBox(verticalLayoutWidget_2);
        winfunc->setObjectName(QString::fromUtf8("winfunc"));
        winfunc->setEditable(true);

        horizontalLayout_7->addWidget(winfunc);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_8 = new QLabel(verticalLayoutWidget_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        lineEdit_sigma = new QLineEdit(verticalLayoutWidget_2);
        lineEdit_sigma->setObjectName(QString::fromUtf8("lineEdit_sigma"));

        horizontalLayout_8->addWidget(lineEdit_sigma);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_9 = new QLabel(verticalLayoutWidget_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_9->addWidget(label_9);

        lineEdit_alpha0 = new QLineEdit(verticalLayoutWidget_2);
        lineEdit_alpha0->setObjectName(QString::fromUtf8("lineEdit_alpha0"));

        horizontalLayout_9->addWidget(lineEdit_alpha0);


        verticalLayout_2->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_10 = new QLabel(verticalLayoutWidget_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_10->addWidget(label_10);

        lineEdit_alpha1 = new QLineEdit(verticalLayoutWidget_2);
        lineEdit_alpha1->setObjectName(QString::fromUtf8("lineEdit_alpha1"));

        horizontalLayout_10->addWidget(lineEdit_alpha1);


        verticalLayout_2->addLayout(horizontalLayout_10);

        tabWidget->addTab(Window, QString());
        melfreq = new QWidget();
        melfreq->setObjectName(QString::fromUtf8("melfreq"));
        verticalLayoutWidget_3 = new QWidget(melfreq);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(30, 10, 311, 321));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_11 = new QLabel(verticalLayoutWidget_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_11->addWidget(label_11);

        lineEdit_nbands = new QLineEdit(verticalLayoutWidget_3);
        lineEdit_nbands->setObjectName(QString::fromUtf8("lineEdit_nbands"));

        horizontalLayout_11->addWidget(lineEdit_nbands);


        verticalLayout_3->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_12 = new QLabel(verticalLayoutWidget_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_12->addWidget(label_12);

        lineEdit_lofreq = new QLineEdit(verticalLayoutWidget_3);
        lineEdit_lofreq->setObjectName(QString::fromUtf8("lineEdit_lofreq"));

        horizontalLayout_12->addWidget(lineEdit_lofreq);


        verticalLayout_3->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_13 = new QLabel(verticalLayoutWidget_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_13->addWidget(label_13);

        lineEdit_hifreq = new QLineEdit(verticalLayoutWidget_3);
        lineEdit_hifreq->setObjectName(QString::fromUtf8("lineEdit_hifreq"));

        horizontalLayout_13->addWidget(lineEdit_hifreq);


        verticalLayout_3->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_14 = new QLabel(verticalLayoutWidget_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_14->addWidget(label_14);

        lineEdit_inverse = new QLineEdit(verticalLayoutWidget_3);
        lineEdit_inverse->setObjectName(QString::fromUtf8("lineEdit_inverse"));

        horizontalLayout_14->addWidget(lineEdit_inverse);


        verticalLayout_3->addLayout(horizontalLayout_14);

        tabWidget->addTab(melfreq, QString());
        Mfcc = new QWidget();
        Mfcc->setObjectName(QString::fromUtf8("Mfcc"));
        verticalLayoutWidget_4 = new QWidget(Mfcc);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(30, 20, 311, 321));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_15 = new QLabel(verticalLayoutWidget_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_15->addWidget(label_15);

        firstMfcc = new QLineEdit(verticalLayoutWidget_4);
        firstMfcc->setObjectName(QString::fromUtf8("firstMfcc"));

        horizontalLayout_15->addWidget(firstMfcc);


        verticalLayout_4->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_16 = new QLabel(verticalLayoutWidget_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_16->addWidget(label_16);

        lastMfcc = new QLineEdit(verticalLayoutWidget_4);
        lastMfcc->setObjectName(QString::fromUtf8("lastMfcc"));

        horizontalLayout_16->addWidget(lastMfcc);


        verticalLayout_4->addLayout(horizontalLayout_16);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        label_17 = new QLabel(verticalLayoutWidget_4);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_17->addWidget(label_17);

        ceplifter = new QLineEdit(verticalLayoutWidget_4);
        ceplifter->setObjectName(QString::fromUtf8("ceplifter"));

        horizontalLayout_17->addWidget(ceplifter);


        verticalLayout_4->addLayout(horizontalLayout_17);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_18 = new QLabel(verticalLayoutWidget_4);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_18->addWidget(label_18);

        htkcompatible = new QLineEdit(verticalLayoutWidget_4);
        htkcompatible->setObjectName(QString::fromUtf8("htkcompatible"));

        horizontalLayout_18->addWidget(htkcompatible);


        verticalLayout_4->addLayout(horizontalLayout_18);

        tabWidget->addTab(Mfcc, QString());
        SettingDialog->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(SettingDialog);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        SettingDialog->setStatusBar(statusbar);

        retranslateUi(SettingDialog);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(SettingDialog);
    } // setupUi

    void retranslateUi(QMainWindow *SettingDialog)
    {
        SettingDialog->setWindowTitle(QApplication::translate("SettingDialog", "MainWindow", 0, QApplication::UnicodeUTF8));
        framesize->setText(QApplication::translate("SettingDialog", "FframeSize", 0, QApplication::UnicodeUTF8));
        framestep->setText(QApplication::translate("SettingDialog", "FframeStep", 0, QApplication::UnicodeUTF8));
        framemode->setText(QApplication::translate("SettingDialog", "FframeMode", 0, QApplication::UnicodeUTF8));
        framecenterspecial->setText(QApplication::translate("SettingDialog", "FframeCenterSpecial", 0, QApplication::UnicodeUTF8));
        config->setText(QApplication::translate("SettingDialog", "creat config", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(framer), QApplication::translate("SettingDialog", "Framer", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("SettingDialog", "winFunc", 0, QApplication::UnicodeUTF8));
        winfunc->clear();
        winfunc->insertItems(0, QStringList()
         << QApplication::translate("SettingDialog", "Han", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "Ham", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "Rec", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "Gau", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "Sin", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "Tri", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "Bar", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "BaH", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "Bla", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "BlH", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingDialog", "Lac", 0, QApplication::UnicodeUTF8)
        );
        label_8->setText(QApplication::translate("SettingDialog", "sigma", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("SettingDialog", "alpha0", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("SettingDialog", "alpha1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Window), QApplication::translate("SettingDialog", "window", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("SettingDialog", "nBands", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("SettingDialog", "lowfreq", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("SettingDialog", "hifreq", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("SettingDialog", "inverse", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(melfreq), QApplication::translate("SettingDialog", "Melfreq", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("SettingDialog", "firstMfcc", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("SettingDialog", "lastMfcc", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("SettingDialog", "cepLifter", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("SettingDialog", "htkcompatible", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Mfcc), QApplication::translate("SettingDialog", "MFCC", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingDialog: public Ui_SettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGDIALOG_H
