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

functional: Modulation spectra, cepstra, etc.

1. Single modulation frequencies... correlation based? fft on full input based?
2. Overlaping windows -> fft -> average modulation spectrum
3. Method 2 allows for average cepstrum  (Method 1 only for small windows and full input fft -> however scaling problem)
4. Method 2 allows for higher level functionals...


Problems: varying window size... esp. minimum required window size..  zero padding?

*/


#include <functionals/functionalModulation.hpp>

#define MODULE "cFunctionalModulation"

#define N_FUNCTS  1

#define NAMES     "ModSpec"

const char *modspecNames[] = {NAMES};

SMILECOMPONENT_STATICS(cFunctionalModulation)

SMILECOMPONENT_REGCOMP(cFunctionalModulation)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CFUNCTIONALMODULATION;
  sdescription = COMPONENT_DESCRIPTION_CFUNCTIONALMODULATION;

  // configure your component's configType:
  SMILECOMPONENT_CREATE_CONFIGTYPE
  SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("frequencies", "Custom list of modulation frequencies for which to compute the magnitudes. The frequencies will be computed via a zero padded FFT over the full input (modFrqUseFFT>0), or via correlation with sine/cosine functions (slower, but efficient for only very few frequencies).", 0, ARRAY_TYPE);
    ct->setField("modFrqUseFFT", "If > 0, use FFT of full input if computing more than 'modFrqUseFFT' modulation frequencies.", 4);
    ct->setField("modFrqUseACF", "If == 1, use ACF (via FFT) to get better temporal resolution for low modulation frequencies.", 0);
    ct->setField("stftModspec", "1 = Enable average Stft (Short Time Fourier Transform) modulation spectrum (and ignore 'frequencies' array, write full spectrum to output). 2 = Same as 1, however, select frequencies based on 'frequencies' array. 0 = disable.", 0)
    ct->setField("stftCepstrum", "1 = Enable average modulation cepstrum based on Stft.", 0);
    ct->setField("stftAcf", "1 = Enable average ACF based on Stft.", 0);
    ct->setField("nModSpecPeaks", "Number of highest modulation spectrum peaks to output the frequencies for.", 0);
    ct->setField("nCepstralPeaks", "Number N of N highest peaks (in range numCeps..N) in cepstrum to output position for (time in seconds).", 0);
    ct->setField("nAcfPeaks", "Number N of N highest peaks in ACF to output position for (time in seconds). This excludes the peak at time 0.", 0);
    ct->setField("modSpecRange", "Frequency range of Stft modulation spectrum output (leave empty (NULL) to output the full range covered by fftWinSize). Format: start.xx - end.xx (in Hz)", (const char*)NULL);
    ct->setField("acfRange", "ACF range to output. Default (NULL) is output full range", (const char*)NULL);
    ct->setField("acfRange_percentage", "ACF range to output. Default (NULL) is output full range", (const char*)NULL);
    ct->setField("numCeps", "If > 0, output only 1..numCeps cepstral coefficients, otherwise (0) output all cepstral coefficients. Only effective if, stftCepstrum==1, or if using nCepstralPeaks>0.", 0);
    ct->setField("numCeps_percentage", "If > 0.0, output only 1..numCeps*N cepstral coefficients.", 0.0);
    ct->setField("fftWinSize", "Window size for Stft FFT in frames.", 100);
    ct->setField("fftWinSize_sec", "Window size for Stft FFT in seconds.", 1.0);
    ct->setField("fftWinFunc", "Stft FFT window function.", "han");
    ct->setField("shortSegmentMethod", "How to deal with short segments, i.e. segments that are shorter than fftWinSize(_sec). One of: 'zeroPad' zero-pad the input to fftWinSize, 'zeroOutput' output all zero values (no computation), 'noOutput' no output frame, input is discarded.");
  )

  SMILECOMPONENT_MAKEINFO_NODMEM(cFunctionalModulation);
}

SMILECOMPONENT_CREATE(cFunctionalModulation)

//-----

cFunctionalModulation::cFunctionalModulation(const char *_name) :
  cFunctionalComponent(_name, N_FUNCTS, modspecNames),
{
}

void cFunctionalModulation::fetchConfig()
{
  stftModspec_ = getInt("stftModspec");


  enab[0] = 1;
  cFunctionalComponent::fetchConfig();
  nEnab += lastCoeff - firstCoeff;
}

// Initialize the sine/cosine table for single modulation frequency estimation
void cFunctionalModulation::initSinCosTable(long Nfrq, const double *frq)
{

}

const char* cFunctionalModulation::getValueName(long i)
{
  const char *n = cFunctionalComponent::getValueName(0);
  // append coefficient number
  if (tmpstr != NULL) free(tmpstr);
  tmpstr = myvprint("%s%i",n,i+firstCoeff);
  return tmpstr;
}

long cFunctionalModulation::processStftFrame()
{
  // compute stft

  // compute acf (if enab.)
  
  // compute cepstrum (if enab.)

}

long cFunctionalModulation::computeStft()
{
  // estimate number of frames...

  // compute stft (and acf / cep) for every frame

}

long cFunctionalModulation::findPeaks()
{
  // general peak finder


}


long cFunctionalModulation::findPeaksFreq()
{
  // find peaks in stft mag and output as mod. freqs.


}

long cFunctionalModulation::findPeaksTime()
{
  // find peaks in cepstrum and ACF and output as mod. freqs.


}


long cFunctionalModulation::selectMagFreqs()
{
  // based on 'frequencies' array, filter the (stft) mag freqs.


}



long cFunctionalModulation::process(FLOAT_DMEM *in, FLOAT_DMEM *inSorted, FLOAT_DMEM *out, long Nin, long Nout)
{
  if (fullModspec) {
    // FFT of full input
    modFrqUseFFT

    // OR: sin/cos table correlations

    // apply ACF for better resolution, if option is set
    modFrqUseACF

    // peak picking

  }
  if (stftModspec || stftCepstrum || stftAcf) {
    // compute average stft stuff

  }
  if (stftModspec) {
    // output modspec stuff
  }
  if (stftCepstrum) {
    // output cepstrum stuff

  }
  if (stftAcf) {
    // output acf stuff

  }  
  return 0;
}

/*
long cFunctionalModulation::process(INT_DMEM *in, INT_DMEM *inSorted, INT_DMEM *out, long Nin, long Nout)
{

  return 0;
}
*/

cFunctionalModulation::~cFunctionalModulation()
{
  if (tmpstr != NULL) free(tmpstr);
}


// TODO: modspec phases might be of interest... or rather the relative phase deviations between mod freqs.

// TODO: 2D modspec component (not functional.. MVR like)

/*
 This will take a log spectrogram, downsampled to 50 freq. bins?
 Time resolution downsampled to 20fps
  Then 2D spec/cep ?  (looks more at the LF components)

 Or fine grained struct of non-downsampled as extra features? (HF components)
*/

