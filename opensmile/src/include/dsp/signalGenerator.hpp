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

Signal source. Generates various noise types and pre-defined signals.

*/


#ifndef __CSIGNALGENERATOR_HPP
#define __CSIGNALGENERATOR_HPP

#include <core/smileCommon.hpp>
#include <core/dataSource.hpp>

#define COMPONENT_DESCRIPTION_CSIGNALGENERATOR "This component provides a signal source. This source generates various noise types and pre-defined signals and value patterns. See the configuration documentation for a list of currently implemented types."
#define COMPONENT_NAME_CSIGNALGENERATOR "cSignalGenerator"

#define NOISE_WHITE   0   // white gaussian noise 'white'
#define SIGNAL_SINE   1   // sinusodial signal (single frequency) 'sine'
#define SIGNAL_CONST  2   // constant value 'const'
#define SIGNAL_RECT   3   // rectangular periodic signal 'rect'
//...

#undef class
class DLLEXPORT cSignalGenerator : public cDataSource {
  private:
    long nValues;
    int randSeed;
    FLOAT_DMEM stddev, mean;
    FLOAT_DMEM constant;
    double signalPeriod, phase;
    double myt ; // current time
    double samplePeriod;
    double lastP;
    double scale;
    double val;

    int * nElements;
    char ** fieldNames;
    int nFields;
    
    int noiseType;
    long lengthFrames;
    long curT;
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    //virtual int myConfigureInstance();
    //virtual int myFinaliseInstance();
    virtual int myTick(long long t);

    virtual int configureWriter(sDmLevelConfig &c);
    virtual int setupNewNames(long nEl);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cSignalGenerator(const char *_name);

    virtual ~cSignalGenerator();
};




#endif // __CSIGNALGENERATOR_HPP
