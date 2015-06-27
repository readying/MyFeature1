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

waveSource : reads PCM WAVE files (RIFF format)

Support for a negative start index was added by Benedikt Gollan (TUM).

*/


#ifndef __WAVE_SOURCE_HPP
#define __WAVE_SOURCE_HPP

#include <core/smileCommon.hpp>
#include <core/dataSource.hpp>

#define COMPONENT_DESCRIPTION_CWAVESOURCE "This component reads an uncompressed RIFF (PCM-WAVE) file and saves it as a stream to the data memory. For most feature extraction tasks you will now require a cFramer component."
#define COMPONENT_NAME_CWAVESOURCE "cWaveSource"

/*
#define BYTEORDER_LE    0
#define BYTEORDER_BE    1
#define MEMORGA_INTERLV 0
#define MEMORGA_SEPCH   1

typedef struct {
  long sampleRate;
  int sampleType;
  int nChan;
  int blockSize;
  int nBPS;       // actual bytes per sample
  int nBits;       // bits per sample (precision)
  int byteOrder;  // BYTEORDER_LE or BYTEORDER_BE
  int memOrga;    // MEMORGA_INTERLV  or MEMORGA_SEPCH
  long nBlocks;  // nBlocks in buffer
} sWaveParameters;
*/

#undef class
class DLLEXPORT cWaveSource : public cDataSource {
  private:
    const char *filename;
    //long buffersize;
    FILE *filehandle;
    sWaveParameters pcmParam;

    int negativestart;
    long negstartoffset;
    double start, end, endrel;
    long startSamples, endSamples, endrelSamples;
    
    int monoMixdown;    // if set to 1, multi-channel files will be mixed down to 1 channel
    long pcmDataBegin;  // in bytes
    long curReadPos;   // in samples
    int eof;

	const char *outFieldName;


    int readWaveHeader();
    int readData(cMatrix *m=NULL);
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    virtual int myConfigureInstance();
    //virtual int myFinaliseInstance();
    virtual int myTick(long long t);

    virtual int configureWriter(sDmLevelConfig &c);
    virtual int setupNewNames(long nEl);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cWaveSource(const char *_name);

    virtual ~cWaveSource();
};




#endif // __EXAMPLE_SOURCE_HPP
