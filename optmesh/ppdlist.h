//
// ppdlist.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _PPDLIST_H
#define _PPDLIST_H

#ifdef __cplusplus
extern "C" {
#endif

extern void write_ppd_file(char *, Sppd *);
extern Sppd *open_ppd(char *);

#ifdef __cplusplus
}
#endif

#endif // _PPDLIST_H
