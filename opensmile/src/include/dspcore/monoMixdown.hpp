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

simple mixer, which adds multiple channels (elements) to a single channel (element)

*/


#ifndef __CMONOMIXDOWN_HPP
#define __CMONOMIXDOWN_HPP

#include <core/smileCommon.hpp>
#include <core/dataProcessor.hpp>

#define COMPONENT_DESCRIPTION_CMONOMIXDOWN "This is a simple mixer, which adds multiple channels (elements) to a single channel (element)."
#define COMPONENT_NAME_CMONOMIXDOWN "cMonoMixdown"

#undef class
class DLLEXPORT cMonoMixdown : public cDataProcessor {
  private:
    int normalise;
    long bufsize;
    cMatrix *matout;
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR

    virtual void fetchConfig();
    //virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);

    // virtual int dataProcessorCustomFinalise();
    // virtual int setupNewNames(long nEl);
    virtual int setupNamesForField(int i, const char*name, long nEl);
    virtual int configureWriter(sDmLevelConfig &c);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cMonoMixdown(const char *_name);

    virtual ~cMonoMixdown();
};




#endif // __CMONOMIXDOWN_HPP
