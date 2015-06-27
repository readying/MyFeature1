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


/*  openSMILE component: cIntensity

compute simplified intensity according to :
 Andreas Kie�ling - Extraktion und Klassifikation prosodischer Merkmale in der automatischen Sprachverarbeitung
 Pg. 156-157

(08/2009) Written by Florian Eyben

*/


#include <lldcore/intensity.hpp>

#define MODULE "cIntensity"

SMILECOMPONENT_STATICS(cIntensity)

SMILECOMPONENT_REGCOMP(cIntensity)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CINTENSITY;
  sdescription = COMPONENT_DESCRIPTION_CINTENSITY;

  // we inherit cVectorProcessor configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cVectorProcessor")
  
  SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("nameAppend",NULL,(const char *)NULL);
    ct->setField("intensity","1 = enable the output of intensity I (mean of squared input values multiplied by a Hamming window)",1);
    ct->setField("loudness","1 = enable the output of loudness L : L = (I/I0)^0.3 ; I0 = 0.000001 (for sample values normalised to the range -1..1)",0);
  )
  SMILECOMPONENT_MAKEINFO(cIntensity);
}

SMILECOMPONENT_CREATE(cIntensity)

//-----

cIntensity::cIntensity(const char *_name) :
  cVectorProcessor(_name),
  hamWin(NULL),
  I0(1.0),
  nWin(0), winSum(0.0)
{

}

void cIntensity::fetchConfig()
{
  cVectorProcessor::fetchConfig();

  intensity = getInt("intensity");
  SMILE_IDBG(2,"intensity = %i\n",intensity);
  loudness = getInt("loudness");
  SMILE_IDBG(2,"loudness = %i\n",loudness);

}

/*
int cIntensity::myConfigureInstance()
{
  int ret=1;
  ret *= cVectorProcessor::myConfigureInstance();
  if (ret == 0) return 0;

//...


  return ret;
}
*/

/*
int cIntensity::configureWriter(const sDmLevelConfig *c)
{

  // you must return 1, in order to indicate configure success (0 indicated failure)
  return 1;
}

*/

int cIntensity::setupNamesForField(int i, const char*name, long nEl)
{
  int n=0;
  long j;

  if (intensity) { addNameAppendField(name, "intensity", 1); n++; }
  if (loudness) { addNameAppendField(name, "loudness", 1); n++; }

  // Create hamming window:
  hamWin = smileDsp_winHam( nEl );
  if (hamWin != NULL) nWin = nEl;
  for (j=0; j<nEl; j++) {
    winSum += hamWin[j];
  }
  if (winSum <= 0.0) winSum = 1.0;

  // I0 constant for 60 dB and max. amplitude = 1.0:  I0 = 10^-6
  I0 = (double)0.000001;

  return n;
}


/*
int cIntensity::myFinaliseInstance()
{
  int ret=1;
  ret *= cVectorProcessor::myFinaliseInstance();
//.....
  return ret;
}
*/

/*
// a derived class should override this method, in order to implement the actual processing
int cIntensity::processVectorInt(const INT_DMEM *src, INT_DMEM *dst, long Nsrc, long Ndst, int idxi) // idxi=input field index
{
  // do domething to data in *src, save result to *dst
  // NOTE: *src and *dst may be the same...
  
  return 0;
}
*/

// a derived class should override this method, in order to implement the actual processing
int cIntensity::processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi) // idxi=input field index
{
  // do domething to data in *src, save result to *dst
  // NOTE: *src and *dst may be the same...
  if (Nsrc == 0) return 0;
  if ((hamWin == NULL)||(nWin<=0)) return 0;

  long i;
  double Im=0.0;

  long safeN = MIN(Nsrc,MIN(nWin,Ndst));
  for (i=0; i<safeN; i++) {
    Im += hamWin[i] * (double)src[i]*(double)src[i];       
  }
  Im /= winSum;

  long n=0;
  if (intensity) { dst[n++] = (FLOAT_DMEM)Im; }
  if (loudness) { dst[n++] = (FLOAT_DMEM)pow( Im/I0 , 0.3 ); }
  
  return n;
}

cIntensity::~cIntensity()
{
  if (hamWin != NULL) free(hamWin);
}

