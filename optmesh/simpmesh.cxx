//
// simpmesh.cxx
// simplifying mesh functions
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
#include <assert.h>

#include "smd.h"

#include "veclib.h"
#include "matlib.h"
#include "params.h"
#include "ppd.h"
#include "ppdlist.h"
#include "ppdvertex.h"
#include "ppdface.h"
#include "ppdedge.h"
#include "mytime.h"

#include "qem.h"
#include "lodprique.h"
#include "star.h"
//#include "slpopt.h"

#include "simpmesh.h"

//
void simpmesh( Sppd *ppd, int npoint )
{
  // Initialize
  initialize_simpmesh( ppd );

  time_start();
  if ( params.qemmode == MODE_VQEM ) {
    // Per-vertex quadric error metric computation
    ppd_calc_vqem( ppd );
  } else {
    // Per-edge quadric error metric computation
    ppd_calc_eqem( ppd );
  }

#if 0
  // Optional pre-pass (if needed)
  if ( params.pre_optimize == TRUE ) {
    ppdslpopt( ppd );
  }
#endif

#if 0  
    fprintf(stdout, "aaaaaaaaaaa\n");
  double sum = 0.0;
  Spvt* vt;
  for ( vt = ppd->spvt; vt != NULL; vt = vt->nxt )
    {
      fprintf(stdout, "v %d qem %g %g %g %g %g\n",
	      vt->no, vt->mat[0], vt->mat[1], vt->mat[2], vt->mat[3],
	      vt->mat[4] );
      sum += vt->mat[0];
    }
  fprintf(stdout, "sum %g\n", sum );

  Sped* ed;
  for ( ed = ppd->sped; ed != NULL; ed = ed->nxt )
    {
      fprintf(stdout, "e %d qem %g %g %g %g %g\n",
	      ed->no, ed->mat[0], ed->mat[1], ed->mat[2], ed->mat[3],
	      ed->mat[4] );
      sum += ed->mat[0];
    }
  fprintf(stdout, "sum %g\n", sum );
#endif  
  //qem2submatrix_test( ppd );
  
  // insert edges into priority queue
  ppd->lodpqh = ppdedge_to_lodpqheap( ppd );

//  #if 0
//    display("bb\n");
  // run simplification
  optlod( ppd, npoint );
  
  time_stop();

#if 0  
  fprintf(stdout, "bbbbbbbbbbbb\n");
  sum = 0.0;
  for ( vt = ppd->spvt; vt != NULL; vt = vt->nxt )
    {
      fprintf(stdout, "v %d qem %g %g %g %g %g\n",
              vt->no, vt->mat[0], vt->mat[1], vt->mat[2], vt->mat[3],
              vt->mat[4] );
      sum += vt->mat[0];
    }
  
  for ( ed = ppd->sped; ed != NULL; ed = ed->nxt )
    {
      fprintf(stdout, "e %d qem %g %g %g %g %g\n",
              ed->no, ed->mat[0], ed->mat[1], ed->mat[2], ed->mat[3],
              ed->mat[4] );
      sum += ed->mat[0];
    }
  fprintf(stdout, "sum %g\n", sum );
#endif  

//    display("cc\n");
  free_lodpqheap( ppd->lodpqh );
}

//
// simplification (L2 error)
// 
void simpmesh_l2norm( Sppd *ppd, double l2norm )
{
  // Initialize
  initialize_simpmesh( ppd );

  time_start();
  if ( params.qemmode == MODE_VQEM ) {
    // Per-vertex quadric error metric computation
    ppd_calc_vqem( ppd );
  } else {
    // Per-edge quadric error metric computation
    ppd_calc_eqem( ppd );
  }

#if 0
  // Optional pre-pass (if needed)
  if ( params.pre_optimize == TRUE ) {
    ppdslpopt( ppd );
  }
#endif
  
  //qem2submatrix_test( ppd );
  
  // insert edges into priority queue
  ppd->lodpqh = ppdedge_to_lodpqheap( ppd );

//  #if 0
//    display("bb\n");
  // run simplification
  optlod_l2norm( ppd, l2norm );
  
  time_stop();
  
//  #endif
//    display("cc\n");
  free_lodpqheap( ppd->lodpqh );
}

//
// simplification setup
//
void initialize_simpmesh( Sppd *ppd )
{
  for ( Spvt *vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    // used in make_eqem2submatrix
    vt->sid = SMDNULL;
  }
}

// 
// Per-vertex QEM (quadric error metric) accumulation
//
void ppd_calc_vqem( Sppd *ppd )
{
  // Per-face plane offset d
  int i;
  Spfc   *fc;
  Vec *vec;
  //double aa, ab, ac, ad;
  double *d = (double *) malloc( ppd->fn * sizeof(double) );
  double *area  = (double *) malloc( ppd->fn * sizeof(double) );
  for ( i = 0, fc = ppd->spfc; fc != (Spfc *) NULL; fc = fc->nxt, ++i ) {
    calc_fnorm( fc );
    vec = &(fc->sphe->vt->vec);
    d[i] = -( fc->nrm.x * vec->x + fc->nrm.y * vec->y + fc->nrm.z * vec->z );
    //area[i] = calc_farea( fc ) * calc_farea( fc );
    area[i] = calc_farea( fc ) * 50000.0;
    //area[i] = 1.0;
  }

  // Accumulate QEM matrix
  Spvt *vt;
  for ( vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {

    Sphe *he = vt->sphe;
    double sum_area = 0.0;
    if ( he == NULL ) continue;
    initialize_matrix4( (double *) vt->mat );
    // Walk incident faces around the vertex
    do { 
      fc = he->bpfc;
      i = fc->no;
      // Accumulate face contribution into QEM matrix
      vt->mat[0]  += ( fc->nrm.x * fc->nrm.x * area[i] / 3.0);
      vt->mat[1]  += ( fc->nrm.y * fc->nrm.y * area[i] / 3.0);
      vt->mat[2]  += ( fc->nrm.z * fc->nrm.z * area[i] / 3.0);
      vt->mat[3]  += ( fc->nrm.x * fc->nrm.y * area[i] / 3.0);
      vt->mat[4]  += ( fc->nrm.z * fc->nrm.x * area[i] / 3.0);
      vt->mat[5]  += ( fc->nrm.y * fc->nrm.z * area[i] / 3.0);
      vt->mat[6]  += ( d[i] * fc->nrm.x * area[i] / 3.0);
      vt->mat[7]  += ( d[i] * fc->nrm.y * area[i] / 3.0);
      vt->mat[8]  += ( d[i] * fc->nrm.z * area[i] / 3.0);
      vt->mat[9]  += ( d[i] * d[i] * area[i] / 3.0);
      sum_area += (area[i] / 3.0);
      he = ppdvertex_ccwnext_halfedge( he );
    } while ( (he != vt->sphe) && (he != NULL) );
//      vt->mat[0] /= sum_area;
//      vt->mat[1] /= sum_area;
//      vt->mat[2] /= sum_area;
//      vt->mat[3] /= sum_area;
//      vt->mat[4] /= sum_area;
//      vt->mat[5] /= sum_area;
//      vt->mat[6] /= sum_area;
//      vt->mat[7] /= sum_area;
//      vt->mat[8] /= sum_area;
//      vt->mat[9] /= sum_area;

  }
  
  free( d );
  free( area );

  // Boundary-edge penalty
  for ( Sped *ed = ppd->sped; ed != NULL; ed = ed->nxt ) {
    
    if ( ed->isBoundary == FALSE ) continue;

    // plane through edge, orthogonal to adjacent face
    // (rfnrm.x) x + (rfnrm.y) y + (rfnrm.z) z + rfd = 0
    fc = (ed->lhe != NULL ) ? ed->lhe->bpfc : ed->rhe->bpfc;

    Vec sub, rfnrm;
    V3Sub( &(ed->ev->vec), &(ed->sv->vec), &sub );
    V3Cross( &(sub), &(fc->nrm), &rfnrm ); V3Normalize( &rfnrm );
    vec = &(ed->sv->vec);
    double rfd = -( rfnrm.x * vec->x + rfnrm.y * vec->y + rfnrm.z * vec->z );

    double penalty = params.discon_penalty * 
      V3DistanceBetween2Points( &(ed->sv->vec), &(ed->ev->vec) );
    vt = ed->sv;
    vt->mat[0]  += ( rfnrm.x * rfnrm.x * penalty);
    vt->mat[1]  += ( rfnrm.y * rfnrm.y * penalty);
    vt->mat[2]  += ( rfnrm.z * rfnrm.z * penalty);
    vt->mat[3]  += ( rfnrm.x * rfnrm.y * penalty);
    vt->mat[4]  += ( rfnrm.z * rfnrm.x * penalty);
    vt->mat[5]  += ( rfnrm.y * rfnrm.z * penalty);
    vt->mat[6]  += ( rfd * rfnrm.x * penalty);
    vt->mat[7]  += ( rfd * rfnrm.y * penalty);
    vt->mat[8]  += ( rfd * rfnrm.z * penalty);
    vt->mat[9]  += ( rfd * rfd * penalty);
   
    vt = ed->ev;
    vt->mat[0]  += ( rfnrm.x * rfnrm.x * penalty);
    vt->mat[1]  += ( rfnrm.y * rfnrm.y * penalty);
    vt->mat[2]  += ( rfnrm.z * rfnrm.z * penalty);
    vt->mat[3]  += ( rfnrm.x * rfnrm.y * penalty);
    vt->mat[4]  += ( rfnrm.z * rfnrm.x * penalty);
    vt->mat[5]  += ( rfnrm.y * rfnrm.z * penalty);
    vt->mat[6]  += ( rfd * rfnrm.x * penalty);
    vt->mat[7]  += ( rfd * rfnrm.y * penalty);
    vt->mat[8]  += ( rfd * rfnrm.z * penalty);
    vt->mat[9]  += ( rfd * rfd * penalty);
  }
    
}

// 
// Per-edge QEM (quadric error metric) accumulation
//
void ppd_calc_eqem( Sppd *ppd )
{
  //
  // Per face: unit normal, plane offset d, area
  //
  Spfc *fc;
  Vec *vec;
  //double aa, ab, ac, ad;
  double *d = (double *) malloc( ppd->fn * sizeof(double) );
  double *area  = (double *) malloc( ppd->fn * sizeof(double) );
  int i;
  for ( i = 0, fc = ppd->spfc; fc != NULL; fc = fc->nxt, ++i ) {
    calc_fnorm( fc );
    vec = &( fc->sphe->vt->vec );
    // Per-face plane offset d
    d[i] = -( fc->nrm.x * vec->x + fc->nrm.y * vec->y + fc->nrm.z * vec->z );
    // face area
    //area[i] = calc_farea( fc ) * calc_farea( fc );
    area[i] = calc_farea( fc ) * 50000.0;
    //area[i] = 1.0;
  }

  // Initialize per-vertex QEM
  for ( Spvt *vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    initialize_matrix4( (double *) vt->mat );
  }
  
  //
  // Per-edge QEM matrix accumulation
  //
  for ( Sped *ed = ppd->sped; ed != NULL; ed = ed->nxt ) {

    // Initialize edge QEM
    initialize_matrix4( (double *) ed->mat );
    double sum_area = 0.0;

    // faces incident to edge
    for ( int j = 0; j < 2; ++j ) {

      if ( j ) {
	if ( ed->rhe != NULL ) fc = ed->rhe->bpfc;
	else fc = NULL;
      } else {
	if ( ed->lhe != NULL ) fc = ed->lhe->bpfc;
	else fc = NULL;
      }
      if ( fc == NULL ) continue;
      
      // Accumulate face contribution into QEM matrix
      i = fc->no;
      ed->mat[0] += ( fc->nrm.x * fc->nrm.x * area[i] / 3.0);
      ed->mat[1] += ( fc->nrm.y * fc->nrm.y * area[i] / 3.0);
      ed->mat[2] += ( fc->nrm.z * fc->nrm.z * area[i] / 3.0);
      ed->mat[3] += ( fc->nrm.x * fc->nrm.y * area[i] / 3.0);
      ed->mat[4] += ( fc->nrm.z * fc->nrm.x * area[i] / 3.0);
      ed->mat[5] += ( fc->nrm.y * fc->nrm.z * area[i] / 3.0);
      ed->mat[6] += ( d[i] * fc->nrm.x * area[i] / 3.0);
      ed->mat[7] += ( d[i] * fc->nrm.y * area[i] / 3.0 );
      ed->mat[8] += ( d[i] * fc->nrm.z * area[i] / 3.0 );
      ed->mat[9] += ( d[i] * d[i] * area[i] / 3.0 );
      sum_area += ( area[i] / 3.0 );
      
    }
//      ed->mat[0] /= sum_area; ed->mat[1] /= sum_area;
//      ed->mat[2] /= sum_area; ed->mat[3] /= sum_area;
//      ed->mat[4] /= sum_area; ed->mat[5] /= sum_area;
//      ed->mat[6] /= sum_area; ed->mat[7] /= sum_area;
//      ed->mat[8] /= sum_area; ed->mat[9] /= sum_area;

    //
    // Boundary-edge penalty
    //
    
    if ( ed->isBoundary == FALSE ) continue;

    // plane through edge, orthogonal to adjacent face
    // (rfnrm.x) x + (rfnrm.y) y + (rfnrm.z) z + rfd = 0
    fc = (ed->lhe != NULL ) ? ed->lhe->bpfc : ed->rhe->bpfc;
    Vec sub, rfnrm;
    V3Sub( &(ed->ev->vec), &(ed->sv->vec), &sub );
    V3Cross( &(sub), &(fc->nrm), &rfnrm ); V3Normalize( &rfnrm );
    vec = &(ed->sv->vec);
    double rfd = -( rfnrm.x * vec->x + rfnrm.y * vec->y + rfnrm.z * vec->z );
    double penalty = params.discon_penalty * 
      V3DistanceBetween2Points( &(ed->sv->vec), &(ed->ev->vec) );
    // Accumulate boundary penalty into QEM matrix
    ed->mat[0] += ( rfnrm.x * rfnrm.x * penalty);
    ed->mat[1] += ( rfnrm.y * rfnrm.y * penalty);
    ed->mat[2] += ( rfnrm.z * rfnrm.z * penalty);
    ed->mat[3] += ( rfnrm.x * rfnrm.y * penalty);
    ed->mat[4] += ( rfnrm.z * rfnrm.x * penalty);
    ed->mat[5] += ( rfnrm.y * rfnrm.z * penalty);
    ed->mat[6] += ( rfd * rfnrm.x * penalty);
    ed->mat[7] += ( rfd * rfnrm.y * penalty);
    ed->mat[8] += ( rfd * rfnrm.z * penalty);
    ed->mat[9] += ( rfd * rfd * penalty);
   
  }

  free( d );
  free( area );
    
}

//
// edge collapse when edge collapse occurs,  QEM build heap keyed by collapse error
// inside this function,  QEM find the vertex that minimizes the QEM cost.
//
LODPQHeap *ppdedge_to_lodpqheap( Sppd *ppd )
{
  LODPQHeap *pqh = create_lodpqheap( ppd->en );

  for ( Sped *ed = ppd->sped; ed != (Sped *) NULL; ed = ed->nxt ) {

    // for test
//      display("edge %d (boundary %d) sv %d (boundary %d) ev %d (bondary %d)\n",
//  	    ed->no, ed->isBoundary,
//  	    ed->sv->no, ed->sv->isBoundary,
//  	    ed->ev->no, ed->ev->isBoundary );
    
    //
    // Here:  edge collapse  legal move determine whether
    //
    if ( isLegalMove( ed, TRUE ) == FALSE ) {
      continue;
    }

    (void) insert_lodpqcont( ed->error, pqh, ed );
  }
    
  return pqh;
}

void optlod( Sppd *ppd, int npoint )
{
  if ( npoint >= ppd->vn ) return;
  
  Sped *ed = next_ed( ppd );

  int eccount = 1;
  double err = 0.0;
  while ( 1 ) {
    display("(%06d) ed %d sv %d (atr %d) ev %d (atr %d) vn %d err %g\n",
	    eccount, ed->no,
	    ed->sv->no, ed->sv->isBoundary,
	    ed->ev->no, ed->ev->isBoundary,
	    ppd->vn, ed->error );
    ed = try_edge_operation( ed, ppd );
    if ( ed != NULL ) err = (sqrt(ed->error) / ppd->length * 100.0);
    ++eccount;

    if ( ed == (Sped *) NULL ) {
      ppd->err = err;
      return;
    }

    // number
    if ( ppd->vn <= npoint ) {
      ppd->err = err;
      return;
    }
    
    if ( ppd->lodpqh->last == 0 ) {
      ppd->err = err;
      return;
    }

  }
}

void optlod_l2norm( Sppd *ppd, double l2norm )
{
  //if ( npoint >= ppd->vn ) return;
  
  Sped *ed = next_ed( ppd );

  int eccount = 1;
  double err = 0.0;
  while ( 1 ) {
    display("(%06d) ed %d sv %d (atr %d) ev %d (atr %d) vn %d err %g\n",
	    eccount, ed->no,
	    ed->sv->no, ed->sv->isBoundary,
	    ed->ev->no, ed->ev->isBoundary,
	    ppd->vn, ed->error );
    
    ed = try_edge_operation( ed, ppd );
    if ( ed != NULL ) err = (sqrt(ed->error) / ppd->length * 100.0);
    
    ++eccount;

    if ( ed == (Sped *) NULL ) {
      ppd->err = err;
      return;
    }
      
    // error
    if ( err >= l2norm ) {
      ppd->err = err;
      return;
    }
    
    if ( ppd->lodpqh->last == 0 ) {
      ppd->err = err;
      return;
    }
  }
}

//
// - edge collapse  legal move whether
// - resulting shape
// check 
//
BOOL isLegalMove( Sped *ed, BOOL isEvalQEM )
{
//    if ( (ed->sv->isBoundary == TRUE) || (ed->ev->isBoundary == TRUE) ) {
//      return FALSE;
//    }
  
  // create star
  Star *star = NULL;
  if ( (star = create_star( ed )) == (Star *) NULL ) {
    free_star( star );
    return FALSE;
  }

  // topological collapse condition check
  if ( collapse_condition( star ) == FALSE ) {
    free_star( star );
    return FALSE;
  }

  if ( isEvalQEM == TRUE ) {
    if ( params.qemmode == MODE_VQEM ) {
      //
      // sv  and  ev  carries  QEM compute sum of 
      //
      double mat[10];
      add_matrix4( (double *) ed->sv->mat, (double *) ed->ev->mat,
		   (double *) mat );
      eval_vqem( ed, mat );
    } else {
      eval_eqem( ed );
//        display("ed %d opt %g %g %g\n", ed->no, ed->ovec.x, ed->ovec.y, ed->ovec.z );
    }
  }
  
  // star edge collapse driver
  staredge_collapse( star );

  // geometrical collapse condition check
  star->starsv->vec.x = ed->ovec.x;
  star->starsv->vec.y = ed->ovec.y;
  star->starsv->vec.z = ed->ovec.z;
  starnorm( star );

  // angle check + triangle shape check
  if ( geom_check( star, params.angle_tol, params.shape_tol ) == FALSE ) {
    free_star( star );
    return FALSE;
  }

  free_star( star );
  return TRUE;
}

//
// eval vertex-based QEM for optimizing 3D coordinate
// mat[10]
//
void eval_vqem( Sped *ed, double *mat )
{
  switch ( params.subdivision ) {
	
  case EVAL_SLP:

    //
    // Subdivision Limit Point  when 
    //
    //
    // vertex stores Original QEM original QEM matrix only
    // (original  QEM matrix precomputed in  ppd_calcqem  computed there)
    // compute limit-point matrix here
    //
    double qeminfmat[11];
    make_qeminfmatrix( ed, (double *) mat, (double *) qeminfmat );
    optimize_infvector( ed, (double *) qeminfmat );

    break;
      
  case EVAL_VERTEX:
      
    //
    // during simplification
    //
    optimize_vector( ed, (double *) mat );
      
    break;
      
  case EVAL_2SUB:

    // compute two-subdivision matrix here
    double qem2submat[10];
    make_vqem2submatrix( ed, (double *) mat, params.gamma, (double *) qem2submat );
    optimize_2subvector( ed, (double *) qem2submat );
	
    break;

  default:
    break;
      
  }
}
  
//
// eval edge-based QEM for optimizing 3D coordinate
// mat[10]
//
void eval_eqem( Sped *ed )
{
  double qem2submat[10];
  make_eqem2submatrix( ed, params.gamma, (double *) qem2submat );
  optimize_2subvector( ed, (double *) qem2submat );
}
  
//
//  collapse testing
//
Sped *try_edge_operation( Sped *ed, Sppd *ppd )
{
  // legal move + geom check
//    display("try begin\n");
  if ( isLegalMove( ed, FALSE ) == FALSE ) {
    DeleteLODPQCont( ed, ppd, NULL );
    return next_ed( ppd );
  }
//    display("try end\n");

  //
  // ppd edge collapse driver
  //
  ed = edge_collapse( ed, ppd );

  return ed;
}

//
// check function whether edge collapse is valid
// referenced by:
// H. Hoppe et al., "Mesh Optimization", in SIGGRAPH 93
//
BOOL collapse_condition( Star *star )
{
  Sped *ppde = star->stare->ed;
  Spvt *ppdsv = star->starsv->vt;
  Spvt *ppdev = star->starev->vt;

  //
  // Condition 1: for every vertex k adjacent to both endpoints i and j,
  // ({i,k},{j,k} in the link) triangle (i,j,k) must be a face of the star.
  int n_vk = 0;
  /* Avoid malloc/free per edge try (optlod inner loop): star->vn is usually small. */
  enum { kVkStackCap = 256 };
  Starvt *vk_stack[kVkStackCap];
  Starvt **vk = vk_stack;
  if ( star->vn > kVkStackCap ) {
    vk = (Starvt **) malloc( (size_t) star->vn * sizeof(Starvt *) );
    if ( vk == NULL ) {
      return FALSE;
    }
  }

  Starvt *starv;
  for ( starv = star->spvt; starv != (Starvt *) NULL; starv = starv->nxt ) {

    // Count how many of {sv,ev} are neighbors of ppdv
    int cnt = 0;
    Spvt *ppdv = starv->vt;
    Sped *e = ppdvertex_first_edge( ppdv );
    do {
      if ( (e->sv == ppdv) && (e->ev == ppdsv) ) ++cnt;
      if ( (e->sv == ppdv) && (e->ev == ppdev) ) ++cnt;
      if ( (e->ev == ppdv) && (e->sv == ppdsv) ) ++cnt;
      if ( (e->ev == ppdv) && (e->sv == ppdev) ) ++cnt;
      e = ppdvertex_next_edge( e, ppdv );
    } while ( (e != ppdvertex_first_edge( ppdv )) && (e != NULL) );

    // Adjacent to both endpoints
    if ( cnt == 2 ) {
      vk[n_vk] = starv; ++n_vk;
    }
  }

  // Face {sv, ev, vk} must exist in the star
  for ( int i = 0; i < n_vk; ++i ) {
    if ( find_starface( star, star->starsv, star->starev, vk[i] ) == NULL ) {
      if ( vk != vk_stack ) {
        free( vk );
      }
      return FALSE;
    }
  }

  if ( vk != vk_stack ) {
    free( vk );
  }

  // Condition 2: collapsing a boundary vertex requires a boundary edge
  if ( (ppdsv->isBoundary == TRUE) || (ppdev->isBoundary == TRUE) ) {
    if ( ppde->isBoundary == FALSE ) return FALSE;
  }

  // Condition 3: minimum star size (interior vs boundary case)
  if ( (ppdsv->isBoundary != TRUE) && (ppdev->isBoundary != TRUE) ) {
    //if (star->vn < 4) return FALSE;
    if (star->vn < 5) return FALSE;
  } else {
    //if (star->vn < 3) return FALSE;
    if (star->vn < 4) return FALSE;
  }

  return TRUE;
}

void DeleteLODPQCont( Sped *ed, Sppd *ppd, LODPQHeap *lodpqh )
{
  LODPQCont *pqc;
  if ( (pqc = ed->pqc) == NULL ) return;

  if ( ppd != NULL ) {
    delete_lodpqcont( pqc->id, ppd->lodpqh );
  } else if ( lodpqh != NULL ) {
    delete_lodpqcont( pqc->id, lodpqh );
  }
}

Sped *next_ed( Sppd *ppd )
{
  return ppd->lodpqh->pqcont[0].ed;
}

//
// edge collapse driver
// 
Sped *edge_collapse( Sped *e, Sppd *ppd )
{
  if ( e == NULL ) return NULL;
  Spvt *vi = e->sv;
  Spvt *vj = e->ev;

  // update sv's coordinate
  vi->vec.x = e->ovec.x;
  vi->vec.y = e->ovec.y;
  vi->vec.z = e->ovec.z;
	  
  int  side = 2; Spfc *f[2];
  if ( e->lhe != NULL ) f[0] = e->lhe->bpfc; 
  else f[0] = NULL;
  if ( e->rhe != NULL ) f[1] = e->rhe->bpfc;
  else f[1] = NULL;
  
  // first gather adjacency
  // i = 0: left, i = 1: right
  int  i;
  Spvt *vk[2];
  Sped *ei[2], *ej[2];
  Sphe *hei[2], *hej[2];
  Spfc *fi[2], *fj[2];
  for ( i = 0; i < side; ++i ) {

    if ( f[i] == NULL ) continue;
    
    vk[i] = get_lm_ppdvertex( f[i], vi, vj );
    ei[i] = get_lm_ppdedge( f[i], vi, vk[i] );
    ej[i] = get_lm_ppdedge( f[i], vj, vk[i] );

    hei[i] = NULL;
    hei[i] = get_lm_ppdhalfedge_rev( f[i], ei[i] );
    if ( hei[i] != NULL ) {
      fi[i] = hei[i]->bpfc;
    } else {
      fi[i] = NULL;
    }

    hej[i] = NULL;
    hej[i] = get_lm_ppdhalfedge_rev( f[i], ej[i] );
    if ( hej[i] != NULL ) {
      fj[i] = hej[i]->bpfc;
    } else {
      fj[i] = NULL;
    }

    // matrix update (ei  of  mat  and  ej  of  mat add matrices )
    if ( params.qemmode == MODE_EQEM ) {
      if ( (ei[i] != NULL) && (ej[i] != NULL) )
	add_matrix4( (double *) ei[i]->mat, (double *) ej[i]->mat,
		     (double *) ei[i]->mat );
    }

  }

  // matrix update 
  if ( params.qemmode == MODE_VQEM ) {

    // vj  of  mat  vi  add to 
    add_matrix4( (double *) vi->mat, (double *) vj->mat, (double *) vi->mat );
    
  } else { // MODE_EQEM

    // vj  of  mat  vi  add to 
    add_matrix4( (double *) vi->mat, (double *) vj->mat, (double *) vi->mat );
    // e  of  mat  vi  add to 
    add_matrix4( (double *) vi->mat, (double *) e->mat, (double *) vi->mat );
    
  }

  //
  // topological operations
  //
  // 1. vi->sphe, vk[i]->sphe unlink 
  // 2. vj adjacent face  he->vt, incident-edge  ed->sv or ed->ev 
  //    vj  from  vi  assign to 
  // 3. gathered adjacency (lhe, rhe  and  mate , etc.) update 
  // 4. vi->sphe, vk[i]->sphe reconnect 

  // 1. vi->sphe, vk[i]->sphe unlink 
  // unlink once here
  vi->sphe = NULL;
  for ( i = 0; i < side; ++i ) {
    if ( f[i] == NULL ) continue;
    vk[i]->sphe = NULL;
  }
  
  // 2. vj adjacent face he->vt, incident-edge  ed->sv or ed->ev  vj  from  vi  assign to 
  change_ppdvertexlinks( vj, vi );
  
  // 3. gathered adjacency (lhe, rhe  and  mate , etc.) update 
  for ( i = 0; i < side; ++i ) {
    
    if ( f[i] == NULL ) continue;
    
    // hej  of  ed  ej  from  ei  assign to 
    //if ( hei[i] != NULL ) hei[i]->ed = ei[i];
    if ( hej[i] != NULL ) hej[i]->ed = ei[i];
      
    // ei  of  lhe or rhe  hej  retarget to 
    if ( ei[i]->lhe == hei[i] ) {
      ei[i]->rhe = hej[i];
    }
    if ( ei[i]->rhe == hei[i] ) {
      ei[i]->lhe = hej[i];
    }
    
    // fi - fj  of  mate create 
    if ( hei[i] != NULL ) hei[i]->mate = hej[i];
    if ( hej[i] != NULL ) hej[i]->mate = hei[i];
    
    // attach boundary-edge info
    if ( fj[i] == NULL ) {
      ei[i]->isBoundary = TRUE;
    }

  }    
  
  // attach boundary-vertex info
  if ( vj->isBoundary == TRUE ) {
    vi->isBoundary = TRUE;
  }

  // 4. vi->sphe , vk[i]->sphe relink 
  for ( i = 0; i < side; ++i ) {

    if ( f[i] == NULL ) continue;
    
    // fi  and  fj at least one of the two exists
    if ( fi[i] != NULL ) {
      if ( vk[i]->sphe == NULL ) 
	vk[i]->sphe = decide_ppdvertex_sphe( vk[i], fi[i] );
      if ( vi->sphe == NULL ) 
	vi->sphe = decide_ppdvertex_sphe( vi, fi[i] );
    }
    if ( fj[i] != NULL ) {
      if ( vk[i]->sphe == NULL ) 
	vk[i]->sphe = decide_ppdvertex_sphe( vk[i], fj[i] );
      if ( vi->sphe == NULL ) 
	vi->sphe = decide_ppdvertex_sphe( vi, fj[i] );
    }
    
    reattach_ppdvertex_sphe( vk[i] );

  }
  
  reattach_ppdvertex_sphe( vi );
  
  // delete
  for ( i = 0; i < side; ++i ) {
    if ( f[i] == NULL ) continue;
    free_ppdface( f[i], ppd );
    DeleteLODPQCont( ej[i], ppd, NULL );
    free_ppdedge( ej[i], ppd );
  }
  free_ppdvertex( vj, ppd );
  DeleteLODPQCont( e, ppd, NULL );
  free_ppdedge( e, ppd );

  //
  // update heap for incident edges
  //
  adjust_lodpqheap( vi, ppd->lodpqh );

  return next_ed( ppd );
}

Spvt *get_lm_ppdvertex( Spfc *f, Spvt *v1, Spvt *v2 )
{
  if ( f == NULL ) return NULL;

  Sphe *he = f->sphe;
  do {
    Spvt *vt = he->vt;
    if ( ( vt != v1 ) && ( vt != v2 ) ) return vt;
  } while ( (he = he->nxt) != f->sphe );
  
  return (Spvt *) NULL;
}

Sped *get_lm_ppdedge( Spfc *f, Spvt *v1, Spvt *v2 )
{
  if ( f == NULL ) return NULL;

  Sphe *he = f->sphe;
  do {
    Sped *e = he->ed;
    if ( ((e->sv == v1) && (e->ev == v2)) ||
	 ((e->ev == v1) && (e->sv == v2)) )
      return e;
  } while ( (he = he->nxt) != f->sphe );
  
  return NULL;
}

Sphe *get_lm_ppdhalfedge_rev( Spfc *fc, Sped *ed )
{
  if ( ed == NULL ) return NULL;
  if ( fc == NULL ) return NULL;

  Sphe *he = fc->sphe;
  do {
    if ( he->ed == ed ) {
      if ( ed->lhe == he ) return ed->rhe;
      if ( ed->rhe == he ) return ed->lhe;
    }
  } while ( (he = he->nxt) != fc->sphe );

  return NULL;
}
      
void change_ppdvertexlinks( Spvt *ev, Spvt *sv )
{
  if ( ev == NULL ) return;

  // ev neighbor vertices on  sv  assign to 
  Sped *ed = ppdvertex_first_edge( ev );
  do {
    if ( ed->sv == ev ) ed->sv = sv;
    if ( ed->ev == ev ) ed->ev = sv;
    ed = ppdvertex_next_edge( ed, ev );
  } while ( (ed != ppdvertex_first_edge( ev )) && (ed != NULL) );
  
  // ev vertices on adjacent faces,  sv  assign to 
  Sphe *he = ev->sphe;
  do {
    //if ( he->vt == sv ) break;
    he->vt = sv;
    he = ppdvertex_ccwnext_halfedge( he );
  } while ( (he != ev->sphe) && (he != NULL) );

}

//
// edge collapse update LOD heap for edges around this edge
//
void adjust_lodpqheap( Spvt *vt, LODPQHeap *lodpqh )
{
  Sped *ed = ppdvertex_first_edge( vt );
  do {

//      display("adj begin\n");
    if ( isLegalMove( ed, TRUE ) == TRUE ) {

//        display("ed error %g\n", ed->error );
      // queue rebalance
      if ( ed->pqc == NULL ) (void) insert_lodpqcont( ed->error, lodpqh, ed );
      else (void) adjust_lodpqcont( ed->pqc->id, lodpqh );
      
    } else {

      if ( ed->pqc != NULL ) DeleteLODPQCont( ed, NULL, lodpqh );

    }

    ed = ppdvertex_next_edge( ed, vt );
    
  } while ( (ed != ppdvertex_first_edge( vt )) && (ed != NULL) );
  
}

void qem2submatrix_test( Sppd *ppd )
{
  for ( Sped *ed = ppd->sped; ed != (Sped *) NULL; ed = ed->nxt ) {
    
    display("ed %d\n", ed->no );
    //
    // Here:  edge collapse  legal move determine whether
    //
    if ( isLegalMove( ed, TRUE ) == FALSE ) {
      display("legal false.\n");
      continue;
    }
    display("legal true.\n");
    
    //
    // sv  and  ev  carries  QEM compute sum of 
    //
    double mat[10];
    add_matrix4( (double *) ed->sv->mat, (double *) ed->ev->mat,
		 (double *) mat );

    // compute two-subdivision matrix here
    display("ed %d sv %d %g %g %g ev %d %g %g %g\n",
	    ed->no,
	    ed->sv->no,
	    ed->sv->vec.x, ed->sv->vec.y, ed->sv->vec.z,
	    ed->ev->no,
	    ed->ev->vec.x, ed->ev->vec.y, ed->ev->vec.z );
    
    double qem2submat[10];
    make_vqem2submatrix( ed, (double *) mat, params.gamma, (double *) qem2submat );
    optimize_2subvector( ed, (double *) qem2submat );
  }
}

