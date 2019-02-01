//
// ppdface.cxx
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <stdlib.h>
#include "smd.h"
#include "veclib.h"

#include "ppdface.h"

//
// PPD Face functions
//

// create face
Spfc *create_ppdface( Sppd *ppd )
{
  Spfc *fc;

  fc = (Spfc *) malloc(sizeof(Spfc));
  
  fc->nxt = (Spfc *) NULL;

  if (ppd->spfc == (Spfc *) NULL) {
    fc->prv = (Spfc *) NULL;
    ppd->epfc = ppd->spfc = fc;
  } else {
    fc->prv   = ppd->epfc;
    ppd->epfc = ppd->epfc->nxt = fc;
  }

  // half edge
  fc->hen   = 0;
  fc->sphe  = (Sphe *) NULL;
  fc->bppt  = (Sppt *) NULL;
  fc->bpso  = (Spso *) NULL;
  // id
  fc->no    = ppd->fid;
  fc->sid   = SMDNULL;
  // face color
  fc->col   = FACEBLUE;

  // star
  fc->starfc = NULL;
  
  ++( ppd->fn );
  ++( ppd->fid );

  return fc;
}

// next ccw vertex in a face
Spvt *other_vertices_face_left( Spfc *fc, Spvt *vt )
{
  Sphe *he;
  
  he = fc->sphe;

  do {
    if ( he->vt == vt ) {
      return he->nxt->vt;
    }
  } while ( (he = he->nxt) != fc->sphe );

  return NULL;
}

// next cw vertex in a face
Spvt *other_vertices_face_right( Spfc *fc, Spvt *vt )
{
  Sphe *he;
  
  he = fc->sphe;

  do {
    if ( he->vt == vt ) {
      return he->prv->vt;
    }
  } while ( (he = he->nxt) != fc->sphe );
  
  return NULL;
}

// free ppdface
void free_ppdface( Spfc *fc, Sppd *ppd )
{
  Sphe   *he;
  
  if (fc == (Spfc *) NULL) return;
  
  // free halfedge
  he = fc->sphe;
  while (he != (Sphe *) NULL) {
    he = free_ppdhalfedge( he, fc );
  }
  
  if ( ppd->spfc == fc ) {
    if ((ppd->spfc = fc->nxt) != (Spfc *) NULL)
      fc->nxt->prv = (Spfc *) NULL;
    else {
      ppd->epfc = (Spfc *) NULL;
    }
  } else if (ppd->epfc == fc) {
    fc->prv->nxt = (Spfc *) NULL;
    ppd->epfc = fc->prv;
    
  } else {
    fc->prv->nxt = fc->nxt;
    fc->nxt->prv = fc->prv;
  }

  free(fc);
  --( ppd->fn );
}

// number
int num_ppdface( Sppd *ppd )
{
  int i = 0;
  Spfc *fc;
  for ( fc = ppd->spfc; fc != NULL; fc = fc->nxt ) {
    ++i;
  }
  
  return i;
}

// an halfedge opposite to a vertex in a face
// this function is valid for only triangle faces
Sphe *ppdface_opposite_halfedge( Spfc *fc, Spvt *vt )
{
  Sphe *he;
  Sped *ed;
  
  he = fc->sphe;
  do {
    ed = he->ed;
    if ( (ed->sv != vt) && (ed->ev != vt) ) return he;
  } while ( (he = he->nxt) != fc->sphe );
  
  return NULL;
}

// compute face normal
void calc_fnorm( Spfc *face )
{
  Sphe   *h;
  Spvt   *v1, *v2, *v3;
  Vec    vec1, vec2, vec3, vec4, vec5;

  h = face->sphe;
  v1 = h->vt;
  v2 = h->nxt->vt;
  v3 = h->nxt->nxt->vt;
  vec1.x = v1->vec.x;
  vec1.y = v1->vec.y;
  vec1.z = v1->vec.z;
  vec2.x = v2->vec.x;
  vec2.y = v2->vec.y;
  vec2.z = v2->vec.z;
  vec3.x = v3->vec.x;
  vec3.y = v3->vec.y;
  vec3.z = v3->vec.z;
  V3Sub( &vec2, &vec1, &vec4 );
  V3Sub( &vec3, &vec1, &vec5 );
  V3Cross( &vec4, &vec5, &(face->nrm) );
  V3Normalize( &(face->nrm) );
}

// compute face area
double calc_farea( Spfc *face )
{
  Sphe   *h;
  Spvt   *v1, *v2, *v3;
  Vec    vec1, vec2, vec3, vec4, vec5, nrm;

  h = face->sphe;
  v1 = h->vt;
  v2 = h->nxt->vt;
  v3 = h->nxt->nxt->vt;
  vec1.x = v1->vec.x;
  vec1.y = v1->vec.y;
  vec1.z = v1->vec.z;
  vec2.x = v2->vec.x;
  vec2.y = v2->vec.y;
  vec2.z = v2->vec.z;
  vec3.x = v3->vec.x;
  vec3.y = v3->vec.y;
  vec3.z = v3->vec.z;
  V3Sub( &vec2, &vec1, &vec4 );
  V3Sub( &vec3, &vec1, &vec5 );
  V3Cross( &vec4, &vec5, &(nrm) );

  return 0.5 * V3Length( &(nrm) );
}

//
// PPD HalfEdge functions
//

// create halfedge
Sphe *create_ppdhalfedge( Spfc *fc )
{
  Sphe *he;

  he = (Sphe *) malloc( sizeof(Sphe) );
  
  if (fc->sphe == (Sphe *) NULL) {	/* first */
    fc->sphe = he;
    he->prv  = he;
    he->nxt  = he;
  } else {
    he->prv  = fc->sphe->prv;
    he->nxt  = fc->sphe;
    fc->sphe->prv->nxt = he;
    fc->sphe->prv = he;
  }
  // id
  he->no   = fc->hen;
  // back face
  he->bpfc = fc;
  // edge
  he->ed   = (Sped *) NULL;
  // start vertex
  he->vt  = (Spvt *) NULL;
  // start normal
  he->nm  = (Spnm *) NULL;
  // mate
  he->mate  = (Sphe *) NULL;

  ++( fc->hen );

  return he;
}

//
// free halfedge
//
Sphe *free_ppdhalfedge( Sphe *he, Spfc *face )
{
  if ( he->nxt == he ) {
    free( he );
    --( face->hen );
    return NULL;
  } else {
    Sphe *newhe = he->nxt;
    he->prv->nxt = he->nxt;
    he->nxt->prv = he->prv;
    free( he );
    --( face->hen );
    return newhe;
  }
}

// not used
Spfc *find_ppdface_ppdedge( Sped *ed, Spvt *sv, Spvt *ev )
{
  if ( ed == NULL ) return NULL;

  if ( (ed->sv == sv) && (ed->ev == ev ) ) {
    
    if ( ed->lhe != NULL ) {
      return ed->lhe->bpfc;
    } else return NULL;
    
  } else if ( (ed->sv == ev) && (ed->ev == sv ) ) {

    if ( ed->rhe != NULL ) {
      return ed->rhe->bpfc;
    } else return NULL;
    
  }
  
  return NULL;
}

Sphe *find_ppdhalfedge( Sppd *ppd, Spvt *vt )
{
  for ( Spfc *fc = ppd->spfc; fc != NULL; fc = fc->nxt ) {
    Sphe *he = fc->sphe;
    do {
      if ( he->vt == vt ) return he;
    } while ( (he = he->nxt) != fc->sphe );
  }
  return NULL;
}

// conversion from a face to a plane
void ppdface_to_plane( Spfc *fc,
		       double *a, double *b, double *c, double *d )
{
  Vec *v1, *v2, *v3;
  Vec p1, p2, pc;

  v1 = &(fc->sphe->vt->vec);
  v2 = &(fc->sphe->nxt->vt->vec);
  v3 = &(fc->sphe->nxt->nxt->vt->vec);
  V3Sub( v2, v1, &p1 );
  V3Sub( v3, v1, &p2 );
  V3Cross( &p1, &p2, &pc );
  *a = pc.x; *b = pc.y; *c = pc.z;
  *d = - ( pc.x * v1->x + pc.y * v1->y + pc.z * v1->z );
}

// halfedge to vector
void ppdhalfedge_to_vec( Sphe *he, Vec *vec )
{
  V3Sub( &(he->nxt->vt->vec), &(he->vt->vec), vec );
  //V3Sub( &(he->vt->vec), &(he->nxt->vt->vec), vec );
  //V3Normalize( vec );
}

// find barycentric coordinates
void ppdface_find_barycentric_coordinate( Spfc *fc, double xparam, double yparam,
					  double *a1, double *a2, double *a3 )
{
  Vec2d vec;
  vec.x = xparam; vec.y = yparam;
  Vec2d *vec1 = &(fc->sphe->vt->uvw);
  Vec2d *vec2 = &(fc->sphe->nxt->vt->uvw);
  Vec2d *vec3 = &(fc->sphe->nxt->nxt->vt->uvw);

  double area = V2TriArea( vec1, vec2, vec3 );
  *a1 = V2TriArea( &vec, vec2, vec3 ) / area;
  *a2 = V2TriArea( &vec, vec3, vec1 ) / area;
  *a3 = V2TriArea( &vec, vec1, vec2 ) / area;
}

// barycentric coordinates
void ppdface_barycentric_coordinate( Spfc *fc, double a1, double a2, double a3,
				     Vec *vec )
{
  Vec *vec1 = &(fc->sphe->vt->vec);
  Vec *vec2 = &(fc->sphe->nxt->vt->vec);
  Vec *vec3 = &(fc->sphe->nxt->nxt->vt->vec);
  vec->x = a1 * vec1->x + a2 * vec2->x + a3 * vec3->x;
  vec->y = a1 * vec1->y + a2 * vec2->y + a3 * vec3->y;
  vec->z = a1 * vec1->z + a2 * vec2->z + a3 * vec3->z;
}

//
// fc 、ホテ讀ヌ。、v1, v2 ーハウー、ホトコナタ
//
Spvt *ppdface_another_vertex( Spfc *fc, Spvt *v1, Spvt *v2 )
{
  if ( fc == NULL ) return NULL;
  
  Sphe *he = fc->sphe;
  do {
    if ( (he->vt != v1) && (he->vt != v2) ) return he->vt;
  } while ( (he = he->nxt) != fc->sphe );
  
  return NULL;
}
