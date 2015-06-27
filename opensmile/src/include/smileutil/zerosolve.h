/* Zero solver.
 * Modified by Florian Eyben, 2010, integration into openSMILE
 */

#ifndef __ZEROSOLVER_H__
#define __ZEROSOLVER_H__

#include <stdlib.h>

#undef __BEGIN_DECLS
#undef __END_DECLS

#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS

#define ZEROSOLVER_DBL_EPSILON        2.2204460492503131e-16

/* two consecutive built-in types as a complex number */
typedef double *       zerosolver_complex_packed ;
typedef float *        zerosolver_complex_packed_float  ;
typedef long double *  zerosolver_complex_packed_long_double ;

typedef const double *       zerosolver_const_complex_packed ;
typedef const float *        zerosolver_const_complex_packed_float  ;
typedef const long double *  zerosolver_const_complex_packed_long_double ;


/* 2N consecutive built-in types as N complex numbers */
typedef double *       zerosolver_complex_packed_array ;
typedef float *        zerosolver_complex_packed_array_float  ;
typedef long double *  zerosolver_complex_packed_array_long_double ;

typedef const double *       zerosolver_const_complex_packed_array ;
typedef const float *        zerosolver_const_complex_packed_array_float  ;
typedef const long double *  zerosolver_const_complex_packed_array_long_double ;



/* Yes... this seems weird. Trust us. The point is just that
   sometimes you want to make it obvious that something is
   an output value. The fact that it lacks a 'const' may not
   be enough of a clue for people in some contexts.
 */
typedef double *       zerosolver_complex_packed_ptr ;
typedef float *        zerosolver_complex_packed_float_ptr  ;
typedef long double *  zerosolver_complex_packed_long_double_ptr ;

typedef const double *       zerosolver_const_complex_packed_ptr ;
typedef const float *        zerosolver_const_complex_packed_float_ptr  ;
typedef const long double *  zerosolver_const_complex_packed_long_double_ptr ;


typedef struct
  {
    long double dat[2];
  }
zerosolver_complex_long_double;

typedef struct
  {
    double dat[2];
  }
zerosolver_complex;

typedef struct
  {
    float dat[2];
  }
zerosolver_complex_float;

#define ZEROSOLVER_REAL(z)     ((z).dat[0])
#define ZEROSOLVER_IMAG(z)     ((z).dat[1])
#define ZEROSOLVER_COMPLEX_P(zp) ((zp)->dat)
#define ZEROSOLVER_COMPLEX_P_REAL(zp)  ((zp)->dat[0])
#define ZEROSOLVER_COMPLEX_P_IMAG(zp)  ((zp)->dat[1])
#define ZEROSOLVER_COMPLEX_EQ(z1,z2) (((z1).dat[0] == (z2).dat[0]) && ((z1).dat[1] == (z2).dat[1]))

#define ZEROSOLVER_SET_COMPLEX(zp,x,y) do {(zp)->dat[0]=(x); (zp)->dat[1]=(y);} while(0)
#define ZEROSOLVER_SET_REAL(zp,x) do {(zp)->dat[0]=(x);} while(0)
#define ZEROSOLVER_SET_IMAG(zp,y) do {(zp)->dat[1]=(y);} while(0)

#define ZEROSOLVER_SET_COMPLEX_PACKED(zp,n,x,y) do {*((zp)+2*(n))=(x); *((zp)+(2*(n)+1))=(y);} while(0)




/* Evaluate polynomial
 *
 * c[0] + c[1] x + c[2] x^2 + ... + c[len-1] x^(len-1)
 *
 * exceptions: none
 */




__inline double zerosolver_poly_eval(const double c[], const int len, const double x)
{
  int i;
  double ans = c[len-1];
  for(i=len-1; i>0; i--) ans = c[i-1] + x * ans;
  return ans;
}

__inline zerosolver_complex zerosolver_poly_complex_eval(const double c[], const int len, const zerosolver_complex z)
{
  int i;
  zerosolver_complex ans;
  ZEROSOLVER_SET_COMPLEX (&ans, c[len-1], 0.0);
  for(i=len-1; i>0; i--) {
    /* The following three lines are equivalent to
       ans = zerosolver_complex_add_real (zerosolver_complex_mul (z, ans), c[i-1]); 
       but faster */
    double tmp = c[i-1] + ZEROSOLVER_REAL (z) * ZEROSOLVER_REAL (ans) - ZEROSOLVER_IMAG (z) * ZEROSOLVER_IMAG (ans);
    ZEROSOLVER_SET_IMAG (&ans, ZEROSOLVER_IMAG (z) * ZEROSOLVER_REAL (ans) + ZEROSOLVER_REAL (z) * ZEROSOLVER_IMAG (ans));
    ZEROSOLVER_SET_REAL (&ans, tmp);
  } 
  return ans;
}

__inline zerosolver_complex zerosolver_complex_poly_complex_eval(const zerosolver_complex c[], const int len, const zerosolver_complex z)
{
  int i;
  zerosolver_complex ans = c[len-1];
  for(i=len-1; i>0; i--) {
    /* The following three lines are equivalent to
       ans = zerosolver_complex_add (c[i-1], zerosolver_complex_mul (x, ans));
       but faster */
    double tmp = ZEROSOLVER_REAL (c[i-1]) + ZEROSOLVER_REAL (z) * ZEROSOLVER_REAL (ans) - ZEROSOLVER_IMAG (z) * ZEROSOLVER_IMAG (ans);
    ZEROSOLVER_SET_IMAG (&ans, ZEROSOLVER_IMAG (c[i-1]) + ZEROSOLVER_IMAG (z) * ZEROSOLVER_REAL (ans) + ZEROSOLVER_REAL (z) * ZEROSOLVER_IMAG (ans));
    ZEROSOLVER_SET_REAL (&ans, tmp);
  }
  return ans;
}


/* Solve for the complex roots of a general real polynomial */

typedef struct 
{ 
  size_t nc ;
  double * matrix ; 
} 
zerosolver_poly_complex_workspace ;

zerosolver_poly_complex_workspace * zerosolver_poly_complex_workspace_alloc (size_t n);
void zerosolver_poly_complex_workspace_free (zerosolver_poly_complex_workspace * w);

int
zerosolver_poly_complex_solve (const double * a, size_t n, 
                        zerosolver_poly_complex_workspace * w,
                        zerosolver_complex_packed_ptr z);

__END_DECLS

#endif /* __ZEROSOLVER_H__ */
