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

number of segments based on delta thresholding

*/


#ifndef __CFUNCTIONALMODULATION_HPP
#define __CFUNCTIONALMODULATION_HPP

#include <core/smileCommon.hpp>
#include <core/dataMemory.hpp>
#include <functionals/functionalComponent.hpp>

#define COMPONENT_DESCRIPTION_CFUNCTIONALMODULATION "  DCT coefficients"
#define COMPONENT_NAME_CFUNCTIONALMODULATION "cFunctionalModulation"

#undef class
class DLLEXPORT cFunctionalModulation : public cFunctionalComponent {
private:
  int firstCoeff, lastCoeff;
  int nCo;
  long N;
  long costableNin;
  FLOAT_DMEM * costable;
  FLOAT_DMEM factor;
  char *tmpstr;

protected:
  SMILECOMPONENT_STATIC_DECL_PR
    virtual void fetchConfig();
  virtual void initCostable(long Nin, long Nout);

public:
  SMILECOMPONENT_STATIC_DECL

    cFunctionalModulation(const char *_name);
  // inputs: sorted and unsorted array of values, out: pointer to space in output array, You may not return MORE than Nout elements, please return as return value the number of actually computed elements (usually Nout)
  virtual long process(FLOAT_DMEM *in, FLOAT_DMEM *inSorted, FLOAT_DMEM *out, long Nin, long Nout);
  //virtual long process(INT_DMEM *in, INT_DMEM *inSorted, INT_DMEM *out, long Nin, long Nout);
  virtual const char * getValueName(long i);

  virtual long getNoutputValues() { return nEnab; }
  virtual int getRequireSorted() { return 0; }

  virtual ~cFunctionalModulation();
};




#endif // __CFUNCTIONALMODULATION_HPP
