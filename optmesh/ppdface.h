//
// ppdface.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _PPDFACE_H
#define _PPDFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Spfc *create_ppdface(Sppd *);
extern Spvt *other_vertices_face_left(Spfc *, Spvt *);
extern Spvt *other_vertices_face_right(Spfc *, Spvt *);
extern void free_ppdface(Spfc *, Sppd *);
extern int num_ppdface(Sppd *);
extern Sphe *ppdface_opposite_halfedge(Spfc *, Spvt *);
extern void calc_fnorm(Spfc *);
extern double calc_farea(Spfc *);
extern Sphe *create_ppdhalfedge(Spfc *);
extern Sphe *free_ppdhalfedge(Sphe *, Spfc *);
extern Spfc *find_ppdface_ppdedge(Sped *, Spvt *, Spvt *);
extern Sphe *find_ppdhalfedge(Sppd *, Spvt *);
extern void ppdface_to_plane(Spfc *, double *, double *, double *, double *);
extern void ppdhalfedge_to_vec(Sphe *, Vec *);
extern void ppdface_find_barycentric_coordinate(Spfc *, double, double, double *, double *, double *);
extern void ppdface_barycentric_coordinate(Spfc *, double, double, double, Vec *);
extern Spvt *ppdface_another_vertex(Spfc *, Spvt *, Spvt *);

#ifdef __cplusplus
}
#endif

#endif // _PPDFACE_H
