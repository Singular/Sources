#ifndef STAIRC_H
#define STAIRC_H
/****************************************
*  Computer Algebra System SINGULAR     *
****************************************/
/* $Id: stairc.h,v 1.5 1997-11-25 15:28:17 pohl Exp $ */
/*
* ABSTRACT
*/

#include "structs.h"

void scComputeHC(ideal s,int  k,poly &hEdge);

intvec * scIndIntvec(ideal S, ideal Q=NULL);
lists scIndIndset(ideal S, BOOLEAN all, ideal Q=NULL);
int scDimInt(ideal  s,ideal Q=NULL);
int scMultInt(ideal  s,ideal Q=NULL);
int scMult0Int(ideal  s,ideal Q=NULL);
void scDegree(ideal  s,ideal Q=NULL);

ideal scKBase(int deg, ideal  s, ideal Q=NULL);

intvec * hHstdSeries(ideal S, intvec *modulweight, ideal Q=NULL);
intvec * hFirstSeries(ideal S, intvec *modulweight, ideal Q=NULL);
intvec * hSecondSeries(intvec *hseries1);

void hLookSeries(ideal S, intvec *modulweight, ideal Q=NULL);

#endif


