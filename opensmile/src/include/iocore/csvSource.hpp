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

CSV (Comma seperated value) file reader. 
This component reads all columns as attributes into the data memory. One line thereby represents one frame or sample. The first line may contain a header with the feature names (see header=yes/no/auto option).

*/


#ifndef __CCSVSOURCE_HPP
#define __CCSVSOURCE_HPP

#include <core/smileCommon.hpp>
#include <core/dataSource.hpp>

#define COMPONENT_DESCRIPTION_CCSVSOURCE "This component reads CSV (Comma seperated value) files. It reads all columns as attributes into the data memory. One line represents one frame. The first line may contain a header with the feature names (see header=yes/no/auto option)."
#define COMPONENT_NAME_CCSVSOURCE "cCsvSource"

#undef class
class DLLEXPORT cCsvSource : public cDataSource {
  private:
    //int nAttr;
    FILE *filehandle;
    const char *filename;

    int *field;
    //int fieldNalloc;
    int nFields, nCols;
    int eof;

    int header;
    char delimChar;

    long lineNr;
    long start;
    long end;

  protected:
    SMILECOMPONENT_STATIC_DECL_PR
    
    void setGenericNames(int nDelim);
    void setNamesFromCSVheader(char *line, int nDelim);

    virtual void fetchConfig();
    //virtual int myConfigureInstance();
    virtual int myFinaliseInstance();
    virtual int myTick(long long t);

    virtual int setupNewNames(long nEl=0);

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cCsvSource(const char *_name);

    virtual ~cCsvSource();
};




#endif // __CCSVSOURCE_HPP
