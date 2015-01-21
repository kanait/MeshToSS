//
// ppdedge.h
//
// Copyright (c) 1997-1999 Takashi Kanai; All rights reserved. 
//

#ifndef _PPDEDGE_H
#define _PPDEDGE_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Sped *create_ppdedge(Sppd *);
extern void free_ppdedge(Sped *, Sppd *);
extern void free_ppdedge_withqtree(Sped *, Sppd *);
extern void FreeSped(Sped *, Sppd *);
extern int num_ppdedge( Sppd * );
//extern Edvt *create_edvtx(Sped *, Spvt *);
extern void insert_edvtx(Sped *, Spvt *, Sped *, double);
extern void free_ppdedgevertex(Sped *);
//extern void free_edvtx(Edvt *, Sped *);
extern void edge_length(Sppd *);
extern Sped *find_ppdedge(Spvt *, Spvt *);
extern Sped *find_ppdedge_new(Spvt *, Spvt *);
extern double normalize_ppdedge(Sppd * );
extern void ppdedge_to_vec( Sped *, Vec * );
extern Spfc *ppdedge_another_fc( Sped *, Spfc * );

#ifdef __cplusplus
}
#endif

#endif // _PPDEDGE_H
