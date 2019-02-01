//
// qem.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _QEM_H
#define _QEM_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern void optimize_vector(Sped *, double *);
extern BOOL InvertSymMatrix3(double *, double *);
extern double calc_error(double *, Vec *);
extern void optimize_2subvector(Sped *, double *);
extern void make_qeminfmatrix(Sped *, double *, double *);
extern void optimize_infvector(Sped *, double *);
extern double calc_inferror(double *, Vec *);
extern void make_vqem2submatrix(Sped *, double *, double, double *);
extern void make_eqem2submatrix(Sped *, double, double *);
extern void loop_odd_vertex(Spvt *, Spvt *, Spvt *, Spvt *, int, double *, Vec *);
extern void loop_even_vertex(Sped *, int *, double *, double *, Vec *, BOOL);
extern void loop_even_vertex_vj(Spvt *, Sped *, int *, double *, double *, Vec *);
extern Sped *ppdedge_first_ring_edge(Sped *);
extern Sped *ppdedge_next_ring_edge(Sped *, Sped *);
extern Sped *ppdedge_prev_ring_edge(Sped *, Sped *);
extern Spvt *ppdedge_ring_another_vt(Sped *, Sped *);
extern Spvt *ppdedge_first_ring_vertex(Sped *, Sped **);
extern Spvt *ppdedge_last_ring_vertex( Sped *, Sped ** );
extern Spvt *ppdedge_next_ring_vertex(Spvt *, Sped *, Sped *, Sped **);
extern Spvt *ppdedge_prev_ring_vertex(Spvt *, Sped *, Sped *, Sped **);
extern Spvt *ppdedge_opposite_vertex(Sped *, Spvt *, Spvt *);
extern void qem_matrix4(double, Vec *, double *, double *);
extern void edge_ring_test(Sppd *);
extern void edge_ring_vertex_test(Sppd *);

// parameters for QEM evaluation 
#define EVAL_VERTEX 0
#define EVAL_SLP    1
#define EVAL_2SUB   2

#ifdef __cplusplus
}
#endif

#endif // _QEM_H
