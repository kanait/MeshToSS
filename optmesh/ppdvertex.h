//
// ppdvertex.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _PPDVERTEX_H
#define _PPDVERTEX_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Spvt *create_ppdvertex(Sppd *);
extern Spvt *list_ppdvertex(Sppd *, Id);
extern void free_ppdvertex(Spvt *, Sppd *);
extern Vted *create_vtxed(Spvt *, Sped *);
extern Spvt *another_vt(Sped *, Spvt *);
extern Vted *find_vtxed(Spvt *, Sped *);
extern double calc_angle(Spvt *, Spvt *, Spvt *);
extern Vted *next_vted(Vted *);
extern Vted *previous_vted(Vted *);
extern void FreePPDVertexEdge(Sppd *);
extern void free_ppdvertexedge(Spvt *);
extern void free_vertexedge(Spvt *, Sped *);
extern void free_vtxed(Vted *, Spvt *);
extern void free_ppdvertex_noedge(Sppd *);
extern Sphe *ppdvertex_ccwnext_halfedge(Sphe *);
extern Sphe *ppdvertex_cwnext_halfedge(Sphe *);
extern int ppdvertex_num_face(Spvt *);
extern int ppdvertex_num_edge(Spvt *);
extern void reattach_ppdvertex_sphe(Spvt *);
extern void reattach_ppdvertex_halfedge(Sppd *);
extern Sphe *decide_ppdvertex_sphe(Spvt *, Spfc *);
extern Sped *ppdvertex_first_edge(Spvt *);
extern Sped *ppdvertex_next_edge(Sped *, Spvt *);
extern Sped *ppdvertex_prev_edge(Sped *, Spvt *);
extern Spvt *ppdvertex_another_boundaryvertex(Spvt *, Spvt *);
extern void print_ppdvertex_neighbor(Spvt *);

#ifdef __cplusplus
}
#endif

#endif // _PPDVERTEX_H
