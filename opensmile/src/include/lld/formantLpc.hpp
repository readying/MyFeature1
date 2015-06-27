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


#ifndef __CFORMANTLPC_HPP
#define __CFORMANTLPC_HPP

#include <core/smileCommon.hpp>
#include <core/vectorProcessor.hpp>

#ifdef BUILD_WITH_GPL3rdP
#ifdef HAVE_GSL_ZSOLVE
#define m__BUILD_GPLVERSION
#endif
#endif

#ifdef m__BUILD_GPLVERSION
#define COMPONENT_DESCRIPTION_CFORMANTLPC "This component computes formant frequencies and bandwidths by solving for the roots of the LPC polynomial. The formant trajectories can and should be smoothed by the cFormantSmoother component."
#else
#define COMPONENT_DESCRIPTION_CFORMANTLPC "This component computes formant frequencies and bandwidths by peak picking in the LP-smoothed spectrum (experimental!). The formant trajectories can and should be smoothed by the cFormantSmoother component."
#endif

#define COMPONENT_NAME_CFORMANTLPC "cFormantLpc"


#undef class
class DLLEXPORT cFormantLpc : public cVectorProcessor {
  private:
    int nFormants;
    int saveFormants;
    int saveIntensity;
    int saveBandwidths;
    int saveNumberOfValidFormants;
    int useLpSpec;
    int nSmooth;
    int medianFilter, octaveCorrection;

    int nLpc;
    long lpcCoeffIdx;
    long lpcGainIdx;
    long lpSpecIdx, lpSpecN;

    double minF, maxF;
    
    double T;
    double *lpc, *roots;
    double *formant, *bandwidth;

    FLOAT_DMEM calcFormantLpc(const FLOAT_DMEM *x, long Nsrc, FLOAT_DMEM * lpc, long nCoeff, FLOAT_DMEM *refl);
  protected:
    SMILECOMPONENT_STATIC_DECL_PR

    virtual void fetchConfig();
    virtual int setupNewNames(long nEl);
    virtual void findInputFields();

    virtual int processVectorInt(const INT_DMEM *src, INT_DMEM *dst, long Nsrc, long Ndst, int idxi);
    virtual int processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cFormantLpc(const char *_name);

    virtual ~cFormantLpc();
};

#endif // __CFORMANTLPC_HPP
