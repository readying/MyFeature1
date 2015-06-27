#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QtGui/QMainWindow>
#include "ui_SettingDialog.h"
#include <iostream>

class SettingDialog : public QMainWindow
{
	Q_OBJECT

public:
	SettingDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SettingDialog();


public slots:
	void configfile();
private:
	Ui::SettingDialog parameters;
	

};

#endif 