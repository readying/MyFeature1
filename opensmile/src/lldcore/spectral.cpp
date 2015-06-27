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

compute spectral features such as flux, roll-off, centroid, etc.

*/


#include <lldcore/spectral.hpp>
#include <math.h>

#define MODULE "cSpectral"


SMILECOMPONENT_STATICS(cSpectral)

SMILECOMPONENT_REGCOMP(cSpectral)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CSPECTRAL;
  sdescription = COMPONENT_DESCRIPTION_CSPECTRAL;

  // we inherit cVectorProcessor configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cVectorProcessor")
  SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("squareInput","1/0 = square input values (e.g. if input is magnitude and not power spectrum)",1);
    ct->setField("bands","bands[n] = LoFrq[Hz]-HiFrq[Hz]  (e.g. 0-250), compute energy in this spectral band (only integer frequencies are allowed!)","250-650",ARRAY_TYPE);
    ct->setField("normBandEnergies","(1/0=yes/no) normalise the band energies to the total frame energy (-> relative band energies)",0);

    ct->setField("rollOff","rollOff[n] = X  (X in the range 0..1), compute X*100 percent spectral roll-off point",0.90,ARRAY_TYPE);
    ct->setField("flux","(1/0=yes/no) enable computation of spectral flux",1);
    ct->setField("centroid","(1/0=yes/no) enable computation of spectral centroid",1);
    ct->setField("maxPos","(1/0=yes/no) enable computation of position of spectral maximum",1);
    ct->setField("minPos","(1/0=yes/no) enable computation of position of spectral minimum",1);
    ct->setField("entropy","(1/0=yes/no) enable computation of spectral entropy",0);
    ct->setField("variance","(1/0=yes/no) enable computation of spectral variance (mpeg7: spectral spread)",0);
    ct->setField("skewness","(1/0=yes/no) enable computation of spectral skewness",0);
    ct->setField("kurtosis","(1/0=yes/no) enable computation of spectral kurtosis",0);
    ct->setField("slope","(1/0=yes/no) enable computation of spectral slope",0);

    ct->setField("sharpness","(1/0=yes/no) enable computation of psychoacoustic parameter 'sharpness'. In order to obtain proper values, use a bark scale spectrum as input (see cSpecScale component).",0);
    ct->setField("tonality","(1/0=yes/no) enable computation of consonance (ratio of consonance/dissonance, based on intervals between spectral peaks). (NOT YET IMPLEMENTED)",0);
    ct->setField("harmonicity","(1/0=yes/no) enable computation of harmonicity (mean of consecutive local min-max differences).",0);
    ct->setField("flatness","(1/0=yes/no) enable computation of spectral flatness (sfm = geometric_mean / arithmetic_mean of spectrum).",0);
    ct->setField("buggyRollOff", "(1/0=yes/no) for backwards feature set compatibility, enable buggy roll-off computation (pre May 2013, pre 2.0 release).", 0);
  )

  SMILECOMPONENT_MAKEINFO(cSpectral);
}

SMILECOMPONENT_CREATE(cSpectral)

//-----


cSpectral::cSpectral(const char *_name) :
  cVectorProcessor(_name),
  squareInput(1), nBands(0), nRollOff(0), entropy(0),
  bandsL(NULL), bandsH(NULL), rollOff(NULL),
  bandsLi(NULL), bandsHi(NULL),
  wghtLi(NULL), wghtHi(NULL),
  fsSec(-1.0), frq(NULL), frqScale(-1), frqScaleParam(0.0),
  buggyRollOff(0)
{}

void cSpectral::fetchConfig()
{
  cVectorProcessor::fetchConfig();
  
  normBandEnergies = getInt("normBandEnergies");

  squareInput = getInt("squareInput");
  if (squareInput) { SMILE_IDBG(2,"squaring of input values enabled"); }

  flux = getInt("flux");
  if (flux) { SMILE_IDBG(2,"spectral flux computation enabled"); }

  centroid = getInt("centroid");
  if (centroid) { SMILE_IDBG(2,"spectral centroid computation enabled"); }

  maxPos = getInt("maxPos");
  if (maxPos) { SMILE_IDBG(2,"spectral max. pos. computation enabled"); }

  minPos = getInt("minPos");
  if (minPos) { SMILE_IDBG(2,"spectral min. pos. computation enabled"); }

  entropy = getInt("entropy");
  if (entropy) { SMILE_IDBG(2,"spectral entropy computation enabled"); }

  variance = getInt("variance");
  if (variance) { SMILE_IDBG(2,"spectral variance computation enabled"); }
  skewness = getInt("skewness");
  if (skewness) { SMILE_IDBG(2,"spectral skewness computation enabled"); }
  kurtosis = getInt("kurtosis");
  if (kurtosis) { SMILE_IDBG(2,"spectral kurtosis computation enabled"); }
  slope = getInt("slope");
  if (slope) { SMILE_IDBG(2,"spectral slope computation enabled"); }
  buggyRollOff = getInt("buggyRollOff");

  int i;
  
  nBands = getArraySize("bands");
  if (nBands > 0) {
    bandsL = (long *)calloc(1,sizeof(long)*nBands);
    bandsH = (long *)calloc(1,sizeof(long)*nBands);
    for (i=0; i<nBands; i++) {
        char *tmp = strdup(getStr_f(myvprint("bands[%i]",i)));
        char *orig = strdup(tmp);
        int l = (int)strlen(tmp);
        int err=0;
        // remove spaces and tabs at beginning and end
//        while ( (l>0)&&((*tmp==' ')||(*tmp=='\t')) ) { *(tmp++)=0; l--; }
//        while ( (l>0)&&((tmp[l-1]==' ')||(tmp[l-1]=='\t')) ) { tmp[l-1]=0; l--; }
        // find '-'
        char *s2 = strchr(tmp,'-');
        if (s2 != NULL) {
          *(s2++) = 0;
          char *ep=NULL;
          long r1 = strtol(tmp,&ep,10);
          if ((r1==0)&&(ep==tmp)) { err=1; }
          else if (r1 < 0) {
                 SMILE_ERR(1,"(inst '%s') low frequency of band %i in '%s'  is out of range (allowed: [0..+inf])",getInstName(),i,orig);
               }
          ep=NULL;
          long r2 = strtol(s2,&ep,10);
          if ((r2==0)&&(ep==tmp)) { err=1; }
          else {
               if (r2 < 0) {
                 SMILE_ERR(1,"(inst '%s') high frequency of band %i in '%s'  is out of range (allowed: [0..+inf])",getInstName(),i,orig);
               }
             }
          if (!err) {
            if (r1 < r2) {
              bandsL[i] = r1;
              bandsH[i] = r2;
            } else {
              bandsL[i] = r2;
              bandsH[i] = r1;
            }
          }
        } else { err=1; }

        if (err==1) {
          SMILE_ERR(1,"(inst '%s') Error parsing bands[%i] = '%s'! (The band range must be X-Y, where X and Y are positive integer numbers specifiying frequency in Hertz!)",getInstName(),i,orig);
          bandsL[i] = -1;
          bandsH[i] = -1;
        }
        free(orig);
        free(tmp);
    }
  }
  
  nRollOff = getArraySize("rollOff");
  if (nRollOff > 0) {
    rollOff = (double*)calloc(1,sizeof(double)*nRollOff);
    for (i=0; i<nRollOff; i++) {
      rollOff[i] = getDouble_f(myvprint("rollOff[%i]",i));
      if (rollOff[i] < 0.0) {
        SMILE_ERR(1,"rollOff[%i] = %f is out of range (allowed 0..1), clipping to 0.0",i,rollOff[i]);
        rollOff[i] = 0.0;
      }
      else if (rollOff[i] > 1.0) {
        SMILE_ERR(1,"rollOff[%i] = %f is out of range (allowed 0..1), clipping to 1.0",i,rollOff[i]);
        rollOff[i] = 1.0;
      }
    }
  }

  sharpness = getInt("sharpness");
  if (sharpness) { SMILE_IDBG(2,"sharpness computation enabled"); }
  tonality = getInt("tonality");
  if (tonality) { SMILE_IDBG(2,"tonality computation enabled"); }
  harmonicity = getInt("harmonicity");
  if (harmonicity) { SMILE_IDBG(2,"harmonicity computation enabled"); }
  flatness = getInt("flatness");
  if (flatness) { SMILE_IDBG(2,"spectral flatness computation enabled"); }

}


int cSpectral::setupNamesForField(int i, const char*name, long nEl)
{
  int newNEl = 0;
  int ii;

  if (fsSec == -1.0) {
    const sDmLevelConfig *c = reader_->getLevelConfig();
    fsSec = c->frameSizeSec;
  }

  for (ii=0; ii<nBands; ii++) {
    if (isBandValid(bandsL[ii],bandsH[ii])) {
      char *xx = myvprint("%s_fband%i-%i",name,bandsL[ii],bandsH[ii]);
      writer_->addField( xx, 1 ); newNEl++; free(xx);
    }
  }

  for (ii=0; ii<nRollOff; ii++) {
    char *xx = myvprint("%s_spectralRollOff%.1f",name,rollOff[ii]*100.0);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }

  if (flux) {
    char *xx = myvprint("%s_spectralFlux",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (centroid) {
    char *xx = myvprint("%s_spectralCentroid",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (maxPos) {
    char *xx = myvprint("%s_spectralMaxPos",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (minPos) {
    char *xx = myvprint("%s_spectralMinPos",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (entropy) {
    char *xx = myvprint("%s_spectralEntropy",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (variance) {
    char *xx = myvprint("%s_spectralVariance",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (skewness) {
    char *xx = myvprint("%s_spectralSkewness",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (kurtosis) {
    char *xx = myvprint("%s_spectralKurtosis",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (slope) {
    char *xx = myvprint("%s_spectralSlope",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (sharpness) {
    char *xx = myvprint("%s_psySharpness",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (tonality) {
    char *xx = myvprint("%s_tonality",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (harmonicity) {
    char *xx = myvprint("%s_harmonicity",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }
  if (flatness) {
    char *xx = myvprint("%s_flatness",name);
    writer_->addField( xx, 1 ); newNEl++; free(xx);
  }

  return newNEl;
}

#if 0
double smileDsp_getSharpnessWeightG(double frq, int frqScale, double param)
{
  if (frqScale != SPECTSCALE_BARK) {
    // transform to linear scale...
    frq = smileDsp_specScaleTransfInv(frq,frqScale,param);
    // ...then from linear to bark
    frq = smileDsp_specScaleTransfFwd(frq,SPECTSCALE_BARK,0.0);
  }
  // get Zwicker's weighting g(z):
  if (frq <= 16.0) {
    return 1.0;
  } else {
    return pow( (frq-16.0)/4.0, 1.5849625 /*log(3.0)/log(2.0)*/ ) + 1.0;
  }
}
#endif

// a derived class should override this method, in order to implement the actual processing
int cSpectral::processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi) // idxi=input field index
{
  long n=0;
  long i,j;

  double _N = (double) ((Nsrc-1)*2);  // assumes FFT magnitude input array!!
  double F0 = 1.0/fsSec;

  if (frq == NULL) { // input block's frequency axis
    const FrameMetaInfo * fmeta = reader_->getFrameMetaInfo();
    if ((fmeta != NULL)&&(idxi < fmeta->N)) {
      // TODO: check input block's type (in fmeta)
      // fmeta->field[idxi].dataType & SPECTRAL == 1 ?

      frq = (double *)(fmeta->field[idxi].info);
      nScale = fmeta->field[idxi].infoSize / sizeof(double);
      
      // old frequency axis computation mode:
      //nScale=0;

      if (nScale != Nsrc) {
        SMILE_IWRN(2,"number of frequency axis points (from info struct) [%i] is not equal to Nsrc [%i] ! Field index: %i (check the processArrayFields option).",nScale,Nsrc,idxi);
        nScale = MIN(nScale,Nsrc);
      }
    }
  }

  if (frqScale == -1) {
      cVectorMeta *mdata = writer_->getLevelMetaDataPtr();
      if (mdata != NULL && mdata->ID == 1001 /* SCALED_SPEC */) {
        frqScale = (int)mdata->fData[6];
        frqScaleParam = (double)mdata->fData[7];
      } else {
        frqScale = SPECTSCALE_LINEAR;
        frqScaleParam = 0.0;
      }
  }


  if (Nsrc<=0) return 0;


  FLOAT_DMEM *_src;
  if (squareInput) {
    _src = (FLOAT_DMEM*)malloc(sizeof(FLOAT_DMEM)*Nsrc);
    for (i=0; i<Nsrc; i++) _src[i] = src[i]*src[i];
  } else {
    _src = (FLOAT_DMEM *)src;  // typecast ok here.. since we treat _src as read-only below...
  }
  
  
  // compute total frame energy
  double frameSum = 0.0;
  if (normBandEnergies) {
    for (i=0; i<Nsrc; i++) {
      frameSum += _src[i];
    }
  }

  // process spectral bands (rectangular "filter")
  for (i=0; i<nBands; i++) {
    if (isBandValid(bandsL[i],bandsH[i])) {
      double idxL;
      double wghtL;
      if ((nScale < Nsrc)||(frq==NULL)) {
        idxL =  (double)bandsL[i] / F0 ;
        wghtL =  ceil(idxL) - idxL;
      } else {
        if (bandsLi == NULL) { // map the frequency to an fft bin index
          int ii,iii;
          bandsLi=(double*)malloc(sizeof(double)*nBands);
          wghtLi=(double*)malloc(sizeof(double)*nBands);
          for (iii=0; iii<nBands; iii++) {
            for (ii=0; ii<nScale; ii++) {
              if (frq[ii] > (double)bandsL[iii]) break;
            }
            if ((ii<nScale)&&(ii>0)) {
              wghtLi[iii] = (frq[ii]-(double)bandsL[iii])/(frq[ii] - frq[ii-1]);
            } else { wghtLi[iii] = 1.0; }
            
            bandsLi[iii] = (double)ii-1.0;
            if (bandsLi[iii] < 0) bandsLi[iii] = 0;
            if (bandsLi[iii] >= Nsrc) bandsLi[iii] = Nsrc;
            //printf("Low: I=%i .. f=%i, frq[%i]=%f, frq[%i-1]=%f, wght=%f\n",iii,bandsL[iii],ii,frq[ii],ii,frq[ii-1],wghtLi[iii]);
          }
          

        }
        idxL = bandsLi[i];
        wghtL = wghtLi[i];
        
      }
      if (wghtL == 0.0) wghtL = 1.0;
//printf("I=%i ... idxL: %f (%f), wghtL: %f  (iR %f  wR %f)\n",i,idxL,floor(idxL)*F0,wghtL,(double)bandsL[i] / F0, ceil((double)bandsL[i] / F0)-((double)bandsL[i] / F0));

      double idxR;
      double wghtR ;
      if ((nScale < Nsrc)||(frq==NULL)) {
        idxR =  (double)bandsH[i] / F0 ;
        wghtR = idxR-floor(idxR);
      } else {
        if (bandsHi == NULL) { // map the frequency to an fft bin index
          int ii, iii;
          bandsHi=(double*)malloc(sizeof(double)*nBands);
          wghtHi=(double*)malloc(sizeof(double)*nBands);
          for (iii=0; iii<nBands; iii++) {
            for (ii=0; ii<nScale; ii++) {
              if (frq[ii] >= (FLOAT_DMEM)bandsH[iii]) break;
            }
            if ((ii<nScale)&&(ii>0)) {
              wghtHi[iii] = ((double)bandsH[iii]-frq[ii-1])/(frq[ii] - frq[ii-1]);
            } else { wghtHi[iii] = 1.0; }
            //if (wghtHi[iii] == 0.0) { wghtHi[iii] = 1.0; }
            if ((ii<nScale)&&(frq[ii] == (FLOAT_DMEM)bandsH[iii])) {
              bandsHi[iii] = (double)ii;
            } else {
              bandsHi[iii] = (double)ii-1.0;
            }
            if (bandsHi[iii] >= Nsrc) bandsHi[iii] = Nsrc-1;
            //printf("High: I=%i .. f=%i, frq[%i]=%f, frq[%i-1]=%f, wght=%f\n",iii,bandsH[iii],ii,frq[ii],ii,frq[ii-1],wghtHi[iii]);
          }
        }
        idxR = bandsHi[i];
        wghtR = wghtHi[i];
      }
      // TODO: interpolation instead of rounding boundaries to next lower bin and next higher bin
      // TODO: spectral band filter shapes...
      // TODO: debug output of actual filter bandwidth & range based on rounded (or interpolated) boundaries!

      if (wghtR == 0.0) wghtR = 1.0;
  //  printf("I=%i ... idxR: %f , wghtR: %f  (iR %f  wR %f)\n",i,idxR,wghtR,(double)bandsH[i] / F0, (double)bandsH[i] / F0-floor((double)bandsH[i] / F0));
  
      long iL = (long)floor(idxL);
      long iR = (long)floor(idxR);

      if (iL >= Nsrc) { iL=iR=Nsrc-1; wghtR=0.0; wghtL=0.0; }
      if (iR >= Nsrc) { iR=Nsrc-1; wghtR = 1.0; } 
      if (iL < 0) iL=0;
      if (iR < 0) iR=0;

      double sum=(double)_src[iL]*wghtL;
      for (j=iL+1; j<iR; j++) sum += (double)_src[j];
      sum += (double)_src[iR]*wghtR;
      
      if (normBandEnergies) {
        if (frameSum > 0.0) {
          dst[n++] = (FLOAT_DMEM)( sum/frameSum );  // normalise band energy to frame energy
        } else {
          dst[n++] = (FLOAT_DMEM)0.0;
        }
      } else {
        dst[n++] = (FLOAT_DMEM)( sum/(double)Nsrc );  // normalise band energy to frame size
      }
    }
  }

  // sum of all spectral amplitudes, required for spectral centroid/sharpness and moments
  double sumA=0.0, sumB=0.0, sumC=0.0, f=0.0;
  for (j=0; j<Nsrc; j++) {
    sumB += (double)_src[j];
  }
  
  // compute rollOff(s):
  FLOAT_DMEM *ro = (FLOAT_DMEM *)calloc(1,sizeof(FLOAT_DMEM)*nRollOff);
  for (j=0; j<Nsrc; j++) {
    sumC += (double)_src[j];
    for (i=0; i<nRollOff; i++) {
      if (buggyRollOff == 1 && i > 0) {
        sumC += (double)_src[j];
      }
      if ((ro[i] == 0.0)&&(sumC >= rollOff[i]*sumB)) {
        if ((nScale >= Nsrc)&&(frq!=NULL)) {
          ro[i] = (FLOAT_DMEM)frq[j];   // TODO: norm frequency ??
        } else {
          ro[i] = (FLOAT_DMEM)j * (FLOAT_DMEM)F0;   // TODO: norm frequency ??
        }
      }
    }
  }
  for (i=0; i<nRollOff; i++) {
    dst[n++] = ro[i];
  }
  free(ro);
  
  // flux
  if (flux) {
    // flux
    cVector *vecPrev = reader_->getFrameRel(2,1,1);
    if (vecPrev != NULL) {
      double normP=1.0, normC=1.0;
/*      if (lldex->current[level]->energy != NULL) {
        normC = lldex->current[level]->energy->rmsEnergy;
        if (normC <= 0.0) normC = 1.0;
      }*/
      FLOAT_DMEM *magP = vecPrev->dataF;
/*      if (prev->energy != NULL) {
        normP = prev->energy->rmsEnergy;
        if (normP <= 0.0) normP = 1.0;
      }*/
      double myA = 0.0; double myB;
      if (squareInput) {
        for (j=1; j<Nsrc; j++) {
          myB = ((double)src[j]/normC - (double)magP[j]/normP);
          myA += myB*myB;
        }
      } else {
        for (j=1; j<Nsrc; j++) {
          myB = (sqrt(fabs((double)src[j]))/normC - sqrt(fabs((double)magP[j]))/normP);
          myA += myB*myB;
        }
      }
      myA /= (double)(Nsrc-1);
      if (myA > 0) {
        dst[n++] = (FLOAT_DMEM)sqrt(myA);
      } else {
        dst[n++] = 0.0;
      }
      
      delete vecPrev;
    } else {
      dst[n++] = 0.0;
    }
  }
  
  // centroid
  FLOAT_DMEM ctr=0.0;
  if (centroid||variance||skewness||kurtosis||slope) { // spectral centroid (mpeg7)
    if ((nScale >= Nsrc)&&(frq!=NULL)) {
      for (j=0; j<Nsrc; j++) {
        sumA += (double)frq[j] * (double)_src[j];
      }
    } else {
      for (j=0; j<Nsrc; j++) {
        sumA += f * (double)_src[j];
        f += F0;
      }
    }

    if (sumB != 0.0) {
      ctr = (FLOAT_DMEM)(sumA/sumB);  // spectral centroid in Hz (unnormalised)  // TODO: normFrequency option
    }

    if (centroid) {
      dst[n++] = ctr;
    }
  }
  
  if ((maxPos)||(minPos)) {
    long maP=1, miP=1;
    FLOAT_DMEM max=_src[1], min=_src[1];
    for (j=2; j<Nsrc; j++) {
      if (_src[j] < min) { min = _src[j]; miP = j; }
      if (_src[j] > max) { max = _src[j]; maP = j; }
    }

    if ((nScale >= Nsrc)&&(frq!=NULL)) {
      if (maxPos) dst[n++] = (FLOAT_DMEM)frq[maP];  // spectral minimum in Hz
      if (minPos) dst[n++] = (FLOAT_DMEM)frq[miP]; // spectral maximum in Hz
    } else {
      if (maxPos) dst[n++] = (FLOAT_DMEM)(maP*F0);  // spectral minimum in Hz
      if (minPos) dst[n++] = (FLOAT_DMEM)(miP*F0); // spectral maximum in Hz
    }
  }

  // spectral entropy
  if (entropy) {
    // TODO: normalise??
    dst[n++] = smileStat_entropy(_src,Nsrc);
  }

  // compute various moments
  double u=ctr; // use centroid as mean value
  double m2 = 0.0;
  double m3 = 0.0;
  double m4 = 0.0;

  if (variance||skewness||kurtosis||slope) { 
/*
if (nScale >= Nsrc && nScale > 0) {
      u=0.0;
      for (i=0; i<nScale; i++) {
        u += (double)frq[i];
      }
      u /= (double)nScale;
    } else { 
      u = ((double)Nsrc*F0)/2.0; // mean of frequencies
    }
*/
    if (variance||skewness||kurtosis) { 
      if ((nScale>=Nsrc)&&(frq!=NULL)) {
        for (i=0; i<Nsrc; i++) {
          double t1 = ((double)frq[i]-u);
          double m = t1*t1 * (double)_src[i];
          m2 += m;
          m *= t1;
          m3 += m;
          m4 += m*t1;
        }
      } else {
        for (i=0; i<Nsrc; i++) {
          double fi = (double)i*F0;
          double t1 = (fi-u);
          double m = t1*t1 * (double)_src[i];
          m2 += m;
          m *= t1;
          m3 += m;
          m4 += m*t1;
        }
      }

      double sigma2=0.0;
      if (sumB != 0.0) {
        sigma2 = m2/sumB;
      }

   // spectral spread (mpeg7) = spectral variance
      if (variance) { // save variance (sigma^2 = m2) in output vector
        dst[n++] = (FLOAT_DMEM)(sigma2);
      }
   
    // spectral skewness 
      if (skewness) { 
        if (sigma2 <= 0.0) {
          dst[n++] = 0.0;
        } else {
          dst[n++] = (FLOAT_DMEM)( m3/(sumB*sigma2*sqrt(sigma2)) );
        }
      }

    // spectral kurtosis
      if (kurtosis) {
        if (sigma2 == 0.0) {
          dst[n++] = 0.0;
        } else {
          dst[n++] = (FLOAT_DMEM)( m4/(sumB*sigma2*sigma2) );
        }
      }

    }

    // spectral slope
    if (slope) {
      double Sf = 0.0;
      double S2f = 0.0;
      double Nind = (double)Nsrc;
//      double num = 0.0;
      if ((nScale >= Nsrc && nScale > 0)&&(frq!=NULL)) {
        //Sf = u * (double)nScale;
        for (i=0; i<nScale; i++) {
          S2f += (double)frq[i] * (double)frq[i] ;
          Sf += (double)frq[i];   //NOTE: if this line is commented out: old code for compatibility, dont' check into SVN!!
//          num += (double)frq[i] * (double)_src[i];
        }
      } else { 
        double NNm1 = Nind * (Nind - 1.0);
        double S1 = NNm1/(double)2.0;  // sum of all i=0..N-1
        double S2 = NNm1*((double)2.0*Nind-(double)1.0)/(double)6.0; // sum of all i^2 for i=0..N-1
        // TODO: check this!!
        Sf = S1 * F0;
        S2f = S2 * F0*F0;
//        for (i=0; i<nScale; i++) {
//          num += (double)i*F0 * (double)_src[i];
        //}
      }

      double deno = (Nind*S2f-Sf*Sf);
//      printf("deno=%f %f %f %f\n",deno,Nind,S2f,Sf);
      double slope = 0.0;
      if (deno != 0.0) slope = (Nind*sumA-Sf*sumB)/deno;

      // slope div. sumB ???

      dst[n++] = (FLOAT_DMEM)(slope*(Nind-1.0));
    }

  }


  if (sharpness) {
     // psychoacoustically correct spectral centroid (= sharpness)
     FLOAT_DMEM ctr=0.0;
     if (sharpness) { // spectral sharpness (weighted centroid, usually in bark scale)
       if ((nScale >= Nsrc)&&(frq!=NULL)) {
         // custom scale, frequency info given in meta data
         for (j=0; j<Nsrc; j++) {
           // convert frequency to bark, if not already in bark
           double f = (double)frq[j];
           if (frqScale != SPECTSCALE_BARK) {
             // transform to linear scale...
             f = smileDsp_specScaleTransfInv(f,frqScale,frqScaleParam);
             // ...then from linear to bark
             f = smileDsp_specScaleTransfFwd(f,SPECTSCALE_BARK,0.0);
           }
           sumA += f * (double)_src[j] * smileDsp_getSharpnessWeightG(f,SPECTSCALE_BARK,0.0);
         }
       } else {
         // linear scale, no frequency axis info given in meta data
         for (j=0; j<Nsrc; j++) {
           double fb = smileDsp_specScaleTransfFwd(f,SPECTSCALE_BARK,0.0);
           sumA += fb * (double)_src[j] * smileDsp_getSharpnessWeightG(fb,SPECTSCALE_BARK,0.0);
           f += F0;
         }
       }

       if (sumB != 0.0) {
         ctr = (FLOAT_DMEM)(sumA/sumB);  // weighted spectral centroid in Bark (unnormalised)
       }

       dst[n++] = (FLOAT_DMEM)(0.11 * ctr); /* scaling for psychoacoustic sharpness */
     }
   }


   if (tonality) {

     dst[n++] = 0.0; /* NOT YET IMPLEMENTED */
   }


   if (harmonicity) {
     FLOAT_DMEM ptpSum=0.0; long nPtp=0;
     FLOAT_DMEM lastPeak = -99.0;

     for (j=2; j<Nsrc-2; j++) {
       // min/max finder
       if ( (_src[j-2] < _src[j] && _src[j-1] < _src[j] && _src[j] > _src[j+1] && _src[j] > _src[j+2])  //max
           || (_src[j-2] > _src[j] && _src[j-1] > _src[j] && _src[j] < _src[j+1] && _src[j] < _src[j+2]) ) // min
       {
         if (lastPeak != -99.0) {
           ptpSum += fabs(_src[j] - lastPeak);
           nPtp++;
         }
         lastPeak = _src[j];
       }
     }
     if (nPtp > 0) {
       ptpSum /= (FLOAT_DMEM)nPtp;
     }
     // normalise to spectral mean (~ frame energy):
     if (sumB != 0.0) {
       ptpSum /= (FLOAT_DMEM)(sumB/(double)Nsrc);
     }

     dst[n++] = ptpSum;
   }

   if (flatness) {
     FLOAT_DMEM sf = 0.0;
     FLOAT_DMEM gmean = 0.0; int nGm = 0;
     if (sumB > 0.0) {
       // compute geometric mean
       for (j=0; j<Nsrc; j++) {
         if (_src[j] >= 0.0) { gmean += log(_src[j]); nGm++; }
       }
       if (nGm > 0) gmean /= (FLOAT_DMEM)nGm;
       gmean = exp(gmean);
       // compute flatness
       sf = gmean / (FLOAT_DMEM)(sumB/(double)Nsrc);
     }
     dst[n++] = sf;
   }

  if (squareInput) { free(_src); }
  
  return 1;
}

cSpectral::~cSpectral()
{
  if (bandsL!=NULL) free(bandsL);
  if (bandsH!=NULL) free(bandsH);
  if (bandsLi!=NULL) free(bandsLi);
  if (bandsHi!=NULL) free(bandsHi);
  if (wghtLi!=NULL) free(wghtLi);
  if (wghtHi!=NULL) free(wghtHi);
  if (rollOff!=NULL) free(rollOff);
}

