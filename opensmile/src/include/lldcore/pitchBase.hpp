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

This component computes pitch via the Harmonic Product Spectrum method.
As input it requires FFT magnitude data. 
Note that the type of input data is not checked, thus be careful when writing your configuration files!

*/


#ifndef __CPITCHBASE_HPP
#define __CPITCHBASE_HPP

#include <core/smileCommon.hpp>
#include <core/vectorProcessor.hpp>

#define COMPONENT_DESCRIPTION_CPITCHBASE "Base class for all pitch classes, no functionality on its own!"
#define COMPONENT_NAME_CPITCHBASE "cPitchBase"


#undef class
class DLLEXPORT cPitchBase : public cVectorProcessor {
  private:
    const char *inputFieldPartial;
    int nCandidates;
    int scores, voicing;
    int F0C1, voicingC1;
    int F0raw;
    int voicingClip;

    double maxPitch, minPitch;

    FLOAT_DMEM *inData;
    FLOAT_DMEM *f0cand, *candVoice, *candScore;

  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    FLOAT_DMEM voicingCutoff;
    int octaveCorrection;

    virtual void fetchConfig();
	  virtual int setupNewNames(long nEl);
    
    //virtual int processVectorInt(const INT_DMEM *src, INT_DMEM *dst, long Nsrc, long Ndst, int idxi);
    virtual int processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi);

    // to be overwritten by child class:
    virtual int pitchDetect(FLOAT_DMEM * inData, long _N, double _fsSec, double baseT, FLOAT_DMEM *f0cand, FLOAT_DMEM *candVoice, FLOAT_DMEM *candScore, long nCandidates);
    virtual int addCustomOutputs(FLOAT_DMEM *dstCur, long NdstLeft);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cPitchBase(const char *_name);
    
    virtual ~cPitchBase();
};




#endif // __CPITCHBASE_HPP
