#include "componentType.h"
#include <string>
using namespace std;
/////wavesource///
QString WaveSource::wavesourceprint(){
	QString out="[waveSource:cWaveSource]\n writer.dmLevel=wave\n";
	return out;
}

///////////Framer///////
Framer::Framer(QString framesize, QString framestep, QString framemode,QString framecenterspecial){
	FramerSize = 0.025;
	FramerStep=0.01;
	FramerMode="fixed";
	FramerCenterSpecial="Left";
}
QString Framer::FrameHead(QString name){
	QString os="instance["+name+"].type=cFramer\n";
	return os;
}

QString Framer::Framerprint(QString name,QString framesize, QString framestep, QString framemode,QString framecenterspecial){
	QString out="["+name+":cFramer]\n"
     +"reader.dmLevel=wave\n writer.dmLevel=waveframe"
	 +"frameSize="+framesize+"\n"
	 +"frameStep="+framestep+"\n"
	 +"frameMode="+framemode+"\n"
	 +"frameCenterSpecial="+framecenterspecial+"\n";
	return out;
}

////////////windower////////////

Windower::Windower(){
	winfunc = "Han";
	sigma="0.4";
	alpha0="0";
	alpha1="0";
}
QString Windower::WindowerHead(QString name){
	
	QString os="instance["+name+"].type=cWindower\n";
	return os;
}

QString Windower::WindowerPrint(QString name,QString winfunc, QString sigma, QString alpha0,QString alpha1){
	QString out="["+name+":cWindower]\n"+
		"reader.dmLevel=waveframe\n writer.dmLevel=window"+
		"frameSize="+winfunc+"\n"
		+"sigma="+sigma+"\n"
		+"alpha0="+alpha0+"\n"
		+"alpha1="+alpha1+"\n";
	return out;
}

//////melspec////
QString Melspec::melspechead(QString name){
	QString os ="instance["+name+"].type=cMelspec\n";
	return os;
}
QString Melspec::melspecprint(QString name,QString nbands,QString lofreq,QString hifreq,QString inverse){
	QString out="["+name+":cMelspec]\n"+
		"reader.dmLevel=window\n writer.dmLevel=melspec"+
		"nBands="+nbands+"\n"
		+"lofreq="+lofreq+"\n"
		+"hifreq="+hifreq+"\n"
		+"inverse="+inverse+"\n";
	return out;
}

//////mfcc/////////
QString Mfcc::mfcchead(QString name){
	QString os="instance["+name+"].type=cMfcc\n";
	return os;
}
QString Mfcc::mfccprint(QString name,QString firstmfcc,QString lastmfcc,QString ceplifter,QString htkcompatible){
	QString out="["+name+":cMfcc]\n"+
		"reader.dmLevel=melspec\n writer.dmLevel=mfcc"+
		"firstMfcc="+firstmfcc+"\n"
		+"lastMfcc="+lastmfcc+"\n"
		+"cepLifter="+ceplifter+"\n"
		+"htkcompatible="+htkcompatible+"\n";
	return out;
}

/////csv///
QString CsvSink::csvsinkprint(QString name,QString filename){
	QString out="[csvsink:cCsvSink]\n reader.dmLevel="+name+"\n"+
		"filename="+filename+".csv";
		return out;
}