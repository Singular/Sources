/****************************************
*  Computer Algebra System SINGULAR     *
****************************************/
/***************************************************************
 *  File:    LPDV__pp_Mult_mm__Template.cc
 *  Purpose: template for LPDV__pp_Mult_mm
 *  Author:  obachman (Olaf Bachmann) (small Additions from
 *           gribo for the Letterplace DVec case)
 *  Created: 10/2012
 *******************************************************************/

#define HAVE_SHIFTBBADVEC

/***************************************************************
 *
 *   Returns:  p*m, ll
 *   ll == pLength(p*m) , if on input ll < 0
 *   pLength(p) - pLength(p*m), if on input ll >= 0
 *   Const:    p, m
 *
 ***************************************************************/
LINKAGE poly LPDV__pp_Mult_mm_Noether__T
  ( poly p, const poly m, 
    const poly spNoether, int &ll, const ring ri )
{
  p_Test(p, ri);
  p_LmTest(m, ri);
  assume(spNoether != NULL);
  if (p == NULL)
  {
    ll = 0;
    return NULL;
  }
  spolyrec rp;
  poly q = &rp, r;
  const unsigned long *spNoether_exp = spNoether->exp;
  number ln = pGetCoeff(m);
  omBin bin = ri->PolyBin;
  DECLARE_LENGTH(const unsigned long length = ri->ExpL_Size);
  DECLARE_ORDSGN(const long* ordsgn = ri->ordsgn);
  const unsigned long* m_e = m->exp;
  pAssume(!n_IsZero__T(ln,ri));
  pAssume1(p_GetComp(m, ri) == 0 || p_MaxComp(p, ri) == 0);
  int l = 0;

  do
  {
    p_AllocBin(r, bin, ri);

#ifdef HAVE_SHIFTBBADVEC
    p_MemCopy__T(r->exp, p->exp, length);
    ri->p_ExpSum(r, p, m, ri);
#else
    p_MemSum__T(r->exp, p->exp, m_e, length);
    p_MemAddAdjust__T(r, ri); //BOCO: Do we need this?
#endif

    p_MemCmp__T(r->exp, spNoether_exp, length, ordsgn, goto Continue, goto Continue, goto Break);

    Break:
    p_FreeBinAddr(r, ri);
    break;

    Continue:
    l++;
    q = pNext(q) = r;
    pSetCoeff0(q, n_Mult__T(ln, pGetCoeff(p), ri));
    pIter(p);
  } while (p != NULL);

  if (ll < 0)
    ll = l;
  else
    ll = pLength(p);

  pNext(q) = NULL;

  p_Test(pNext(&rp), ri);
  return pNext(&rp);
}
