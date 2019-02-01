//
// octree.h
// Octree data structures
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _OCTREE_H
#define _OCTREE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _otree OTree;
typedef struct _orange ORange;
typedef struct _opoint OPoint;
typedef struct _oface OFace;

struct _orange {
  double xmin;
  double xmax;
  double ymin;
  double ymax;
  double zmin;
  double zmax;
};

struct _otree {

  int   level;
  Id    id;
  int   opn;
  OPoint *sopnt;
  OPoint *eopnt;
  int   ofn;
  OFace *soface;
  OFace *eoface;
  ORange or_;
  OTree *child[8];
  OTree *parent;
  OTree *top;

  // used top only
  int on;
  int maxotlevel;
};

struct _opoint {

  // vertex
  Spvt *vt;

  // parent octree
  OTree *ot;

  OPoint *prv, *nxt;
  
};

struct _oface {

  // face
  Spfc *fc;

  // parent octree
  OTree *ot;

  OFace *prv, *nxt;
};

// functions
extern OTree *create_otreechild(OTree *, int, int, double, double, double, double, double, double);
extern OTree *create_otreechild_or(OTree *, int, int, ORange *);
extern OPoint *insert_otree_point(Spvt *, OTree *);
extern OFace *insert_otree_face(Spfc *, OTree *);
extern void free_ppdotree(OTree *);
extern void free_ppdotree_child(OTree *);
extern void free_ppdopoint(OTree *);
extern void delete_otree_point(OPoint *, OTree *);
extern void free_ppdoface(OTree *);
extern void delete_otree_face(OFace *, OTree *);
extern int decide_orange_child_number(Vec *, ORange *, ORange *);
extern void subdivide_orange(ORange *, int, ORange *);
extern OTree *init_otree(double, double, double, double, double, double);
extern void ppdvertex_to_ppdotree(Sppd *, OTree *);
extern void ppdface_to_ppdotree(Sppd *, OTree *);
extern void decide_oface_child(OTree *, Spfc *);
extern void calc_point_to_face_distance(OTree *, Sppd *);
extern void point_to_face_distance_child(OTree *, Sppd *);
extern double point_face_distance(Spvt *, Spfc *);
extern BOOL zup(double);
extern BOOL zdown(double);
extern BOOL zero(double);
extern BOOL up(double);
extern BOOL down(double);

#ifdef __cplusplus
}
#endif

#endif // _OCTREE_H
