#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

#include <QtGui/QMainWindow>
#include "ui_SettingDialog.h"
#include <string>
#include <fstream>
#include <QTextStream>

using namespace std;
class WaveSource{
public:
	QString wavesourceprint();
	
};
 
class Framer{
	QString FramerSize;
	QString FramerStep;
	QString FramerMode;
	QString FramerCenterSpecial;
	QString name;
public:
	Framer(QString,QString,QString,QString);
	QString FrameHead(QString name);
	QString Framerprint(QString,QString framesize, QString framestep, QString framemode,QString framecenterspecial);

};

class Windower{
	QString winfunc;
	QString sigma;
	QString alpha0;
	QString alpha1;
	QString name;
public:
	Windower();
	QString WindowerHead(QString name);
	QString WindowerPrint(QString,QString,QString,QString,QString);


};
 
class Melspec{
public:
	QString melspechead(QString);
	QString melspecprint(QString,QString,QString,QString,QString);
};
class Mfcc{
public:
	QString mfcchead(QString);
	QString mfccprint(QString,QString,QString,QString,QString);
};
class CsvSink{
public:
	QString csvsinkprint(QString,QString);
};

#endif