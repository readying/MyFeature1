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

computes spectral features such as flux, roll-off, centroid, etc.

*/


#ifndef __CSPECTRAL_HPP
#define __CSPECTRAL_HPP

#include <core/smileCommon.hpp>
#include <core/vectorProcessor.hpp>

#define COMPONENT_DESCRIPTION_CSPECTRAL "This component computes spectral features such as flux, roll-off, centroid, and user defined band energies (rectangular summation of FFT magnitudes), etc."
#define COMPONENT_NAME_CSPECTRAL "cSpectral"

#undef class
class DLLEXPORT cSpectral : public cVectorProcessor {
  private:
    int frqScale;
    int normBandEnergies;
    double frqScaleParam;

    int squareInput, flux, centroid, nBands, nRollOff, entropy;
    int variance, skewness, kurtosis, slope;
    int sharpness, tonality, harmonicity, flatness;

    long nScale;
    int maxPos, minPos;
    long *bandsL, *bandsH; // frequencies
    double *bandsLi, *bandsHi; // indicies (fft bins), real values
    double *wghtLi, *wghtHi; // indicies (fft bins), real values
    double *rollOff;
    double fsSec;
    const double *frq;
    int buggyRollOff;
    
    int isBandValid(long start, long end)
    {
      if ((start>=0)&&(end>0)) return 1;
      else return 0;
    }
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR

    virtual void fetchConfig();

    virtual int setupNamesForField(int i, const char*name, long nEl);
    virtual int processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi);


  public:
    SMILECOMPONENT_STATIC_DECL
    
    cSpectral(const char *_name);

    virtual ~cSpectral();
};




#endif // __CSPECTRAL_HPP
