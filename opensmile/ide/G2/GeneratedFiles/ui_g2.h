/********************************************************************************
** Form generated from reading UI file 'g2.ui'
**
** Created: Thu Jun 25 10:14:20 2015
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_G2_H
#define UI_G2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_G2Class
{
public:
    QAction *actionOpen;
    QWidget *centralWidget;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *G2Class)
    {
        if (G2Class->objectName().isEmpty())
            G2Class->setObjectName(QString::fromUtf8("G2Class"));
        G2Class->resize(600, 400);
        actionOpen = new QAction(G2Class);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        centralWidget = new QWidget(G2Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(360, 120, 75, 23));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(60, 150, 201, 141));
        G2Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(G2Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        G2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(G2Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        G2Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(G2Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        G2Class->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);

        retranslateUi(G2Class);

        QMetaObject::connectSlotsByName(G2Class);
    } // setupUi

    void retranslateUi(QMainWindow *G2Class)
    {
        G2Class->setWindowTitle(QApplication::translate("G2Class", "G2", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("G2Class", "Open", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("G2Class", "PushButton", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("G2Class", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class G2Class: public Ui_G2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_G2_H
