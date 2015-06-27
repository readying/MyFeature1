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

/* openSMILE plugin loader */

#include <smileCommon.hpp>
#include <smileLogger.hpp>
#include <componentManager.hpp>


//++ include all your component headers here: ----------------
#include <plug_exampleSink.hpp>


//++ ---------------------------------------------------------

// dll export for msvc++
#ifdef _MSC_VER 
#define DLLEXPORT_P __declspec(dllexport)
#else
#define DLLEXPORT_P
#endif


#define MODULE "pluginLoader"

static DLLLOCAL const registerFunction complist[] = {
//++ add your component register functions here: -------------
  cExampleSinkPlugin::registerComponent,


//++ ---------------------------------------------------------
  NULL  // last element must always be NULL to terminate the list !!
};

//Library:
extern "C" DLLEXPORT_P sComponentInfo * registerPluginComponent(cConfigManager *_confman, cComponentManager *_compman) {
  registerFunction f;
  sComponentInfo *master=NULL;
  sComponentInfo *cur = NULL, *last = NULL;
  int i=0;
  f = complist[i];
  while (f!=NULL) {
    cur = (f)(_confman, _compman);
	cur->next = NULL;
    if (i==0) master = cur;
    else { last->next = cur; }
    last = cur;
    f=complist[++i];
  }
  return master;
}



