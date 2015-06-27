#include "SettingDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "componentType.h"

using namespace std;

SettingDialog::SettingDialog(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	parameters.setupUi(this);
	connect(parameters.config,SIGNAL(clicked()),this,SLOT(configfile()));


}
SettingDialog::~SettingDialog()
{

}




///////Configure File///////


void SettingDialog::configfile(){
	///INPUT
	QString framesize=parameters.lineedit_framesize->text();
	QString framestep=parameters.lineedit_framestep->text();
	QString framemode=parameters.lineedit_framemode->text();
	QString framecenterspecial=parameters.lineedit_framecenterspecial->text();

	QString winfun=parameters.winfunc->currentText();
	QString sigma=parameters.lineEdit_sigma->text();
	QString alpha0=parameters.lineEdit_alpha0->text();
	QString alpha1=parameters.lineEdit_alpha1->text();
	
	QString nbands=parameters.lineEdit_nbands->text();
	QString lofreq=parameters.lineEdit_lofreq->text();
	QString hifreq=parameters.lineEdit_hifreq->text();
	QString inverse=parameters.lineEdit_inverse->text();

	QString firstmfcc=parameters.firstMfcc->text();
	QString lastmfcc=parameters.lastMfcc->text();
	QString ceplifter=parameters.ceplifter->text();
	QString htkcompatible=parameters.htkcompatible->text();


	

	////creat manager configuration////
	Framer frame(framesize,framestep,framemode,framecenterspecial);
	Windower windower;
	WaveSource wavesource;
	Melspec melspec;
	Mfcc mfcc;
	CsvSink csvsink;


	QString allmanager="instance[dataMemory].type = cDataMemory\ninstance[waveSource].type = cWaveSource\n"
						+frame.FrameHead("frame")+windower.WindowerHead("window")
						+melspec.melspechead("melspec")+mfcc.mfcchead("mfcc")
						+"instance[csvSink].type = cCsvSink\n";

	////creat configuration sections////
	QString allsection="/////the wavesource section////\n"+wavesource.wavesourceprint()+
		"////the framer section////\n"+frame.Framerprint("frame",framesize,framestep,framemode,framecenterspecial)
		+"////the windower section////\n"+windower.WindowerPrint("window",winfun,sigma,alpha0,alpha1)
		+"////the melfreq section/////\n"+melspec.melspecprint("melspec",nbands,lofreq,hifreq,inverse)
		+"////the mfcc section////////\n"+mfcc.mfccprint("mfcc",framesize,lastmfcc,ceplifter,htkcompatible)
		+"////the csvsink section/////\n"+csvsink.csvsinkprint("mfcc","mfcc");


	//////////creat config file//////////
	QFile file("mfcc.conf");
	if(!file.open(QIODevice::WriteOnly )  )
	{  std::cout<<"Cannot open file for writing:"<<qPrintable(file.errorString())<<endl;
		
		return;   
	}
	QTextStream out(&file);  
	out <<" ;===== component manager configuration (= list of enabled components!) =====\n"<<endl
		<<"[componentInstances:cComponentManager]"<<endl
		<<" // this line configures the default data memory:"<<endl
	    <<allmanager<<endl<<endl<<endl<<endl
		<<"// ============= component configuration sections begin here ==============\n"
		<<allsection<<endl;

	
}


	
	