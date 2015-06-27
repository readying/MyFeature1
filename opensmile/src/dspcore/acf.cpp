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

TODO:
Autocorrelation Function (ACF)

simple windowed ACF

(further TODO: continuous ACF (overlap add OR time domain?)

*/

#include <dspcore/acf.hpp>

#define MODULE "cAcf"

SMILECOMPONENT_STATICS(cAcf)

SMILECOMPONENT_REGCOMP(cAcf)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CACF;
  sdescription = COMPONENT_DESCRIPTION_CACF;

  // we inherit cVectorProcessor configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cVectorProcessor")
    SMILECOMPONENT_IFNOTREGAGAIN(
    ct->setField("nameAppend", NULL, "acf");
    ct->setField("usePower","= square input values; you must leave this at default 1, when using fft magnitude as input for ACF. For cepstrum this will be set to 0 by default, unless you explicitely give the value 1.",1);
    ct->setField("cepstrum","1 = compute the cepstrum instead of the standard ACF. This applies a log() to the magnitudes bevore transforming from the spectral domain back to the time domain. You might want to set 'nameAppend=cepstrum' when using this option.",0);
    ct->setField("inverse", "1 = do the inverse transform of cepstrum or ACF. The output is an FFT magnitude spectrum.", 0);
    ct->setField("cosLifterCepstrum", "1 = apply cosine lifter function to the cepstrum, effectively applying a Hanning window function to the cepstrum.", 0);
    ct->setField("expBeforeAbs", "1 = perform exp operation before computing magnitude spectrum (for inverse cepstrum only).", 1);
    ct->setField("symmetricData", "1 = treat data in acf/cepstral domain as symmetric data, i.e. output only half of the buffer or symmetrically duplicate the input before applying fft.", 1);
  )

  SMILECOMPONENT_MAKEINFO(cAcf);
}

SMILECOMPONENT_CREATE(cAcf)

//-----

cAcf::cAcf(const char *_name) :
cVectorProcessor(_name),
symmetricData(0),
expBeforeAbs(0),
cosLifterCepstrum(0),
usePower(0), cepstrum(0), inverse(0),
data(NULL),
w(NULL),
ip(NULL),
winFunc(NULL)
{
}

void cAcf::fetchConfig()
{
  cVectorProcessor::fetchConfig();
  symmetricData = getInt("symmetricData");
  expBeforeAbs = getInt("expBeforeAbs");
  cepstrum = getInt("cepstrum");
  if (cepstrum) {
    SMILE_IDBG(2,"computing cesptrum instead of ACF");
  }
  inverse = getInt("inverse");
  if (inverse) {
    SMILE_IDBG(2,"computing inverse transform.");
  }
  if (cepstrum) {
    if (isSet("usePower")) {
      usePower = getInt("usePower");
    } else {
      usePower = 0;
    }
  } else {
    usePower = getInt("usePower");
  }
  if (usePower) { SMILE_IDBG(2,"Squaring magnitude spectrum input to become power spectrum."); }
  cosLifterCepstrum = getInt("cosLifterCepstrum");
}

int cAcf::setupNamesForField(int i, const char*name, long nEl)
{
  long nOutEl;
  // TODO: truncate to half length output option for compatibility with old Acf/Cepstral pitch
  if (symmetricData) {
    if (inverse) {
      nOutEl = nEl + 1;
    } else {
      nOutEl = nEl - 1;  // FIXME: removed -1 from nEl, this might break ACF pitch in old configs! Check!
    }
  } else {
    if (inverse) {
      nOutEl = nEl / 2 + 1;
    } else {
      nOutEl = (nEl-1) * 2;
    }
  }
  int ret = cVectorProcessor::setupNamesForField(i, name, nOutEl);
  if (cepstrum && cosLifterCepstrum) {
    // compute liftering function
    winFunc[i] = (FLOAT_DMEM*)malloc(sizeof(FLOAT_DMEM) * nOutEl);
    if (symmetricData) {
      double * wh = smileDsp_winHan(nOutEl * 2);
      if (wh != NULL) {
        for (int j = 0; j < nOutEl; j++) {
          winFunc[i][j] = (FLOAT_DMEM)wh[nOutEl - j];
        }
        free(wh);
      }
    } else {
      double * wh = smileDsp_winHan(nOutEl);
      if (wh != NULL) {
        for (int j = 0; j < nOutEl / 2; j++) {
          winFunc[i][j] = (FLOAT_DMEM)wh[nOutEl / 2 - j];
        }
        for (int j = nOutEl/2; j < nOutEl; j++) {
          winFunc[i][j] = (FLOAT_DMEM)wh[nOutEl / 2 - j + nOutEl - 1];
        }
        free(wh);
      }
    }
  }
  return ret;
}

int cAcf::dataProcessorCustomFinalise()
{
  if (ip == NULL) ip = (int**)multiConfAlloc();
  if (w == NULL) w = (FLOAT_TYPE_FFT**)multiConfAlloc();
  if (data == NULL) data = (FLOAT_TYPE_FFT**)multiConfAlloc();
  if (winFunc == NULL) winFunc = (FLOAT_DMEM**)multiConfAlloc();

  return cVectorProcessor::dataProcessorCustomFinalise();
}

// a derived class should override this method, in order to implement the actual processing
int cAcf::processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi) // idxi=input field index
{
  long i,n;
  idxi=getFconf(idxi);
  FLOAT_TYPE_FFT *_data = data[idxi];
  int *_ip = ip[idxi];
  FLOAT_TYPE_FFT *_w = w[idxi];

  if (inverse) {
    long N = Nsrc;
    if (symmetricData) {
      N = Nsrc * 2;
    }
    if (_data == NULL) {
      _data = (FLOAT_TYPE_FFT*)malloc(sizeof(FLOAT_TYPE_FFT)*N);
    }
    if (_ip==NULL) _ip = (int *)calloc(1,sizeof(int)*(N+2));
    if (_w==NULL) _w = (FLOAT_TYPE_FFT *)calloc(1,sizeof(FLOAT_TYPE_FFT)*(N*5)/4+2);

    // normal fft  (time -> spec)  // TODO: check dim of data etc.
    if (cepstrum && cosLifterCepstrum) {
      for (i = 0; i < Nsrc; i++) {
        _data[i] = (FLOAT_TYPE_FFT)src[i] * winFunc[idxi][i];
      }
    } else {
      for (i = 0; i < Nsrc; i++) {
        _data[i] = (FLOAT_TYPE_FFT)src[i];
      }
    }

    if (symmetricData) {
      for (i = Nsrc; i < N; i++) {
        _data[i] = _data[N - 1 - i];
      }
    }

    rdft(N, 1, _data, _ip, _w);

    // do exp for inverse cepstrum
    if (cepstrum && expBeforeAbs) {
      _data[0] = exp(_data[0]);
      _data[1] = exp(_data[1]);
      for (i = 2; i < N - 1; i += 2) {
        // exp on real part, imag to zero
        _data[i] = exp(_data[i]);
        _data[i+1] = 0.0;
      }
    }

    // compute magnitude (should be the real data... but just in case..)
    dst[0] = fabs(_data[0]);
    for (i = 2; i < N - 1; i += 2) {
      if (i>>1 < Ndst) {
        dst[i>>1] = sqrt((_data[i] * _data[i]) + (_data[i + 1] * _data[i + 1]));
      }
    }
    dst[Ndst - 1] = fabs(_data[1]);

    // subtract 1 for inverse cepstrum
    if (cepstrum) {
      if (!expBeforeAbs) {
        for (i = 0; i < Ndst; i ++) {
          dst[i] = exp(dst[i]) - 1.0;
          if (dst[i] < 0.0) dst[i] = 0.0;
        }
      } else {
        for (i = 0; i < Ndst; i ++) {
          dst[i] -= 1.0;
          if (dst[i] < 0.0) dst[i] = 0.0;
        }
      }
    }

    // if usePower==1 take the square root when in ACF mode... (inverse behaviour as the other way round)
    if (usePower) {
      for (i = 0; i < Ndst; i ++) {
        dst[i] = sqrt(dst[i]);
      }
    }
  } else {
    // copy & square the fft magnitude
    FLOAT_DMEM *_src = NULL;
    if (usePower) {
      _src = (FLOAT_DMEM*)malloc(sizeof(FLOAT_DMEM)*Nsrc);
      if (src==NULL) OUT_OF_MEMORY;
      for (n=0; n<Nsrc; n++) {
        _src[n] = src[n]*src[n];
      }
      src = _src;
    }

    long N = (Nsrc-1)*2;  // FIXME: Remove -1 from NSrc??
    // check for power of 2!!
    if (!smileMath_isPowerOf2(N)) {
      SMILE_IERR(1,"(Nsrc-1)*2 = %i is not a power of 2, this is required for acf!! make sure the input data really is fft magnitude data!",N);
      return 0;
    }

    // data preparation for inverse fft:
    if (_data == NULL) {
      _data = (FLOAT_TYPE_FFT*)malloc(sizeof(FLOAT_TYPE_FFT)*N);
    }
    if (_ip==NULL) _ip = (int *)calloc(1,sizeof(int)*(N+2));
    if (_w==NULL) _w = (FLOAT_TYPE_FFT *)calloc(1,sizeof(FLOAT_TYPE_FFT)*(N*5)/4+2);

    if (cepstrum) {
      if (src[0] > 0.0) {
        _data[0] = (FLOAT_TYPE_FFT)(log(src[0] + 1.0));
      } else {
        _data[0] = 0.0;
      }
      if (src[Nsrc - 1] > 0.0) {
        _data[1] = (FLOAT_TYPE_FFT)(log(src[Nsrc - 1] + 1.0));
      } else {
        _data[1] = 0.0;
      }
      for (i=2; i<N-1; i += 2) {
        if (src[i>>1] > 0.0) {
          _data[i] = (FLOAT_TYPE_FFT)log(src[i>>1] + 1.0);
        } else {
          _data[i] = 0.0;
        }
        _data[i+1] = 0.0;
      }
    } else {
      _data[0] = (FLOAT_TYPE_FFT)(src[0]);
      _data[1] = (FLOAT_TYPE_FFT)(src[Nsrc-1]);
      for (i=2; i<N-1; i += 2) {
        _data[i] = (FLOAT_TYPE_FFT)(src[i>>1]);
        _data[i+1] = 0.0;
      }
    }

    // inverse fft
    rdft(N, -1, _data, _ip, _w);

    // NOTE: The symmetricData option is implicitly handled by (i<N)&&(i<Ndst) and Ndst limiting the output size.
    // TODO : debug the symmetricData option when doing cepstrum and inv cepstrum
    if (cepstrum) {
      if (cosLifterCepstrum) {
        for (i=0; (i<N)&&(i<Ndst); i++) {
          dst[i] = (FLOAT_DMEM)_data[i] / (FLOAT_DMEM)Nsrc * winFunc[idxi][i];
        }
      } else {
        for (i=0; (i<N)&&(i<Ndst); i++) {
          dst[i] = (FLOAT_DMEM)_data[i] / (FLOAT_DMEM)Nsrc;
        }
      }
    } else {
      for (i=0; (i<N)&&(i<Ndst); i++) {
        dst[i] = (FLOAT_DMEM)fabs(_data[i]) / (FLOAT_DMEM)Nsrc;
      }
    }

    // cleanup
    if ((usePower)&&(_src!=NULL)) free((void *)_src);
  }
  data[idxi] = _data;
  ip[idxi] = _ip;
  w[idxi] = _w;
  return 1;
}

cAcf::~cAcf()
{
  multiConfFree(ip);
  multiConfFree(w);
  multiConfFree(data);
  multiConfFree(winFunc);
}

