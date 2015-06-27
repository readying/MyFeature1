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


#ifndef COMMANDLINE_PARSER_HPP
#define COMMANDLINE_PARSER_HPP

#include <core/smileCommon.hpp>

#ifdef __WINDOWS
#ifndef __MINGW32
// windows stdlib.h does not seem to define those, even though msdn library says it would ???
#define EINVAL 22
#define ERANGE 34
#else
#include <errno.h>
#endif
#else
#include <errno.h>
#endif

#define N_ALLOC_BLOCK 50
#define STRLEN_NAME 255

#define CMDOPT_BOOLEAN  0
#define CMDOPT_INT      1
#define CMDOPT_DOUBLE   2
#define CMDOPT_STR      3

struct sCmdlineOpt {
  char name[STRLEN_NAME];
  char abbr;
  char *description;
  int type;
  int dfltInt;
  double dfltDouble;
  char *dfltStr;
  int argMandatory;
  int isMandatory;
  int isSet;
  int nArgSet;
};

#undef class
class DLLEXPORT cCommandlineParser {

  private:
    int argc;
    char **argv;
    int N,Nalloc;
    int active;
    struct sCmdlineOpt * opt;

    int getWrIdx();
    int addOpt( const char *name, char abbr, const char *description, int argMandatory, int isMandatory);

    /* find index of option "name", return index upon success, else return -1 */
    int findOpt( const char *name ) const;

  public:

    cCommandlineParser(int _argc, char ** _argv);

    int addBoolean( const char *name, char abbr, const char *description , int dflt=0, int argMandatory=0, int isMandatory=0 );
    int addInt( const char *name, char abbr, const char *description , int dflt=0, int argMandatory=1, int isMandatory=0 );
    int addDouble( const char *name, char abbr, const char *description, double dflt=0.0, int argMandatory=1, int isMandatory=0 );
    int addStr( const char *name, char abbr, const char *description, const char *dflt=NULL, int argMandatory=1, int isMandatory=0 );

    void showUsage(const char *binname=NULL);
    int doParse(int ignDupl=0, int ignUnknown=1);  // return value: 0 on normal parse, -1 if usage was requested with '-h' (in this case the application should terminate after doParse() has finished

    int getBoolean( const char *name ) const;
    int getInt( const char *name ) const;
    int isSet( const char *name ) const;

    double getDouble( const char *name ) const;
    const char * getStr( const char *name ) const;

    ~cCommandlineParser();
};

#endif // COMMANDLINE_PARSER_HPP
