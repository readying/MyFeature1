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

portAudio dataSink for live audio playback
(?? known to work on windows, linux, and mac)

*/


#ifndef __CPORTAUDIODUPLEX_HPP
#define __CPORTAUDIODUPLEX_HPP

#include <core/smileCommon.hpp>
#include <core/dataProcessor.hpp>

#ifdef HAVE_PORTAUDIO

#include <portaudio.h>

#define COMPONENT_DESCRIPTION_CPORTAUDIODUPLEX "dataProcessor for full-duplex playback and recording of live audio using PortAudio library"
#define COMPONENT_NAME_CPORTAUDIODUPLEX "cPortaudioDuplex"


#define PA_STREAM_STOPPED 0
#define PA_STREAM_STARTED 1

#undef class
class DLLEXPORT cPortaudioDuplex : public cDataProcessor {
  private:
    PaStream *stream;
    long paFrames;
    int deviceId;
    int streamStatus;
    int listDevices;
    int numDevices;
    int lastDataCount;
    int isPaInit;
    
    long audioBuffersize;
    double audioBuffersize_sec;
    //long mBuffersize;    

    int monoMixdownPB, monoMixdownREC;    // if set to 1, multi-channel files will be mixed down to 1 channel

    int eof, abort; // NOTE : when setting abort, first lock the callbackMtx!!!
    int channels, sampleRate, nBits, nBPS;
    
    //int setupDevice();

  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);


    virtual int configureWriter(sDmLevelConfig &c);
    virtual int setupNewNames(long nEl);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    // variables for the callback method:
    //smileMutex dataFlagMtx;
    smileMutex callbackMtx;
    smileCond  callbackCond;
    int dataFlagR, dataFlagW;
    cMatrix *callbackMatrix;
    
    cPortaudioDuplex(const char *_name);

    void printDeviceList();

    int startDuplex();
    int stopDuplex();
    int stopDuplexWait();
    
    cDataReader * getReader() { return reader_; }
    cDataWriter * getWriter() { return writer_; }

    /*void setReadDataFlag() {
      smileMutexLock(dataFlagMtx);
      dataFlag = 1;
      //lastDataCount=0;
      smileMutexUnlock(dataFlagMtx);
    }*/

    int getNBPS() { return nBPS; }
    int getNBits() { return nBits; }
    int getChannels() { return channels; }
    int getSampleRate() { return sampleRate; }
    
    int isAbort() { return abort; }
    int isMonoMixdownPB() { return monoMixdownPB; }
    int isMonoMixdownREC() { return monoMixdownREC; }
    
    virtual ~cPortaudioDuplex();
};


#endif // HAVE_PORTAUDIO


#endif // __CPORTAUDIODUPLEX_HPP
