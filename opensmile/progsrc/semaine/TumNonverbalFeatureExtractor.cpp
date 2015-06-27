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


/*
*  TumOpenSMILE.cpp
*  semaine
*  
*/
#include <core/smileCommon.hpp>

#ifdef HAVE_SEMAINEAPI


#include <semaine/TumNonverbalFeatureExtractor.h>

#undef MODULE
#define MODULE "TumNonverbalFeatureExtractor.SEMAINEcomponent"

#include <cstdlib>
#include <sstream>

#include <semaine/util/XMLTool.h>
#include <semaine/datatypes/xml/EMMA.h>
#include <semaine/datatypes/xml/EmotionML.h>
#include <semaine/datatypes/xml/SemaineML.h>

using namespace semaine::util;
using namespace semaine::datatypes::xml;
using namespace XERCES_CPP_NAMESPACE;

namespace semaine {
  namespace components {
    namespace smile {


    TumNonverbalFeatureExtractor::TumNonverbalFeatureExtractor(cComponentManager *_cMan, cConfigManager *_conf) throw(CMSException) : 
      ComponentForSmile("TumNonverbalFeatureExtractor",_cMan,_conf,true,false)
    {
      if (userinfoSenderInfo.topic != NULL) {
        userinfoSender = new EmmaSender(userinfoSenderInfo.topic, userinfoSenderInfo.componentName );
        senders.push_back(userinfoSender);
      } else {
        // TODO: use semaine exception here...
        SMILE_WRN(1,"userinfoSender.topic == NULL in TumFeatureExtractor, please check semaineCfg section in config file (no emma XML messages with user speaking status and gender will be sent!).");
      }
      
      if (nonverbalSenderInfo.topic != NULL) {
        nonverbalSender = new EmmaSender(nonverbalSenderInfo.topic, nonverbalSenderInfo.componentName );
        senders.push_back(nonverbalSender);
      } else {
        // TODO: use semaine exception here...
        SMILE_WRN(1,"nonverbalSender.topic == NULL in TumFeatureExtractor, please check semaineCfg section in config file (no emma XML messages with non-linguistic vocalisation will be sent!).");
      }



    }



    TumNonverbalFeatureExtractor::~TumNonverbalFeatureExtractor()
    {
      if (cMan != NULL) {
        //cMan->requestAbort();
        //smileThreadJoin( smileMainThread );
      }
    }

    void TumNonverbalFeatureExtractor::customStartIO() throw(CMSException)
    {
      if (cMan == NULL) { SMILE_ERR(1,"componentManager (cMan) is NULL, smileMainThread can not be started!"); }
      else {
        // main openSMILE initialisation
        //cMan->createInstances(0); // 0 = do not read config (we already did that ..)

        //wait for component manager to become ready....
        while (!cMan->isReady()) { smileSleep(100); } // TODO: make the isReady variable in cMan thread safe...!

        /* connect all the feature senders / receivers, etc. */
        setSmileEMMAsender(nonverbalSenderInfo.OSinstanceName, nonverbalSender);
        setSmileEMMAsender(userinfoSenderInfo.OSinstanceName, userinfoSender);
      }
      
    }

    void TumNonverbalFeatureExtractor::react(SEMAINEMessage * m) throw (std::exception) 
    {
    }

    void TumNonverbalFeatureExtractor::act() throw(CMSException)
    {
    }


    } // namespace smile
  } // namespace components
} // namespace semaine


#endif // HAVE_SEMAINEAPI
