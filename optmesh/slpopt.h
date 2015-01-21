//
// slpopt.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _SLPOPT_H
#define _SLPOPT_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern void ppdslpopt(Sppd *);
extern void ppdslpopt_initialize(Sppd *);
extern void ppdslpopt_finish(Sppd *);
extern Semat *create_slpopt_emat(Sppd *);
extern void create_slpopt_vector(Sppd *, Semat *);
extern void slpopt_lambda(Sppd *);
extern void solvec_ppdvertex(Sppd *, Semat *);
extern Semat *init_emat(int);
extern void free_emat(Semat *);
extern void printemat(Semat *);

#ifdef __cplusplus
}
#endif

#endif // _SLPOPT_H
