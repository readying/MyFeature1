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

compute jitter from waveform and F0 estimate

This component has 2 readers (!)

*/


#include <lld/pitchJitter.hpp>

#define MODULE "cPitchJitter"

SMILECOMPONENT_STATICS(cPitchJitter)

SMILECOMPONENT_REGCOMP(cPitchJitter)
{
  SMILECOMPONENT_REGCOMP_INIT

  scname = COMPONENT_NAME_CPITCHJITTER;
  sdescription = COMPONENT_DESCRIPTION_CPITCHJITTER;

  // we inherit cDataProcessor configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cDataProcessor")
  if (ct->setField("F0reader", "Configuration of the dataMemory reader sub-component which is used to read the F0 estimate from a pitch component output (e.g. cPitchShs).",
                  sconfman->getTypeObj("cDataReader"), NO_ARRAY, DONT_FREE) == -1) {
     rA=1; // if subtype not yet found, request , re-register in the next iteration
  }
/*
if (ct->setField("exWriter", "dataMemory writer for pcm voice excitation signal",
                  sconfman->getTypeObj("cDataReader"), NO_ARRAY, DONT_FREE) == -1) {
     rA=1; // if subtype not yet found, request , re-register in the next iteration
  }
  */
  SMILECOMPONENT_IFNOTREGAGAIN(
    //ct->setField("F0dmLevel","data memory level to read f0 estimate from","pitch");
    ct->setField("F0field","The name of the field in 'F0reader.dmLevel' containing the F0 estimate (in Hz) (usually F0final or F0raw)","F0final");
    ct->setField("searchRangeRel","The relative search range for period deviations (Jitter): maxT0, minT0 = (1.0 +/- searchRangeRel)*T0",0.25);
    ct->setField("jitterLocal","1 = enable computation of F0 jitter (period length variations). jitterLocal = the average absolute difference between consecutive periods, divided by the average period length of all periods in the frame",0);
    ct->setField("jitterDDP","1 = enable computation of F0 jitter (period length variations). jitterDDP = the average absolute difference between consecutive differences between consecutive periods, divided by the average period length of all periods in the frame",0);
    ct->setField("jitterLocalEnv","1 = compute envelope of jitterLocal (i.e. fill jitter values in unvoiced frames with value of last voiced segment). Use this in conjunction with statistical functionals such as means.",0);
    ct->setField("jitterDDPEnv","1 = compute envelope of jitterDDP (i.e. fill jitter values in unvoiced frames with value of last voiced segment). Use this in conjunction with statistical functionals such as means.",0);
    
    ct->setField("shimmerLocal","1 = enable computation of F0 shimmer (amplitude variations). shimmerLocal = the average absolute difference between the interpolated peak amplitudes of consecutive periods, divided by the average peak amplitude of all periods in the frame", 0);
    ct->setField("shimmerLocalDB","1 = enable computation of F0 shimmer (amplitude variations) in decibel (dB). shimmerLocal = the average absolute difference between the interpolated peak amplitudes of consecutive periods, divided by the average peak amplitude of all periods in the frame", 0);
    ct->setField("shimmerLocalEnv","1 = compute envelope of shimmerLocal (i.e. fill shimmer values in unvoiced frames with value of last voiced segment). Use this in conjunction with statistical functionals such as means.", 0);
    ct->setField("shimmerLocalDBEnv","1 = compute envelope of shimmerLocalDB (i.e. fill shimmer values in unvoiced frames with value of last voiced segment). Use this in conjunction with statistical functionals such as means.", 0);

    ct->setField("harmonicERMS","1 = output of harmonic component RMS energy (energy average period waveform).",0);
    ct->setField("noiseERMS","1 = output of noise component RMS energy (energy of difference signal between repeated average period waveform and actual signal).",0);
    ct->setField("linearHNR","1 = output of harmonics to noise ratio computed from waveform signal (= harmonicERMS/noiseERMS)",0);
    ct->setField("logHNR","1 = output of logarithmic harmonics to noise ratio computed from waveform signal, using natural logarithm (base e) (logHNR = log(harmonicERMS/noiseERMS) )",0);
    ct->setField("lgHNRfloor","minimal value logHNR can be, to avoid very large negative numbers for small harmonic energies.",-100.0);
    // TODO: this option is not fully supported by the functionals component, for example.
    // winToVecProcessor cannot deal with variable frame rate, i.e. does not yet support collecting frames by looking at tmeta fields...
    ct->setField("onlyVoiced","1 = produce output only for voiced frames. I.e. do not output 0 jitter/shimmer values for unvoiced frames. WARNING: this option is not fully supported by the functionals component, yet.",0);

    ct->setField("periodLengths","1 = enable output of individual period lengths",0);
    ct->setField("periodStarts","1 = enable output of individual period start times",0);
    ct->makeMandatory(ct->setField("inputMaxDelaySec", "The maximum possible delay of the F0 input wrt. to the waveform in seconds. This occurs mainly for viterbi smoothing, for example. IT IS IMPORTANT that you set this parameter with care (summing up all delays like bufferLength of the viterbi smoother, etc.), otherwise the processing will hang or abort before the actual end of the input!", 2.0));
  )
  
  SMILECOMPONENT_MAKEINFO(cPitchJitter);
}


SMILECOMPONENT_CREATE(cPitchJitter)

//-----

cPitchJitter::cPitchJitter(const char *_name) :
  cDataProcessor(_name), minF0(25.0), // <- TODO: read this from pitch field metadata
    lastIdx(0), lastMis(0), out(NULL), F0reader(NULL), lastT0(0.0), lastDiff(0.0),
    Nout(0), lastJitterLocal(0.0), lastJitterDDP(0.0), lastShimmerLocal(0.0),
    lastJitterLocal_b(0.0), lastJitterDDP_b(0.0), lastShimmerLocal_b(0.0)
{
  char *tmp = myvprint("%s.F0reader",getInstName());
  F0reader = (cDataReader *)(cDataReader::create(tmp));
  if (F0reader == NULL) {
    COMP_ERR("Error creating dataReader '%s'",tmp);
  }
  if (tmp!=NULL) free(tmp);
}

void cPitchJitter::fetchConfig()
{
  cDataProcessor::fetchConfig();

  // load all configuration parameters you will later require fast and easy access to here:

  F0field = getStr("F0field");
  SMILE_IDBG(2,"F0field = '%s'",F0field);
  
  searchRangeRel = getDouble("searchRangeRel");
  SMILE_IDBG(2,"searchRangeRel = %f",searchRangeRel);

  onlyVoiced = getInt("onlyVoiced");
  SMILE_IDBG(2,"onlyVoiced = %i",onlyVoiced);

  jitterLocal = getInt("jitterLocal");
  SMILE_IDBG(2,"jitterLocal = %i",jitterLocal);
  jitterDDP = getInt("jitterDDP");
  SMILE_IDBG(2,"jitterDDP = %i",jitterDDP);

  jitterLocalEnv = getInt("jitterLocalEnv");
  SMILE_IDBG(2,"jitterLocalEnv = %i",jitterLocalEnv);
  jitterDDPEnv = getInt("jitterDDPEnv");
  SMILE_IDBG(2,"jitterDDPEnv = %i",jitterDDPEnv);

  shimmerLocalDB = getInt("shimmerLocalDB");
  shimmerLocal = getInt("shimmerLocal");
  SMILE_IDBG(2,"shimmerLocal = %i",shimmerLocal);
  shimmerLocalDBEnv = getInt("shimmerLocalDBEnv");
  shimmerLocalEnv = getInt("shimmerLocalEnv");
  SMILE_IDBG(2,"shimmerLocalEnv = %i",shimmerLocalEnv);

  harmonicERMS = getInt("harmonicERMS");
  noiseERMS = getInt("noiseERMS");
  linearHNR = getInt("linearHNR");
  logHNR = getInt("logHNR");
  lgHNRfloor = (FLOAT_DMEM)getDouble("lgHNRfloor");
  input_max_delay_ = getInt("inputMaxDelaySec");
}




int cPitchJitter::configureReader(const sDmLevelConfig &c)
{
  // check if F0 level period and pcm input period match
  double Tf0 = F0reader->getLevelT();
  //double T = reader_->getLevelT();
  if (c.T == Tf0) {
    SMILE_IERR(1,"pcm level frame period must be << F0 level frame period! pcm data should be stream data and not frame data!");
  }

  // set blocksize of wave reader to 5*max pitch period (2.0/minF0)
  blocksizeR_sec_ = (long)ceil(5.0/(minF0)) + input_max_delay_;
  if (c.T > 0.0) {
    blocksizeR_ = (long) ceil (blocksizeR_sec_ / c.T);
  } else {
    SMILE_IERR(1, "Cannot properly set the reader blocksize in frames from blocksize in seconds, as the input level is not periodic (lcfg.T==0!). Non-periodic waveform input levels are not supported for pitchJitter!");
    return 0;
  }
  // FIXME: when the F0 is delayed due to post-smoothing (viterbi, etc.)
  //        then data in the wave level might have expired, or reading
  //        will block the processing.
  //        Thus, we must signal the wave reader the max possible delay
  //        so that the wave buffersize can be set correctly.
  //        We don't know the delay, as there is no standard mechanism
  //        we can use to read it from the pitch components.
  //        Therefore we provide a mandatory config option and let the user
  //        set it correctly!
  int ret = cDataProcessor::configureReader(c);
  F0reader->setBlocksize(1);
  return ret;
}

void cPitchJitter::mySetEnvironment()
{
  cDataProcessor::mySetEnvironment();
  F0reader->setComponentEnvironment(getCompMan(), -1, this);
}

int cPitchJitter::myRegisterInstance(int *runMe)
{
  int ret = cDataProcessor::myRegisterInstance(runMe);
  ret *= F0reader->registerInstance();
  return ret;
}

int cPitchJitter::myConfigureInstance()
{
  if (!(F0reader->configureInstance())) return 0;
  if (!(F0reader->finaliseInstance())) return 0;

  int ret = cDataProcessor::myConfigureInstance();
  return ret;
}

int cPitchJitter::configureWriter(sDmLevelConfig &c) 
{
  // check that we have mono input!!
  if (c.fmeta->Ne > 1) {
    SMILE_IERR(1,"this component must read mono (1 channel) wave input (your input currently has %i channels)! Use the monomixdown option in the wave-source!",c.fmeta->Ne);
    return 0;
  }

  const sDmLevelConfig *cfg = F0reader->getLevelConfig();
  c.T = cfg->T;
  c.frameSizeSec = cfg->frameSizeSec;
  c.basePeriod = cfg->basePeriod;
  c.growDyn = cfg->growDyn;
  c.isRb = cfg->isRb;
  c.lastFrameSizeSec = cfg->frameSizeSec;
  c.lenSec = cfg->lenSec;
  return 1; /* success */
}

/* You shouldn't need to touch this....
int cPitchJitter::myConfigureInstance()
{
  int ret = cDataProcessor::myConfigureInstance();
  return ret;
}
*/

/*
  Do what you like here... this is called after the input names and number of input elements have become available, 
  so you may use them here.
*/
/*
int cPitchJitter::dataProcessorCustomFinalise()
{
  
  return 1;
}
*/


/* 
  Use setupNewNames() to freely set the data elements and their names in the output level
  The input names are available at this point, you can get them via reader->getFrameMeta()
  Please set "namesAreSet" to 1, when you do set names
*/

int cPitchJitter::setupNewNames(long nEl) 
{
  // find pitch input field:
  const FrameMetaInfo * fmeta = F0reader->getFrameMetaInfo();
  int ri=0;
  long idx = fmeta->findFieldByPartialName( F0field , &ri );
  if (nEl <= 0) nEl = reader_->getLevelN();
  if (idx < 0) {
    F0fieldIdx = 0;
    SMILE_IWRN(2,"Requested input field '*%s*' not found, defaulting to use 0th field!",F0field);
  } else {
    F0fieldIdx = fmeta->fieldToElementIdx( idx ) + ri;
  }
  
  int n=0;
  if (jitterLocal) { writer_->addField("jitterLocal",1); n++; }
  if (jitterDDP) { writer_->addField("jitterDDP",1); n++; }
  if (jitterLocalEnv) { writer_->addField("jitterLocEnv",1); n++; }
  if (jitterDDPEnv) { writer_->addField("jitterDEnv",1); n++; }
  if (shimmerLocal) { writer_->addField("shimmerLocal",1); n++; }
  if (shimmerLocalDB) { writer_->addField("shimmerLocalDB",1); n++; }
  if (shimmerLocalEnv) { writer_->addField("shimmerLocEnv",1); n++; }
  if (shimmerLocalDBEnv) { writer_->addField("shimmerLocDBEnv",1); n++; }
  if (harmonicERMS) { writer_->addField("harmonicERMS",1); n++; }
  if (noiseERMS) { writer_->addField("noiseERMS",1); n++; }
  if (linearHNR) { writer_->addField("linearHNR",1); n++; }
  if (logHNR) { writer_->addField("logHNR",1); n++; }
  namesAreSet_ = 1;
  Nout = n;
  return n;
}

/*
  If you don't use setupNewNames() you may set the names for each input field by overwriting the following method:
*/
/*
int cPitchJitter::setupNamesForField( TODO )
{
  // DOC TODO...
}
*/

int cPitchJitter::myFinaliseInstance()
{
  int ret = cDataProcessor::myFinaliseInstance();
  if (ret) {
    // do all custom init stuff here... 
    // e.g. allocating and initialising memory (which is not used before, e.g. in setupNames, etc.),
    // loading external data, 
    // etc.

    // ...

  }
  return ret;
}

double cPitchJitter::crossCorr(FLOAT_DMEM *x, long Nx, FLOAT_DMEM *y, long Ny)
{
  long N = MIN(Nx,Ny);
  long i;
  double cc = 0.0;
  double mx = 0.0;
  double my = 0.0;
  double nx = 0;
  double ny = 0;
  for (i=0; i<N; i++) {
    mx += x[i];
    my += y[i];
  }
  mx /= (double)N;
  my /= (double)N;
  for (i=0; i<N; i++) {
    cc += (x[i]-mx)*(y[i]-my);
    nx += (x[i]-mx)*(x[i]-mx);
    ny += (y[i]-mx)*(y[i]-mx);
  }
  cc /= sqrt(nx)*sqrt(ny);
  //cc /= (double)N;
  return cc;
}

// get difference of peak amplitude in the two given frames
// save interpolated index of max peak in first frame in *maxI0 if != NULL
// save interpolated index of max peak in second frame in *maxI1 if != NULL
FLOAT_DMEM cPitchJitter::amplitudeDiff(FLOAT_DMEM *x, long Nx, FLOAT_DMEM *y, long Ny, double *maxI0, double *maxI1, FLOAT_DMEM *_A0, FLOAT_DMEM *_A1)
{
  long i;
  long N = MIN(Nx,Ny);
  double A0=1.0, A1=1.0;
  // analyse first frame:
  long mI=1; FLOAT_DMEM max=x[1];
  for (i=1; i<Nx-1; i++) {
    if (x[i] > max) { max=x[i]; mI = i; }
  }
  double mi = smileMath_quadFrom3pts((double)(mI-1),x[mI-1],(double)(mI),x[mI],(double)(mI+1),x[mI+1],&A0,NULL);
  //printf("A0 %f  x[%i] %f  Nx=%i\n",A0,mI,x[mI],Nx);
  if (maxI0 != NULL) *maxI0 = mi;
  // analyse second frame:
  mI=1; max=y[1];
  for (i=1; i<Ny-1; i++) {
    if (y[i] > max) { max=y[i]; mI = i; }
  }
  mi = smileMath_quadFrom3pts((double)(mI-1),y[mI-1],(double)(mI),y[mI],(double)(mI+1),y[mI+1],&A1,NULL);
  if (maxI1 != NULL) *maxI1 = mi;
  //printf("A1 %f  x[%i] %f  Ny=%i\n",A1,mI,y[mI],Ny);
  
  // save amplitudes
  if (_A0 != NULL) *_A0 = (FLOAT_DMEM)A0;
  if (_A1 != NULL) *_A1 = (FLOAT_DMEM)A1;
  
  // compute relative amplitude difference:
  return (FLOAT_DMEM)fabs(A0-A1);
}

int cPitchJitter::myTick(long long t)
{
  if (isEOI()) return 0; // TODO: check if we need to flush jitter data OR we can still read from input....?

  long i;

  // get next pitch frame:
  cVector *fvec = F0reader->getNextFrame();
  if (fvec != NULL) {
    FLOAT_DMEM F0 = 0.0;
    if (F0fieldIdx < fvec->N) {
      F0 = fvec->dataF[F0fieldIdx];  // F0 estimate
    }
    long lenF = fvec->tmeta->lengthFrames;

    double T = reader_->getLevelT(); // sample period
    long startVidx = (long)round(fvec->tmeta->time / T);  // start in samples
    double pitchT = fvec->tmeta->period; // pitch frame period


    long ppLen = (long)ceil(pitchT/T);  // step size in samples of pitch frames
    long ovl = lenF - ppLen;  // pitch frame overlap (in samples)


    // New method to determine toRead samples:
    //  We need to read at least ppLen+lastMix, otherwise we fall behind the frame rate
    //  We need at least two pitch periods + serach range tolerance
    //  We must read at max. lenF+lastMis
    long toRead = ppLen+lastMis;  // number of samples we should read?
    double T0 = 0.0; // f0 period (seconds)
    double Tf = 0.0; // f0 period in samples, floating point
    long T0f = 0; // f0 period in samples as integer
    // upper / lower T0 bounds:
    double T0min = 0.0;  // lower bound of f0 period search range
    double T0max = 0.0;  // upper bound of f0 period search range
    long T0minF = 0;  // bounds as integer (samples)
    long T0maxF = 0;   // bounds as integer (samples)
    if (F0 > 0.0) {
      T0 = 1.0/F0;
      Tf = T0/T;
      T0f = (long)round(Tf);
      T0min = (1.0-searchRangeRel)*Tf;
      T0max = (1.0+searchRangeRel)*Tf;
      T0minF = (long)floor(T0min);
      T0maxF = (long)ceil(T0max);
      long two_pp = 2 * T0maxF + 2;
      if (toRead < two_pp) {
        toRead = two_pp;
      }
    }
    long maxRead = lastMis+lenF;  // maxmimum number of samples we may read...
    if (toRead > maxRead) {
      SMILE_IWRN(3, "It appears that we need to read more wave samples than the length of the input F0 frame to read two full pitch periods at F0=%f with tolerance (searchRangeRel = %f) : toRead = %i, maxRead = %i. Limiting toRead=maxRead.\n", F0, searchRangeRel, toRead, maxRead);
      toRead = maxRead;
    }

    // check start idx:
    if (startVidx - lastMis != lastIdx) {
      SMILE_IWRN(3,"Discontinuity in input indices... %i != %i  (v: %i, lastm: %i, tr %i, mr %i). This warning is ok, if you encounter it at the beginning and a pitchSmoother component is causing a delay of the pitch frames.", startVidx - lastMis, lastIdx, startVidx, lastMis, toRead, maxRead);
      lastIdx = startVidx;
      if (toRead > lenF) { toRead = lenF; } 
      if (maxRead > lenF) { maxRead = lenF; } 
    }
    
    //printf("F0(%i) = %f, vidx = %lld, lenF %lld  startV %lld, lastIdx %lld, toRead %lld maxRead %lld\n", F0fieldIdx, F0, fvec->tmeta->vIdx, lenF, startVidx, lastIdx, toRead, maxRead);
    // get pcm data
    cMatrix *mat = reader_->getMatrix(lastIdx, toRead /* = pitch frame length */);
    //cMatrix *mat = reader_->getMatrix(lastIdx, toRead /* = matrix length */);

    if (mat == NULL) {
      // TODO: print this error message only if lastIdx is well behind the current F0 frame vIdx equivalent!
      SMILE_IERR(1, "no pcm data read!  lastIdx %ld  toRead %ld. Check inpuMaxDelaySec option! It needs to be increased.", lastIdx, toRead);
      lastIdx += toRead;
      return 0;
    }
    if (maxRead < 1 || mat->dataF==NULL) {
      SMILE_IERR(1,"maxRead < 1 or mat->dataF==NULL, something is wrong (probably tmeta info on the pitch input level is not set correctly, thus the pitch frame length is read as 0; please debug the component that produces the pitch data this component reads!)");
      return 0;
    }

    // jitter computation variables:
    FLOAT_DMEM nPeriodsLocal = 0;
    FLOAT_DMEM nPeriodsDDP = 0;
    FLOAT_DMEM nPeriods = 0;
    FLOAT_DMEM avgPeriod = 0.0;
    FLOAT_DMEM JitterDDP=0.0;  // Praat: ddp
    FLOAT_DMEM JitterLocal=0.0;
    FLOAT_DMEM JitterPPQ=0.0;
    // shimmer computation variables:
    FLOAT_DMEM avgAmp=0.0;
    FLOAT_DMEM avgAmpDiff=0.0;
    // HNR computation variables:
    FLOAT_DMEM eH = 0.0;
    FLOAT_DMEM eN = 0.0;
    FLOAT_DMEM HNR = 0.0;
    FLOAT_DMEM lgHNR = 0.0;
    
    // start the waveform matching for voiced segments
    long start = 0;
    if (F0 > 0.0) { // voiced frame


      // create a buffer for storing pitch period boundaries (required for second pass HNR computation)
      int numPeriods = 0;
      long * periodBuffer;
      if (T0f > 0) periodBuffer = (long *)calloc(1,sizeof(long)*(maxRead/T0minF+3));
      else periodBuffer = (long *)calloc(1,sizeof(long)*(maxRead+2));

      // buffer for computing average period waveform
      FLOAT_DMEM * avgPeriodWf = (FLOAT_DMEM*)calloc(1,sizeof(FLOAT_DMEM) * (T0f+1));

      //// waveform matching  (TODO: sync to amplitude max.)
      double *cc = (double*)calloc(1,sizeof(double)*((int)(T0maxF-T0minF)+1));

      long os = start;
      long pp = 0;
      // TODO: check what happens, if the while loop below does not run at all!
      //       if pp is uninitialised, then valgrind will complain in the line:
      // for (j=periodBuffer[i]; j<MIN(periodBuffer[i+1],periodBuffer[i]+T0f); j++) {

      //printf("read N %i  2*T0maxF %i\n",mat->nT,2*T0maxF);
      // TODO: multi-channel (mat->N > 1) support!
      //printf("  start %i  toRead %i  T0maxF %i  mat-nt %i\n",start, toRead, 2*T0maxF, mat->nT);
      while (( start< mat->nT - 2*T0maxF ) && (start < toRead)) {
        long _Tf;
        for (_Tf = T0minF; _Tf <= T0maxF; _Tf++) {
          long mid = start+_Tf;
          //long end = start+2*_Tf;
          //if (end < vec->nT) {
          cc[_Tf-T0minF] = crossCorr(mat->dataF+start,_Tf,mat->dataF+mid,_Tf);
          //} 
// FIXME: cross correlation with previous and next period, then average to get better estimate for current period length?
// FIXME: re-evaluate period length and position after band-pass comb filtering (phase shift??) and peak detection for boundaries!
// FIXME: sync correlation to max amplitude boundaries once found  (via lastMis variable..)
        }

        // peak picking
        double max=cc[T0f-T0minF]; long maxI=-1;
        //printf("T0maxF %i T0minF %i T0 %f, F0 %f, T0min %f , T0max %f T %f\n",T0maxF,T0minF,T0,F0,T0min,T0max,T);
        for (i=1; i<T0maxF-T0minF-1; i++) {
          //printf("cc[%i] = %f\n",i,cc[i]);
          if ( (cc[i-1] < cc[i]) && (cc[i] > cc[i+1]) ) { // peak
            if (maxI == -1) {
              maxI = i; max = cc[i];
            } else {
              if (cc[i] > max) {
                maxI = i; max = cc[i];
              }
            }
          }
        }

        // assign new start
        os = start;
        pp = T0minF+maxI;
        

        // compute Jitter and Shimmer
        //printf("  jitter: %i  pp=%i (maxI: %i)\n",pp-T0f,pp,maxI);
        if (maxI >= 0) {
          start += pp;
          // get amplitude differences  for shimmer computation:
          double max0 = 0;
          FLOAT_DMEM _a0=0;
          FLOAT_DMEM ad=0;
 // FIXME: compute shimmer with a lag of one period... only if we know the end position of the next period, compute shimmer! 
 // FIXME: smooth the period waveforms with a narrow band-pass (comb!?) before computing amplitude to improve noise robustness!
 //        --> better:  generate a sine (not dirac pulse... because f0 alone is sinusoidal) and correlate that with a single period to obtain period phase and magnitude...?
          ad = amplitudeDiff(mat->dataF+os, pp, mat->dataF+start, pp, &max0, NULL, &_a0, NULL);
          periodBuffer[numPeriods++] = os;
          // compute averaged pitch period waveform for HNR
          for (i=0; i<T0f; i++) {
            avgPeriodWf[i] += *(mat->dataF+(os+i));
          }

          // parabolic interpolation of maxI
          double conf = 0.0;
          double ccI = 0.0;
          double maxId = fabs(  ( (double)T0minF + smileMath_quadFrom3pts((double)(maxI-1),cc[maxI-1],(double)(maxI),cc[maxI],(double)(maxI+1),cc[maxI+1],&ccI,&conf) ) ) * T;
          
          // TODO: save instantaneous periods to extra file... with proper timestamps (e.g. for sonic visualiser).

          if (ccI > 0.5) { // detect valid period only if CC > 0.5 ! TODO: make this configurable

          avgPeriod += (FLOAT_DMEM)maxId;
          nPeriods += 1.0;

          if (lastT0 > 0.0) {
            FLOAT_DMEM diff = (FLOAT_DMEM)fabs(lastT0-maxId);
            JitterLocal += diff; 
            nPeriodsLocal += 1.0;
            if (lastDiff > 0.0) {
              JitterDDP += fabs(lastDiff-diff);
              nPeriodsDDP += 1.0;
            }
            lastDiff = diff;
          }
          lastT0 = (FLOAT_DMEM)maxId;

          // debug output
          // (TODO: save wave data properly to a pcm level, and save period start/end times..
          // TODO: save average waveform to output level...
           //saveFloatDmemVectorWlen_bin("dataJ.dat",mat->dataF+os,start-os,1);

          // Shimmer:
          avgAmp += _a0;
          avgAmpDiff += ad;

          }
        } else {
//          printf("  T0f: %i\n",T0f);
          start += T0f;
        }
      }


      periodBuffer[numPeriods++] = start;

      //HNR, for last period not yet processed: compute averaged pitch period waveform for HNR
      // AND energy of averaged pitch period waveform (harmonic energy: H)
      FLOAT_DMEM Eh = 0.0;
      for (i=0; i<T0f && start+i < mat->nT; i++) {
        avgPeriodWf[i] += *(mat->dataF+(start+i));
        avgPeriodWf[i] /= (FLOAT_DMEM)numPeriods;
        if (i>2 && i<T0f-2) /* skip possibly unreliable beginning and end frames */
          Eh += avgPeriodWf[i]*avgPeriodWf[i];
      }

      if (T0f-4 > 0) {
        Eh /= (FLOAT_DMEM)(T0f - 4);
      }
      Eh = sqrt(Eh);

      //HNR, 2nd pass: compute energy of diff between each period and averaged waveform (noise energy N)
      long j;
      FLOAT_DMEM En = 0.0; long nEn = 0;
      if (pp > 0) {
        periodBuffer[numPeriods++] = start+pp;
      }

      for (i=0; i<numPeriods-1; i++) {
        long n = 0;
        for (j=periodBuffer[i]; j<MIN(periodBuffer[i+1],periodBuffer[i]+T0f); j++) {
          FLOAT_DMEM delta = mat->dataF[j] - avgPeriodWf[n++];
          En += delta*delta; nEn++;
        }
      }
      if (nEn > 0) En /= (FLOAT_DMEM)nEn;
      En = sqrt(En);

      // export variables
      eH = Eh;
      eN = En;
      if (En > 0.0) {
        HNR = Eh/En;
        if (HNR > 0.0) {
          lgHNR = log(HNR);
        } else {
          lgHNR = lgHNRfloor;
        }
      }

      lastMis = toRead - start;

      free(cc);
      free(periodBuffer);
      free(avgPeriodWf);

    } else { // for unvoiced frames:
      start = toRead;
      lastMis = 0;
      // reset memory:
      lastT0 = 0.0; lastDiff=0.0;
      lastJitterDDP = 0.0; lastJitterLocal = 0.0;
      lastShimmerLocal = 0.0;

      //saveFloatDmemVectorWlen_bin("dataJ.dat",mat->dataF,toRead,1);  

      // HNR:
      // noise energy is now signal RMS energy.. TODO!
      if (noiseERMS || linearHNR || logHNR) {
        long i; double E=0.0;
        for (i=0; i<mat->nT; i++) {
          E += mat->dataF[i]*mat->dataF[i];
        }
        E /= (double)mat->nT;

        // export variables
        eH = 0.0;
        HNR = 0.0;
        eN = (FLOAT_DMEM)sqrt(E);
        lgHNR = lgHNRfloor;
      }
    }

    lastIdx += start;

    // build output vector:
    if (Nout == 0) return 0;

    if (onlyVoiced && (F0==0.0)) { return 0; }

    long n=0;
    if (out == NULL) out = new cVector(Nout,DMEM_FLOAT);
    if (out == NULL) OUT_OF_MEMORY;

    
    if ((nPeriods>0.0)&&(nPeriodsLocal > 0.0)&&(F0>0.0)) {
      JitterLocal /= nPeriodsLocal;
      lastJitterLocal_b = lastJitterLocal = JitterLocal / (avgPeriod/nPeriods);
    }
    if (jitterLocal) {
      //printf("avgperiod = %f (over %f)  (F0=%f)\n", 1.0/(avgPeriod/nPeriods), nPeriods, F0);
      if ((nPeriods>0.0)&&(nPeriodsLocal > 0.0)&&(F0>0.0)) {
        //JitterLocal /= nPeriodsLocal;
        if (lastJitterLocal > 1.0) lastJitterLocal = 1.0;
        out->dataF[n] = lastJitterLocal; // = JitterLocal / (avgPeriod/nPeriods);
      } else {
        if ((nPeriods == 0.0)&&(F0>0.0)) {
          if (lastJitterLocal > 1.0) lastJitterLocal = 1.0;
          out->dataF[n] = lastJitterLocal;
        } else {
          out->dataF[n] = 0.0;
        }
      }
      n++;
    }
    if (jitterLocalEnv) {
      if (lastJitterLocal_b > 1.0) lastJitterLocal_b = 1.0;
      out->dataF[n] = lastJitterLocal_b;
      n++;
    }

    if ((nPeriods>0.0)&&(nPeriodsDDP > 0.0)&&(F0>0.0)) {
      JitterDDP /= nPeriodsDDP;
      lastJitterDDP_b = lastJitterDDP = JitterDDP / (avgPeriod/nPeriods);
    }
    if (jitterDDP) {
      if ((nPeriods>0.0)&&(nPeriodsDDP > 0.0)&&(F0>0.0)) {
        //JitterDDP /= nPeriodsDDP;
        if (lastJitterDDP > 1.0) lastJitterDDP = 1.0;
        out->dataF[n] = lastJitterDDP;
      } else {
        if ((nPeriods == 0.0)&&(F0>0.0)) {
          if (lastJitterDDP > 1.0) lastJitterDDP = 1.0;
          out->dataF[n] = lastJitterDDP;
        } else {
          out->dataF[n] = 0.0;
        }
      }
      n++;
    }

    if (jitterDDPEnv) {
      if (lastJitterDDP_b > 1.0) lastJitterDDP_b = 1.0;
      out->dataF[n] = lastJitterDDP_b;
      n++;
    }
    
    if ((nPeriods>0.0)&&(F0>0.0)) {
      if (avgAmp > 0.0) {
        lastShimmerLocal_b = lastShimmerLocal = (avgAmpDiff/avgAmp);
      } else {
        lastShimmerLocal=0.0;
      }
    }
    if (shimmerLocal || shimmerLocalDB) { // shimmer local
      if ((nPeriods>0.0)&&(F0>0.0)) {
        if (lastShimmerLocal > 1.0) lastShimmerLocal = 1.0;
        if (shimmerLocal) {
          out->dataF[n] = lastShimmerLocal;
          n++;
        } 
        if (shimmerLocalDB) {
          out->dataF[n] = (FLOAT_DMEM)smileDsp_amplitudeRatioToDB(lastShimmerLocal + 1.0);
          n++;
        }
      } else {
        if ((nPeriods == 0.0)&&(F0>0.0)) {
          if (lastShimmerLocal > 1.0) lastShimmerLocal = 1.0;
          if (shimmerLocal) {
            out->dataF[n] = lastShimmerLocal;
            n++;
          }
          if (shimmerLocalDB) {
            out->dataF[n] = (FLOAT_DMEM)smileDsp_amplitudeRatioToDB(lastShimmerLocal + 1.0);
            n++;
          }
        } else {
          if (shimmerLocal) {
            out->dataF[n] = 0.0;
            n++;
          }
          if (shimmerLocalDB) {
            out->dataF[n] = 0.0;
            n++;
          }
        }
      }
    }
    if (shimmerLocalEnv) {
      if (lastShimmerLocal_b > 1.0) lastShimmerLocal_b = 1.0;
      out->dataF[n] = lastShimmerLocal_b;
      n++;
    }

    if (harmonicERMS) {
//      printf("XX   Eh %f\n", eH);
      out->dataF[n++] = eH;
    }
    if (noiseERMS) {
      out->dataF[n++] = eN;
    }
    if (linearHNR) {
      out->dataF[n++] = HNR;
    }
    if (logHNR) {
      if (lgHNR < lgHNRfloor) lgHNR = lgHNRfloor;
      out->dataF[n++] = lgHNR;
    }

    out->tmetaReplace(fvec->tmeta);
    writer_->setNextFrame(out);

    return 1;
  }


////////////////////////////////////

#if 0
    double startVidx = fvec->tmeta->time / fvec->tmeta->period;
    
    double pitchT = fvec->tmeta->period; // pitch frame period
    double T = reader_->getLevelT(); // sample period

    double T0 = 1.0/F0; // pitch period

    long toRead = (long)ceil((pitchT+T0)/T)+lastMis;
    //double len = fvec->tmeta->lengthSec;

    double Tf = T0/T; // f0 period in frames (samples)
    long T0f = (long)round(Tf);

    // upper / lower T0 bounds:
    double T0min = (1.0-searchRangeRel)*T0;
    double T0max = (1.0+searchRangeRel)*T0;
    long T0minF = (long)floor(T0min/T);
    long T0maxF = (long)ceil(T0max/T);

    // maxmimum number of frames we may read...
    long maxAvail = lenF+lastMis;

    // get pcm data
    long nRead = maxAvail; // MIN(maxAvail,toRead);
    cMatrix *mat = reader_->getMatrix(lastIdx,nRead);

    lastIdx += nRead;
    if (mat == NULL) { 
      // frame not available..? If F0 frame was available this can only mean we have to catch up...
      
      // TODO : write NULL jitter/shimmer frame for proper sync?
      //...
      printf("Not avail\n");
      return 0; 
    }

    if (F0 > 0.0) {
      // waveform matching  (TODO: sync to amplitude max.)
      double *cc = (double*)calloc(1,sizeof(double)*(T0maxF-T0minF+1));

      long start = 0;
      //printf("read N %i  2*T0maxF %i\n",mat->nT,2*T0maxF);
      // TODO: multi-channel (mat->N > 1) support!
      while (start<mat->nT - 2*T0maxF) {
        long _Tf;
        for (_Tf = T0minF; _Tf <= T0maxF; _Tf++) {
          long mid = start+_Tf;
          long end = start+2*_Tf;
          //if (end < vec->nT) {
          cc[_Tf-T0minF] = crossCorr(mat->dataF+start,_Tf,mat->dataF+mid,_Tf);
          //} 
        }

        // peak picking and (TODO: parabolic interpolation)
        double max=cc[T0f-T0minF]; long maxI=-1;
        //printf("T0maxF %i T0minF %i T0 %f, F0 %f, T0min %f , T0max %f T %f\n",T0maxF,T0minF,T0,F0,T0min,T0max,T);
        for (i=1; i<T0maxF-T0minF-1; i++) {
          printf("cc[%i] = %f\n",i,cc[i]);
          if ( (cc[i-1] < cc[i]) && (cc[i] > cc[i+1]) ) { // peak
            if (maxI == -1) {
              maxI = i; max = cc[i];
            } else {
              if (cc[i] > max) {
                maxI = i; max = cc[i];
              }
            }
          }
        }
        // TODO: parabolic interpolation of maxI for jitter comp. ??

        // assign new start
        long os = start;
        long pp = T0minF+maxI;
        if (maxI >= 0) {
          printf("jitter: %i  (maxI: %i)\n",pp-T0f,maxI);
          start += pp;
        } else {
          start += T0f;
          printf("no match\n");
        }
        saveFloatDmemVectorWlen_bin("dataJ.dat",mat->dataF+os,start-os,1);  


        // save period start...
      }



      free(cc);

      lastMis = nRead - start;

    } else {
      // compute lastMis (i.e.: maxAvail - lastPeriodEnd)
      lastMis = 0;
      //lastIdx += start;
    }
    
     // zero jitter/shimmer for unvoiced frames ?
  // lastIdx += lenF;
  // printf("unvoiced \n");
  //}
    
  // build output vector

  // save output
    //writer->setNextFrame(vecO);
    return 1;  
  } 
#endif
  return 0;

}


cPitchJitter::~cPitchJitter()
{
  // cleanup...
  if (out != NULL) delete out;
  if (F0reader != NULL) delete F0reader;
}

