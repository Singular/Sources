/*
 * This was automaticly generated by modgen
 * version 0.2
 * module pcv
 * Don't edit this file
 */


#line 10 "pcv.cc"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include <locals.h>
#include "pcv.h"
#line 19 "pcv.cc"

idhdl enter_id(char *name, char *value, idtyp t)
{
  idhdl h;

  h=enterid(mstrdup(name),0, t, &IDROOT, TRUE/*FALSE*/);
  if(h!=NULL) {
     switch(t) {
         case STRING_CMD: IDSTRING(h) = mstrdup(value);break;
         case PACKAGE_CMD: break;
         case PROC_CMD: break;
     }
  } else
      Warn("Cannot create '%s'\n", name);
  return(h);
}

#line 37 "pcv.cc"

idhdl add_singular_proc(char *procname, int line,
                       long pos, long end, BOOLEAN pstatic)
{
  idhdl h;
  procinfov pi;

  h = enter_id(procname, NULL, PROC_CMD);
  if(h == NULL) return NULL;

  pi->procname = mstrdup(procname);
  pi->language = LANG_SINGULAR;
  pi->ref = 1;
  pi->is_static = pstatic;
  pi->data.s.proc_start = pos;
  pi->data.s.def_end    = pos;
  pi->data.s.help_start = 0L;
  pi->data.s.help_end   = 0L;
  pi->data.s.body_start = pos;
  pi->data.s.body_end   = end;
  pi->data.s.proc_end   = end;
  pi->data.s.example_start = 0L;
  pi->data.s.proc_lineno = line;
  pi->data.s.body_lineno = line;
  pi->data.s.example_lineno = 0;
  pi->data.s.body = NULL;
  pi->data.s.help_chksum = 0;

  return(h);
}

void fill_help_package(idhdl pl);
void fill_example_package(idhdl pl);

#line 2 "pcv.mod"
/*
 *
 *  Test mod fuer modgen
 */

#include <stdio.h>
#include "Pcv.h"
#line 11 "pcv.mod"


#line 84 "pcv.cc"
extern "C"
int mod_init(int(*iiAddCproc)())
{
  THREAD_VAR idhdl h;
  THREAD_VAR idhdl helphdl = enter_id("Help", NULL, PACKAGE_CMD);
  THREAD_VAR idhdl examplehdl = enter_id("Example", NULL, PACKAGE_CMD);


   if( helphdl == NULL)
     Warn("Cannot create help-package\n");
   else fill_help_package(helphdl);

   if( examplehdl == NULL)
     Warn("Cannot create example-package\n");
   else fill_example_package(examplehdl);
#line 15 "pcv.mod"
  enter_id("version","$Id$", STRING_CMD);
#line 25 "pcv.mod"
  enter_id("info","
LIBRARY: pcv.so  CONVERSION BETWEEN POLYS AND COEF VECTORS
AUTHOR:  Mathias Schulze, email: mschulze@mathematik.uni-kl.de

 MinDeg(p);      min deg of monomials of poly p
 P2CV(l,d0,d1);  list of coef vectors from deg d0 to d1 of polys in list l
 CV2P(l,d0,d1);  list of polys with coef vectors from deg d0 to d1 in list l
 Dim(d0,d1);     number of monomials from deg d0 to d1
 Basis(d0,d1);   list of monomials from deg d0 to d1
", STRING_CMD);

#line 31 "pcv.mod"
  iiAddCproc("pcv","MinDeg",FALSE, mod_MinDeg);

#line 37 "pcv.mod"
  iiAddCproc("pcv","P2CV",FALSE, mod_P2CV);

#line 51 "pcv.mod"
  iiAddCproc("pcv","CV2P",FALSE, mod_CV2P);

#line 66 "pcv.mod"
  iiAddCproc("pcv","Dim",FALSE, mod_Dim);

#line 81 "pcv.mod"
  iiAddCproc("pcv","Basis",FALSE, mod_Basis);

  return 0;
}

#line 132 "pcv.cc"
/* Help section */
void fill_help_package(idhdl pl) {
  namespaceroot->push(IDPACKAGE(pl), IDID(pl));

#line 137 "pcv.cc"
  namespaceroot->pop();
}  /* End of Help section */

/* Example section */
void fill_example_package(idhdl pl) {
  namespaceroot->push(IDPACKAGE(pl), IDID(pl));

#line 145 "pcv.cc"
  namespaceroot->pop();
} /* End of Example section */

#line 31 "pcv.mod"
BOOLEAN mod_MinDeg(leftv res, leftv h)
{
#line 31 "pcv.mod"
#line 153 "pcv.cc"

#line 32 "pcv.mod"
#line 156 "pcv.cc"
  leftv v = h, v_save;
  int tok = NONE, index = 0;
  sleftv sp; leftv zp = &sp;
  poly p;
#line 32 "pcv.mod"


#line 33 "pcv.mod"
#line 165 "pcv.cc"
  if(v==NULL) goto mod_MinDeg_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, POLY_CMD))==0)
     goto mod_MinDeg_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, POLY_CMD, index, v, zp))
     goto mod_MinDeg_error;
  v = v_save;
  p = (poly)zp->Data();
  if(v!=NULL) { tok = v->Typ(); goto mod_MinDeg_error; }

#line 33 "pcv.mod"

#line 34 "pcv.mod"
#line 181 "pcv.cc"
  res->rtyp = INT_CMD;
  res->data = (void *)pcvMinDeg(p);
  if(res->data != NULL) return FALSE;
  else return TRUE;


  mod_MinDeg_error:
    Werror("MinDeg(`%s`) is not supported", Tok2Cmdname(tok));
    Werror("expected MinDeg('poly')");
    return TRUE;
}

#line 37 "pcv.mod"
BOOLEAN mod_P2CV(leftv res, leftv h)
{
#line 37 "pcv.mod"
#line 198 "pcv.cc"

#line 38 "pcv.mod"
#line 201 "pcv.cc"
  leftv v = h, v_save;
  int tok = NONE, index = 0;
  sleftv spl; leftv zpl = &spl;
  lists pl;
  sleftv sd0; leftv zd0 = &sd0;
  int d0;
  sleftv sd1; leftv zd1 = &sd1;
  int d1;
#line 38 "pcv.mod"



  /* check if current RingHandle is set */
  if(currRingHdl == NULL)
  {
    WerrorS("no ring active");
    return TRUE;
  }

#line 47 "pcv.mod"
#line 222 "pcv.cc"
  if(v==NULL) goto mod_P2CV_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, LIST_CMD))==0)
     goto mod_P2CV_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, LIST_CMD, index, v, zpl))
     goto mod_P2CV_error;
  v = v_save;
  pl = (lists)zpl->Data();
  if(v==NULL) goto mod_P2CV_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, INT_CMD))==0)
     goto mod_P2CV_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, INT_CMD, index, v, zd0))
     goto mod_P2CV_error;
  v = v_save;
  d0 = (int)zd0->Data();
  if(v==NULL) goto mod_P2CV_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, INT_CMD))==0)
     goto mod_P2CV_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, INT_CMD, index, v, zd1))
     goto mod_P2CV_error;
  v = v_save;
  d1 = (int)zd1->Data();
  if(v!=NULL) { tok = v->Typ(); goto mod_P2CV_error; }

#line 47 "pcv.mod"

#line 48 "pcv.mod"
#line 258 "pcv.cc"
  res->rtyp = LIST_CMD;
  res->data = (void *)pcvP2CV(pl, d0, d1);
  if(res->data != NULL) return FALSE;
  else return TRUE;


  mod_P2CV_error:
    Werror("P2CV(`%s`) is not supported", Tok2Cmdname(tok));
    Werror("expected P2CV('list','int','int')");
    return TRUE;
}

#line 51 "pcv.mod"
BOOLEAN mod_CV2P(leftv res, leftv h)
{
#line 52 "pcv.mod"
#line 275 "pcv.cc"

#line 53 "pcv.mod"
#line 278 "pcv.cc"
  leftv v = h, v_save;
  int tok = NONE, index = 0;
  sleftv spl; leftv zpl = &spl;
  lists pl;
  sleftv sd0; leftv zd0 = &sd0;
  int d0;
  sleftv sd1; leftv zd1 = &sd1;
  int d1;
#line 53 "pcv.mod"



  /* check if current RingHandle is set */
  if(currRingHdl == NULL)
  {
    WerrorS("no ring active");
    return TRUE;
  }

#line 62 "pcv.mod"
#line 299 "pcv.cc"
  if(v==NULL) goto mod_CV2P_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, LIST_CMD))==0)
     goto mod_CV2P_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, LIST_CMD, index, v, zpl))
     goto mod_CV2P_error;
  v = v_save;
  pl = (lists)zpl->Data();
  if(v==NULL) goto mod_CV2P_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, INT_CMD))==0)
     goto mod_CV2P_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, INT_CMD, index, v, zd0))
     goto mod_CV2P_error;
  v = v_save;
  d0 = (int)zd0->Data();
  if(v==NULL) goto mod_CV2P_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, INT_CMD))==0)
     goto mod_CV2P_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, INT_CMD, index, v, zd1))
     goto mod_CV2P_error;
  v = v_save;
  d1 = (int)zd1->Data();
  if(v!=NULL) { tok = v->Typ(); goto mod_CV2P_error; }

#line 62 "pcv.mod"

#line 63 "pcv.mod"
#line 335 "pcv.cc"
  res->rtyp = LIST_CMD;
  res->data = (void *)pcvCV2P(pl, d0, d1);
  if(res->data != NULL) return FALSE;
  else return TRUE;


  mod_CV2P_error:
    Werror("CV2P(`%s`) is not supported", Tok2Cmdname(tok));
    Werror("expected CV2P('list','int','int')");
    return TRUE;
}

#line 66 "pcv.mod"
BOOLEAN mod_Dim(leftv res, leftv h)
{
#line 67 "pcv.mod"
#line 352 "pcv.cc"

#line 68 "pcv.mod"
#line 355 "pcv.cc"
  leftv v = h, v_save;
  int tok = NONE, index = 0;
  sleftv sd0; leftv zd0 = &sd0;
  int d0;
  sleftv sd1; leftv zd1 = &sd1;
  int d1;
#line 68 "pcv.mod"



  /* check if current RingHandle is set */
  if(currRingHdl == NULL)
  {
    WerrorS("no ring active");
    return TRUE;
  }

#line 77 "pcv.mod"
#line 374 "pcv.cc"
  if(v==NULL) goto mod_Dim_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, INT_CMD))==0)
     goto mod_Dim_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, INT_CMD, index, v, zd0))
     goto mod_Dim_error;
  v = v_save;
  d0 = (int)zd0->Data();
  if(v==NULL) goto mod_Dim_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, INT_CMD))==0)
     goto mod_Dim_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, INT_CMD, index, v, zd1))
     goto mod_Dim_error;
  v = v_save;
  d1 = (int)zd1->Data();
  if(v!=NULL) { tok = v->Typ(); goto mod_Dim_error; }

#line 77 "pcv.mod"

#line 78 "pcv.mod"
#line 400 "pcv.cc"
  res->rtyp = INT_CMD;
  res->data = (void *)pcvDim(d0, d1);
  if(res->data != NULL) return FALSE;
  else return TRUE;


  mod_Dim_error:
    Werror("Dim(`%s`) is not supported", Tok2Cmdname(tok));
    Werror("expected Dim('int','int')");
    return TRUE;
}

#line 81 "pcv.mod"
BOOLEAN mod_Basis(leftv res, leftv h)
{
#line 82 "pcv.mod"
#line 417 "pcv.cc"

#line 83 "pcv.mod"
#line 420 "pcv.cc"
  leftv v = h, v_save;
  int tok = NONE, index = 0;
  sleftv sd0; leftv zd0 = &sd0;
  int d0;
  sleftv sd1; leftv zd1 = &sd1;
  int d1;
#line 83 "pcv.mod"



  /* check if current RingHandle is set */
  if(currRingHdl == NULL)
  {
    WerrorS("no ring active");
    return TRUE;
  }

#line 92 "pcv.mod"
#line 439 "pcv.cc"
  if(v==NULL) goto mod_Basis_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, INT_CMD))==0)
     goto mod_Basis_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, INT_CMD, index, v, zd0))
     goto mod_Basis_error;
  v = v_save;
  d0 = (int)zd0->Data();
  if(v==NULL) goto mod_Basis_error;
  tok = v->Typ();
  if((index=iiTestConvert(tok, INT_CMD))==0)
     goto mod_Basis_error;
  v_save = v->next;
  v->next = NULL;
  if(iiConvert(tok, INT_CMD, index, v, zd1))
     goto mod_Basis_error;
  v = v_save;
  d1 = (int)zd1->Data();
  if(v!=NULL) { tok = v->Typ(); goto mod_Basis_error; }

#line 92 "pcv.mod"

#line 93 "pcv.mod"
#line 465 "pcv.cc"
  res->rtyp = LIST_CMD;
  res->data = (void *)pcvBasis(d0, d1);
  if(res->data != NULL) return FALSE;
  else return TRUE;


  mod_Basis_error:
    Werror("Basis(`%s`) is not supported", Tok2Cmdname(tok));
    Werror("expected Basis('int','int')");
    return TRUE;
}


#line 478 "pcv.cc"
