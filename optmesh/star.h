//
// star.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _STAR_H
#define _STAR_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Star *create_star(Sped *);
extern void CreateStarVertex(Star *, Spvt *);
extern void CreateStarFace(Star *, Spvt *);
extern void CreateStarEdge(Star *);
extern void delete_starlink(Star *);
extern Star *init_star(void);
extern Starvt *create_starvertex(Star *);
extern Starvt *find_starvertex(Star *, Spvt *);
extern void free_starvertex(Starvt *, Star *);
extern Stared *create_staredge(Star *);
extern Stared *find_staredge(Star *, Sped *);
extern void free_staredge(Stared *, Star *);
extern Starfc *create_starface(Star *);
extern void free_starface(Starfc *, Star *);
extern void free_star(Star *);
extern void staredge_collapse(Star *);
extern Starvt *get_lm_starvertex(Starfc *, Starvt *, Starvt *);
extern Stared *get_lm_staredge(Starfc *, Starvt *, Starvt *);
extern Starfc *get_lm_starface_rev(Starfc *, Stared *);
extern void change_staredgefacelink(Stared *, Starfc *, Starfc *);
extern void change_starfaceedgelink(Starfc *, Stared *, Stared *);
extern void change_staredgevertexlink(Star *, Starvt *, Starvt *);
extern void change_starfacevertexlink(Star *, Starvt *, Starvt *);
extern void starnorm(Star *);
extern void calc_starfacenorm(Starfc *);
extern BOOL geom_check( Star *, double, double );
extern double calc_starfcompactness( Starfc * );
extern double calc_starfacearea( Starfc * );
extern Starfc *find_starface( Star *, Starvt *, Starvt *, Starvt * );
extern void write_star_ppd_file( char *, Star * );
extern void print_star( Star * );

#ifdef __cplusplus
}
#endif

#endif // _STAR_H
