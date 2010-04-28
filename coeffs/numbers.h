#ifndef NUMBERS_H
#define NUMBERS_H
/****************************************
*  Computer Algebra System SINGULAR     *
****************************************/
/* $Id$ */
/*
* ABSTRACT: interface to coefficient aritmetics
*/
#include "coeffs.h"

#define SHORT_REAL_LENGTH 6 // use short reals for real <= 6 digits

#define n_Copy(n, r)          (r)->cfCopy(n,r)
#define n_Delete(n, r)        (r)->cfDelete(n,r)
#define n_Mult(n1, n2, r)     (r)->nMult(n1, n2,r)
#define n_Add(n1, n2, r)      (r)->nAdd(n1, n2,r)
#define n_IsZero(n, r)        (r)->nIsZero(n,r)
#define n_Equal(n1, n2, r)    (r)->nEqual(n1, n2,r)
#define n_Neg(n, r)           (r)->nNeg(n,r)
#define n_Sub(n1, n2, r)      (r)->nSub(n1, n2,r)
#define n_GetChar(r)          ((r)->ch)
#define n_Init(i, r)          (r)->cfInit(i,r)
#define n_IsOne(n, r)         (r)->nIsOne(n,r)
#define n_IsMOne(n, r)        (r)->nIsMOne(n,r)
#define n_GreaterZero(n, r)   (r)->nGreaterZero(n,r)
#define n_Write(n, r)         (r)->cfWrite(n,r)
#define n_Normalize(n, r)     (r)->nNormalize(n,r)
#define n_Gcd(a, b, r)        (r)->nGcd(a,b,r)
#define n_IntDiv(a, b, r)     (r)->nIntDiv(a,b,r)
#define n_Div(a, b, r)        (r)->nDiv(a,b,r)
#define n_Invers(a, r)        (r)->nInvers(a,r)
#define n_ExactDiv(a, b, r)   (r)->nExactDiv(a,b,r)
#define n_Test(a,r)           (r)->nDBTest(a,r,__FILE__,__LINE__)

#define n_InpMult(a, b, r)    (r)->nInpMult(a,b,r)
#define n_Power(a, b, res, r) (r)->nPower(a,b,res,r)
#define n_Size(n,r)           (r)->nSize(n,r)
#define n_GetDenom(N,r)       (r)->cfGetDenom((N),r)
#define n_GetNumerator(N,r)   (r)->cfGetNumerator((N),r)

#define n_New(n, r)           nNew(n)

/* variables */
extern unsigned short fftable[];

/* prototypes */
void           nNew(number * a);
#define        nInit(i) n_Init(i,currRing)
#define nWrite(A) n_Write(A,currRing)

#define nTest(a) (1)
#define nDelete(A) (currRing)->cf->cfDelete(A,currRing)
#define nGetDenom(N) (currRing->cf->cfGetDenom((N),currRing))
#define nGetNumerator(N) (currRing->cf->cfGetNumerator((N),currRing))

#define nSetMap(R) (currRing->cf->cfSetMap(R,currRing))

void nDummy1(number* d);
void ndDelete(number* d, const coeffs r);
void nDummy2(number &d);
number ndGcd(number a, number b, const coeffs);
number ndCopy(number a, const coeffs r);
void   ndInpMult(number &a, number b, const coeffs r);
number ndInpAdd(number &a, number b, const coeffs r);

#ifdef LDEBUG
void nDBDummy1(number* d,char *f, int l);
#endif
#define nGetChar() n_GetChar(currRing)

void nInitChar(coeffs r);
void nKillChar(coeffs r);
void nSetChar(coeffs r);

#define nDivBy0 "div by 0"

// dummy routines
void   nDummy2(number& d); // nNormalize...

// Tests:
#ifdef HAVE_RINGS
static inline BOOLEAN nField_is_Ring_2toM(const coeffs r)
{ return (r->ringtype == 1); }

static inline BOOLEAN nField_is_Ring_ModN(const coeffs r)
{ return (r->ringtype == 2); }

static inline BOOLEAN nField_is_Ring_PtoM(const coeffs r)
{ return (r->ringtype == 3); }

static inline BOOLEAN nField_is_Ring_Z(const coeffs r)
{ return (r->ringtype == 4); }

static inline BOOLEAN nField_is_Ring(const coeffs r)
{ return (r->ringtype != 0); }

static inline BOOLEAN nField_is_Domain(const coeffs r)
{ return (r->ringtype == 4 || r->ringtype == 0); }

static inline BOOLEAN nField_has_Units(const coeffs r)
{ return ((r->ringtype == 1) || (r->ringtype == 2) || (r->ringtype == 3)); }
#else
#define nField_is_Ring(A) (0)
#endif


#ifdef _TRY
#ifndef ABS
#define ABS(x) ((x)<0?(-(x)):(x))
#endif
#endif



#ifdef _TRY
#define rField_is_Q(r) nField_is_Q(r)
#define rField_is_long_R(r) nField_is_long_R(r) 
#define rField_is_long_C(r) nField_is_long_C(r)
#define rField_is_R(r) nField_is_R(r)
#define rField_is_Zp(r) nField_is_Zp(r)
#endif


#ifdef HAVE_RINGS
static inline BOOLEAN nField_is_Zp(const coeffs r)
{ return (r->ringtype == 0) && (r->ch > 1) && (r->parameter==NULL); }

static inline BOOLEAN nField_is_Zp(const coeffs r, int p)
{ return (r->ringtype == 0) && (r->ch > 1 && r->ch == ABS(p) && r->parameter==NULL); }

static inline BOOLEAN nField_is_Q(const coeffs r)
{ return (r->ringtype == 0) && (r->ch == 0) && (r->parameter==NULL); }


static inline BOOLEAN nField_is_numeric(const coeffs r) /* R, long R, long C */
{ return (r->ringtype == 0) && (r->ch ==  -1); }

static inline BOOLEAN nField_is_R(const coeffs r)
{
  if (nField_is_numeric(r) && (r->float_len <= (short)SHORT_REAL_LENGTH))
      return (r->ringtype == 0) && (r->parameter==NULL);
  return FALSE;
}

static inline BOOLEAN nField_is_GF(const coeffs r)
{ return (r->ringtype == 0) && (r->ch > 1) && (r->parameter!=NULL); }

static inline BOOLEAN nField_is_GF(const coeffs r, int q)
{ return (r->ringtype == 0) && (r->ch == q); }

static inline BOOLEAN nField_is_Zp_a(const coeffs r)
{ return (r->ringtype == 0) && (r->ch < -1); }

static inline BOOLEAN nField_is_Zp_a(const coeffs r, int p)
{ return (r->ringtype == 0) && (r->ch < -1 ) && (-(r->ch) == ABS(p)); }

static inline BOOLEAN nField_is_Q_a(const coeffs r)
{ return (r->ringtype == 0) && (r->ch == 1); }

static inline BOOLEAN nField_is_long_R(const coeffs r)
{
  if (nField_is_numeric(r) && (r->float_len >(short)SHORT_REAL_LENGTH))
    return (r->ringtype == 0) && (r->parameter==NULL);
  return FALSE;
}

static inline BOOLEAN nField_is_long_C(const coeffs r)
{
  if (nField_is_numeric(r))
    return (r->ringtype == 0) && (r->parameter!=NULL);
  return FALSE;
}

#else


static inline BOOLEAN nField_is_Zp(const coeffs r)
{ return (r->ch > 1) && (r->parameter==NULL); }

static inline BOOLEAN nField_is_Zp(const coeffs r, int p)
{ return (r->ch > 1 && r->ch == ABS(p) && r->parameter==NULL); }

static inline BOOLEAN nField_is_Q(const coeffs r)
{ return (r->ch == 0) && (r->parameter==NULL); }

static inline BOOLEAN nField_is_numeric(const coeffs r) /* R, long R, long C */
{ return (r->ch ==  -1); }

static inline BOOLEAN nField_is_R(const coeffs r)
{
  if (nField_is_numeric(r) && (r->float_len <= (short)SHORT_REAL_LENGTH))
    return (r->parameter==NULL);
  return FALSE;
}

static inline BOOLEAN nField_is_GF(const coeffs r)
{ return (r->ch > 1) && (r->parameter!=NULL); }

static inline BOOLEAN nField_is_GF(const coeffs r, int q)
{ return (r->ch == q); }

static inline BOOLEAN nField_is_Zp_a(const coeffs r)
{ return (r->ch < -1); }

static inline BOOLEAN nField_is_Zp_a(const coeffs r, int p)
{ return (r->ch < -1 ) && (-(r->ch) == ABS(p)); }

static inline BOOLEAN nField_is_Q_a(const coeffs r)
{ return (r->ch == 1); }

static inline BOOLEAN nField_is_long_R(const coeffs r)
{
  if (nField_is_numeric(r) && (r->float_len >(short)SHORT_REAL_LENGTH))
    return (r->parameter==NULL);
  return FALSE;
}

static inline BOOLEAN nField_is_long_C(const coeffs r)
{
  if (nField_is_numeric(r))
    return (r->parameter!=NULL);
  return FALSE;
}
#endif

static inline BOOLEAN nField_has_simple_inverse(const coeffs r)
/* { return (r->ch>1) || (r->ch== -1); } *//* Z/p, GF(p,n), R, long_R, long_C*/
#ifdef HAVE_RINGS
{ return (r->ringtype > 0) || (r->ch>1) || ((r->ch== -1) && (r->float_len < 10)); } /* Z/2^n, Z/p, GF(p,n), R, long_R, long_C*/
#else
{ return (r->ch>1) || ((r->ch== -1) && (r->float_len < 10)); } /* Z/p, GF(p,n), R, long_R, long_C*/
#endif

static inline BOOLEAN nField_has_simple_Alloc(const coeffs r)
{ return (nField_is_Zp(r)
       || nField_is_GF(r)
#ifdef HAVE_RINGS
       || nField_is_Ring_2toM(r)
#endif
       || nField_is_R(r));
}
/* Z/p, GF(p,n), R: nCopy, nNew, nDelete are dummies*/

static inline BOOLEAN nField_is_Extension(const coeffs r)
{ return (nField_is_Q_a(r)) || (nField_is_Zp_a(r)); } /* Z/p(a) and Q(a)*/

#endif
