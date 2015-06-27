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


/*  openSMILE component: dbA

applies dbX weighting to fft magnitudes

*/


#ifndef __CDBA_HPP
#define __CDBA_HPP

#include <core/smileCommon.hpp>
#include <core/vectorProcessor.hpp>
#include <math.h>

#define COMPONENT_DESCRIPTION_CDBA "This component performs dbX (dbA,dbB,dbC,...) equal loudness weighting of FFT bin magnitudes. Currently only dbA weighting is implemented."
#define COMPONENT_NAME_CDBA "cDbA"

#define CURVE_DBA 0
#define CURVE_DBB 1
#define CURVE_DBC 2

extern void computeDBA(FLOAT_DMEM *x, long blocksize, FLOAT_DMEM F0);

#undef class
class DLLEXPORT cDbA : public cVectorProcessor {
  private:
    int curve;
    int usePower;
    FLOAT_DMEM **filterCoeffs;
    
    int computeFilters( long blocksize, double frameSizeSec, int idxc );

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
    
    cDbA(const char *_name);

    virtual ~cDbA();
};




#endif // __CDBA_HPP
