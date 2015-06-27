/*F***************************************************************************
 * openSMILE - the Munich open source Multimedia Interpretation by Large-scale
 * Extraction toolkit
 * 
 * (c) 2008-2013, Florian Eyben, Felix Weninger, Florian Gross, Martin
 * Woellmer, Bjoern Schuller
 * 
 * Technische Universität München (TUM), 80333 Munich, Germany
 * 
 * openSMILE is distributed under a research only license. 
 * 
 * You may use, copy, reproduce, and distribute this Software for any
 * non-commercial purpose. Some purposes which can be non-commercial are
 * teaching, academic research, public demonstrations and personal
 * experimentation. You may not use or distribute this Software or any
 * derivative works in any form for commercial purposes. 
 * 
 * See LICENSE.txt in the top level source directory for details.
 * 
 * THIS SOFTWARE COMES "AS IS", WITH NO WARRANTIES. THIS MEANS NO EXPRESS,
 * IMPLIED OR STATUTORY WARRANTY, INCLUDING WITHOUT LIMITATION, WARRANTIES OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ANY WARRANTY AGAINST
 * INTERFERENCE WITH YOUR ENJOYMENT OF THE SOFTWARE OR ANY WARRANTY OF TITLE
 * OR NON-INFRINGEMENT. THERE IS NO WARRANTY THAT THIS SOFTWARE WILL FULFILL
 * ANY OF YOUR PARTICULAR PURPOSES OR NEEDS. ALSO, YOU MUST PASS THIS
 * DISCLAIMER ON WHENEVER YOU DISTRIBUTE THE SOFTWARE OR DERIVATIVE WORKS.
 * NEITHER TUM NOR ANY CONTRIBUTOR TO THE SOFTWARE WILL BE LIABLE FOR ANY
 * DAMAGES RELATED TO THE SOFTWARE OR THIS LICENSE AGREEMENT, INCLUDING
 * DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL OR INCIDENTAL DAMAGES, TO THE
 * MAXIMUM EXTENT THE LAW PERMITS, NO MATTER WHAT LEGAL THEORY IT IS BASED ON.
 * ALSO, YOU MUST PASS THIS LIMITATION OF LIABILITY ON WHENEVER YOU DISTRIBUTE
 * THE SOFTWARE OR DERIVATIVE WORKS.
 ***************************************************************************E*/


/*  openSMILE component:

waveSink:
writes data to an uncompressed PCM WAVE file

*/


#ifndef __CWAVESINKCUT_HPP
#define __CWAVESINKCUT_HPP

#include <core/smileCommon.hpp>
#include <core/dataSink.hpp>
#include <iocore/waveSource.hpp>

#define COMPONENT_DESCRIPTION_CWAVESINKCUT "This component writes data to uncompressed PCM WAVE files. Only chunks, based on timings received via smile messages are written to files. The files may have consecutive numbers appended to the file name."
#define COMPONENT_NAME_CWAVESINKCUT "cWaveSinkCut"

#undef class
class DLLEXPORT cWaveSinkCut : public cDataSink {
  private:
    const char *fileExtension;
    const char *filebase;
    const char *fileNameFormatString;
    int multiOut;
    long curFileNr;
    
    long forceSampleRate;
    long preSil, postSil;
    int turnStart, turnEnd, nPre, nPost, vIdxStart, vIdxEnd, curVidx;
    long curStart, curEnd;
    int isTurn, endWait;

    FILE * fHandle;
    void *sampleBuffer; long sampleBufferLen;

  	int nBitsPerSample;
	  int nBytesPerSample;
	  int sampleFormat;
    int nChannels;
    long fieldSize;
	
    long nOvl;

  	long curWritePos;   // in samples??
	  long nBlocks;

    int writeWaveHeader();
    int writeDataFrame(cVector *m=NULL);
    char * getCurFileName();
    int showSegmentTimes_;

  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);
    virtual int processComponentMessage( cComponentMessage *_msg );

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cWaveSinkCut(const char *_name);

    virtual ~cWaveSinkCut();
};




#endif // __CWAVESINKCUT_HPP
