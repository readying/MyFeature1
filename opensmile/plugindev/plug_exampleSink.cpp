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

example dataSink
writes data to data memory...

*/


#include <plug_exampleSink.hpp>

#define MODULE "cExampleSinkPlugin"


SMILECOMPONENT_STATICS(cExampleSinkPlugin)

SMILECOMPONENT_REGCOMP(cExampleSinkPlugin)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CEXAMPLESINK;
  sdescription = COMPONENT_DESCRIPTION_CEXAMPLESINK;

  // we inherit cDataSink configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cDataSink")
  
  SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("filename","file to dump values to","dump.txt");
    ct->setField("lag","output data <lag> frames behind",0);
  )
  SMILECOMPONENT_MAKEINFO(cExampleSinkPlugin);
}

SMILECOMPONENT_CREATE(cExampleSinkPlugin)

//-----

cExampleSinkPlugin::cExampleSinkPlugin(const char *_name) :
  cDataSink(_name)
{
  // ...
printf("I'm here...\n");
}

void cExampleSinkPlugin::fetchConfig()
{
  cDataSink::fetchConfig();
printf("I'm here in fetchConfig...\n");
  
  filename = getStr("filename");
printf("filename=%s\n",filename);
printf("log=%i\n",(long)&smileLog);
smileLog.message(FMT("test"), 2, "cYEAH");

  SMILE_DBG(2,"filename = '%s'",filename);
  lag = getInt("lag");
  SMILE_DBG(2,"lag = %i",lag);
}

/*
int cExampleSink::myConfigureInstance()
{
  int ret=1;
  ret *= cDataSink::myConfigureInstance();
  // ....
  //return ret;
}
*/

/*
int cExampleSink::myFinaliseInstance()
{
  int ret=1;
  ret *= cDataSink::myFinaliseInstance();
  // ....
  //return ret;
}
*/

int cExampleSinkPlugin::myTick(long long t)
{
  SMILE_DBG(4,"tick # %i, reading value vector:",t);
  cVector *vec= reader_->getFrameRel(lag);  //new cVector(nValues+1);
  if (vec == NULL) return 0;
  else reader_->nextFrame();

  long vi = vec->tmeta->vIdx;
  double tm = vec->tmeta->time;
  
  // now print the vector:
  SMILE_IMSG(2,"FUCKING AWSOME PLUGIN OUTPUT:");
  int i;
  for (i=0; i<vec->N; i++) {
    printf("  (a=%i vi=%i, tm=%fs) %s.%s = %f\n",reader_->getCurR(),vi,tm,reader_->getLevelName(),vec->name(i),vec->dataF[i]);
  }

// SMILE_PRINT("%i",var1,)


  // tick success
  return 1;
}


cExampleSinkPlugin::~cExampleSinkPlugin()
{
  // ...
}

