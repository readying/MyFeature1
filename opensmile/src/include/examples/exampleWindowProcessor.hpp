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

example of a windowProcessor component

*/


#ifndef __CEXAMPLEWINDOWPROCESSOR_HPP
#define __CEXAMPLEWINDOWPROCESSOR_HPP

#include <core/smileCommon.hpp>
#include <core/windowProcessor.hpp>

#define COMPONENT_DESCRIPTION_CEXAMPLEWINDOWPROCESSOR "This is an example of a cWindowProcessor descendant. It has no meaningful function, this component is intended as a template for developers."
#define COMPONENT_NAME_CEXAMPLEWINDOWPROCESSOR "cExampleWindowProcessor"

#undef class
class DLLEXPORT cExampleWindowProcessor : public cWindowProcessor {
  private:
    FLOAT_DMEM k;

  protected:
    SMILECOMPONENT_STATIC_DECL_PR


    virtual void fetchConfig();
/*
    virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);
*/

    //virtual int configureWriter(const sDmLevelConfig *c);

   // buffer must include all (# order) past samples
    virtual int processBuffer(cMatrix *_in, cMatrix *_out, int _pre, int _post );
    
/*
    virtual int setupNamesForField(int i, const char*name, long nEl);
*/
    
  public:
    SMILECOMPONENT_STATIC_DECL
    
    cExampleWindowProcessor(const char *_name);

    virtual ~cExampleWindowProcessor();
};




#endif // __CEXAMPLEWINDOWPROCESSOR_HPP
