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


/*  openSMILE component: htkSink

HTK binary feature file output

*/


#include <iocore/htkSink.hpp>

#define MODULE "cHtkSink"


SMILECOMPONENT_STATICS(cHtkSink)

SMILECOMPONENT_REGCOMP(cHtkSink)
{
  SMILECOMPONENT_REGCOMP_INIT;
  scname = COMPONENT_NAME_CHTKSINK;
  sdescription = COMPONENT_DESCRIPTION_CHTKSINK;

  // we inherit cDataSink configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cDataSink");
  
  SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("filename","HTK parameter file to write to (and create)","smileoutput.htk");
    ct->setField("lag","If > 0, enable output of data <lag> frames behind",0,0,0);
    ct->setField("append","1 = append to existing file (0 = don't append)",0);
    ct->setField("parmKind","HTK parmKind header field (0=WAVEFORM, 1=LPC, 2=LPREFC, 3=LPCEPSTRA, 4=LPDELCEP, 5=IREFC, 6=MFCC, 7=FBANK (log), 8=MELSPEC (linear), 9=USER, 10=DISCRETE, 11=PLPCC ;\n   Qualifiers (added): 64=_E, 128=_N, 256=_D, 512=_A, 1024=_C, 2048=_Z, 4096=_K, 8192=_0)",9);
  )

  SMILECOMPONENT_MAKEINFO(cHtkSink);
}

SMILECOMPONENT_CREATE(cHtkSink)

//-----

cHtkSink::cHtkSink(const char *_name) :
  cDataSink(_name),
  filehandle(NULL),
  filename(NULL),
  nVec(0),
  vecSize(0),
  period(0.0)
{
  bzero(&header, sizeof(sHTKheader));
  if ( smileHtk_IsVAXOrder() ) vax = 1;
  else vax = 0;
}

void cHtkSink::fetchConfig()
{
  cDataSink::fetchConfig();
  
  filename = getStr("filename");
  SMILE_IDBG(2,"filename = '%s'",filename);

  lag = getInt("lag");
  SMILE_IDBG(2,"lag = %i",lag);

  append = getInt("append");
  if (append) { SMILE_IDBG(2,"append to file is enabled"); }

  parmKind = (uint16_t)getInt("parmKind");
  SMILE_IDBG(2,"parmKind = %i",parmKind);
}

/*
int cHtkSink::myConfigureInstance()
{
  int ret=1;
  ret *= cDataSink::myConfigureInstance();
  // ....
  //return ret;
}
*/

int cHtkSink::writeHeader()
{
  if (filehandle==NULL) return 0;

  header.nSamples = nVec;
  header.samplePeriod = (uint32_t)round(period*10000000.0);
  if ((uint32_t)sizeof(float) * vecSize >= (uint32_t)2<<16) {
    SMILE_IERR(1,"vecSize overflow for HTK output: vecSize (%i) > max. HTK vecSize (%i)! limiting vecSize",(uint32_t)sizeof(float) * (uint32_t)vecSize,(uint32_t)2<<16);
    vecSize = (2<<16) - 1;
  }
  header.sampleSize = (uint16_t)(sizeof(float) * vecSize);
  header.parmKind = parmKind;  

  return smileHtk_writeHeader(filehandle,&header);
}

/*
int cHtkSink::readHeader()
{
  if (filehandle==NULL) return 0;
  if (!fread(&header, sizeof(sHTKheader), 1, filehandle)) {
    SMILE_IERR(1,"error reading header from file '%s'",filename);
    return 0;
  }
  prepareHeader(&header); // convert to host byte order
  return 1;
}
*/

int cHtkSink::myFinaliseInstance()
{
  int ap=0;

  int ret = cDataSink::myFinaliseInstance();
  if (ret==0) return 0;

  period = reader_->getLevelT();
  vecSize = reader_->getLevelN();
  
  if (append) {
    // check if file exists:
    filehandle = fopen(filename, "rb");
    if (filehandle != NULL) {
      if (!readHeader()) {
        SMILE_ERR(1,"error reading header from file '%s' (which seems to exist)! we cannot append to that file!");
        // TODO: force overwrite via config file option in this case...
        ret = 0;
      }
      if (ret) {
        if (header.samplePeriod != (uint32_t)round(period*10000000.0)) {
          SMILE_ERR(1,"cannot append to '%s': samplePeriod mismatch (should be: %i, in file on disk: %i)",filename,(uint32_t)round(period*10000000.0),header.samplePeriod);
          ret = 0;
        }
        if (header.sampleSize != (uint16_t)(sizeof(float) * vecSize)) {
          SMILE_ERR(1,"cannot append to '%s': sampleSize mismatch (should be: %i, in file on disk: %i)",filename,(uint16_t)(sizeof(float) * vecSize),header.sampleSize);
          ret = 0;
        }
      }
      nVec = header.nSamples;
      fclose(filehandle); filehandle = NULL;
      if (ret==0) return 0;
      filehandle = fopen(filename, "ab");
      ap=1;
    } else {
      filehandle = fopen(filename, "wb");
    }
  } else {
    filehandle = fopen(filename, "wb");
  }
  if (filehandle == NULL) {
    COMP_ERR("Error opening binary file '%s' for writing (component instance '%s', type '%s')",filename, getInstName(), getTypeName());
  }
  
  if (!ap) {
    // write dummy htk header ....
    writeHeader();
  }
  
  return ret;
}


int cHtkSink::myTick(long long t)
{
  if (filehandle == NULL) return 0;

  SMILE_DBG(4,"tick # %i, reading value vector (lag=%i):",t,lag);
  cVector *vec= reader_->getFrameRel(lag);  //new cVector(nValues+1);
  if (vec == NULL) return 0;

  // now print the vector:
  int i; float *tmp = (float *)malloc(sizeof(float)*vec->N);
  if (tmp==NULL) OUT_OF_MEMORY;

  if (vec->type == DMEM_FLOAT) {
    for (i=0; i<vec->N; i++) {
      tmp[i] = (float)(vec->dataF[i]);
      if (vax) smileHtk_SwapFloat(tmp+i);
    }
  } else if (vec->type == DMEM_INT) {
    for (i=0; i<vec->N; i++) {
      tmp[i] = (float)(vec->dataI[i]);
      if (vax) smileHtk_SwapFloat(tmp+i);
    }
  } else {
    SMILE_ERR(1,"unknown data type %i",vec->type);
    return 0;
  }

  int ret = 1;
  
  if (!fwrite(tmp,sizeof(float),vec->N,filehandle)) {
    SMILE_ERR(1,"Error writing to raw feature file '%s'!",filename);
    ret = 0;
  } else {
    //reader->nextFrame();
    nVec++;
  }

  free(tmp);

  // tick success
  return ret;
}


cHtkSink::~cHtkSink()
{
  writeHeader();
  fclose(filehandle);
}

