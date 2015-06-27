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


#ifndef __CRNNSINK_HPP
#define __CRNNSINK_HPP

#include <core/smileCommon.hpp>

#ifdef BUILD_RNN
#include <core/dataSink.hpp>
#include <rnn/rnn.hpp>

#define COMPONENT_DESCRIPTION_CRNNSINK "This is an example of a cDataSink descendant. It reads data from the data memory and prints it to the console. This component is intended as a template for developers."
#define COMPONENT_NAME_CRNNSINK "cRnnSink"

#undef class




class DLLEXPORT cRnnSink : public cDataSink {
  private:
    FILE *outfile;
    FILE *outfileC;
    const char *netfile;
    const char *actoutput;
    const char *classoutput;
    char *classlabels;
    const char **classlabelArr;
    long nClasses;
    int ctcDecode;
    cNnRnn *rnn;
    cRnnNetFile net;
    FLOAT_NN *in;
    FLOAT_DMEM *out;
    int lasti;
    int printConnections;

  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    //cRnnWeightVector *createWeightVectorFromLine(char *line);
    //int loadNet(const char *filename);
    
//    int findPeepWeights(unsigned long id);
    //int findWeights(unsigned long idFrom, unsigned long idTo);
    //cNnLSTMlayer *createLstmLayer(int i, int idx, int dir=LAYER_DIR_FWD);
    // create a network from a successfully loaded net config file (loadNet function)
    //int createNet();
    
    virtual void fetchConfig();
    virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cRnnSink(const char *_name);

    virtual ~cRnnSink();
};


#endif // BUILD_RNN

#endif // __CRNNSINK_HPP
