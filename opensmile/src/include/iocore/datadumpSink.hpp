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

ARFF file output (for WEKA)

*/


#ifndef __CDATADUMPSINK_HPP
#define __CDATADUMPSINK_HPP

#include <core/smileCommon.hpp>
#include <core/dataSink.hpp>

#define COMPONENT_DESCRIPTION_CDATADUMPSINK "This component writes dataMemory data to a raw binary file (e.g. for matlab import). The binary file consits of 32-bit float values representing the data values, concattenated frame by frame along the time axis. The first two float values in the file resemble the file header, an thus indicate the dimension of the matrix (1: size of frames, 2: number of frames in file). The total file size in bytes is thus <size of frames>x<number of frames>x4 + 2."
#define COMPONENT_NAME_CDATADUMPSINK "cDatadumpSink"

#undef class
class DLLEXPORT cDatadumpSink : public cDataSink {
  private:
    FILE * filehandle;
    const char *filename;
    int lag;
    int append;
    long nVec,vecSize;
    
    void writeHeader();
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    //virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);


  public:
    //static sComponentInfo * registerComponent(cConfigManager *_confman);
    //static cSmileComponent * create(const char *_instname);
    SMILECOMPONENT_STATIC_DECL
    
    cDatadumpSink(const char *_name);

    virtual ~cDatadumpSink();
};




#endif // __CDATADUMPSINK_HPP
