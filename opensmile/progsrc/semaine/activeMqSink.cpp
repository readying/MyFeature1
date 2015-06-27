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

ActiveMQ feature sender

*/


#include <semaine/activeMqSink.hpp>

#define MODULE "cActiveMqSink"

#ifdef HAVE_SEMAINEAPI

SMILECOMPONENT_STATICS(cActiveMqSink)

SMILECOMPONENT_REGCOMP(cActiveMqSink)
{
  SMILECOMPONENT_REGCOMP_INIT

  scname = COMPONENT_NAME_CACTIVEMQSINK;
  sdescription = COMPONENT_DESCRIPTION_CACTIVEMQSINK;

  // we inherit cDataSink configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cDataSink")
  
    SMILECOMPONENT_IFNOTREGAGAIN( {}
//    ct->makeMandatory(ct->setField("topic","ActiveMQ topic to send to",(const char *)NULL)); // semaine.data.analysis.features.voice
//    ct->setField("lag","output data <lag> frames behind",0);
  )

  SMILECOMPONENT_MAKEINFO(cActiveMqSink);
}

SMILECOMPONENT_CREATE(cActiveMqSink)

//-----

cActiveMqSink::cActiveMqSink(const char *_name) :
  cDataSink(_name),
  featureSender(NULL),
  meta(NULL),
  warned(0)
{
}

void cActiveMqSink::fetchConfig()
{
  cDataSink::fetchConfig();
}

/*
int cActiveMqSink::myConfigureInstance()
{
  int ret=1;

  ret *= cDataSink::myConfigureInstance();
  
  return ret;
}
*/

int cActiveMqSink::myTick(long long t)
{
  if (featureSender == NULL) {
    if (warned==0) {
      SMILE_IERR(1,"the semaine featureSender has not been set (it is NULL!), please check the log output of the semaine components, and check the semaineCfg section in the config file! (this warning will be shown only once, the system will not work!)");
      warned = 1;
    }
    return 0;
  }

  SMILE_DBG(4,"tick # %i, reading value vector:",t);
  cVector *vec= reader->getNextFrame();  //new cVector(nValues+1);
  if (vec == NULL) return 0;
  //else reader->nextFrame();

  long vi = vec->tmeta->vIdx;
  double tm = vec->tmeta->time;

  int n,i;
  n = vec->N;
  if (!featureSender->areFeatureNamesSet()) {
    std::vector<std::string> tumFeatureNames(n);
    for (i=0; i<n; i++) {
      std::string name(vec->fmeta->getName(i)); 
      tumFeatureNames[i] = name;
    }
    //    tumFeatureNames[i]="speaking";
    //      tumFeatureNames[i+1]="seqNr";
    featureSender->setFeatureNames(tumFeatureNames);
  }

  std::vector<float> features(n);
  for (i=0; i<n; i++) {
    features[i] = (float)(vec->dataF[i]);
  }
  //    features[i] = speakingStatus;
  //    features[i+1] = seqNr;
  //    seqNr++;

//printf("about to send\n");
// NOTE: (FIXME) sometimes the activeMq sender hangs here:
//               restarting activeMq helps to solve this problem
//               nevertheless it should be investigated...
  if (meta != NULL) {
    featureSender->sendFeatureVector(features, meta->getTime(), false);
  } else {
    featureSender->sendFeatureVector(features, 0, false);
  }
//printf("after sending\n");

  // tick success
  return 1;
}


cActiveMqSink::~cActiveMqSink()
{
}

#endif // HAVE_SEMAINEAPI
