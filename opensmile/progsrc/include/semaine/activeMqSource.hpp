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

example dataSource
writes data to data memory...

*/


#ifndef __CACTIVEMQSOURCE_HPP
#define __CACTIVEMQSOURCE_HPP

#include <core/smileCommon.hpp>
#undef class

#ifdef HAVE_SEMAINEAPI

//#ifdef HAVE_SEMAINE_CONFIG_H  // <- this causes problems in linux
#ifndef _MSC_VER
#include <semaine/config.h>
#endif

#include <cms/CMSException.h>
#include <semaine/components/Component.h>
#include <semaine/cms/receiver/FeatureReceiver.h>

#include <core/dataSource.hpp>

#define COMPONENT_DESCRIPTION_CACTIVEMQSOURCE "dataSource which reads data vectors from the SEMAINE ActiveMQ server"
#define COMPONENT_NAME_CACTIVEMQSOURCE "cActiveMqSource"

class cActiveMqSource : public cDataSource {
  private:
    int nValues;
    float fStep;
    const char *vadRecv;

    MetaMessenger *meta; 
    int lag; int warned;
    int running;

    cVector *vec;

  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    virtual void fetchConfig();
    //virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);
    
    virtual int setupNewNames(long nEl);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cActiveMqSource(const char *_name);
    cDataWriter * getWriter() { return writer; }

    cVector *getVec() { return vec; }

    // this method sets the semaine feature sender to use... do not call it when the system is running, only call it during setup!!
    //void setFeatureReceiver(FeatureReceiver *_receiver, MetaMessenger *_meta=NULL) { featureReceiver = _receiver; meta=_meta; }

    void waveFileReceive(void *data, long long N);

    int isRunning() { return running; }

    virtual ~cActiveMqSource();
};


#endif // HAVE_SEMAINEAPI

#endif // __CACTIVEMQSOURCE_HPP
