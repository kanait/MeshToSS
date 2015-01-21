//
// ppdnormal.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _PPDPART_H
#define _PPDPART_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Sppt *create_ppdpart(Sppd *);
extern void free_ppdpart(Sppt *, Sppd *);

#ifdef __cplusplus
}
#endif

#endif // _PPDPART_H
