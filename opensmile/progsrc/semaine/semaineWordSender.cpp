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

the SEMAINE Emma message sender for openSMILE

*/


#include <semaine/semaineWordSender.hpp>
//#include <math.h>

#define MODULE "cSemaineWordSender"

#ifdef HAVE_SEMAINEAPI

SMILECOMPONENT_STATICS(cSemaineWordSender)

SMILECOMPONENT_REGCOMP(cSemaineWordSender)
{
  SMILECOMPONENT_REGCOMP_INIT
  scname = COMPONENT_NAME_CSEMAINEWORDSENDER;
  sdescription = COMPONENT_DESCRIPTION_CSEMAINEWORDSENDER;

  // configure your component's configType:

  SMILECOMPONENT_CREATE_CONFIGTYPE
  //ct->setField("reader", "dataMemory interface configuration (i.e. what slot to read from)", sconfman->getTypeObj("cDataReader"), NO_ARRAY, DONT_FREE);
  ct->setField("dummy","nothing",0);
  SMILECOMPONENT_IFNOTREGAGAIN( {} )

  SMILECOMPONENT_MAKEINFO(cSemaineWordSender);
}

SMILECOMPONENT_CREATE(cSemaineWordSender)



//-----

cSemaineWordSender::cSemaineWordSender(const char *_name) :
  cSemaineEmmaSender(_name)
{
}


// this includes the juliusResult struct/class definition
#include <classifiers/julius/kwsjKresult.h>

void cSemaineWordSender::sendKeywords( cComponentMessage *_msg )
{
  int i;
  juliusResult *k = (juliusResult *)(_msg->custData);
  if (k==NULL) return;

  int nW = 0;
  for (i=0; i<k->numW; i++) {
    // check for non-verbals.... and remove them, only preceed if words are left
    if (k->word[i][0] != '*') nW++;
  }
  if (nW == 0) return;

  char strtmp[150];
  sprintf(strtmp,"%.2f",_msg->floatData[0]);
  std::string valStr(strtmp);
  long long startTime = smileTimeToSemaineTime(_msg->userTime1);
  sprintf(strtmp,"%ld",startTime);
  std::string startTm(strtmp);
  sprintf(strtmp,"%ld",(long long)round((_msg->userTime2 - _msg->userTime1)*1000.0));
  std::string duration(strtmp);

  // Create and fill a simple EMMA document
  XERCESC_NS::DOMDocument * document = XMLTool::newDocument(EMMA::E_EMMA, EMMA::namespaceURI, EMMA::version);
  XMLTool::setPrefix(document->getDocumentElement(), "emma");

  XERCESC_NS::DOMElement * sequence = XMLTool::appendChildElement(document->getDocumentElement(), EMMA::E_SEQUENCE);
  XMLTool::setAttribute(sequence, EMMA::A_OFFSET_TO_START, startTm);
  XMLTool::setAttribute(sequence, EMMA::A_DURATION, duration);
  XMLTool::setPrefix(sequence, "emma");

  for (i=0; i<k->numW; i++) {

    // split combined keywords (TALK_TO_POPPY) etc. at the special character "_" and put them in individual tags
    /*
    char * tr = strdup(k->word[i]);
    char * tmp = tr;
    char * x = NULL;
    do {
      x = (char *)strchr(tmp, '_');
      // separate at '_'
      if (x!=NULL) {
        *x = 0;
      }

      // remove spaces
      //while (*tmp==' ') { tmp++; }
      //size_t l = strlen(tmp);
      //while (tmp[l-1] == ' ') { tmp[l-1] = 0; l--; }

      // append an xml keyword tag
      XERCESC_NS::DOMElement * interpretation = XMLTool::appendChildElement(sequence, EMMA::E_INTERPRETATION);
      sprintf(strtmp,"%ld",startTime + (long long)round((k->start[i])*1000.0));
      std::string offs(strtmp);
      sprintf(strtmp,"%s",tmp);
      std::string keyword(strtmp);
      sprintf(strtmp,"%.3f",k->conf[i]);
      std::string confidence(strtmp);
      XMLTool::setAttribute(interpretation, EMMA::A_OFFSET_TO_START, offs);
      XMLTool::setAttribute(interpretation, EMMA::A_TOKENS, keyword);
      XMLTool::setAttribute(interpretation, EMMA::A_CONFIDENCE, confidence);
      XMLTool::setPrefix(interpretation, "emma");


      // analyse next part of string, if present
      if (x != NULL) {
        tmp = x+1;
      }
    } while (x !=  NULL);
    free(tr);


*/
    // one word:
    
    if (k->word[i][0] != '*') {

      XERCESC_NS::DOMElement * interpretation = XMLTool::appendChildElement(sequence, EMMA::E_INTERPRETATION);
      sprintf(strtmp,"%ld",startTime + (long long)round((k->start[i])*1000.0));
      std::string offs(strtmp);
      sprintf(strtmp,"%s",k->word[i]);
      std::string keyword(strtmp);
      sprintf(strtmp,"%.3f",k->conf[i]);
      std::string confidence(strtmp);
      XMLTool::setAttribute(interpretation, EMMA::A_OFFSET_TO_START, offs);
      XMLTool::setAttribute(interpretation, EMMA::A_TOKENS, keyword);
      XMLTool::setAttribute(interpretation, EMMA::A_CONFIDENCE, confidence);
      XMLTool::setPrefix(interpretation, "emma");

    }
  }


  // Now send it
  sendDocument(document);
}




int cSemaineWordSender::processComponentMessage( cComponentMessage *_msg ) 
{ 
  if (isMessageType(_msg,"asrKeywordOutput")) {  
    SMILE_IDBG(3,"received 'asrKeywordOutput' message");
    sendKeywords(_msg);
    return 1;  // message was processed
  }
  
  return 0; // if message was not processed
}  




#endif //HAVE_SEMAINEAPI

