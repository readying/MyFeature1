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

LPC, compute LPC coefficients from wave data (PCM) frames

*/


#ifndef __CLPC_HPP
#define __CLPC_HPP


#include <core/smileCommon.hpp>
#include <core/vectorProcessor.hpp>

#include <dspcore/fftXg.h>

#define COMPONENT_DESCRIPTION_CLPC "This component computes linear predictive coding (LPC) coefficients from PCM frames. Burg's algorithm and the standard ACF/Durbin based method are implemented for LPC coefficient computation. The output of LPC filter coefficients, reflection coefficients, residual signal, and LP spectrum is supported."

#define COMPONENT_NAME_CLPC "cLpc"

#define LPC_METHOD_ACF   0
//#define LPC_METHOD_ACF2   1
#define LPC_METHOD_BURG   5


#undef class
class DLLEXPORT cLpc : public cVectorProcessor {
  private:
    int p;
    int saveLPCoeff, saveRefCoeff;
    int residual;
    int method;
    int lpGain;
    int forwardRes;
    int lpSpectrum;

    double lpSpecDeltaF;
    int lpSpecBins;

    FLOAT_DMEM *latB;
    FLOAT_DMEM lastGain;
    FLOAT_TYPE_FFT *lSpec;

    int *_ip;
    FLOAT_TYPE_FFT *_w;

    FLOAT_DMEM *acf;
    FLOAT_DMEM *lpCoeff, *lastLpCoeff, *refCoeff;

    FLOAT_DMEM *burgB1, *burgB2, *burgAA;

   // int calcLpcBurg (FLOAT_DMEM x[], long n, FLOAT_DMEM a[], int m, FLOAT_DMEM *xms);

    // calcLpc returns the lpc gain (error?)
    //FLOAT_DMEM calcLpcAcf(FLOAT_DMEM * acf, int _p, FLOAT_DMEM *lpc, FLOAT_DMEM *refl=NULL);
    FLOAT_DMEM calcLpc(const FLOAT_DMEM *x, long Nsrc, FLOAT_DMEM * lpc, long nCoeff, FLOAT_DMEM *refl);

    //void autoCorrF(const FLOAT_DMEM *x, FLOAT_DMEM *acf, int n, int lag);

  protected:
    SMILECOMPONENT_STATIC_DECL_PR

    virtual void fetchConfig();
    virtual int setupNamesForField(int i, const char*name, long nEl);
    //virtual int myFinaliseInstance();

    virtual int processVectorInt(const INT_DMEM *src, INT_DMEM *dst, long Nsrc, long Ndst, int idxi);
    virtual int processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cLpc(const char *_name);

    virtual ~cLpc();
};


#endif // __CLPC_HPP
