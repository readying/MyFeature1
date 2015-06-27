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

do elementary operations on vectors 
(i.e. basically everything that does not require history or context,
 everything that can be performed on single vectors w/o external data (except for constant parameters, etc.))

*/



#include <other/vectorOperation.hpp>
#include <cmath>

#define MODULE "cVectorOperation"


SMILECOMPONENT_STATICS(cVectorOperation)

SMILECOMPONENT_REGCOMP(cVectorOperation)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CVECTOROPERATION;
  sdescription = COMPONENT_DESCRIPTION_CVECTOROPERATION;

  // we inherit cVectorProcessor configType and extend it:
  SMILECOMPONENT_INHERIT_CONFIGTYPE("cVectorProcessor")

  // if the inherited config type was found, we register our configuration variables
  SMILECOMPONENT_IFNOTREGAGAIN( {} // <- this is only to avoid compiler warnings...
    // name append has a special role: it is defined in cDataProcessor, and can be overwritten here:
      // if you set description to NULL, the existing description will be used, thus the following call can
      // be used to update the default value:
    //ct->setField("nameAppend",NULL,"processed");
  ct->setField("operation","A string which specifies the type of operation to perform:\n   norm = normalise vector length (euclidean norm, L2) to 1\n   nr1 = normalise range to +1, -1\n   nr0 = normalise range to +1, 0\n   mul = multiply vector by param1\n   add = add param1 to each element\n   log = compute natural logarithm\n   lgA = compute logarithm to base param1\n   nl1 = normalise vector sum (L1 norm) to 1\n   sqrt = compute square root\n   pow = take values to the power of param1\n   exp = raise param1 to the power of the vector elements\n   ee = raise the base e to the power of the vector elements\n   abs = take absolute value of each element\n   agn = add Gaussian noise with mean param1 and std.dev. param2\n   min = take the min of vector and param1\n   max = take the max of vector and param1\n   sum = compute sum of vector elements, there will be a single output only\n   ssm = compute sum of squared vector elements, there will be a single output only\n   ll1 = compute sum of vector elements normalised by the number of vector elements, there will be a single output only\n   ll2 = compute euclidean length (root of sum of squares normalised by vector length), there will be a single output only\n   fla(tten) = flattening of comb filter energy spectra, as in 2007 ICASSP Paper and Ballroom dance style recognition.","norm");
  ct->setField("param1","parameter 1",1.0);
  ct->setField("param2","parameter 2",1.0);
  ct->setField("logfloor","floor for log operation",0.0000001);
  ct->setField("powOnlyPos","if 'operation' = 'pow', do not take negative values to the power of 'param1'; instead, output 0. This is necessary to avoid 'nan' values if the exponent is rational. ",0);
  ct->setField("nameBase","base of output feature name when performing n->1 mapping operations (currently 'euc' and 'sum')","vector");
  )

  // The configType gets automatically registered with the config manger by the SMILECOMPONENT_IFNOTREGAGAIN macro
  
  // we now create out sComponentInfo, including name, description, success status, etc. and return that
  SMILECOMPONENT_MAKEINFO(cVectorOperation);
}

SMILECOMPONENT_CREATE(cVectorOperation)

//-----

cVectorOperation::cVectorOperation(const char *_name) :
  cVectorProcessor(_name),
  param1(0.0),
  param2(0.0),
  nameBase(NULL)
{

}

void cVectorOperation::fetchConfig()
{
  cVectorProcessor::fetchConfig();

  param1 = (FLOAT_DMEM)getDouble("param1");
  param2 = (FLOAT_DMEM)getDouble("param2");
  powOnlyPos = (int)getInt("powOnlyPos");

  const char * op = getStr("operation");
  operation=VOP_NORMALISE;
  if (!strncmp(op,"nor",3)) {
    operation=VOP_NORMALISE;
  } else if (!strncmp(op,"nr1",3)) {
      operation=VOP_NORM_RANGE1;
  } else if (!strncmp(op,"nr0",3)) {
      operation=VOP_NORM_RANGE0;
  } else if (!strncmp(op,"mul",3)) {
    operation=VOP_MUL;
  } else if (!strncmp(op,"add",3)) {
    operation=VOP_ADD;
  } else if (!strncmp(op,"log",3)) {
    operation=VOP_LOG;
  } else if (!strncmp(op,"lgA",3)) {
    operation=VOP_LOGA;
    if (param1 <= 0) {
      SMILE_IWRN(1,"log-base cannot be negative! setting base to exp(1) (-> natural logarithm)");
      param1 = (FLOAT_DMEM)std::exp(1.0);
    }
    if (param1 == 1.0) {
      SMILE_IWRN(1,"log-base cannot be 1.0! setting base to exp(1) (-> natural logarithm)");
      param1 = (FLOAT_DMEM)std::exp(1.0);
    }
  } else if (!strncmp(op,"nl1",3)) {
    operation=VOP_NORMALISE_L1;
  } else if (!strncmp(op,"sqr",3)) {
    operation=VOP_SQRT;
  } else if (!strncmp(op,"pow",3)) {
    operation=VOP_POW;
  } else if (!strncmp(op,"exp",3)) {
    operation=VOP_EXP;
  } else if (!strncmp(op,"ee",2)) {
    operation=VOP_E;
  } else if (!strncmp(op,"abs",3)) {
    operation=VOP_ABS;
  } else if (!strncmp(op,"agn",3)) {
    operation=VOP_AGN;
  } else if (!strncmp(op,"min",3)) {
    operation=VOP_MIN;
  } else if (!strncmp(op,"max",3)) {
    operation=VOP_MAX;
  } else if (!strncmp(op,"sum",3)) {
    operation=VOP_X_SUM;
  } else if (!strncmp(op,"ssm",3)) {
    operation=VOP_X_SUMSQ;
  } else if (!strncmp(op,"ll1",3)) {
    operation=VOP_X_L1;
  } else if (!strncmp(op,"ll2",3)) {
    operation=VOP_X_L2;
  } else if (!strncmp(op,"fla",3)) {
      operation=VOP_FLATTEN;
  } else {
    SMILE_IERR(1,"unknown operation '%s' specified in config file.",op);
  }

  nameBase = getStr("nameBase");

  logfloor = (FLOAT_DMEM)getDouble("logfloor");
  if (logfloor <= 0) { 
    SMILE_IWRN(1,"log-floor cannot be <= 0 ! setting to 0.00000000001 !");
    logfloor = (FLOAT_DMEM)0.00000000001; 
  }
}

int cVectorOperation::setupNewNames(long ni)
{
  long No=0;

  if (operation > VOP_X) {
    No = 1;
    if (operation == VOP_X_SUM) addNameAppendField(nameBase,"sum",1,0);
    if (operation == VOP_X_SUMSQ) addNameAppendField(nameBase,"sumSq",1,0);
    if (operation == VOP_X_L1) addNameAppendField(nameBase,"lengthL1norm",1,0);
    if (operation == VOP_X_L2) addNameAppendField(nameBase,"lengthL2norm",1,0);

    namesAreSet_ = 1;
  }

  return No;
}


// a derived class should override this method, in order to implement the actual processing
int cVectorOperation::processVectorInt(const INT_DMEM *src, INT_DMEM *dst, long Nsrc, long Ndst, int idxi) // idxi=input field index
{
  // do domething to data in *src, save result to *dst
  // NOTE: *src and *dst may be the same...
  
  return 1;
}

// a derived class should override this method, in order to implement the actual processing
int cVectorOperation::processVectorFloat(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi) // idxi=input field index
{
  // do domething to data in *src, save result to *dst
  // NOTE: *src and *dst may be the same...
  FLOAT_DMEM sum =0.0;
  FLOAT_DMEM max, min, range;
  long i;

  switch(operation) {

    case VOP_NORMALISE:

      // normalise L2 norm of all incoming vectors
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        sum+=src[i]*src[i];
      }
      if (sum <= 0.0) {
        sum = 1.0;
      }
      sum = sqrt(sum/(FLOAT_DMEM)(MIN(Nsrc,Ndst)));
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        dst[i] = src[i] / sum;
      }

      break;

    case VOP_NORMALISE_L1:

      // normalise L1 norm of all incoming vectors
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        sum+=src[i];
      }
      if (sum <= 0.0) {
        sum = 1.0;
      }
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        dst[i] = src[i] / sum;
      }

      break;

    case VOP_NORM_RANGE0:
    case VOP_NORM_RANGE1:

       // normalise vector range
       max=src[0]; min=src[0];
       for (i=1; i<MIN(Nsrc,Ndst); i++) {
         if (src[i] > max) max = src[i];
         if (src[i] < min) min = src[i];
       }
       range = max-min; if (range == 0.0) range = 1.0;

       if (operation == VOP_NORM_RANGE0) { // 0..1
         for (i=0; i<MIN(Nsrc,Ndst); i++) {
           dst[i] = (src[i] - min) / range;
         }
       } else { // VOP_NORM_RANGE1 : -1 .. +1
         range *= 0.5;
         for (i=0; i<MIN(Nsrc,Ndst); i++) {
           dst[i] = ( (src[i] - min) / range ) - (FLOAT_DMEM)1.0;
         }
       }
       break;


    case VOP_ADD:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        dst[i] = src[i] + param1;
      }
      break;

    case VOP_MUL:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        dst[i] = src[i] * param1;
      }
      break;

    case VOP_LOG:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        if (src[i]>logfloor) {
          dst[i] = std::log(src[i]);
        } else {
          dst[i] = std::log(logfloor);
        }
      }
      break;

    case VOP_LOGA:
      sum = std::log(param1);
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        if (src[i]>logfloor) {
          dst[i] = std::log(src[i]) / sum;
        } else {
          dst[i] = std::log(logfloor) / sum;
        }
      }
      break;

    case VOP_SQRT:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        if (src[i]>0.0) {
          dst[i] = std::sqrt(src[i]);
        } else {
          dst[i] = 0.0;
        }
      }
      break;

    case VOP_POW:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        if (powOnlyPos) {
          if (src[i]>0.0) {
            dst[i] = std::pow(src[i],param1);
          } else {
            dst[i] = 0.0;
          }
        } else {
          dst[i] = std::pow(src[i],param1);
        }
      }
      break;

    case VOP_EXP:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        dst[i] = std::pow(param1,src[i]);
      }
      break;
    
    case VOP_E:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        dst[i] = std::exp(src[i]);
      }
      break;

    case VOP_ABS:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        dst[i] = fabs(src[i]);
      }
      break;

    case VOP_AGN:
      for (i = 0; i < MIN(Nsrc,Ndst); i++) {
        dst[i] = src[i] + param2 * (FLOAT_DMEM)gnGenerator() + param1;
      }
      break;

    case VOP_MIN:
      for (i = 0; i < MIN(Nsrc,Ndst); i++) {
        if (src[i] < param1) {
          dst[i] = src[i];
        } else {
          dst[i] = param1;
        }
      }

    case VOP_MAX:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        if (src[i] > param1) {
          dst[i] = src[i];
        } else {
          dst[i] = param1;
        }
      }
      break;

    case VOP_X_SUM:
      dst[0] = 0.0;
      for (i=0; i<Nsrc; i++) {
        dst[0] += src[i];
      }
      break;

   case VOP_X_SUMSQ:
      dst[0] = 0.0;
      for (i=0; i<Nsrc; i++) {
        dst[0] += src[i]*src[i];
      }
      break;

    case VOP_X_L1:
      dst[0] = 0.0;
      for (i=0; i<Nsrc; i++) {
        dst[0] += src[i];
      }
      if (Nsrc > 0) dst[0] /= (FLOAT_DMEM)Nsrc;
      break;

    case VOP_X_L2:
      dst[0] = 0.0;
      for (i=0; i<Nsrc; i++) {
        dst[0] += src[i]*src[i];
      }
      if (dst[0] > 0.0) dst[0] = sqrt(dst[0]);
      if (Nsrc > 0) dst[0] /= (FLOAT_DMEM)Nsrc;
      break;

    case VOP_FLATTEN:
      if (Nsrc > 0) {
        FLOAT_DMEM m1=0.0,m2=0.0;
        for (i=0; i<MIN(Nsrc,5); i++) {
          m1 += src[i];
        } m1 /= (FLOAT_DMEM)(MIN(Nsrc,5));
        for (i=MAX(0,Nsrc-5); i<Nsrc; i++) {
          m2 += src[i];
        } m2 /= (FLOAT_DMEM)(MIN(Nsrc,5));
        FLOAT_DMEM slope = (m1-m2)/(FLOAT_DMEM)Nsrc;
        for (i=0; i<MIN(Nsrc,Ndst); i++) {
          dst[i] = src[i] - m2 - slope * (FLOAT_DMEM)(Nsrc-i);
        }
      }
      break;

    default:
      for (i=0; i<MIN(Nsrc,Ndst); i++) {
        dst[i] = src[i];
      }

  }
  return 1;
}


// Code contributed by Felix Weninger
// Gaussian noise generator
double cVectorOperation::gnGenerator() {
  static bool haveNumber = false;
  static double number = 0.0;
  if (haveNumber) {
    haveNumber = false;
    return number;
  }
  else {
    double q = 2.0;
    double x, y;
    // Use Polar Method to obtain normally distributed random numbers.
    while (q > 1.0) {
        x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        q = x * x + y * y;
    }
    double z = -2.0 * log(q) / q;
    number = y * sqrt(z);
    haveNumber = true;
    return x * sqrt(z);
  }
}


cVectorOperation::~cVectorOperation()
{
}

