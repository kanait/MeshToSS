//
// simpmesh.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _SIMPMESH_H
#define _SIMPMESH_H

#ifdef __cplusplus
extern "C" {
#endif

// functions  
extern void simpmesh(Sppd *, int );
extern void simpmesh_l2norm(Sppd *, double );
extern void initialize_simpmesh(Sppd *);
extern void ppd_calc_vqem(Sppd *);
extern void ppd_calc_eqem(Sppd *);
extern LODPQHeap *ppdedge_to_lodpqheap(Sppd *);
extern void optlod(Sppd *, int);
extern void optlod_l2norm(Sppd *, double);
extern BOOL isLegalMove(Sped *, BOOL);
extern void eval_vqem(Sped *, double *);
extern void eval_eqem(Sped *);
extern Sped *try_edge_operation(Sped *, Sppd *);
extern BOOL collapse_condition(Star *);
extern void DeleteLODPQCont(Sped *, Sppd *, LODPQHeap *);
extern Sped *next_ed(Sppd *);
extern Sped *edge_collapse(Sped *, Sppd *);
extern Spvt *get_lm_ppdvertex(Spfc *, Spvt *, Spvt *);
extern Sped *get_lm_ppdedge(Spfc *, Spvt *, Spvt *);
extern Sphe *get_lm_ppdhalfedge_rev(Spfc *, Sped *);
extern void change_ppdvertexlinks(Spvt *, Spvt *);
extern void adjust_lodpqheap(Spvt *, LODPQHeap *);
extern void qem2submatrix_test(Sppd *);

// QEM mode
#define MODE_VQEM 0
#define MODE_EQEM 1

#ifdef __cplusplus
}
#endif

#endif // _SIMPMESH_H
