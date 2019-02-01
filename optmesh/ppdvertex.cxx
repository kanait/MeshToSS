//
// ppdvertex.cxx
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
#include <math.h>

#include "smd.h"
#include "veclib.h"

#include "ppdvertex.h"

/***************************************
  Vertex
****************************************/

// create vertex
Spvt *create_ppdvertex( Sppd *ppd )
{
  Spvt *vt;

  vt = (Spvt *) malloc(sizeof(Spvt));

  vt->nxt = (Spvt *) NULL;
  if (ppd->spvt == (Spvt *) NULL) {
    vt->prv   = (Spvt *) NULL;
    ppd->epvt = ppd->spvt = vt;
  } else {
    vt->prv   = ppd->epvt;
    ppd->epvt = ppd->epvt->nxt = vt;
  }

  // 2D vector initialize (for mapping)
  vt->uvw.x = 0.0;
  vt->uvw.y = 0.0;
  
  // id 
  vt->no   = ppd->vid;

  // vertex color
  vt->col  = PNTGREEN;

  // save id
  vt->sid  = SMDNULL;

  // type (new)
  //vt->type = VERTEX_ORIGINAL; 

  // back solid
  vt->bpso = (Spso *) NULL;

  // vt - halfedge
  vt->sphe = (Sphe *) NULL;
	
  // vt - edge
  vt->ven  = 0;
  vt->svted = (Vted *) NULL;
  vt->evted = (Vted *) NULL;
  
  // vt - halfedge
  vt->sphe = (Sphe *) NULL;
  
  // for creating hppdlod
  //vt->atr   = VTXINTERNAL;
  vt->isBoundary = FALSE;

  // for use only SLP optimize
  vt->n_degree = 0;
  vt->lambda = NULL;
  
  // star
  vt->starvt = NULL;

  // subdiv boundary
  vt->isSubdivType = SUBDIV_EVEN;
  vt->isSubdivBoundary = FALSE;
  
  // for evaluation
//   vt->en = 0;
  //vt->fn = 0;

  // for making QEM Infinity Matrix
  //vt->checked = FALSE;
  
  ++( ppd->vn );
  ++( ppd->vid );

  return vt;
}

Spvt *list_ppdvertex( Sppd *ppd, Id id )
{
  Spvt *vt;
  
  for ( vt = ppd->spvt; vt != (Spvt *) NULL; vt = vt->nxt ) {
    if ( vt->no == id ) return vt;
  }
  return (Spvt *) NULL;
}

// number
// int num_ppdvertex( Sppd *ppd )
// {
//   int i = 0;
//   Spvt *vt;
//   for ( vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
//     if ( vt->type == VERTEX_ORIGINAL ) ++i;
//   }
//   return i;
// }
    
void free_ppdvertex( Spvt *vt, Sppd *ppd )
{
  if (vt == (Spvt *) NULL) return;
  
  if (ppd->spvt == vt) {
    if ((ppd->spvt = vt->nxt) != (Spvt *) NULL)
      vt->nxt->prv = (Spvt *) NULL;
    else {
      ppd->epvt = (Spvt *) NULL;
    }
  } else if (ppd->epvt == vt) {
    vt->prv->nxt = (Spvt *) NULL;
    ppd->epvt = vt->prv;
  } else {
    vt->prv->nxt = vt->nxt;
    vt->nxt->prv = vt->prv;
  }

  //free_ppdvertexedge( vt );
  
  // for spm
//   if ( vt->lambda != NULL ) free( vt->lambda );
//   if ( vt->vec2d != NULL) free( vt->vec2d );
  
  //display("deleted vt %d\n", vt->no );
  free( vt );
  --( ppd->vn );
}

//
// neighboring edges of a vertex
//

Vted *create_vtxed(Spvt *vertex, Sped *ed)
{
  Vted *ve;

  if (ed == (Sped *) NULL) return (Vted *) NULL;
  
  ve = (Vted *) malloc(sizeof(Vted));

  ve->nxt = (Vted *) NULL;
  if (vertex->svted == (Vted *) NULL) {
    ve->prv       = (Vted *) NULL;
    vertex->evted = vertex->svted = ve;
  } else {
    ve->prv       = vertex->evted;
    vertex->evted = vertex->evted->nxt = ve;
  }

  // edge
  ve->ed = ed;
  // vertex
  ve->vt    = vertex;

  ++( vertex->ven );
  
  return ve;
}

Spvt *another_vt(Sped *ed, Spvt *vt)
{
  if (ed->sv == vt) return ed->ev;
  else return ed->sv;
}

/* Vted *another_vted(Sped *ed, Vted *ve) */
/* { */
/*   if ( (ed->ve1 == (Vted *) NULL) || (ed->ve2 == (Vted *) NULL) ) */
/*     display("okashii!!\n"); */
       
/*   if (ed->ve1 == ve) return ed->ve2; */
/*   else ed->ve1; */
/* } */

Vted *find_vtxed( Spvt *vt, Sped *ed )
{
  Vted *vted;
  
  for ( vted = vt->svted; vted != (Vted *) NULL; vted = vted->nxt ) {
    if ( vted->ed == ed ) return vted;
  }
  return (Vted *) NULL;
}

/* for ppdharmonic */
double calc_angle(Spvt *v1, Spvt *v2, Spvt *v3)
{
  double cs, cs_angle;
  int    i;
  Vec2d  sub1, sub2;
  
  V2Sub( &(v2->uvw), &(v1->uvw), &(sub1) );
  V2Sub( &(v3->uvw), &(v1->uvw), &(sub2) );
  
  cs = V2Cosign(&(sub1), &(sub2));

  if ( fabs(cs + 1.0) < SMDZEROEPS ) {
    return 180.0;
  }
  
  /* + 、ォ - 、ォ、ホネスト熙ホ、゜ */
  i = V2Signi(&(sub1), &(sub2));
/*   sn = V2Sign(&(sub1), &(sub2)); */

  cs_angle = RTOD * acos(cs);
/*   display("cs %g (cs_angle) %g sign %d\n", cs, cs_angle, i); */
  
  if (fabs(cs) < SMDZEROEPS) {
    if ( i == SMD_ON ) {    /* plus */
      return 90.0;
    } else { /* minus */
      return 270.0;
    }
  }
  
/*   if (fabs(sn) < SMDZEROEPS) { */
/*     if (cs > 0.0) */
/*       return 0.0; */
/*     if (cs < 0.0) */
/*       return 180.0; */
/*   } */
  
  if (!i) {
    cs_angle = ( 360.0 - cs_angle );
  }
  
  return cs_angle;
}

Vted *next_vted(Vted *ve)
{
  Vted *nve;
  
  nve = ve->nxt;
  if ( nve == NULL ) {
    nve = ve->vt->svted;
  }
  return nve;
}
	
Vted *previous_vted(Vted *ve)
{
  Vted *pve;
  
  pve = ve->prv;
  if ( pve == NULL) {
    pve = ve->vt->evted;
  }
  return pve;
}
	
void FreePPDVertexEdge( Sppd *ppd )
{
  for ( Spvt *vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    free_ppdvertexedge( vt );
  }
}
    
void free_ppdvertexedge(Spvt *vt)
{
  Vted *ve, *venxt;

  for (ve = vt->svted; ve != (Vted *) NULL; ve = venxt) {
    venxt = ve->nxt;
    free_vtxed( ve, vt );
  }
  vt->ven = 0;
  vt->svted = vt->evted = (Vted *) NULL;
}

void free_vertexedge( Spvt *vt, Sped *ed )
{
  Vted *vted;
  
  if ( ed == (Sped *) NULL ) return;
  
  for ( vted = vt->svted; vted != (Vted *) NULL; vted = vted->nxt ) {
    if ( vted->ed == ed ) {
      free_vtxed( vted, vt );
      return;
    }
  }

}
      
void free_vtxed( Vted *ve, Spvt *vt )
{
  if (ve == (Vted *) NULL) return;

  ve->ed = (Sped *) NULL;
  ve->vt = (Spvt *) NULL;
  
  if (vt->svted == ve) {
    if ((vt->svted = ve->nxt) != (Vted *) NULL)
      ve->nxt->prv = (Vted *) NULL;
    else {
      vt->evted = (Vted *) NULL;
    }
  } else if (vt->evted == ve) {
    ve->prv->nxt = (Vted *) NULL;
    vt->evted = ve->prv;
  } else {
    ve->prv->nxt = ve->nxt;
    ve->nxt->prv = ve->prv;
  }
  free(ve);
  --( vt->ven );
}

// for open_ppd
void free_ppdvertex_noedge( Sppd *ppd )
{
  Spvt *nvt;
  for ( Spvt *vt = ppd->spvt; vt != (Spvt *) NULL; vt = nvt ) {
    nvt = vt->nxt;
    if ( !(vt->ven) ) free_ppdvertex( vt, ppd );
  }
}

//
// counter-clockwise order neighboring halfedges of a vertex
// 
Sphe *ppdvertex_ccwnext_halfedge( Sphe *he )
{
  if ( he == NULL ) return NULL;
  
  Sphe *phe = he->prv;
  return phe->mate;
} 

//
// clockwise order neighboring halfedges of a vertex
// 
Sphe *ppdvertex_cwnext_halfedge( Sphe *he )
{
  if ( he == NULL ) return NULL;
  if ( he->mate != NULL ) {
    return he->mate->nxt;
  } else {
    return NULL;
  }
} 

// number of neighboring faces of a vertex
int ppdvertex_num_face( Spvt *vt )
{
  int count = 0;
  Sphe *he;
  
  if ( (he = vt->sphe) == NULL ) return count;
  do {
    ++count;
    he = ppdvertex_ccwnext_halfedge( he );
  } while (  (he != vt->sphe) && (he != NULL) );
  
  return count;
}

// number of neighboring edges (vertices) of a vertex
// degree
int ppdvertex_num_edge( Spvt *vt )
{
  int count = 0;
  Sped *ed, *fed;
  ed = fed = ppdvertex_first_edge( vt );
  do {
    ++count;
    ed = ppdvertex_next_edge( ed, vt );
  } while ( (ed != NULL) && (ed != fed) );
  return count;
}
  
// re-attaching ppdvertex's halfedge link
// so that it links a beginning halfedge in counter-clockwise order.
void reattach_ppdvertex_sphe( Spvt *vt )
{
  //  
  if ( vt->isBoundary == FALSE ) return;
  
  Sphe *he = vt->sphe;
  if ( he == NULL ) return;
  
  while ( he->mate != NULL ) {
    he = ppdvertex_cwnext_halfedge( he );
  }
  vt->sphe = he;
}

void reattach_ppdvertex_halfedge( Sppd *ppd )
{
  for ( Spvt *vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    reattach_ppdvertex_sphe ( vt );
  }
}

Sphe *decide_ppdvertex_sphe( Spvt *vt, Spfc *fc )
{
  Sphe *he = fc->sphe;
  do {
    if ( he->vt == vt ) return he;
  } while ( (he = he->nxt) != fc->sphe );
  return NULL;
}
//
//
Sped *ppdvertex_first_edge( Spvt *vt )
{
  if ( vt == NULL ) return NULL;
  if ( vt->sphe == NULL ) return NULL;
  return vt->sphe->ed;
}

Sped *ppdvertex_next_edge( Sped *ed, Spvt *vt )
{
  if ( ed == NULL ) return NULL;
  
  Sphe *he = NULL;
  if ( ed->lhe != NULL ) {
    if ( ed->lhe->vt == vt ) {
      he = ed->lhe;
    }
  }
  if ( ed->rhe != NULL ) {
    if ( ed->rhe->vt == vt ) {
      he = ed->rhe;
    }
  }

  if ( he == NULL ) return NULL;
  
  // おそらくこれはこんなに複雑でなくてよい
  if ( he->prv != NULL ) return he->prv->ed;
  else return NULL;
}

Sped *ppdvertex_prev_edge( Sped *ed, Spvt *vt )
{
  if ( ed == NULL ) return NULL;
  
  Sphe *he = NULL;
  if ( ed->lhe != NULL ) {
    if ( ed->lhe->vt != vt ) {
      he = ed->lhe;
    }
  }
  if ( ed->rhe != NULL ) {
    if ( ed->rhe->vt != vt ) {
      he = ed->rhe;
    }
  }

  if ( he == NULL ) return NULL;
  
  // おそらくこれはこんなに複雑でなくてよい
  if ( he->nxt != NULL ) return he->nxt->ed;
  else return NULL;
}


//
// vt のまわりの v0 とは違う境界頂点を探す
//
Spvt *ppdvertex_another_boundaryvertex( Spvt *vt, Spvt *v0 )
{
  if ( vt == NULL ) return NULL;
  if ( v0 == NULL ) return NULL;

  Sped *ed, *fed;
  ed = fed = ppdvertex_first_edge( vt );
  do {

    if ( ed->isBoundary == TRUE ) {
      Spvt *avt = another_vt( ed, vt );
      if ( avt != v0 ) return avt;
    }
      
    ed = ppdvertex_next_edge( ed, vt );
  } while ( (ed != NULL) && (ed != fed) );
  
  return NULL;
}
  

void print_ppdvertex_neighbor( Spvt *vt )
{
  if ( vt == NULL ) return;

  display("vertex %d\n", vt->no );
  display("\tface: ");
  Sphe *he = vt->sphe;
  do {
    display("%d ", he->bpfc->no );
    he = ppdvertex_ccwnext_halfedge( he );
  } while ( (he != vt->sphe) && (he != NULL) );
  display("\n");
  display("\tedge: ");
  Sped *e = ppdvertex_first_edge( vt );
  do {
    display("%d ", e->no );
    e = ppdvertex_next_edge( e, vt );
  } while ( (e != ppdvertex_first_edge( vt )) && (e != NULL) );
  display("\n");
}

