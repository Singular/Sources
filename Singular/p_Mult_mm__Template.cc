/****************************************
*  Computer Algebra System SINGULAR     *
****************************************/
/***************************************************************
 *  File:    p_Mult_mm__Template.cc
 *  Purpose: template for p_Mult_n
 *  Author:  obachman (Olaf Bachmann)
 *  Created: 8/00
 *  Version: $Id: p_Mult_mm__Template.cc,v 1.7 2000-11-08 13:14:43 obachman Exp $
 *******************************************************************/

/***************************************************************
 *
 *   Returns:  p*m
 *   Const:    m
 *   Destroys: p  
 *
 ***************************************************************/
poly p_Mult_mm(poly p, const poly m, const ring ri)
{
  p_Test(p, ri);
  p_LmTest(m, ri);
  if (p == NULL) return NULL;
  pAssume(m != NULL);
  poly q = p;
  number ln = pGetCoeff(m);
  number pn;
  DECLARE_LENGTH(const unsigned long length = ri->ExpL_Size);
  const unsigned long* m_e = m->exp;
  pAssume(!n_IsZero(ln,ri));

  while (p != NULL)
  {
    pn = pGetCoeff(p);
    pSetCoeff0(p, n_Mult(ln, pn, ri));
    n_Delete(&pn, ri);
    p_MemAdd(p->exp, m_e, length);
    p_MemAddAdjust(p, ri);
    p = pNext(p);
  }
  p_Test(q, ri);
  return q;
}


