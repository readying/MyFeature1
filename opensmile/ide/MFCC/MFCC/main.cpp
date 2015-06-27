#include "mfcc.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MFCC w;
	w.show();
	return a.exec();
}
