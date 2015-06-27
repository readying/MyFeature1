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


/*  openSMILE component: cNullSink
----------------------------------
NULL sink: reads data vectors from data memory and destroys them without writing them anywhere
----------------------------------

11/2009  Written by Florian Eyben
*/


#include <core/nullSink.hpp>

#define MODULE "cNullSink"


SMILECOMPONENT_STATICS(cNullSink)

SMILECOMPONENT_REGCOMP(cNullSink)
{
  SMILECOMPONENT_REGCOMP_INIT

  scname = COMPONENT_NAME_CNULLSINK;
  sdescription = COMPONENT_DESCRIPTION_CNULLSINK;

  // we inherit cDataSink configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cDataSink")
  
  SMILECOMPONENT_IFNOTREGAGAIN( {} )

  SMILECOMPONENT_MAKEINFO(cNullSink);
}

SMILECOMPONENT_CREATE(cNullSink)

//-----

cNullSink::cNullSink(const char *_name) :
  cDataSink(_name)
{
}

int cNullSink::myTick(long long t)
{
  SMILE_IDBG(4,"tick # %i, reading value vector to NULL sink",t);
  cVector *vec= reader_->getNextFrame();
  if (vec == NULL) return 0;
  else 
    // tick success
    return 1;
}


cNullSink::~cNullSink()
{
}

