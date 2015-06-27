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


/*  openSMILE component: valbased selector

vector selector based on threshold of value <idx>

*/


#include <other/valbasedSelector.hpp>

#define MODULE "cValbasedSelector"

SMILECOMPONENT_STATICS(cValbasedSelector)

SMILECOMPONENT_REGCOMP(cValbasedSelector)
{
  SMILECOMPONENT_REGCOMP_INIT

  scname = COMPONENT_NAME_CVALBASEDSELECTOR;
  sdescription = COMPONENT_DESCRIPTION_CVALBASEDSELECTOR;

  // we inherit cDataProcessor configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cDataProcessor")
  
  SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("threshold","Threshold for selection (see also 'invert' option)",1.0);
    ct->setField("idx", "The index of element to base the selection decision on. Currently only 1 element is supported, NO vector based thresholds etc. are possible.",0);
    ct->setField("invert", "1 = output the frame when element[idx] < threshold ; 0 = output the frame if element[idx] => threshold",0);
    ct->setField("allowEqual", "if this option is set to 1, output the frame also, when element[idx] == threshold",0);
    ct->setField("removeIdx", "1 = remove field element[idx] in output vector ; 0 = keep it",0);
    ct->setField("zeroVec" , "1 = instead of not writing output to the output level if selection threshold is not met, output a vector with all values set to 'outputVal', which is 0 by default (removeIdx options still has the same effect).", 0);
    ct->setField("outputVal" , "Value all output elements will be set to when 'zeroVec=1'.", 0.0)
  )
  
  SMILECOMPONENT_MAKEINFO(cValbasedSelector);
}


SMILECOMPONENT_CREATE(cValbasedSelector)

//-----

cValbasedSelector::cValbasedSelector(const char *_name) :
  cDataProcessor(_name),
  myVec(NULL), idx(0), invert(0), removeIdx(0), threshold(0.0), outputVal(0.0), zerovec(0), elI(0)
{
}

void cValbasedSelector::fetchConfig()
{
  cDataProcessor::fetchConfig();

  // load all configuration parameters you will later require fast and easy access to here:

  threshold = (FLOAT_DMEM)getDouble("threshold");
  SMILE_DBG(2,"threshold = %f",threshold);

  idx = getInt("idx");
  invert = getInt("invert");
  allowEqual = getInt("allowEqual");
  removeIdx = getInt("removeIdx");

  outputVal = (FLOAT_DMEM)getDouble("outputVal");
  zerovec = getInt("zeroVec");
}


int cValbasedSelector::setupNamesForField(int i, const char*name, long nEl)
{
  elI+=nEl;
  if (removeIdx && elI-nEl <= idx && idx < elI) nEl--;

  if (nEl > 0) {
    if (copyInputName_) {
      addNameAppendField( name, nameAppend_, nEl );
    } else {
      addNameAppendField( NULL, nameAppend_, nEl );
    }
  }
  return nEl;
}

int cValbasedSelector::myFinaliseInstance()
{
  return cDataProcessor::myFinaliseInstance();
}


int cValbasedSelector::myTick(long long t)
{
  SMILE_IDBG(4,"tick # %i, processing value vector",t);

  // get next frame from dataMemory
  cVector *vec = reader_->getNextFrame();
  if (vec == NULL) {
    return 0;
  }

  // add offset
  int i = idx;
  if (i >= vec->N) i = vec->N-1;
  FLOAT_DMEM val = vec->dataF[i];
  
  int copy = 0;
  if ( ((invert)&&(val < threshold)) || ((!invert)&&(val > threshold)) || (allowEqual && (val==threshold)) ) {
    copy = 1;
  }
  
  if (copy) {
    // TODO: remove threshold value if removeIdx == 1
    if (removeIdx == 1) {

      if (myVec==NULL) myVec = new cVector(vec->N-1 , vec->type);
      int j,n=0;
      for(j=0; j<vec->N; j++) {
        if (j!=i) {
          myVec->dataF[n] = vec->dataF[j];
          n++;
        }
      }
      myVec->tmetaReplace(vec->tmeta);
      //myVec->tmeta->cloneFrom();
      // save to dataMemory
      writer_->setNextFrame(myVec);
      //delete myVec;

    } else {
      // save to dataMemory
      writer_->setNextFrame(vec);
    }
  } else {
    if (zerovec==1) {
      int j;
      // output a zero (or predefined value) vector
      if (removeIdx == 1) {
        if (myVec==NULL) myVec = new cVector(vec->N-1 , vec->type);
        for(j=0; j<myVec->N; j++) {
          myVec->dataF[j] = outputVal;
        }
        myVec->tmetaReplace(vec->tmeta);
        writer_->setNextFrame(myVec);
      } else {
        for(j=0; j<vec->N; j++) {
          vec->dataF[j] = outputVal;
        }
        writer_->setNextFrame(vec);
      }
    }
  }

  return 1;
}


cValbasedSelector::~cValbasedSelector()
{
  // cleanup...
  if (myVec!=NULL) delete myVec;
}

