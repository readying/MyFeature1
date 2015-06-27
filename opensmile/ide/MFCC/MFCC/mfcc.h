#ifndef MFCC_H
#define MFCC_H

#include <QtGui/QMainWindow>
#include "ui_mfcc.h"
#include "SettingDialog.h"

class MFCC : public QMainWindow
{
	Q_OBJECT

public:
	MFCC(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MFCC();
	

public slots:
	void openFile();
	void setting();
	//void run();
	
private:
	Ui::MFCCClass ui;

};

#endif // MFCC_H
