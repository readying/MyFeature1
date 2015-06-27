/*F******************************************************************************
 *
 * openSMILE - open Speech and Music Interpretation by Large-space Extraction
 *       the open-source Munich Audio Feature Extraction Toolkit
 * Copyright (C) 2008-2009  Florian Eyben, Martin Woellmer, Bjoern Schuller
 *
 *
 * Institute for Human-Machine Communication
 * Technische Universitaet Muenchen (TUM)
 * D-80333 Munich, Germany
 *
 *
 * If you use openSMILE or any code from openSMILE in your research work,
 * you are kindly asked to acknowledge the use of openSMILE in your publications.
 * See the file CITING.txt for details.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ******************************************************************************E*/


#ifndef __SMILE_UTIL_SPLINE_H
#define __SMILE_UTIL_SPLINE_H

/* you may remove this include if you are using smileUtil outside of openSMILE */
//#include <smileCommon.hpp>
/* --------------------------------------------------------------------------- */
#ifndef __SMILE_COMMON_H
#define __SMILE_COMMON_H

// this is a minimal set of defines if we are using smileUtil outside of openSMILE
// on linux you should consider compiling with -DHAVE_INTTYPES_H option (see smileTypes.hpp)

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef _MSC_VER // Visual Studio specific macro
  #ifdef BUILDING_DLL
    #define DLLEXPORT __declspec(dllexport)
//    #define class class __declspec(dllexport)
  #else
    #define DLLEXPORT __declspec(dllimport)
//    #define class class __declspec(dllimport)
  #endif
  #define DLLLOCAL
#else
    #define DLLEXPORT
    #define DLLLOCAL
#endif

#include <stdlib.h>
#include <stdio.h>

#endif  // __SMILE_COMMON_H


#ifndef __SMILE_LOGGER_HPP
#define __SMILE_LOGGER_HPP
#define LOGLEVEL 2
#define SMILE_PRINT(...) fprintf(stderr,__VA_ARGS__)
#define SMILE_PRINTL(level, ...) { if (level <= LOGLEVEL) fprintf(stderr,__VA_ARGS__); }

#define SMILE_MSG(level, ...) { if (level <= LOGLEVEL) { fprintf(stderr,"MSG(%i): ",level) ; fprintf(stderr,__VA_ARGS__); } }
#define SMILE_IMSG(level, ...) { if (level <= LOGLEVEL) { fprintf(stderr,"MSG(%i): ",level) ; fprintf(stderr,__VA_ARGS__); } }

#define SMILE_ERR(level, ...) { if (level <= LOGLEVEL) { fprintf(stderr,"ERR(%i): ",level) ; fprintf(stderr,__VA_ARGS__); } }
#define SMILE_IERR(level, ...) { if (level <= LOGLEVEL) { fprintf(stderr,"ERR(%i): ",level) ; fprintf(stderr,__VA_ARGS__); } }

#define SMILE_WRN(level, ...) { if (level <= LOGLEVEL) { fprintf(stderr,"WRN(%i): ",level) ; fprintf(stderr,__VA_ARGS__); } }
#define SMILE_IWRN(level, ...) { if (level <= LOGLEVEL) { fprintf(stderr,"WRN(%i): ",level) ; fprintf(stderr,__VA_ARGS__); } }


#ifdef DEBUG
#define SMILE_DBG(level, ...) { if (level <= LOGLEVEL) { fprintf(stderr,"DBG(%i): ",level) ; fprintf(stderr,__VA_ARGS__); } }
#define SMILE_IDBG(level, ...) { if (level <= LOGLEVEL) { fprintf(stderr,"DBG(%i): ",level) ; fprintf(stderr,__VA_ARGS__); } }
#endif
#endif  // __SMILE_LOGGER_HPP

#include <core/smileTypes.hpp>


//#include <smileutil/smileUtil.h>

#ifdef __cplusplus
extern "C" {
#endif

/****** spline functions ******/
/*
        Given arrays a[1..n] and y[1..n] containing a tabulated function, i.e.,
        y[i] = f(x[i]), with x[1] < x[2] < ... < x[n], and given values yp1 and
        ypn for the first derivative of the interpolating function at point
        1 and n, respectively, this routine returns an array y2[1..n] that
        contains the second derivative of the interpolating function at the
        tabulated point x.
        If yp1 and/or ypn are greaterequal 10^30, the routine is signaled to
        set the corresponding boundary condition for a natuarl spline, with
        zero second derivative on that boundary.
*/
// u: is optional pointer to a workspace pointer (smileMath_spline will allocate
//    a vector there if the pointer pointed to is NULL
//    the calling code is responsible of freeing this memory with free() at any 
//    later time which seems convenient (i.e. at the end of all calculations)
DLLEXPORT int smileMath_spline (const double *x, const double *y, long n, double yp1, double ypn, double *y2, double **workspace);

/* spline interpolation
        Given arrays xa[1..n] and ya[1..n] containing a tabulated function,
        i.e., y[i] = f(x[i]), with x[1] < x[2] < ... < x[n], and given the
        array y2a[1..n] which is the output of NUMspline above, and given
        a value of x, this routine returns an interpolated value y.
*/
DLLEXPORT int smileMath_splint (double xa[], double ya[], double y2a[], long n, double x, double *y);

/* compute LPC coefficients with Burg's method (N. Anderson (1978)):
     N. Anderson (1978), "On the calculation of filter coefficients for maximum entropy spectral analysis", in Childers, Modern Spectrum Analysis, IEEE Press, 252-255.
   x : wave samples, 
   n : number of samples
   a : array to hold the coefficients
   m : number of coefficients desired
   gain : optional pointer to FLOAT_DMEM, which will be filled with the computed LPC gain
   burgB1, burgB2, burgAA : pointers to pointers to work area memory, 
     will be initialised automtically on first use, 
     must be freed by calling application on exit ;
     if these pointers are NULL, calcLpcBurg will allocate 
     the work area at the beginning of the function and
     free it at the end of the function 
     (the latter is ineffective if memory allocation is slow)
   return value: status 1 = computation successfull / 0 = failure during computation (mostly numeric trouble)
 */
DLLEXPORT int smileDsp_calcLpcBurg(const FLOAT_DMEM *x, long n, FLOAT_DMEM *a, int m, FLOAT_DMEM *gain, FLOAT_DMEM **burgB1, FLOAT_DMEM **burgB2, FLOAT_DMEM **burgAA);


/* peak enhancement in a linear magnitude spectrum */
DLLEXPORT int smileDsp_specEnhanceSHS(double *a, long n);

/* smooth a magnitude spectrum (linear) */
DLLEXPORT void smileDsp_specSmoothSHS(double *a, long n);

#ifdef __cplusplus
}
#endif

#endif // __SMILE_UTIL_SPLINE_H

