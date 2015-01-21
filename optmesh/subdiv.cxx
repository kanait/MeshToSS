//
// subdiv.cxx
// subdivisionn routines
// 
// Copyright (c) 2000 Takashi Kanai; All rights reserved. 
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <stdlib.h>
#include <math.h>

#include "smd.h"

#include "ppd.h"
#include "ppdsolid.h"
#include "ppdpart.h"
#include "ppdvertex.h"
#include "ppdface.h"

#include "subdiv.h"

Sppd* ppdsubdiv( Sppd* ppd, int subdiv_type )
{
  ppdsubdiv_initialize( ppd );

  Sppd* subppd = create_ppd();
  subppd->spso = create_ppdsolid( subppd );
  subppd->sppt = create_ppdpart( subppd );
  subppd->sppt->bpso = subppd->spso;

  // level
  subppd->sub_level = ppd->sub_level + 1;
  ppd->child = subppd;
  subppd->parent = ppd;
  
  int i;
  
  // even vertex の生成 (id は ppd vertex に対応)
  Spvt** even = (Spvt**) malloc( ppd->vn * sizeof(Spvt*) );
  for ( i = 0; i < ppd->vn; ++i )
    {
      even[i] = create_ppdvertex( subppd );
      even[i]->bpso = subppd->spso;
    }
  // odd vertex  の生成 (id は ppd edge に対応)
  Spvt** odd = (Spvt**) malloc( ppd->en * sizeof(Spvt*) );
  for ( i = 0; i < ppd->en; ++i )
    {
      odd[i] = create_ppdvertex( subppd );
      odd[i]->bpso = subppd->spso;
    }

  // エッジに境界をつけるためのフラッグ
//    int en = 0;
//    for ( Spfc* efc = ppd->spfc; efc != NULL; efc = efc->nxt )
//      {
//        en += (2 * efc->hen);
//      }
  
//    BOOL* edflag = (BOOL*) malloc( 12 * ppd->fn * sizeof(BOOL) );
  // 分割面の生成 ( 4-to-1 subdivision )
//    i = 0;
  for ( Spfc* fc = ppd->spfc; fc != NULL; fc = fc->nxt )
    {
      Spvt* v0 = fc->sphe->vt;
      Spvt* v1 = fc->sphe->nxt->vt;
      Spvt* v2 = fc->sphe->nxt->nxt->vt;
      Sped* e0 = fc->sphe->ed;
      Sped* e1 = fc->sphe->nxt->ed;
      Sped* e2 = fc->sphe->nxt->nxt->ed;

      // face no.0 (v0, e0, e2)
      Spfc* sub_fc = create_ppdface( subppd );
      sub_fc->bpso = subppd->spso;
      sub_fc->bppt = subppd->sppt;
      Sphe* sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = even[v0->sid];
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e0->sid];
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e2->sid];

//        edflag[i] = TRUE; i++;
//        edflag[i] = FALSE; i++;
//        edflag[i] = TRUE; i++;
      
      // face no.1 (v1, e1, e0)
      sub_fc = create_ppdface( subppd );
      sub_fc->bpso = subppd->spso;
      sub_fc->bppt = subppd->sppt;
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = even[v1->sid];
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e1->sid];
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e0->sid];
      
//        edflag[i] = TRUE; i++;
//        edflag[i] = FALSE; i++;
//        edflag[i] = TRUE; i++;
      
      // face no.2 (v2, e2, e1)
      sub_fc = create_ppdface( subppd );
      sub_fc->bpso = subppd->spso;
      sub_fc->bppt = subppd->sppt;
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = even[v2->sid];
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e2->sid];
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e1->sid];
      
//        edflag[i] = TRUE; i++;
//        edflag[i] = FALSE; i++;
//        edflag[i] = TRUE; i++;
      
      // face no.3 (e0, e1, e2)
      sub_fc = create_ppdface( subppd );
      sub_fc->bpso = subppd->spso;
      sub_fc->bppt = subppd->sppt;
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e0->sid];
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e1->sid];
      sub_he = create_ppdhalfedge( sub_fc );
      sub_he->vt = odd[e2->sid];
      
//        edflag[i] = FALSE; i++;
//        edflag[i] = FALSE; i++;
//        edflag[i] = FALSE; i++;

      Spvt* nv0 = even[v0->sid]; nv0->isSubdivType = SUBDIV_EVEN;
      Spvt* nv1 = even[v1->sid]; nv1->isSubdivType = SUBDIV_EVEN;
      Spvt* nv2 = even[v2->sid]; nv2->isSubdivType = SUBDIV_EVEN;
      Spvt* ne0 = odd[e0->sid]; ne0->isSubdivType = SUBDIV_ODD;
      Spvt* ne1 = odd[e1->sid]; ne1->isSubdivType = SUBDIV_ODD;
      Spvt* ne2 = odd[e2->sid]; ne2->isSubdivType = SUBDIV_ODD;
      
      if ( e0->isSubdivBoundary == TRUE )
	{
	  nv0->isSubdivBoundary = TRUE;
	  ne0->isSubdivBoundary = TRUE;
	  nv1->isSubdivBoundary = TRUE;
	}
      if ( e1->isSubdivBoundary == TRUE )
	{
	  nv1->isSubdivBoundary = TRUE;
	  ne1->isSubdivBoundary = TRUE;
	  nv2->isSubdivBoundary = TRUE;
	}
      if ( e2->isSubdivBoundary == TRUE )
	{
	  nv2->isSubdivBoundary = TRUE;
	  ne2->isSubdivBoundary = TRUE;
	  nv0->isSubdivBoundary = TRUE;
	}
      
    }
//    display("i %d\n", i );

  // 頂点座標の計算

  switch ( subdiv_type )
    {
    case SUBDIV_LOOP:
      loop_calculation( subppd, even, odd, ppd );
      break;
    default:
      break;
    }

  // エッジ
  ppd_make_edges( subppd );

  calc_ppd_boundary( subppd );

  reattach_ppdvertex_halfedge( subppd );

  // 法線, 境界フラッグをつける
  i = 0;
  for ( Spfc* sfc = subppd->spfc; sfc != NULL; sfc = sfc->nxt )
    {
      calc_fnorm( sfc );

//        Sphe* he = sfc->sphe;
//        he->ed->isSubdivBoundary = edflag[i]; i++;
//        he->nxt->ed->isSubdivBoundary = edflag[i]; i++;
//        he->nxt->nxt->ed->isSubdivBoundary = edflag[i]; i++;
    }

//    free( edflag );

  // 境界フラッグをつける
  for ( Sped* ed = subppd->sped; ed != NULL; ed = ed->nxt )
    {
      Spvt* sv = ed->sv;
      Spvt* ev = ed->ev;
      if ( (sv->isSubdivType == SUBDIV_ODD) &&
	   (ev->isSubdivType == SUBDIV_ODD) ) {
	continue;
      }
      
      if ( (sv->isSubdivBoundary == TRUE) &&
	   (ev->isSubdivBoundary == TRUE) )
	{
	  ed->isSubdivBoundary = TRUE;
	}
      
    }
  // 
  
  return subppd;
}

void ppdsubdiv_initialize( Sppd* ppd )
{
  int i;
  Spvt* vt;
  for (i = 0, vt = ppd->spvt; vt != NULL; vt = vt->nxt, ++i )
    {
      vt->sid = i;
    }
  Sped* ed;
  for (i = 0, ed = ppd->sped; ed != NULL; ed = ed->nxt, ++i )
    {
      ed->sid = i;
    }
}

//
// loop's subdivision の頂点座標の計算
//
void loop_calculation( Sppd* subppd, Spvt** even, Spvt** odd, Sppd* ppd )
{
  // odd vertex の座標計算
  for ( Spvt* vt = ppd->spvt; vt != NULL; vt = vt->nxt )
    {
      if ( vt->isBoundary == FALSE )
	{
	  loop_even_internal( vt, &(even[vt->sid]->vec) );
	}
      else
	{
	  loop_even_boundary( vt, &(even[vt->sid]->vec) );
	}
    }

  // even vertex の座標計算
  for ( Sped* ed = ppd->sped; ed != NULL; ed = ed->nxt )
    {
      if ( ed->isBoundary == FALSE )
	{
	  loop_odd_internal( ed, &(odd[ed->sid]->vec) );
	}
      else
	{
	  loop_odd_boundary( ed, &(odd[ed->sid]->vec) );
	}
    }
}

void loop_even_internal( Spvt* vt, Vec* vec )
{
  Vec sum;
  sum.x = sum.y = sum.z = 0.0;

  int k = 0;
  Sped *ed, *fed;
  ed = fed = ppdvertex_first_edge( vt );
  do {

    Spvt* vi = another_vt( ed, vt );

    // eps0 への値の格納
    sum.x += vi->vec.x;
    sum.y += vi->vec.y;
    sum.z += vi->vec.z;
    ++k;

    ed = ppdvertex_next_edge( ed, vt );
  } while ( (ed != NULL) && (ed != fed) ); 
  
  //
  // k0, beta0 の計算 (v0 に関する値)
  //
  double beta = calc_beta( k );

  //
  // alpha の計算
  //
  double dk = (double) k;
  double alpha = 1.0 - dk * beta;
  
  vec->x = alpha * vt->vec.x + beta * sum.x;
  vec->y = alpha * vt->vec.y + beta * sum.y;
  vec->z = alpha * vt->vec.z + beta * sum.z;
  
}

void loop_even_boundary( Spvt* vt, Vec* vec )
{
  Sped *e1 = ppdvertex_first_edge( vt );
  Spvt* v1 = another_vt( e1, vt );
  Spvt* v2 = ppdvertex_another_boundaryvertex( vt, v1 );

  vec->x = VAL68 * vt->vec.x + VAL18 * (v1->vec.x + v2->vec.x);
  vec->y = VAL68 * vt->vec.y + VAL18 * (v1->vec.y + v2->vec.y);
  vec->z = VAL68 * vt->vec.z + VAL18 * (v1->vec.z + v2->vec.z);
}


void loop_odd_internal( Sped* ed, Vec* vec )
{
  Spvt* v0 = ed->sv;
  Spvt* v1 = ed->ev;
  Spvt* v2 = ppdface_another_vertex( ed->lhe->bpfc, ed->sv, ed->ev );
  Spvt* v3 = ppdface_another_vertex( ed->rhe->bpfc, ed->sv, ed->ev );

  vec->x = VAL38 * (v0->vec.x + v1->vec.x) + VAL18 * (v2->vec.x + v3->vec.x);
  vec->y = VAL38 * (v0->vec.y + v1->vec.y) + VAL18 * (v2->vec.y + v3->vec.y);
  vec->z = VAL38 * (v0->vec.z + v1->vec.z) + VAL18 * (v2->vec.z + v3->vec.z);
}

void loop_odd_boundary( Sped* ed, Vec* vec )
{
  vec->x = VAL12 * (ed->sv->vec.x + ed->ev->vec.x);
  vec->y = VAL12 * (ed->sv->vec.y + ed->ev->vec.y);
  vec->z = VAL12 * (ed->sv->vec.z + ed->ev->vec.z);
}

//
// valence の beta を求める関数
//
double calc_beta( int valence )
{
  double beta;
  double dval = (double) valence;

  if ( valence != 3 ) {
    double val = VAL38 + cos( 2.0 * SMDPI / dval) / 4.0;
    beta = (VAL58 - val*val) / dval;
  } else {
    beta = BETA3;
  }

  return beta;
}

// for level 0 ppd
void set_subdiv_boundary( Sppd* ppd )
{
  if ( ppd->sub_level != 0 ) return;
  
  for ( Sped* ed = ppd->sped; ed != NULL; ed = ed->nxt )
    {
      ed->isSubdivBoundary = TRUE;
      ed->sv->isSubdivBoundary = TRUE;
      ed->ev->isSubdivBoundary = TRUE;
      ed->sv->isSubdivType = SUBDIV_EVEN;
      ed->ev->isSubdivType = SUBDIV_EVEN;
    }
}

Sppd* subdiv_top( Sppd* ppd )
{
  if ( ppd == NULL ) return NULL;
  Sppd* tppd = ppd;
  while ( tppd->parent != NULL )
    {
      tppd = tppd->parent;
    }
  return tppd;
}
  
