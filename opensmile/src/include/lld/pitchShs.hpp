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


#ifndef __CPITCHSHS_HPP
#define __CPITCHSHS_HPP

#include <core/smileCommon.hpp>
#include <lldcore/pitchBase.hpp>

#define COMPONENT_DESCRIPTION_CPITCHSHS "This component computes the fundamental frequency via the Sub-Harmonic-Sampling (SHS) method (this is related to the Harmonic Product Spectrum method)."
#define COMPONENT_NAME_CPITCHSHS "cPitchShs"


#undef class
class DLLEXPORT cPitchShs : public cPitchBase {
  private:
    int nHarmonics;
    int greedyPeakAlgo;
    FLOAT_DMEM Fmint, Fstept;
    FLOAT_DMEM nOctaves, nPointsPerOctave;
    FLOAT_DMEM *SS;
    FLOAT_DMEM *Fmap;
    FLOAT_DMEM compressionFactor;
    double base;

  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    virtual int setupNewNames(long nEl);
    
    // to be overwritten by child class:
    virtual int pitchDetect(FLOAT_DMEM * inData, long _N, double _fsSec, double baseT, FLOAT_DMEM *f0cand, FLOAT_DMEM *candVoice, FLOAT_DMEM *candScore, long nCandidates);
    virtual int addCustomOutputs(FLOAT_DMEM *dstCur, long NdstLeft);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cPitchShs(const char *_name);
    
    virtual ~cPitchShs();
};




#endif // __CPITCHSHS_HPP
