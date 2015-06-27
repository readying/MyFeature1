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

computes (or rather estimates) semi-tone spectrum from fft spectrum

*/


#ifndef __CTONESPEC_HPP
#define __CTONESPEC_HPP

#include <core/smileCommon.hpp>
#include <core/vectorProcessor.hpp>
#include <math.h>

#define COMPONENT_DESCRIPTION_CTONESPEC "This component computes (or rather estimates) a semi-tone spectrum from an FFT magnitude spectrum."
#define COMPONENT_NAME_CTONESPEC "cTonespec"

#undef class
class DLLEXPORT cTonespec : public cVectorProcessor {
  private:
    int nOctaves, nNotes;
    int usePower, dbA;
  #ifdef DEBUG
    int printBinMap, printFilterMap;
  #endif
  
    FLOAT_DMEM firstNote;  // frequency of first note
    FLOAT_DMEM lastNote;   // frequency of last note (will be computed)

    FLOAT_DMEM **pitchClassFreq;
    FLOAT_DMEM **distance2key;
    FLOAT_DMEM **filterMap;
    FLOAT_DMEM **db;
    
    int **binKey;
    int **pitchClassNbins;
    int **flBin;
    
    int filterType;

    void computeFilters( long blocksize, double frameSizeSec, int idxc );
    void setPitchclassFreq( int idxc );
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR

    virtual void fetchConfig();
    //virtual int myConfigureInstance();
    //virtual int myFinaliseInstance();
    //virtual int myTick(long long t);

    //virtual int configureWriter(const sDmLevelConfig *c);
    virtual int dataProcessorCustomFinalise();
    virtual int setupNamesForField(int i, const char*name, long nEl);
    //virtual int processVectorInt(const INT_DMEM *src, INT_DMEM *dst, long Nsrc, long Ndst, int idxi);
    virtual int processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cTonespec(const char *_name);

    virtual ~cTonespec();
};




#endif // __CTONESPEC_HPP
