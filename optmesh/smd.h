//
// smd.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _SMD_H
#define _SMD_H

#ifdef __cplusplus
extern "C" {
#endif

typedef	int Id;

//
//for define BOOL
//
#ifndef _WINDOWS
typedef unsigned short BOOL;
#define TRUE  1
#define FALSE 0
#endif

//
// define BUFSIZ
//
#include <stdio.h>

// fundermental items list

typedef struct _vec {
  double	x;
  double	y;
  double	z;
} Vec;

typedef struct _vec2d {
  double	x;
  double	y;
} Vec2d;

typedef struct _mat3 {		/* 3-by-3 matrix */
  double   elm[3][3];
} Matrix3;

typedef struct _mat4 {		/* 4-by-4 matrix */
  double   elm[4][4];
} Matrix4;

typedef double **Vmtx;

/* matrix structure for linbcg */
typedef struct _semat {
  int    rnum;			/* matrix row */
  int    cnum;			/* matrix column */
  int    num;
  int    *ija;
  double *sa;
  /* constant vectors */
  double *bx, *by, *bz;
  /* solution vectors */
  double *xx, *yy, *zz;
} Semat;

/* "Material" structure */

typedef struct _material {
  float ambient[4];
  float diffuse[4];
  float emission[4];
  float specular[4];
  float shininess[1];
} Material;

/* "Light" structure */

#if 0
typedef struct _light {
  int       enable;
  float   ambient[4];
  float   diffuse[4];
  float   specular[4];
  float   position[4];
  float   lmodel_ambient[4];
  float   local_view[1];
} Light;
#endif

/*************** PPD file format *********************
  node type parameters for memory allocation routines
*******************************************************/

#define PPD_PPD         0
#define PPD_SOLID	1
#define PPD_PART	2
#define PPD_FACE	3
#define PPD_HEDGE       4
#define PPD_NORMAL	5
#define PPD_VERTEX	6
#define PPD_EDGE	7
#define PPD_LOOP	8
#define PPD_POINT	9
#define PPD_UVW		10
#define PPD_TRANS	11
#define PPD_ROT		12
#define PPD_OPT_WEIGHT  13
#define PPD_MATERIAL	14
#define PPD_FNODE	15
#define PPD_NONE	16
#define PPD_HEADER	17

#define TRIANGLE        3

// for ppd structure
typedef struct _sppd Sppd;
typedef struct _spso Spso;
typedef struct _sppt Sppt;
typedef struct _sphe Sphe;
typedef struct _spfc Spfc;
typedef struct _spnm Spnm;
typedef struct _spvt Spvt;
typedef struct _sped Sped;
// for use only in open_ppd 
typedef struct _vted Vted;

// star structure
typedef struct _star    Star;
typedef struct _starvt  Starvt;
typedef struct _stared  Stared;
typedef struct _starfc  Starfc;

// lodpqheap structure
typedef struct _lodpqheap LODPQHeap;
typedef struct _lodpqcont LODPQCont;

// file type
#define FILE_NONE         0
#define FILE_VRML         1
#define FILE_EVRML_POLY   2
#define FILE_EVRML_SUBDIV 3

//
// ppd structure
//
struct _sppd {
  
  int sn; int sid; Spso *spso, *epso; // Solid
  int pn; int pid; Sppt *sppt, *eppt; // Part
  int fn; int fid; Spfc *spfc, *epfc; // Face
  int nn; int nid; Spnm *spnm, *epnm; // Normal
  int vn; int vid; Spvt *spvt, *epvt; // Vertex
  int en; int eid; Sped *sped, *eped; // Edge

  // original vertex num;
  int nvt_org;
  
  // Material
  Material matl;

  // Misc (transformation information)
  double scale;
  Vec    center;

  // for simpmesh
  LODPQHeap *lodpqh;

  // for subdivision
  int sub_level;
  Sppd* parent;
  Sppd* child;

  // for conv dis
  double dis; // value from 0 to 1
  
  //for smooth shading
  double smooth_angle;

  // file type
  int file_type;

  // length
  double length;

  // L2-norm error
  double err;
  
  char   filename[BUFSIZ];
};

// ppd solid structure
struct _spso {
  Id no;
  Spso *nxt;
  Spso *prv;
};

// ppd part structure
struct _sppt {
  Id   no;  
  Spso *bpso;
  Sppt *nxt;
  Sppt *prv;
};

// ppd face structure
struct _spfc {
  
  Id    col;
  Id    no;
  Id    sid;
  Vec   nrm;                   // normal vector
  int   hen;                   // number of halfedge
  Sphe  *sphe;
  Spso  *bpso;                 // back pointer to ppdsolid
  Sppt  *bppt;                 // back pointer to ppdpart
  Spfc  *nxt, *prv;
  
  // for star
  Starfc *starfc;
  
};

// ppd halfedge structure
struct _sphe {
  
  Id    no;
  Spvt  *vt;
  Spnm  *nm;
  Sped  *ed;
  Sphe  *mate;
  Spfc  *bpfc;
  
  Sphe  *nxt, *prv;
  
};

// ppd normal structure
struct _spnm {
  
  Id   sid;
  Id   no;

  // for ppdnorm
  Id   vn;
  
  Vec  vec;
  Spso *bpso;
  
  Spnm *nxt, *prv;
};

// vertex -> edge
struct _vted {
  
  Sped  *ed;
  Spvt  *vt;
  Vted *nxt, *prv;
  
};

#define EMPTY   0
#define EXIST   1
#define DELETED 2

// constants for creating hppdlod
// vertex attributes

#define BOUNDARY  1
#define INTERNAL  2

#define VTXINTERNAL     INTERNAL
#define VTXEXTERNAL     BOUNDARY

// edge attributes
#define EDGEBOUNDARY    BOUNDARY
#define EDGEINTERNAL    INTERNAL

#define SUBDIV_ODD  0
#define SUBDIV_EVEN 1
  
//
// ppd vertex structure
//
struct _spvt {
  
  // original
  Id    sid;                      // vertex Identifier for save 
  Id    no;                       // vertex exist or not flag
  Id    col;                      // vertex color
  Id    type;                     // vertex type
  Sphe  *sphe;                    // start neighboring half-edge
  Spso  *bpso;                    // back pointer to ppdsolid
  Spvt  *nxt, *prv;
  Vec   vec;                      // position
  Vec2d uvw;                      // texture coordinates

  // boundary information
  BOOL isBoundary;
  
  // for use only in open_ppd
  int ven; Vted *svted, *evted;   // vertex -> edge

  // for use only SLP optimize
  int n_degree;
  double *lambda;
  
  // QEM matrix
  // mat[0] - mat[5]: ケヤホ の要素
  // mat[6] - mat[8]: ベクトル B の要素
  // mat[9]         : 定数 C の要素
  double mat[10];
  
  // for making QEM Infinity Matrix
  //BOOL checked;
  // for star
  Starvt *starvt;

  // subdiv boundary
  int isSubdivType;
  BOOL isSubdivBoundary;
  
  // for evaluation
  //int en, fn;
  
};

//
// ppd edge structure
//
struct _sped {
  
  // original
  Id    sid;
  Id    no;                      // edge exist or not flag
  Id    col;                     // edge color
  Id    type;                    // edge type
  Spvt  *sv, *ev;                // start end Vertex
  Sphe  *lhe, *rhe;              // left and right halfedges
  Sped  *nxt, *prv;
  
  // boundary information
  BOOL isBoundary;

  // edge length
  double length;
  
  // Edge-based QEM matrix
  // mat[0] - mat[5]: 行列A の要素
  // mat[6] - mat[8]: ベクトル B の要素
  // mat[9]         : 定数 C の要素
  double mat[10];
  
  // for star
  Stared *stared;

  // for lodprique
  LODPQCont *pqc;
  int   pq_type;

  // for lod
  Vec    ovec;
  double error;

  // subdiv boundary
  BOOL isSubdivBoundary;
};

//
// open ppd structure
//
typedef struct _oppd Oppd;
typedef struct _oso  Oso;
typedef struct _opt  Opt;
typedef struct _ovt  Ovt;
typedef struct _onm  Onm;
typedef struct _ofc  Ofc;

struct _oppd {
  Oso *oso;
  Opt *opt;
  Onm *onm;
  Ovt *ovt;
  Ofc *ofc;
};

struct _oso {
  Spso *so;
};

struct _opt {
  Sppt *pt;
};

struct _onm {
  Spnm *nm;
  Spso *so;
};

struct _ovt {
  Spvt *vt;
  Spso *so;
};

struct _ofc {
  Spfc *fc;
  Spso *so;
  Sppt *pt;
};

//
// star structure
//

struct _starvt {

  Id   no;
  Spvt *vt;
  Vec  vec;
  
  Starvt *nxt, *prv;
  
};

struct _stared {

  Id   no;
  Sped *ed;

  Starvt *sv, *ev;
  Starfc *rf, *lf;
  
  Stared *nxt, *prv;

};

struct _starfc {

  Id   no;
  Vec    nrm;
  Starvt *vt[TRIANGLE];
  Stared *ed[TRIANGLE];
  Spfc   *fc;
  Starfc *nxt, *prv;

};

struct _star {

  int vn; Starvt *spvt, *epvt;
  int en; Stared *sped, *eped;
  int fn; Starfc *spfc, *epfc;

  Stared *stare;
  Starvt *starsv, *starev;

  BOOL isBoundary;
  
  Vec    svec, evec;
};

// priority queue for hppdlod
struct _lodpqcont {
  
  double length;
  Id   id;
  Sped *ed;
  
};
  
struct _lodpqheap {
  
  LODPQCont *pqcont;
  int    size;
  int    last;
  
};

//
// for save VRML
//
#define SAVE_VRML_SUBDIV   0
#define SAVE_VRML_POLY     1
  
//
// general defines and macros
//

#define SMD_ON          1
#define SMD_OFF         0
#define SMDNULL         -1
#define SMDZERO         0

#define PNTBLUE         2
#define PNTGREEN        1
#define PNTRED          0

#define EDGEWHITE       2
#define EDGEBLUE        1
#define EDGERED         0

#define FACEBLUE        2
#define FACEGREEN       1
#define FACERED         0

#define LOOPBLUE        1
#define LOOPRED         0

#define NOTCALCURATED   0
#define CALCURATED      1
#define SUCCEED         1
#define FAIL            0
#define INSIDE          0
#define OUTSIDE         1
#define ONLINE          2
#define INTERSECTION    3

// misc
#define SMDEPS          1.0e-16
#define SMDZEROEPS      1.0e-06
#define SMDZEROEPS2     1.0e-12
#define SMDPI		3.14159265358979
#define PITIMES2	6.283185	/* 2 * pi */
#define PIOVER2		1.570796	/* pi / 2 */
#define NE		2.718282	/* the venerable e */
#define SQRT2		1.414214	/* sqrt(2) */
#define SQRT3		1.732051	/* sqrt(3) */
#define GOLDEN		1.618034	/* the golden ratio */
#define DTOR		0.017453	/* convert degrees to radians */
#define RTOD		57.29578	/* convert radians to degrees */

#define MULTI           10000.0

//#ifndef WIN32
extern void display(char *,...);
//#endif

#ifdef __cplusplus
}
#endif

#endif // _SMD_H
