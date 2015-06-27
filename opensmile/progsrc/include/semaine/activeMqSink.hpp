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

example dataSink:
reads data from data memory and outputs it to console/logfile (via smileLogger)
this component is also useful for debugging

*/


#ifndef __CACTIVEMQSINK_HPP
#define __CACTIVEMQSINK_HPP

#include <core/smileCommon.hpp>
#undef class

//#define HAVE_SEMAINEAPI
#ifdef HAVE_SEMAINEAPI

//#ifdef HAVE_SEMAINE_CONFIG_H  // <- this causes problems in linux
#ifndef _MSC_VER
#include <semaine/config.h>
#endif

#include <cms/CMSException.h>
#include <semaine/components/Component.h>
#include <semaine/cms/sender/FeatureSender.h>

#include <core/dataSink.hpp>

#define COMPONENT_DESCRIPTION_CACTIVEMQSINK "activeMq dataSink, this component sends data as messages to an ActiveMQ server"
#define COMPONENT_NAME_CACTIVEMQSINK "cActiveMqSink"

class cActiveMqSink : public cDataSink {
private:
  FeatureSender *featureSender;   
  MetaMessenger *meta; 
  int lag; int warned;

protected:
  SMILECOMPONENT_STATIC_DECL_PR

  virtual void fetchConfig();
  //virtual int myConfigureInstance();
  //virtual int myFinaliseInstance();
  virtual int myTick(long long t);

public:
  SMILECOMPONENT_STATIC_DECL

  cActiveMqSink(const char *_name);
  cDataReader * getReader() { return reader; }

  // this method sets the semaine feature sender to use... do not call it when the system is running, only call it during setup!!
  void setFeatureSender(FeatureSender *_sender, MetaMessenger *_meta=NULL) { featureSender = _sender; meta=_meta; }

  virtual ~cActiveMqSink();
};


#endif // HAVE_SEMAINEAPI

#endif // __CACTIVEMQSINK_HPP
