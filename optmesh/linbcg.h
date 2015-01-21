//
// linbcg.h
// from Numerical Recipes in C 
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _LINBCG_H
#define _LINBCG_H

#ifdef __cplusplus
extern "C" {
#endif

extern int linbcg(Semat *, double *, double *, int, double, int, int *, double *);
extern double snrm(int, double *, int);
extern void atimes(Semat *, int, double *, double *, int);
extern void asolve(Semat *, int, double *, double *, int);
// extern void dsprsin(HGfc *, Semat *); 
extern void dsprsax(double *, int *, double *, double *, int);
extern void dsprstx(double *, int *, double *, double *, int);
extern double *dvector(long, long);
extern void free_dvector(double *, long, long);

#ifdef __cplusplus
}
#endif

#endif // _LINBCG_H
