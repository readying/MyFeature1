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


/*  openSMILE component: cHtkSource
-----------------------------------

HTK Source:

Reads data from an HTK parameter file.

-----------------------------------

11/2009 - Written by Florian Eyben
*/


#ifndef __CHTKSOURCE_HPP
#define __CHTKSOURCE_HPP

#include <core/smileCommon.hpp>
#include <core/dataSource.hpp>
//#include <htkSink.hpp>   // include this for the HTK header type and endianness functions

#define COMPONENT_DESCRIPTION_CHTKSOURCE "This component reads data from binary HTK parameter files."
#define COMPONENT_NAME_CHTKSOURCE "cHtkSource"

class cHtkSource : public cDataSource {
  private:
    sHTKheader head;
    int vax;
    const char * featureName;
    int N; // <-- sampleSize
    float *tmpvec;

    FILE *filehandle;
    const char *filename;
    int eof;
 
    void prepareHeader( sHTKheader *h )
    {
      smileHtk_prepareHeader(h);
    }

    int readHeader() 
    {
      return smileHtk_readHeader(filehandle,&head);
    }

  protected:
    SMILECOMPONENT_STATIC_DECL_PR


    virtual void fetchConfig();
    virtual int configureWriter(sDmLevelConfig &c);
    virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);

    virtual int setupNewNames(long nEl=0);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cHtkSource(const char *_name);

    virtual ~cHtkSource();
};




#endif // __CHTKSOURCE_HPP