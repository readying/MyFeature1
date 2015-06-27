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

LibSVM file output

*/


#ifndef __CLIBSVMSINK_HPP
#define __CLIBSVMSINK_HPP

#include <core/smileCommon.hpp>
#include <core/dataSink.hpp>

#define COMPONENT_DESCRIPTION_CLIBSVMSINK "This component writes data to a text file in LibSVM feature file format. For the 'on-the-fly' classification component see 'cLibsvmliveSink'."
#define COMPONENT_NAME_CLIBSVMSINK "cLibsvmSink"


#undef class
class DLLEXPORT cLibsvmSink : public cDataSink {
  private:
    FILE * filehandle;
    const char *filename;
    int lag;
    int append, timestamp;
    const char *instanceBase, *instanceName;
    
    int targetNumAll;
    int nClasses;
    long nInst;
    long inr;
    char **classname;
    int *target;

    int getClassIndex(const char *_name)
    {
      int i;
      if (_name == NULL) return -1;
      if (classname == NULL) return -1;
      for (i=0; i<nClasses; i++) {
        if ((classname[i]!=NULL)&&(!strcmp(_name,classname[i]))) return i;
      }
      return -1;
    }

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
    
    cLibsvmSink(const char *_name);

    virtual ~cLibsvmSink();
};




#endif // __CLIBSVMSINK_HPP
