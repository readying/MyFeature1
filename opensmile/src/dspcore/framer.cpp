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

data framer
 
*/


#include <dspcore/framer.hpp>
//#include <math.h>

#define MODULE "cFramer"


SMILECOMPONENT_STATICS(cFramer)

SMILECOMPONENT_REGCOMP(cFramer)
{
  SMILECOMPONENT_REGCOMP_INIT


  scname = COMPONENT_NAME_CFRAMER;
  sdescription = COMPONENT_DESCRIPTION_CFRAMER;

  // we inherit cWinToVecProcessor configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cWinToVecProcessor")
  
  SMILECOMPONENT_IFNOTREGAGAIN( {} 

  )
  
  SMILECOMPONENT_MAKEINFO(cFramer);
}

SMILECOMPONENT_CREATE(cFramer)

//-----

cFramer::cFramer(const char *_name) :
  cWinToVecProcessor(_name)
{

}


// this must return the multiplier, i.e. the vector size returned for each input element (e.g. number of functionals, etc.)
int cFramer::getMultiplier()
{
  return frameSizeFrames;
}

// idxi is index of input element
// row is the input row
// y is the output vector (part) for the input row
int cFramer::doProcess(int idxi, cMatrix *row, FLOAT_DMEM*y)
{
  // copy row to matrix... simple memcpy here
  memcpy(y,row->dataF,row->nT*sizeof(FLOAT_DMEM));
  // return the number of components in y!!
  return row->nT;
}
int cFramer::doProcess(int idxi, cMatrix *row, INT_DMEM*y)
{
  // copy row to matrix... simple memcpy here
  memcpy(y,row->dataI,row->nT*sizeof(INT_DMEM));
  // return the number of components in y!!
  return row->nT;
}


cFramer::~cFramer()
{
}

