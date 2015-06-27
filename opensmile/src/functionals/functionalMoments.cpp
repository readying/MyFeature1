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

functionals: statistical moments

*/



#include <functionals/functionalMoments.hpp>

#define MODULE "cFunctionalMoments"


#define FUNCT_VAR        0
#define FUNCT_STDDEV     1
#define FUNCT_SKEWNESS   2
#define FUNCT_KURTOSIS   3
#define FUNCT_AMEAN      4

#define N_FUNCTS  5

#define NAMES     "variance","stddev","skewness","kurtosis","amean"

const char *momentsNames[] = {NAMES};  // change variable name to your class...

SMILECOMPONENT_STATICS(cFunctionalMoments)

SMILECOMPONENT_REGCOMP(cFunctionalMoments)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CFUNCTIONALMOMENTS;
  sdescription = COMPONENT_DESCRIPTION_CFUNCTIONALMOMENTS;

  // configure your component's configType:
  SMILECOMPONENT_CREATE_CONFIGTYPE

  SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("variance","1/0=enable/disable output of variance",1);
    ct->setField("stddev","1/0=enable/disable output of standard deviation",1);
    ct->setField("skewness","1/0=enable/disable output of skewness",1);
    ct->setField("kurtosis","1/0=enable/disable output of kurtosis",1);
    ct->setField("amean","1/0=enable/disable output of arithmetic mean",0);
  )

  SMILECOMPONENT_MAKEINFO_NODMEM(cFunctionalMoments);
}

SMILECOMPONENT_CREATE(cFunctionalMoments)

//-----

cFunctionalMoments::cFunctionalMoments(const char *_name) :
  cFunctionalComponent(_name,N_FUNCTS,momentsNames)
{
}

void cFunctionalMoments::fetchConfig()
{
  if (getInt("variance")) enab[FUNCT_VAR] = 1;
  if (getInt("stddev")) enab[FUNCT_STDDEV] = 1;
  if (getInt("skewness")) enab[FUNCT_SKEWNESS] = 1;
  if (getInt("kurtosis")) enab[FUNCT_KURTOSIS] = 1;
  if (getInt("amean")) enab[FUNCT_AMEAN] = 1;

  cFunctionalComponent::fetchConfig();
}

long cFunctionalMoments::process(FLOAT_DMEM *in, FLOAT_DMEM *inSorted, FLOAT_DMEM min, FLOAT_DMEM max, FLOAT_DMEM mean, FLOAT_DMEM *out, long Nin, long Nout)
{
  int i;
  if ((Nin>0)&&(out!=NULL)) {
    double m2=0.0, m3=0.0, m4=0.0;

    double Nind = (double)Nin;
    double tmp, tmp2;
    FLOAT_DMEM *in0=in;
    
    for (i=0; i<Nin; i++) {
      tmp = ((double)*(in++) - mean); // ?? * p(x) ??
      tmp2 = tmp*tmp;
      m2 += tmp2;
      tmp2 *= tmp;
      m3 += tmp2;
      m4 += tmp2*tmp;
    }
    m2 /= Nind;  // variance

    int n=0;
    if (enab[FUNCT_VAR]) out[n++]=(FLOAT_DMEM)m2;
    double sqm2=sqrt(m2);
    if (enab[FUNCT_STDDEV]) {
      if (m2 > 0.0) out[n++]=(FLOAT_DMEM)sqm2;
      else out[n++] = 0.0;
    }
    if (enab[FUNCT_SKEWNESS]) {
      if (m2 > 0.0) out[n++]=(FLOAT_DMEM)( m3/(Nind*m2*sqm2) );
      else out[n++] = 0.0;
    }
    if (enab[FUNCT_KURTOSIS]) {
      if (m2 > 0.0) out[n++]=(FLOAT_DMEM)( m4/(Nind*m2*m2) );
      else out[n++] = 0.0;
    }
    if (enab[FUNCT_AMEAN]) out[n++]=(FLOAT_DMEM)mean;
    return n;
  }
  return 0;
}

/*
long cFunctionalMoments::process(INT_DMEM *in, INT_DMEM *inSorted, INT_DMEM *out, long Nin, long Nout)
{

  return 0;
}
*/

cFunctionalMoments::~cFunctionalMoments()
{
}

