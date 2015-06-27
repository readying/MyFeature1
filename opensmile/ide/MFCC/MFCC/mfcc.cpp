#include "mfcc.h"
#include <QFileDialog>
#include <QMessageBox>
#include "ui_mfcc.h"

MFCC::MFCC(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	//openfile

	connect(ui.open,SIGNAL(triggered()),this,SLOT(openFile()));
	//parameters

	connect(ui.parameters,SIGNAL(clicked()),this,SLOT(setting()));
	//run

	//connect(ui.run,SIGNAL(clicked()),this,SLOT(run()));



}


void MFCC::openFile()
{
		QString fileName = QFileDialog::getOpenFileName(this,tr("Open signals"),".",tr("Wave files(*.wav)"));
		if(fileName.length() == 0) { 
			QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files.")); 
		} else { 
			QMessageBox::information(NULL, tr("Path"), tr("You selected ")); 
		} 
}


void MFCC::setting(){

SettingDialog *childwindow = new SettingDialog(this);
childwindow->show();

}

#if 0
void MFCC::run()
{
	// checkbox
	if(ui.MFCC->isChecked()){
		emit extractfeature();}     /////define extractfeature
}
#endif




MFCC::~MFCC()
{

}
