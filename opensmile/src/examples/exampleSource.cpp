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

example dataSource
writes data to data memory...

*/


#include <examples/exampleSource.hpp>
#define MODULE "cExampleSource"

SMILECOMPONENT_STATICS(cExampleSource)

SMILECOMPONENT_REGCOMP(cExampleSource)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CEXAMPLESOURCE;
  sdescription = COMPONENT_DESCRIPTION_CEXAMPLESOURCE;

  // we inherit cDataSource configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cDataSource")
  
  SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("nValues","The number of random values to generate",1);
    ct->setField("randSeed","The random seed",1.0);
  )

  SMILECOMPONENT_MAKEINFO(cExampleSource);
}

SMILECOMPONENT_CREATE(cExampleSource)

//-----

cExampleSource::cExampleSource(const char *_name) :
  cDataSource(_name)
{
  // ...
}

void cExampleSource::fetchConfig()
{
  cDataSource::fetchConfig();
  
  nValues = getInt("nValues");
  SMILE_DBG(2,"nValues = %i",nValues);
  randSeed = getDouble("randSeed");
  SMILE_DBG(2,"nValues = %f",randSeed);
}

/*
int cExampleSource::myConfigureInstance()
{
  int ret = cDataSource::myConfigureInstance();
  // ...

  return ret;
}
*/

int cExampleSource::configureWriter(sDmLevelConfig &c)
{
  // configure your writer by setting values in &c

  return 1;
}

// NOTE: nEl is always 0 for dataSources....
int cExampleSource::setupNewNames(long nEl)
{
  // configure dateMemory level, names, etc.
  writer_->addField("randVal",nValues);
  writer_->addField("const");
  // ...

  allocVec(nValues+1);
  return 1;
}

/*
int cExampleSource::myFinaliseInstance()
{
  return cDataSource::myFinaliseInstance();
}
*/

int cExampleSource::myTick(long long t)
{
  SMILE_DBG(4,"tick # %i, writing value vector",t);

  // todo: fill with random values...
  vec_->dataF[0] = (FLOAT_DMEM)t+(FLOAT_DMEM)3.3;
  writer_->setNextFrame(vec_);
  
  return 1;
}


cExampleSource::~cExampleSource()
{
  // ..
}
