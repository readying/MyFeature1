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


/*  openSMILE component: delta coefficients

compute delta regression using regression formula

*/



#ifndef __CDELTAREGRESSION_HPP
#define __CDELTAREGRESSION_HPP

#include <core/smileCommon.hpp>
#include <core/windowProcessor.hpp>

#define COMPONENT_DESCRIPTION_CDELTAREGRESSION "This component computes delta regression coefficients using the regression equation from the HTK book."
#define COMPONENT_NAME_CDELTAREGRESSION "cDeltaRegression"

#undef class
class DLLEXPORT cDeltaRegression : public cWindowProcessor {
private:
  int halfWaveRect, absOutput;
  int deltawin;
  FLOAT_DMEM norm;

  int zeroSegBound;
  int onlyInSegments;

protected:
  SMILECOMPONENT_STATIC_DECL_PR


  int isNoValue(FLOAT_DMEM x) {
    if (onlyInSegments && x==0.0) return 1;
    if (isnan(x)) return 1;
    return 0;
  }

  virtual void fetchConfig();

  //virtual int configureWriter(const sDmLevelConfig *c);
  //virtual int setupNamesForField(int i, const char*name, long nEl);

  // buffer must include all (# order) past samples
  virtual int processBuffer(cMatrix *_in, cMatrix *_out, int _pre, int _post );


public:
  SMILECOMPONENT_STATIC_DECL

    cDeltaRegression(const char *_name);

  virtual ~cDeltaRegression();
};


#endif // __CDELTAREGRESSION_HPP
