#ifndef G2_H
#define G2_H

#define __WINDOWS

#include <QtGui/QMainWindow>
#include "ui_g2.h"
#include <QFileDialog>



#include <core/smileCommon.hpp>
#include <core/configManager.hpp>
#include <core/commandlineParser.hpp>
#include <core/componentManager.hpp>

#define MODULE "SMILExtract"


/************** Ctrl+C signal handler **/
#include  <signal.h>



class G2 : public QMainWindow
{
	Q_OBJECT

public:
	G2(QWidget *parent = 0, Qt::WFlags flags = 0);
	~G2();

	public slots:
		// 提取特征
		 int SmileExtract();

		 // 打开wav文件
		 void OpenWav();

private:
	Ui::G2Class ui;
	
	QString wavPath;
};

#endif // G2_H
