//
// ppdnormal.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _PPDNORMAL_H
#define _PPDNORMAL_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Spnm *create_ppdnormal(Sppd *);
extern void free_ppdnormal(Spnm *, Sppd *);
extern void ppdnorm(Sppd *, double );
extern void freeppdnorm( Sppd * );

#ifdef __cplusplus
}
#endif

#endif // _PPDNORMAL_H
