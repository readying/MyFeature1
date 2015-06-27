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


/*  SmileUtilSpline
    =========

contains modular DSP functions for splines

*/

#include <smileutil/smileUtilSpline.h>

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
int smileMath_spline (const double *x, const double *y, long n, double yp1, double ypn, double *y2, double **workspace)
{
  double p, qn, sig, un, *u = NULL;
  long i, k;

  if (workspace!=NULL) u = *workspace;
  if (u==NULL) u = (double*)malloc(sizeof(double)*(n-1));

  if (yp1 > 0.99e30) {
    y2[0] = u[0] = 0.0;
  } else {
    y2[0] = -0.5;
    u[0] = (3.0 / (x[1] - x[0])) * ((y[1] - y[0]) / (x[1] - x[0]) - yp1);
  }

  for (i=1; i < n-1; i++) { // for (i=2; i <= n-1; i++) {
    sig = (x[i] - x[i-1]) / (x[i+1] - x[i-1]);
    p = sig * y2[i-1] + 2.0;
    y2[i] = (sig - 1.0) / p;
    u[i] = (y[i+1] - y[i]) / (x[i+1] - x[i]) - (y[i] - y[i-1]) / (x[i] - x[i-1]);
    u[i] = (6.0 * u[i] / (x[i+1] - x[i-1]) - sig * u[i-1]) / p;
  }

  if (ypn > 0.99e30) {
    qn = un = 0.0;
  } else {
    qn = 0.5;
    un = (3.0 / (x[n-1] - x[n-2])) * (ypn - (y[n-1] - y[n-2]) / (x[n-1] - x[n-2]));
  }

  y2[n-1] = (un - qn * u[n-2]) / (qn * y2[n-2] + 1.0);
  for (k=n-2; k >= 0; k--) { // for (k=n-1; k >= 1; k--) {
    y2[k] = y2[k] * y2[k+1] + u[k];
  }

  if (workspace == NULL) {
    free(u);
  } else {
    *workspace = u;
  }
  
  return 1;
}


/* spline interpolation
        Given arrays xa[1..n] and ya[1..n] containing a tabulated function,
        i.e., y[i] = f(x[i]), with x[1] < x[2] < ... < x[n], and given the
        array y2a[1..n] which is the output of NUMspline above, and given
        a value of x, this routine returns an interpolated value y.
*/
int smileMath_splint (double xa[], double ya[], double y2a[], long n, double x, double *y)
{
  long klo, khi, k;
  double h, b, a;

  klo = 1; khi = n;
  while (khi-klo > 1)
  {
    k = (khi + klo) >> 1;
    if (xa[k-1] > x) khi = k;
    else klo = k;
  }
  khi--; klo--;
  h = xa[khi] - xa[klo];
  if (h == 0.0) {
    printf("smileMath: splint() : bad input value (h=0)\n");
    return 0;
  }
  a = (xa[khi] - x) / h;
  b = (x - xa[klo]) / h;
  *y = a * ya[klo] + b * ya[khi]+((a * a * a - a) * y2a[klo] +
    (b * b * b - b) * y2a[khi]) * (h * h) / 6.0;
  return 1;
}



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
 */
int smileDsp_calcLpcBurg (const FLOAT_DMEM *x, long n, FLOAT_DMEM *a, int m, FLOAT_DMEM *gain, FLOAT_DMEM **burgB1, FLOAT_DMEM **burgB2, FLOAT_DMEM **burgAA)
{
  long i = 1, j; int status = 0;
  FLOAT_DMEM p = 0.0;
  FLOAT_DMEM *aa = NULL;
  FLOAT_DMEM xms = 0.0;
  FLOAT_DMEM *b1 = NULL, *b2 = NULL;

  if (x == NULL) return 0;
  if (a == NULL) return 0;
  if ((n<m)||(m<=0)) return 0;
  
  if (burgB1 != NULL) b1 = *burgB1;
  if (b1 == NULL) b1 = (FLOAT_DMEM*)calloc(1,sizeof(FLOAT_DMEM)*n);

  if (burgB2 != NULL) b2 = *burgB2;
  if (b2 == NULL) b2 = (FLOAT_DMEM*)calloc(1,sizeof(FLOAT_DMEM)*n);

  if (burgAA != NULL) aa = *burgAA;
  if (aa == NULL) aa = (FLOAT_DMEM*)calloc(1,sizeof(FLOAT_DMEM)*m);

  /* (3) */
  for (j = 0; j < n; j++) {  // for (j = 1; j <= n; j++) {
    p += x[j] * x[j];
  }

  xms = p / n;
  if (xms <= 0) goto end;

  /* (9) */
  b1[0] = x[0];       // b1[1] = x[1];
  b2[n - 2] = x[n-1]; // b2[n - 1] = x[n];
  for (j = 1; j < n - 1; j++) { // for (j = 2; j <= n - 1; j++) {
    b1[j] = b2[j - 1] = x[j];
  }

  for (i = 0; i < m; i++) { // for (i = 1; i <= m; i++) {

    /* (7) */
    FLOAT_DMEM num = 0.0, denum = 0.0;
    for (j = 0; j < n - i - 1; j++) {  // for (j = 1; j <= n - i; j++) {  
      num += b1[j] * b2[j];
      denum += b1[j] * b1[j] + b2[j] * b2[j];
    }

    if (denum <= 0) goto end;

    a[i] = (FLOAT_DMEM)2.0 * num / denum;

    /* (10) */
    xms *= (FLOAT_DMEM)1.0 - a[i] * a[i];

    /* (5) */
    for (j = 0; j < i; j++) {  // for (j = 1; j <= i - 1; j++) {    
      a[j] = aa[j] - a[i] * aa[i - j - 1]; //  aa[i-j]
    }

    if (i < m-1) { // if (i < m) {
    
      /* (8) */
      /* Watch out: i -> i+1 */
      for (j = 0; j <= i; j++) { // for (j = 1; j <= i; j++) {   
        aa[j] = a[j];
      }
      for (j = 0; j < n - i - 2; j++) { // for (j = 1; j <= n - i - 1; j++)
        b1[j] -= aa[i] * b2[j];
        b2[j] = b2[j + 1] - aa[i] * b1[j + 1];
      }
    }
  }

  status = 1;

end:
  if (burgB1 != NULL) *burgB1 = b1;
  else if (b1 != NULL) free(b1);
  if (burgB2 != NULL) *burgB2 = b2;
  else if (b2 != NULL) free(b2);
  if (burgAA != NULL) *burgAA = aa;
  else if (aa != NULL) free(aa);

  for (j = 0; j < i; j++) a[j] = -a[j]; // invert coefficients for compatibility with ACF method's lpcs
  for (j = i; j < m; j++) a[j] = 0.0;   // pad remaining coefficients with zeroes
  // adjust gain:
  if (gain != NULL) *gain = xms * (FLOAT_DMEM)n;
  return status;
}

/* peak enhancement in a linear magnitude spectrum */
int smileDsp_specEnhanceSHS (double *a, long n)
{
  long i, j, nmax = 0, *posmax;
  posmax = (long *)calloc(1,sizeof(long)*( (n + 1) / 2 + 1/*?*/ ));
  if ( (n < 2) || (posmax == NULL)) return 0;

  if (a[0] > a[1]) posmax[nmax++] = 0;
  
  for (i=1; i < n-1; i++) {  // for (i=2; i <= n-1; i++) {
    if (a[i] > a[i-1] && a[i] >= a[i+1]) {
      posmax[nmax++] = i; // ++nmax] = i;
    }
  }

  if (a[n-1] > a[n-2]) posmax[nmax++] = n-1;

  if (nmax == 1) {
    for (j=0; j <= posmax[1]-3; j++) a[j] = 0; // for (j=1; j <= posmax[1]-3; j++) a[j] = 0;
    for (j=posmax[1]+3; j < n; j++) a[j] = 0; // for (j=posmax[1]+3; j <= n; j++) a[j] = 0;
  }
  else {
    for (i=1; i < nmax; i++) { // for (i=2; i <= nmax; i++) {
      for (j=posmax[i-1]+3; j <= posmax[i]-3; j++) a[j] = 0;
    }
  }

  free(posmax);
  return 1;
}

/* smooth a magnitude spectrum (linear) */
void smileDsp_specSmoothSHS (double *a, long n)
{
  double ai, aim1 = 0; long i;
  for (i=0; i < n-1; i++) { // for (i=1; i <= n-1; i++)
    ai = a[i]; a[i] = (aim1 + 2.0 * ai + a[i+1]) / 4.0; aim1 = ai;
  }
}


