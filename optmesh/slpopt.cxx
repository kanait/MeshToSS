//
// slpopt.cxx
// optimize using slp (Subdivision Limit Position)
// 
// Copyright (c) 1999-2000 Takashi Kanai; All rights reserved. 
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "smd.h"

#include "ppd.h"
#include "ppdvertex.h"
#include "ppdface.h"
#include "ppdedge.h"
#include "veclib.h"
#include "matlib.h"
#include "linbcg.h"
#include "file.h"

#include "qem.h"
#include "subdiv.h"

#include "slpopt.h"

void ppdslpopt( Sppd *ppd )
{

  // ス魘  ppdslpopt_initialize( ppd );
  
  // matrix creation process
  Semat *emat = create_slpopt_emat( ppd );
  create_slpopt_vector( ppd, emat );

  // iteration process
  int    iter;
  double rsq;
  (void) linbcg( emat, emat->bx, emat->xx, 1, SMDZEROEPS, 1000, &iter, &rsq );
  (void) linbcg( emat, emat->by, emat->yy, 1, SMDZEROEPS, 1000, &iter, &rsq );
  (void) linbcg( emat, emat->bz, emat->zz, 1, SMDZEROEPS, 1000, &iter, &rsq );

  // vectors -> ppdvertex
  solvec_ppdvertex( ppd, emat );

  //free emat, vtxvt
  free_emat( emat );

  ppdslpopt_finish( ppd );
}  

// 
// 初期化
// 
void ppdslpopt_initialize( Sppd *ppd )
{
  // 次数の決定
  // lambda, vec2d の領域確保
  Spvt *vt;
  int i;
  for ( i = 0, vt = ppd->spvt; vt != NULL; vt = vt->nxt, ++i ) {
    // 頂点番号の初期化
    vt->sid = i;
    // 次数の決定
    vt->n_degree = ppdvertex_num_edge( vt );

    // lambda initialize (valence + 1)
    vt->lambda = (double *) malloc( (vt->n_degree + 1) * sizeof(double) );
  }

}

// 
// 初期化
// 
void ppdslpopt_finish( Sppd *ppd )
{
  // lambda, vec2d の領域削除
  for ( Spvt *vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    free( vt->lambda );  vt->lambda = NULL;
  }
}

Semat *create_slpopt_emat( Sppd *ppd )
{
  // alpha, kai の決定
  slpopt_lambda( ppd );

  // マトリックスの行数を数える
  int vn = 0;
  Spvt *vt;
  for ( vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    vn += vt->n_degree;
  }

  //
  // create indexed sparse matrix
  //
  Semat *emat = init_emat( ppd->vn );
  emat->num = vn + ppd->vn + 1;
  emat->sa  = (double *) malloc(emat->num * sizeof(double));
  int i;
  for (i = 0; i < emat->num; ++i) emat->sa[i] = 0.0;
  emat->ija = (int *) malloc(emat->num * sizeof(int));
  
  //
  // indexed sparse matrix 各行の要素に値を格納する
  //
  emat->ija[0] = emat->cnum + 1;
  int j = emat->cnum;
  Sped *ed, *fed;
  for ( vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    
    emat->sa[vt->sid] = vt->lambda[0];

    ed = ppdvertex_first_edge( vt );
    fed = ed;
    i = 1;
    do {
      ++j;
      emat->sa[j]  = vt->lambda[i];
      emat->ija[j] = another_vt( ed, vt )->sid;
      ++i;
      ed = ppdvertex_next_edge( ed, vt );
    } while ( (ed != NULL) && (ed != fed) );
    
    emat->ija[vt->sid + 1] = j+1;
  }
  return emat;
}

void create_slpopt_vector( Sppd *ppd, Semat *emat )
{
  int i = 0;
  // initialize vector
  for ( i = 0; i < ppd->vn; ++i ) {
    emat->bx[i] = emat->by[i] = emat->bz[i] = 0.0;
    emat->xx[i] = emat->yy[i] = emat->zz[i] = 0.0;
  }

  // -A^{-1}b の計算
  Spvt *vt;
  for ( vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    double inv[10];
    i = vt->sid;
    if ( InvertSymMatrix3( (double *) vt->mat, (double *) inv ) == TRUE ) {
      emat->bx[i] = -1.0 * (inv[0]*vt->mat[6] + inv[3]*vt->mat[7] + inv[4]*vt->mat[8]);
      emat->by[i] = -1.0 * (inv[3]*vt->mat[6] + inv[1]*vt->mat[7] + inv[5]*vt->mat[8]);
      emat->bz[i] = -1.0 * (inv[4]*vt->mat[6] + inv[5]*vt->mat[7] + inv[2]*vt->mat[8]);
    } else {
      emat->bx[i] = vt->vec.x;
      emat->by[i] = vt->vec.y;
      emat->bz[i] = vt->vec.z;
    }
  }
}

// λを決めるための関数
void slpopt_lambda( Sppd *ppd )
{
  for (Spvt *vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {

    double dval = (double) vt->n_degree;
    double beta;
    if ( vt->n_degree != 3 ) {
      double val = VAL38 + (cos( 2.0 * SMDPI / dval ) / 4.0);
      beta = (VAL58 - val*val) / dval;
    } else {
      beta = BETA3;
    }
    double kai = 1.0 / ( (VAL38 / beta) + dval );

    double alpha;
    if ( vt->isBoundary != TRUE ) {
      // 内部頂点用処理
      alpha = 1.0 - dval * kai;
    } else {
      // 境界頂点用処理
      alpha = 0.6;
    }
    vt->lambda[0] = alpha;
    
    int i = 1;
    Sped *ed = ppdvertex_first_edge( vt );
    do {
      if ( vt->isBoundary != TRUE ) {
	// 内部頂点用処理
	vt->lambda[i] = kai;
      } else {
	// 境界頂点用処理
	if ( another_vt( ed, vt )->isBoundary == TRUE ) {
	  vt->lambda[i] = kai;
	} else {
	  vt->lambda[i] = 0.0;
	}
      }
      ++i;
      ed = ppdvertex_next_edge( ed, vt );
    } while ( (ed != NULL) && (ed != ppdvertex_first_edge( vt )) );
  }
}

void solvec_ppdvertex( Sppd *ppd, Semat *emat )
{
  for ( Spvt *vt = ppd->spvt; vt != NULL; vt = vt->nxt ) {
    
    vt->vec.x = emat->xx[ vt->sid ];
    vt->vec.y = emat->yy[ vt->sid ];
    vt->vec.z = emat->zz[ vt->sid ];
    
  }
}

//
// Semat functions
//
Semat *init_emat(int num)
{
  Semat *emat;
  
  emat = (Semat *) malloc(sizeof(Semat));
  emat->rnum = num;
  emat->cnum = num;
  // constant vectors initialize
  emat->bx = (double *) malloc(num * sizeof(double));
  emat->by = (double *) malloc(num * sizeof(double));
  emat->bz = (double *) malloc(num * sizeof(double));
  // solution vectors initialize
  emat->xx = (double *) malloc(num * sizeof(double));
  emat->yy = (double *) malloc(num * sizeof(double));
  emat->zz = (double *) malloc(num * sizeof(double));

  emat->sa  = NULL;
  emat->ija = NULL;
  
  return emat;
}

void free_emat( Semat *emat )
{
  if ( emat->sa != NULL)  free(emat->sa);
  if ( emat->ija != NULL) free(emat->ija);
  free(emat->bx);
  free(emat->by);
  free(emat->bz);
  free(emat->xx);
  free(emat->yy);
  free(emat->zz);
  free(emat);
}

void printemat(Semat *emat)
{
  int i;
  
  for (i = 0; i < emat->cnum; ++i) {
    printf("%d (b) %f %f\n", i, emat->bx[i], emat->by[i]);
    printf("%d (x) %f %f\n", i, emat->xx[i], emat->yy[i]);
  }
  for (i = 0; i < emat->num; ++i) 
    printf("(%d) (sa) %f (ija) %d\n", i, emat->sa[i], emat->ija[i]);
}

