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


#ifndef __CHTKSINK_HPP
#define __CHTKSINK_HPP

#include <core/smileCommon.hpp>
#include <core/smileComponent.hpp>
#include <core/dataSink.hpp>

#define COMPONENT_DESCRIPTION_CHTKSINK "This component writes dataMemory data to a binary HTK parameter file."
#define COMPONENT_NAME_CHTKSINK "cHtkSink"


#undef class
class DLLEXPORT cHtkSink : public cDataSink {
  private:
    FILE * filehandle;
    int vax;
    const char *filename;
    int lag;
    int append;
    uint16_t parmKind;
    uint32_t vecSize;
    uint32_t nVec;
    double period;
    sHTKheader header;

    void prepareHeader( sHTKheader *h )
    {
      smileHtk_prepareHeader(h);
    }

    int writeHeader();
    
    int readHeader() 
    {
      return smileHtk_readHeader(filehandle,&header);
    }
    
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
    
    cHtkSink(const char *_name);

    virtual ~cHtkSink();
};




#endif // __CHTKSINK_HPP
