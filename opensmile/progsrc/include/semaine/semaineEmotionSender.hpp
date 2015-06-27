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

The SEMAINE dimensional affect/emotion sender

*/


#ifndef __CSEMAINEEMOTIONSENDER_HPP
#define __CSEMAINEEMOTIONSENDER_HPP

#include <core/smileCommon.hpp>

#ifdef HAVE_SEMAINEAPI

#include <core/smileComponent.hpp>
#include <semaine/semaineEmmaSender.hpp>
#include <classifiers/libsvmliveSink.hpp>

#ifndef _MSC_VER
#include <semaine/config.h>
#endif
#include <cms/CMSException.h>
#include <semaine/components/Component.h>
#include <semaine/cms/sender/EmmaSender.h>

#include <semaine/util/XMLTool.h>
#include <semaine/datatypes/xml/EMMA.h>
#include <semaine/datatypes/xml/EmotionML.h>
#include <semaine/datatypes/xml/SemaineML.h>

#define COMPONENT_DESCRIPTION_CSEMAINEEMOTIONSENDER "the SEMAINE Emma message sender for sending keywords (ASR output)"
#define COMPONENT_NAME_CSEMAINEEMOTIONSENDER "cSemaineEmotionSender"

using namespace semaine::cms::sender;
using namespace semaine::util;
using namespace semaine::datatypes::xml;
using namespace XERCES_CPP_NAMESPACE;

#define XERCESC_NS XERCES_CPP_NAMESPACE

/*
#define N_GENDER_HISTORY 14
#define GENDER_M  1
#define GENDER_F  2
*/

#undef class
class cSemaineEmotionSender : public cSemaineEmmaSender {
  private:
    const char *intensityStr;
    const char *unpredictabilityStr;

    void sendInterestC( cComponentMessage *_msg );

    void sendDimensionFSRE( cComponentMessage *_msg, std::string dimensionStr )
    {
      sendDimension( _msg , dimensionStr, EmotionML::VOC_FSRE_DIMENSION_DEFINITION );
    }

    void sendDimension( cComponentMessage *_msg, std::string dimensionStr, std::string dimensionVoc );
    void sendDimensionsFSRE_I( cComponentMessage *_msg );

  protected:
    SMILECOMPONENT_STATIC_DECL_PR

    virtual void fetchConfig();
    virtual int processComponentMessage( cComponentMessage *_msg );

   
  public:
    SMILECOMPONENT_STATIC_DECL

    cSemaineEmotionSender(const char *_name);
    virtual ~cSemaineEmotionSender() {}

};


#endif // HAVE_SEMAINEAPI

#endif // __CSEMAINEEMOTIONSENDER_HPP
