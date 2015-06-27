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

concatenates vectors from multiple levels and copy to another level

*/


#include <other/vectorConcat.hpp>

#define MODULE "cVectorConcat"

SMILECOMPONENT_STATICS(cVectorConcat)

SMILECOMPONENT_REGCOMP(cVectorConcat)
{
  SMILECOMPONENT_REGCOMP_INIT

  scname = COMPONENT_NAME_CVECTORCONCAT;
  sdescription = COMPONENT_DESCRIPTION_CVECTORCONCAT;

  // we inherit cVectorProcessor configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cVectorProcessor")
  
    SMILECOMPONENT_IFNOTREGAGAIN( {}
    //ct->setField("expandFields", "expand fields to single elements, i.e. each field in the output will correspond to exactly one element in the input [not yet implemented]", 0);
  )

  SMILECOMPONENT_MAKEINFO(cVectorConcat);
}

SMILECOMPONENT_CREATE(cVectorConcat)

//-----

cVectorConcat::cVectorConcat(const char *_name) :
  cVectorProcessor(_name)
{
}


int cVectorConcat::processVectorInt(const INT_DMEM *src, INT_DMEM *dst, long Nsrc, long Ndst, int idxi) // idxi=input field index
{
  if (dst!=src)
    memcpy( dst, src,  MIN(Ndst,Nsrc)*sizeof(INT_DMEM) );
  return 1;
}

int cVectorConcat::processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi) // idxi=input field index
{
  if (dst!=src)
    memcpy( dst, src,  MIN(Ndst,Nsrc)*sizeof(FLOAT_DMEM) );
  return 1;
}

cVectorConcat::~cVectorConcat()
{
}

