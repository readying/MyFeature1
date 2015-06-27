/********************************************************************************
** Form generated from reading UI file 'mfcc.ui'
**
** Created: Sat Jun 27 06:16:36 2015
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MFCC_H
#define UI_MFCC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MFCCClass
{
public:
    QAction *open;
    QWidget *centralWidget;
    QCheckBox *MFCC;
    QPushButton *parameters;
    QPushButton *run;
    QMenuBar *menuBar;
    QMenu *File;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MFCCClass)
    {
        if (MFCCClass->objectName().isEmpty())
            MFCCClass->setObjectName(QString::fromUtf8("MFCCClass"));
        MFCCClass->resize(600, 400);
        open = new QAction(MFCCClass);
        open->setObjectName(QString::fromUtf8("open"));
        centralWidget = new QWidget(MFCCClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MFCC = new QCheckBox(centralWidget);
        MFCC->setObjectName(QString::fromUtf8("MFCC"));
        MFCC->setGeometry(QRect(50, 80, 71, 16));
        parameters = new QPushButton(centralWidget);
        parameters->setObjectName(QString::fromUtf8("parameters"));
        parameters->setGeometry(QRect(130, 80, 75, 23));
        run = new QPushButton(centralWidget);
        run->setObjectName(QString::fromUtf8("run"));
        run->setGeometry(QRect(240, 80, 75, 23));
        MFCCClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MFCCClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        File = new QMenu(menuBar);
        File->setObjectName(QString::fromUtf8("File"));
        MFCCClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MFCCClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MFCCClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MFCCClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MFCCClass->setStatusBar(statusBar);

        menuBar->addAction(File->menuAction());
        File->addAction(open);

        retranslateUi(MFCCClass);

        QMetaObject::connectSlotsByName(MFCCClass);
    } // setupUi

    void retranslateUi(QMainWindow *MFCCClass)
    {
        MFCCClass->setWindowTitle(QApplication::translate("MFCCClass", "MFCC", 0, QApplication::UnicodeUTF8));
        open->setText(QApplication::translate("MFCCClass", "open", 0, QApplication::UnicodeUTF8));
        MFCC->setText(QApplication::translate("MFCCClass", "MFCC", 0, QApplication::UnicodeUTF8));
        parameters->setText(QApplication::translate("MFCCClass", "parameters", 0, QApplication::UnicodeUTF8));
        run->setText(QApplication::translate("MFCCClass", "run", 0, QApplication::UnicodeUTF8));
        File->setTitle(QApplication::translate("MFCCClass", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MFCCClass: public Ui_MFCCClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MFCC_H
