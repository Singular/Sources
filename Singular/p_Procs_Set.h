/****************************************
*  Computer Algebra System SINGULAR     *
****************************************/
/***************************************************************
 *  File:    p_ProcsSet.h
 *  Purpose: Procedures for setting p_Procs at run time
 *  Note:    this file is included by p_Procs_Dynamic/Static.cc
 *           The macros 
 *              DoSetProc(what, field, length, ord)
 *              InitSetProc(field, length ord)
 *           have to be defined before this file is included
 *  Author:  obachman (Olaf Bachmann)
 *  Created: 12/00
 *  Version: $Id: p_Procs_Set.h,v 1.2 2000-12-12 08:44:50 obachman Exp $
 *******************************************************************/

// extract p_Procs properties from a ring
static inline p_Field p_FieldIs(ring r)
{
  if (rField_is_Zp(r)) return FieldZp;          
  if (rField_is_R(r)) return FieldR;
  if (rField_is_GF(r)) return FieldGF;
  if (rField_is_Q(r)) return FieldQ;
#ifdef HAVE_MORE_FIELDS_IMPLEMENTED
  if (rField_is_long_R(r)) return FieldLong_R;
  if (rField_is_long_C(r)) return FieldLong_C;
  if (rField_is_Zp_a(r)) return FieldZp_a;
  if (rField_is_Q_a(r)) return FieldQ_a;
#endif
  return FieldGeneral;
}

static inline p_Length p_LengthIs(ring r)
{
  assume(r->ExpL_Size > 0);
  // here is a quick hack to take care of p_MemAddAdjust
  if (r->NegWeightL_Offset != NULL) return LengthGeneral;
  if (r->ExpL_Size == 1) return LengthOne;
  if (r->ExpL_Size == 2) return LengthTwo;
  if (r->ExpL_Size == 3) return LengthThree;
  if (r->ExpL_Size == 4) return LengthFour;
  if (r->ExpL_Size == 5) return LengthFive;
  if (r->ExpL_Size == 6) return LengthSix;
  if (r->ExpL_Size == 7) return LengthSeven;
  if (r->ExpL_Size == 8) return LengthEight;
  return LengthGeneral;
}

static inline int p_IsNomog(long* sgn, int l)
{
  int i;
  for (i=0;i<l;i++)
    if (sgn[i] > 0) return 0;
  
  return 1;
}

static inline int p_IsPomog(long* sgn, int l)
{
  int i;
  for (i=0;i<l;i++)
    if (sgn[i] < 0) return 0;
  return 1;
}

static inline p_Ord p_OrdIs(ring r)
{
  long* sgn = r->ordsgn;
  long l = r->ExpL_Size;
  int zero = 0;
  
  if (sgn[l-1] == 0) 
  {
    l--;
    zero = 1;
  }
  
  // we always favour the pomog cases
  if (p_IsPomog(sgn,l)) return (zero ? OrdPomogZero : OrdPomog);
  if (p_IsNomog(sgn,l)) return (zero ? OrdNomogZero : OrdNomog);
  
  assume(l > 1);
  
  if (sgn[0] == -1 && p_IsPomog(&sgn[1], l-1))
    return (zero ? OrdNegPomogZero : OrdNegPomog);
  if (sgn[l-1] == -1 && p_IsPomog(sgn, l-1))
    return (zero ? OrdPomogNegZero : OrdPomogNeg);

  if (sgn[0] == 1 && p_IsNomog(&sgn[1], l-1)) 
    return (zero ? OrdPosNomogZero : OrdPosNomog);
  if (sgn[l-1] == 1 && p_IsNomog(sgn, l-1))
    return (zero ? OrdNomogPosZero : OrdNomogPos);

  assume(l > 2);
  
  if (sgn[0] == 1 && sgn[1] == 1 && p_IsNomog(&sgn[2], l-2))
    return (zero ? OrdPosPosNomogZero : OrdPosPosNomog);

  if (sgn[0] == 1 && sgn[l-1] == 1 && p_IsNomog(&sgn[1], l-2))
    return (zero ? OrdPosNomogPosZero : OrdPosNomogPos);

  if (sgn[0] == -1 && sgn[1] == 1 && p_IsNomog(&sgn[2], l-2))
    return (zero ? OrdNegPosNomogZero : OrdNegPosNomog);

  return OrdGeneral;
}

// fields of this struct are set by DoSetProc
static p_Procs_s *_p_procs;

#ifdef RDEBUG 
// if set, then SetProcs sets only names, instead of functions
static int set_names = 0;
#endif

// Choose a set of p_Procs
void p_ProcsSet(ring r, p_Procs_s* p_Procs)
{
  p_Field     field = p_FieldIs(r);
  p_Length    length = p_LengthIs(r);
  p_Ord       ord = p_OrdIs(r);
  
  assume(p_Procs != NULL);
#ifdef RDEBUG
  memset(p_Procs, 0, sizeof(p_Procs_s));
#endif
  _p_procs = p_Procs;
  assume(IsValidSpec(field, length, ord));

  InitSetProcs(field, length, ord);
  SetProcs(field, length, ord);
  assume(
    (p_Procs->p_Delete != NULL) &&
    (p_Procs->p_ShallowCopyDelete != NULL) &&
    (p_Procs->p_Mult_nn != NULL) &&
    (p_Procs->pp_Mult_nn != NULL) &&
    (p_Procs->p_Copy != NULL) &&
    (p_Procs->pp_Mult_mm != NULL) &&
    (p_Procs->pp_Mult_mm_Noether != NULL) &&
    (p_Procs->p_Mult_mm != NULL) &&
    (p_Procs->p_Add_q != NULL) &&
    (p_Procs->p_Neg != NULL) &&
    (p_Procs->pp_Mult_Coeff_mm_DivSelect != NULL) &&
    (p_Procs->p_Merge_q != NULL) &&
    (p_Procs->p_kBucketSetLm != NULL) &&
    (p_Procs->p_Minus_mm_Mult_qq != NULL));
  assume(p_Procs->pp_Mult_mm_Noether != pp_Mult_mm_Noether__FieldGeneral_LengthGeneral_OrdGeneral || 
         p_Procs->p_Minus_mm_Mult_qq == p_Minus_mm_Mult_qq__FieldGeneral_LengthGeneral_OrdGeneral || 
         r->OrdSgn == 1 || r->LexOrder);
}

#ifdef RDEBUG 
void p_Debug_GetSpecNames(const ring r, char* &field, char* &length, char* &ord)
{
  p_Field     e_field = p_FieldIs(r);
  p_Length    e_length = p_LengthIs(r);
  p_Ord       e_ord = p_OrdIs(r);
  
  field  = p_FieldEnum_2_String(p_FieldIs(r));
  length = p_LengthEnum_2_String(p_LengthIs(r));
  ord    = p_OrdEnum_2_String(p_OrdIs(r));
}

void p_Debug_GetProcNames(const ring r, p_Procs_s* p_Procs)
{
  set_names = 1;
  p_ProcsSet(r, p_Procs);
  set_names = 0;
}
#endif // RDEBUG
