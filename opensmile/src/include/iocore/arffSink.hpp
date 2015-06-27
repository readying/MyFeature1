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

ARFF file output (for WEKA)

*/


#ifndef __ARFF_SINK_HPP
#define __ARFF_SINK_HPP

#include <core/smileCommon.hpp>
#include <core/dataSink.hpp>

#define COMPONENT_DESCRIPTION_CARFFSINK "This component writes dataMemory data to an ARFF file (WEKA). Depending on your config an instance name field, a frame index, and a frame time field can be added as well as multiple class/target attributes. See the config type documentation for more details."
#define COMPONENT_NAME_CARFFSINK "cArffSink"

#undef class
class DLLEXPORT cArffSink : public cDataSink {
  private:
    FILE * filehandle;
    double frameTimeAdd;
    const char *filename;
    int lag;
    int append, timestamp, number, prname, frameLength;
    int instanceNameFromMetadata, useTargetsFromMetadata;
    const char *relation;
    const char *instanceBase, *instanceName;

    int printDefaultClassDummyAttribute;    
    int nClasses; long nInst;
    long inr;
    char **classname;
    char **classtype;
    
    char **targetall;
    char ***targetinst;
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    //virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);


  public:
    //static sComponentInfo * registerComponent(cConfigManager *_confman);
    //static cSmileComponent * create(const char *_instname);
    SMILECOMPONENT_STATIC_DECL
    
    cArffSink(const char *_name);

    virtual ~cArffSink();
};




#endif // __ARFF_SINK_HPP
