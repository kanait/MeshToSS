//
// ppdsolid.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _PPDSOLID_H
#define _PPDSOLID_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Spso *create_ppdsolid(Sppd *);
extern void free_ppdsolid(Spso *, Sppd *);

#ifdef __cplusplus
}
#endif

#endif // _PPDSOLID_H
