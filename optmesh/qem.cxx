//
// qem.cxx
// Quadric Error Metric (QEM) functions
// from Garland and Heckbert's SIGGRAPH 97 paper:
// "Surface simplification using quadric error metrics"
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
#include "ppdvertex.h"
#include "ppdedge.h"
#include "ppdface.h"
#include "subdiv.h"

#include "qem.h"

// for test
extern Sppd *global_ppd;

////////////////////////////////////////////////////////////////////////
//
// $B%H%3%J%?!&%k!#%7!&%1!"%[(B QEM $B!"%[%-%i%5%5!#!"%3%L%J%c%$%9%3%D%N%/!"%[%-%i%5%5(B
//
////////////////////////////////////////////////////////////////////////

// mat[10]
void optimize_vector( Sped *ed, double *mat )
{
  // $B%*%f%1%d%[N7W;(B
  double inv[10];
  Vec  vec;
  if ( InvertSymMatrix3( (double *) mat, (double *) inv ) == TRUE ) {

    // optimized vector
    vec.x = -1.0 * (inv[0]*mat[6] + inv[3]*mat[7] + inv[4]*mat[8]);
    vec.y = -1.0 * (inv[3]*mat[6] + inv[1]*mat[7] + inv[5]*mat[8]);
    vec.z = -1.0 * (inv[4]*mat[6] + inv[5]*mat[7] + inv[2]*mat[8]);
    ed->error = calc_error( (double *) mat, &vec );
    ed->ovec.x = vec.x;
    ed->ovec.y = vec.y;
    ed->ovec.z = vec.z;
    
  } else { // Inverse failed.

    // $B;OE@(B
    vec.x = ed->sv->vec.x;
    vec.y = ed->sv->vec.y;
    vec.z = ed->sv->vec.z;
    ed->error = calc_error( (double *) mat, &vec );
    
    // $B=*E@(B
    Vec vec1;
    double error;
    vec1.x = ed->ev->vec.x;
    vec1.y = ed->ev->vec.y;
    vec1.z = ed->ev->vec.z;
    error = calc_error( (double *) mat, &vec1 );
    if ( error < ed->error ) {
      ed->error = error;
      vec.x = vec1.x;
      vec.y = vec1.y;
      vec.z = vec1.z;
    }
    
    // $BCf4VE@(B
    vec1.x = (ed->sv->vec.x + ed->ev->vec.x) / 2.0;
    vec1.y = (ed->sv->vec.y + ed->ev->vec.y) / 2.0;
    vec1.z = (ed->sv->vec.z + ed->ev->vec.z) / 2.0;
    error = calc_error( (double *) mat, &vec1 );
    if ( error < ed->error ) {
      ed->error = error;
      vec.x = vec1.x;
      vec.y = vec1.y;
      vec.z = vec1.z;
    }

    ed->ovec.x = vec.x;
    ed->ovec.y = vec.y;
    ed->ovec.z = vec.z;
  }

}

// inverse matrix is adj(mat) / det(mat)
// for optimize_vector
BOOL InvertSymMatrix3( double *mat, double *inv )
{
  double det = ( mat[0]*mat[1]*mat[2] + 2.0 * mat[3]*mat[4]*mat[5]
		 - mat[0]*mat[5]*mat[5] - mat[1]*mat[4]*mat[4]
		 - mat[2]*mat[3]*mat[3] );
  //  display("det %g\n", det );

  if ( fabs(det) < 1.0e-12 ) return FALSE;
  
  inv[0] = (mat[1]*mat[2] - mat[5]*mat[5]) / det;
  inv[1] = (mat[0]*mat[2] - mat[4]*mat[4]) / det;
  inv[2] = (mat[0]*mat[1] - mat[3]*mat[3]) / det;
  inv[3] = (mat[4]*mat[5] - mat[2]*mat[3]) / det;
  inv[4] = (mat[3]*mat[5] - mat[1]*mat[4]) / det;
  inv[5] = (mat[3]*mat[4] - mat[0]*mat[5]) / det;
  inv[6] = 0.0;
  inv[7] = 0.0; 
  inv[8] = 0.0; 
  inv[9] = 0.0;

  return TRUE;
}

double calc_error( double *mat, Vec *vec )
{
  return ( mat[0] * vec->x * vec->x +
	   mat[1] * vec->y * vec->y +
	   mat[2] * vec->z * vec->z +
	   2.0 * mat[3] * vec->x * vec->y + 
	   2.0 * mat[4] * vec->x * vec->z + 
	   2.0 * mat[5] * vec->y * vec->z +
	   2.0 * mat[6] * vec->x + 
	   2.0 * mat[7] * vec->y + 
	   2.0 * mat[8] * vec->z +
	   mat[9] );
}

// calculate optimized vector for 2-sub 
// mat[10]
void optimize_2subvector( Sped *ed, double *mat )
{
  // $B5U9TNs$N7W;;(B
  double inv[10];
  Vec  vec;
  display("ed %d sv %g %g %g ev %g %g %g\n",
	  ed->no,
	  ed->sv->vec.x, ed->sv->vec.y, ed->sv->vec.z, 
	  ed->ev->vec.x, ed->ev->vec.y, ed->ev->vec.z );
  
  if ( InvertSymMatrix3( (double *) mat, (double *) inv ) == TRUE ) {

    // optimized vector
    vec.x = -1.0 * (inv[0]*mat[6] + inv[3]*mat[7] + inv[4]*mat[8]);
    vec.y = -1.0 * (inv[3]*mat[6] + inv[1]*mat[7] + inv[5]*mat[8]);
    vec.z = -1.0 * (inv[4]*mat[6] + inv[5]*mat[7] + inv[2]*mat[8]);
    ed->error = calc_error( (double *) mat, &vec );
    ed->ovec.x = vec.x;
    ed->ovec.y = vec.y;
    ed->ovec.z = vec.z;
    display("optimized. %g %g %g\n", vec.x, vec.y, vec.z );
    
  } else { // Inverse failed.

    // $B;OE@(B
    vec.x = ed->sv->vec.x;
    vec.y = ed->sv->vec.y;
    vec.z = ed->sv->vec.z;
    ed->error = calc_error( (double *) mat, &vec );
    
    // $B=*E@(B
    Vec vec1;
    double error;
    vec1.x = ed->ev->vec.x;
    vec1.y = ed->ev->vec.y;
    vec1.z = ed->ev->vec.z;
    error = calc_error( (double *) mat, &vec1 );
    if ( error < ed->error ) {
      ed->error = error;
      vec.x = vec1.x;
      vec.y = vec1.y;
      vec.z = vec1.z;
    }
    
    // $BCf4VE@(B
    vec1.x = (ed->sv->vec.x + ed->ev->vec.x) / 2.0;
    vec1.y = (ed->sv->vec.y + ed->ev->vec.y) / 2.0;
    vec1.z = (ed->sv->vec.z + ed->ev->vec.z) / 2.0;
    error = calc_error( (double *) mat, &vec1 );
    if ( error < ed->error ) {
      ed->error = error;
      vec.x = vec1.x;
      vec.y = vec1.y;
      vec.z = vec1.z;
    }

    ed->ovec.x = vec.x;
    ed->ovec.y = vec.y;
    ed->ovec.z = vec.z;
  }

}

//
// make QEM infinity matrix
//
void make_qeminfmatrix( Sped *ed, double *mat, double *qimat )
{
  unsigned short type = ( (ed->sv->isBoundary == FALSE) ||
			  (ed->ev->isBoundary == FALSE) )
    ? INTERNAL : BOUNDARY;
  //
  // gather 1-ring neighborhood's information
  //
  int kappa = 0;
  Vec eps; eps.x = eps.y = eps.z = 0.0;
  Sped *edj = NULL; Sped *nedj = NULL; Sped *dedj = NULL;
  Spvt *vj = ppdedge_first_ring_vertex( ed, &edj );
  do {
    eps.x += vj->vec.x;
    eps.y += vj->vec.y;
    eps.z += vj->vec.z;
    ++kappa;
    vj = ppdedge_next_ring_vertex( vj, ed, edj, &nedj  );
    edj = nedj;
  } while ( (vj != ppdedge_first_ring_vertex( ed, &dedj )) && (vj != NULL) );

  //
  // alpha eps (Epsilon) $B$N7W;;(B
  //

  // valence $B$r(B double $B7?$K$7$?$b$N(B
  double dval = (double) kappa;
  double beta = calc_beta( kappa );
  double kai = 1.0 / ( (VAL38 / beta) + dval );

  double alpha;
  if ( type == INTERNAL ) {
    // $BFbItD:E@MQ=hM}(B
    alpha = 1.0 - dval * kai;
    eps.x *= kai; eps.y *= kai; eps.z *= kai;
  } else {
    // $B6-3&D:E@MQ=hM}(B
    alpha = 0.6;
    eps.x *= 0.2; eps.y *= 0.2; eps.z *= 0.2;
  }
  
  //
  // QEM Inf Matrix $B$N3JG<(B
  //

  // qimat[0]$B!A(Bqimat[5]: A
  // qimat[6]$B!A(Bqimat[8]: Ae + b
  // qimat[9]          : c+
  // qimat[10]         : alpha
  Vec ae;
  ae.x = mat[0] * eps.x + mat[3] * eps.y + mat[4] * eps.z;
  ae.y = mat[3] * eps.x + mat[1] * eps.y + mat[5] * eps.z;
  ae.z = mat[4] * eps.x + mat[5] * eps.y + mat[2] * eps.z;
  qimat[0] = mat[0];
  qimat[1] = mat[1];
  qimat[2] = mat[2];
  qimat[3] = mat[3];
  qimat[4] = mat[4];
  qimat[5] = mat[5];
  qimat[6] = ae.x + mat[6];
  qimat[7] = ae.y + mat[7];
  qimat[8] = ae.z + mat[8];
  qimat[9] = (eps.x * ae.x + eps.y * ae.y + eps.z * ae.z +
	      2.0 * eps.x * mat[6] +
	      2.0 * eps.y * mat[7] +
	      2.0 * eps.z * mat[8] +
	      mat[9]);
  qimat[10] = alpha;
  
}

//
// calcluate optimized infinity vector
// mat[11]
//
void optimize_infvector( Sped *ed, double *mat )
{
  // $B5U9TNs$N7W;;(B
  double inv[10];
  Vec  vec;
  if ( InvertSymMatrix3( (double *) mat, (double *) inv ) == TRUE ) {

    // optimized vector
    vec.x = -1.0 * (inv[0]*mat[6] + inv[3]*mat[7] + inv[4]*mat[8]) / mat[10];
    vec.y = -1.0 * (inv[3]*mat[6] + inv[1]*mat[7] + inv[5]*mat[8]) / mat[10];
    vec.z = -1.0 * (inv[4]*mat[6] + inv[5]*mat[7] + inv[2]*mat[8]) / mat[10];
    ed->error = calc_inferror( (double *) mat, &vec );
    ed->ovec.x = vec.x;
    ed->ovec.y = vec.y;
    ed->ovec.z = vec.z;
    
  } else { // Inverse failed.

    // $B;OE@(B
    vec.x = ed->sv->vec.x;
    vec.y = ed->sv->vec.y;
    vec.z = ed->sv->vec.z;
    ed->error = calc_inferror( (double *) mat, &vec );

    // $B=*E@(B
    Vec vec1;
    double error;
    vec1.x = ed->ev->vec.x;
    vec1.y = ed->ev->vec.y;
    vec1.z = ed->ev->vec.z;
    error = calc_inferror( (double *) mat, &vec1 );
    if ( error < ed->error ) {
      ed->error = error;
      vec.x = vec1.x;
      vec.y = vec1.y;
      vec.z = vec1.z;
    }

    // $BCf4VE@(B
    vec1.x = (ed->sv->vec.x + ed->ev->vec.x) / 2.0;
    vec1.y = (ed->sv->vec.y + ed->ev->vec.y) / 2.0;
    vec1.z = (ed->sv->vec.z + ed->ev->vec.z) / 2.0;
    error = calc_error( (double *) mat, &vec1 );
    if ( error < ed->error ) {
      ed->error = error;
      vec.x = vec1.x;
      vec.y = vec1.y;
      vec.z = vec1.z;
    }

    ed->ovec.x = vec.x;
    ed->ovec.y = vec.y;
    ed->ovec.z = vec.z;
  }

}

double calc_inferror( double *mat, Vec *vec )
{
  double a = mat[10];
  double a2 = a * a;
  return ( a2 * mat[0] * vec->x * vec->x +
	   a2 * mat[1] * vec->y * vec->y +
	   a2 * mat[2] * vec->z * vec->z +
	   2.0 * a2 * mat[3] * vec->x * vec->y + 
	   2.0 * a2 * mat[4] * vec->x * vec->z + 
	   2.0 * a2 * mat[5] * vec->y * vec->z +
	   2.0 * a * mat[6] * vec->x + 
	   2.0 * a * mat[7] * vec->y + 
	   2.0 * a * mat[8] * vec->z +
	   mat[9] );
}

//
// make 2-subdivision matrix
//
// $B0z?t(B:
// double mat[10]: ed->sv, ed->ev $B$N(B QEM $B$NOB(B
// double qsmat[10]: $B:#2s$ND:E@7W;;MQ$N%^%H%j%/%9(B
//
// $B$3$N4X?t$NCf$G7W;;$7$J$1$l$P$J$i$J$$JQ?t(B
// - double alpha0, Vec eps0: v0 $B$rFs2s:YJ,3d$7$?$H$-$N:BI8CM$K4X$9$k78?t(B
//   (v0sub2 = alpha0 * v0 + eps0)
// - double alpha0j, Vec eps0j: v0j $B$r0l2s:YJ,3d$7$?$H$-$N:BI8CM$K4X$9$k78?t(B
//   (v0jsub1 = alpha0j * v0 + eps0j)
// - double mat0j[10]: mat[10] $B$H(B vj $B$N(B QEM $B$NOB(B
// 
void make_vqem2submatrix( Sped *ed, double *mat, double gamma, double *qsmat )
{
  //
  // $B$^$:0l2s:YJ,3d$N:BI8CM(B (v0s1, v0js1) $B$r7W;;$9$k(B
  //

  int type;
  if ( (ed->sv->isBoundary == TRUE) || (ed->sv->isBoundary == TRUE) ) {
    type = BOUNDARY;
  } else {
    type = INTERNAL;
  }

  double alpha0 = 0.0; Vec eps0;
  if ( type == BOUNDARY ) {
    
    Sped *e1, *e2;
    Spvt *v1 = ppdedge_first_ring_vertex( ed, &e1 );
    Spvt *v2 = ppdedge_last_ring_vertex( ed, &e2 );

    // al0s1$B!$(Beps0s1 $B$N7W;;(B(v0 $B$K4X$9$kCM(B)
    double al0s1 = VAL68; Vec eps0s1;
    eps0s1.x = eps0s1.y = eps0s1.z = 0.0;
    eps0s1.x += ( VAL18 * v1->vec.x );
    eps0s1.y += ( VAL18 * v1->vec.y );
    eps0s1.z += ( VAL18 * v1->vec.z );
    eps0s1.x += ( VAL18 * v2->vec.x );
    eps0s1.y += ( VAL18 * v2->vec.y );
    eps0s1.z += ( VAL18 * v2->vec.z );

    // al0js1, eps0js1 $B$N7W;;(B
    double al0js1[2]; Vec eps0js1[2];
    al0js1[0] = VAL12;
    eps0js1[0].x = VAL12 * v1->vec.x;
    eps0js1[0].y = VAL12 * v1->vec.y;
    eps0js1[0].z = VAL12 * v1->vec.z;
    al0js1[1] = VAL12;
    eps0js1[1].x = VAL12 * v2->vec.x;
    eps0js1[1].y = VAL12 * v2->vec.y;
    eps0js1[1].z = VAL12 * v2->vec.z;

    // alpha0$B!$(Beps0 $B$N7W;;(B
    alpha0 = VAL68 * al0s1 + VAL18 * al0js1[0] + VAL18 * al0js1[1];
    eps0.x = VAL68 * eps0s1.x + VAL18 * eps0js1[0].x + VAL18 * eps0js1[1].x;
    eps0.y = VAL68 * eps0s1.y + VAL18 * eps0js1[0].y + VAL18 * eps0js1[1].y;
    eps0.z = VAL68 * eps0s1.z + VAL18 * eps0js1[0].z + VAL18 * eps0js1[1].z;
    
  } else {
  
    //
    // k0, beta0, al0s1$B!$(Beps0s1 $B$N7W;;(B(v0 $B$K4X$9$kCM(B)
    //
    int k0; double beta0;
    double al0s1; Vec eps0s1;
    loop_even_vertex( ed, &k0, &beta0, &al0s1, &eps0s1, FALSE );
  
    ////////////////////////////////////////////////////////////////////////
    //
    // alpha0$B!$(Beps0 $B$N7W;;(B
    //  (v0sub2 = alpha0 * v0 + eps0)
    //  - eps0js1 $B$N7W;;$b$3$3$G9T$J$&(B
    //   (v0s1 = al0s1 * v0 + eps0s1)
    //
    ////////////////////////////////////////////////////////////////////////

    // eps0 $B$N=i4|2=(B
    eps0.x = eps0.y = eps0.z = 0.0;

    // v0s2 $B$N9`$N78?t7W;;(B (al0s2)
    double dk0 = (double) k0;
    double al0s2 = 1.0 - dk0 * beta0;
    //    display("k0 %d beta0 %g\n", k0, beta0);

    // al0s1 $B$H(B al0s2 $B$r9g$o$;$?$b$N(B
    alpha0 += (al0s1 * al0s2);

    // v0s1 $B$N9`$N78?t7W;;(B (eps0s1)
    eps0.x += (al0s2 * eps0s1.x);
    eps0.y += (al0s2 * eps0s1.y);
    eps0.z += (al0s2 * eps0s1.z);

#ifdef _VQEM_ADD_EVAL

    // vj $B$N3JG<G[Ns$NNN0h3NJ](B ($B8e$G;H$&$?$a(B)
    Spvt **vjarray = (Spvt **) malloc( k0 * sizeof(Spvt *) );
  
    //
    // alpha0j$B!$(Beps0j $B$NNN0h3NJ](B
    //  
    double *alpha0j = (double *) malloc( k0 * sizeof(double) );
    Vec *eps0j = (Vec *) malloc( k0 * sizeof(Vec) );

#endif

    int j = 0;
    Sped *edj = NULL; Sped *nedj = NULL; Sped *dedj = NULL;
    Spvt *vj = ppdedge_first_ring_vertex( ed, &edj );
    do {

      ////////////////////////////////////////////////////////////////////////
      //
      // alpha0$B!$(Beps0 $B$N7W;;$NB3$-(B
      //  (v0sub2 = alpha0 * v0 + eps0)
      //  - eps0js1 $B$N7W;;$b$3$3$G9T$J$&(B
      //   (v0s1 = al0s1 * v0 + eps0s1)
      //
      ////////////////////////////////////////////////////////////////////////

      //
      // v0js1: v_{0j}^{1}
      //
      
      // vj+1 $B$N7W;;(B
      Sped *edjp1 = NULL;
      Spvt *vjp1 = ppdedge_next_ring_vertex( vj, ed, edj, &edjp1  );
    
      // vj-1 $B$N7W;;(B
      Sped *edjm1 = NULL;
      Spvt *vjm1 = ppdedge_prev_ring_vertex( vj, ed, edj, &edjm1  );
    
      // al0js1, eps0js1
      double al0js1; Vec eps0js1;
      loop_odd_vertex( NULL, vj, vjp1, vjm1, 1, &al0js1, &eps0js1 );
    
      // v0s2 $B$N78?t$r3]$1$k(B
      alpha0 += (al0js1 * beta0);
      eps0.x += (eps0js1.x * beta0);
      eps0.y += (eps0js1.y * beta0);
      eps0.z += (eps0js1.z * beta0);

#ifdef _VQEM_ADD_EVAL
    
      // vj $B$N3JG<(B
      vjarray[j] = vj;
    
      ////////////////////////////////////////////////////////////////////////
      //
      // alpha0j$B!$(Beps0j $B$N7W;;(B
      //  (v0jsub2 = alpha0j * v0 + eps0j)
      //
      ////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////
      // v0js1: v_{0j}^{1}
      // $B>e$G7W;;$7$?(B al0js1, eps0js1 $B$r;H$&(B
      //

      //////////////////////////////////////////////////////////////////////
      // v0s1: v_{0}^{1}
      // $B>e$G7W;;$7$?(B al0s1, eps0s1 $B$r;H$&(B
      //
    
      //////////////////////////////////////////////////////////////////////
      // vjs1:  v_{j}^{1}
      // aljs1, epsjs1
      // $B$3$l$O$A$H$a$s$I$/$5$$(B
      //

      double aljs1; Vec epsjs1;
      if ( vj->isBoundary == TRUE ) {

	// $B6-3&$N(B even vertex
	// v0 $B$O<0$NCf$KF~$i$J$$(B
	aljs1 = 0.0;
	epsjs1.x = VAL68 * vj->vec.x + VAL18 * (vjp1->vec.x + vjm1->vec.x);
	epsjs1.y = VAL68 * vj->vec.y + VAL18 * (vjp1->vec.y + vjm1->vec.y);
	epsjs1.z = VAL68 * vj->vec.z + VAL18 * (vjp1->vec.z + vjm1->vec.z);
	
      } else {
	
	int kj; double betaj;
	loop_even_vertex_vj( vj, ed, &kj, &betaj, &aljs1, &epsjs1 );
	
      }

      //////////////////////////////////////////////////////////////////////
      // v0jp1s1: v_{0j+1}^{1}
      // v0, vjp1, vj, vjp2 (vj, vjp1 $B$O$9$G$K>e$G7W;;$7$F$$$k(B)
      //
    
      // vj+2 $B$N7W;;(B
      Sped *edjp2 = NULL;
      Spvt *vjp2 = ppdedge_next_ring_vertex( vjp1, ed, edjp1, &edjp2 );
    
      // al0jp1s1, eps0jp1s1
      double al0jp1s1; Vec eps0jp1s1;
      loop_odd_vertex( NULL, vjp1, vj, vjp2, 1, &al0jp1s1, &eps0jp1s1 );

      //////////////////////////////////////////////////////////////////////
      // v0jm1s1: v_{0j-1}^{1}
      // v0, vjm1, vj, vjm2 (vj, vjm1 $B$O$9$G$K>e$G7W;;$7$F$$$k(B)
      //

      // vj-2 $B$N7W;;(B
      Sped *edjm2 = NULL;
      Spvt *vjm2 = ppdedge_prev_ring_vertex( vjm1, ed, edjm1, &edjm2 );
    
      // al0jm1s1, eps0jm1s1
      double al0jm1s1; Vec eps0jm1s1;
      loop_odd_vertex( NULL, vjm1, vj, vjm2, 1, &al0jm1s1, &eps0jm1s1 );
    
      //////////////////////////////////////////////////////////////////////
      // vjjp1s1: v_{jj+1}^{1}
      // v0, vjp1, vj, v0jjp1 (vj, vjp1 $B$O$9$G$K>e$G7W;;$7$F$$$k(B)
      //

      // aljjp1s1, epsjjp1s1
      double aljjp1s1; Vec epsjjp1s1;
      if ( (vj->isBoundary == TRUE) && (vjp1->isBoundary == TRUE) ) {

	// $B6-3&$N(B odd vertex
	// v0 $B$O<0$NCf$KF~$i$J$$(B
	aljjp1s1 = 0.0;
	epsjjp1s1.x = VAL12 * ( vj->vec.x + vjp1->vec.x );
	epsjjp1s1.y = VAL12 * ( vj->vec.y + vjp1->vec.y );
	epsjjp1s1.z = VAL12 * ( vj->vec.z + vjp1->vec.z );
	
      } else {
	
	// v0jj+1 $B$N7W;;(B
	Spvt *v0jjp1 = ppdedge_opposite_vertex( ed, vj, vjp1 );
	assert( v0jjp1 != NULL );
	// aljjp1s1, epsjjp1s1
	loop_odd_vertex( vj, vjp1, NULL, v0jjp1, 3, &aljjp1s1, &epsjjp1s1 );
	
      }
    
      //////////////////////////////////////////////////////////////////////
      // vjjm1s1: v_{jj-1}^{1}
      // v0, vjm1, vj, v0jjm1 (vj, vjp1 $B$O$9$G$K>e$G7W;;$7$F$$$k(B)
      //

      // aljjm1s1, epsjjm1s1
      double aljjm1s1; Vec epsjjm1s1;
      if ( (vj->isBoundary == TRUE) && (vjm1->isBoundary == TRUE) ) {
      
	// $B6-3&$N(B odd vertex
	// v0 $B$O<0$NCf$KF~$i$J$$(B
	aljjm1s1 = 0.0;
	epsjjm1s1.x = VAL12 * ( vj->vec.x + vjm1->vec.x );
	epsjjm1s1.y = VAL12 * ( vj->vec.y + vjm1->vec.y );
	epsjjm1s1.z = VAL12 * ( vj->vec.z + vjm1->vec.z );

      } else {
	
	// v0jj-1 $B$N7W;;(B
	Spvt *v0jjm1 = ppdedge_opposite_vertex( ed, vj, vjm1 );
	assert( v0jjm1 != NULL );
	loop_odd_vertex( vj, vjm1, NULL, v0jjm1, 3, &aljjm1s1, &epsjjm1s1 );

      }
    
      //
      // $B$^$H$a(B
      //

      // $BFbItD:E@$N(B ordinary even vertex $B$N7W;;$KAjEv(B
      // alpha0j
      alpha0j[j] = 0.0;
      alpha0j[j] += (( 10.0 * al0js1
		       + al0s1
		       + aljs1
		       + al0jp1s1
		       + al0jm1s1
		       + aljjp1s1
		       + aljjm1s1 ) / 16.0);

      // eps0j
      eps0j[j].x = eps0j[j].y = eps0j[j].z = 0.0;
      eps0j[j].x += (( 10.0 * eps0js1.x
		       + eps0s1.x
		       + epsjs1.x
		       + eps0jp1s1.x
		       + eps0jm1s1.x
		       + epsjjp1s1.x
		       + epsjjm1s1.x ) / 16.0);
      eps0j[j].y += (( 10.0 * eps0js1.y
		       + eps0s1.y
		       + epsjs1.y
		       + eps0jp1s1.y
		       + eps0jm1s1.y
		       + epsjjp1s1.y
		       + epsjjm1s1.y ) / 16.0);
      eps0j[j].z += (( 10.0 * eps0js1.z
		       + eps0s1.z
		       + epsjs1.z
		       + eps0jp1s1.z
		       + eps0jm1s1.z
		       + epsjjp1s1.z
		       + epsjjm1s1.z ) / 16.0);
    
#endif
    
      //
      // $B<!$NNY@\D:E@(B
      //
      vj = ppdedge_next_ring_vertex( vj, ed, edj, &nedj  );
      edj = nedj;
      ++j;
    
    } while ( (vj != ppdedge_first_ring_vertex( ed, &dedj )) && (vj != NULL) );
    
  }
  //////////////////////////////////////////////////////////////////////
  //
  // QEM 2sub Matrix $B$N3JG<(B
  //
  //////////////////////////////////////////////////////////////////////
  
  
  // $B78?t(B
  double coeff0  = 1.0;

#ifdef _VQEM_ADD_EVAL

  double coeff0j = gamma;

#endif

  // qsmat $B$N=i4|2=(B
  initialize_matrix4( (double *) qsmat );
  //
  // A_{0} $B$N9`$NOB;;(B
  //

  // mat0 -> mat
  double qsmatA0[10];
  qem_matrix4( alpha0, &eps0, (double *) mat, (double *) qsmatA0 );
  multivalue_matrix4( (double *) qsmatA0, coeff0, (double *) qsmatA0 );
  add_matrix4( (double *) qsmat, (double *) qsmatA0, (double *) qsmat );
  
#ifdef _VQEM_ADD_EVAL

  //
  // A_{0j} $B$N9`$NOB;;(B
  //
  for ( j = 0; j < k0; ++j ) {

    //  mat0j[j][10]: mat[10] $B$H(B vj $B$N(B QEM$B$H$NOB;;(B
    //
    double mat0j[10];
    add_matrix4( (double *) mat, (double *) vjarray[j]->mat, (double *) mat0j );

    // A_{0j} $B$N9`$N%^%H%j%/%9$NOB;;(B
    double qsmatA0j[10];
    qem_matrix4( alpha0j[j], &(eps0j[j]), (double *) mat0j, (double *) qsmatA0j );
    multivalue_matrix4( (double *) qsmatA0j, coeff0j, (double *) qsmatA0j );
    add_matrix4( (double *) qsmat, (double *) qsmatA0j, (double *) qsmat );
    
  }
  
  free( vjarray );
  free( alpha0j );
  free( eps0j );

#endif
  
}

////////////////////////////////////////////////////////////////////////
//
// $B%(%C%8%Y!<%9$N(B QEM $B$N7W;;!$:GE,2=:BI8$N7W;;(B
//
////////////////////////////////////////////////////////////////////////

//
// make 2-subdivision edge QEM matrix
//
// $B0z?t(B:
// double qsmat[10]: $B:#2s$ND:E@7W;;MQ$N(B edge-based QEM matrix
//
// $B$3$N4X?t$NCf$G7W;;$7$J$1$l$P$J$i$J$$JQ?t(B
// - double alpha0, Vec eps0: v0 $B$rFs2s:YJ,3d$7$?$H$-$N:BI8CM$K4X$9$k78?t(B
//   (v0sub2 = alpha0 * v0 + eps0)
// - double alpha0j, Vec eps0j: v0j $B$r0l2s:YJ,3d$7$?$H$-$N:BI8CM$K4X$9$k78?t(B
//   (v0jsub1 = alpha0j * v0 + eps0j)
// - double mat0[10]: ej $B$N(B QEM $B$NOB(B
// - double mat0j[10]: ej $B$N(B QEM
//
#define TWO 2

void make_eqem2submatrix( Sped *ed, double gamma, double *qsmat )
{
  //
  // $B$^$:0l2s:YJ,3d$N:BI8CM(B (v0s1, v0js1) $B$r7W;;$9$k(B
  //

  int type;
  if ( (ed->sv->isBoundary == TRUE) || (ed->sv->isBoundary == TRUE) ) {
    type = BOUNDARY;
  } else {
    type = INTERNAL;
  }

  // used if type == INTERNAL
  int k0 = 0; int j = 0;
  Sped *edj = NULL;
  Spvt *vj = NULL;
  double *alpha0j = NULL;
  Vec *eps0j = NULL;
  double **mat0j = NULL;
  Spvt **vjarray = NULL;

  // used for only BOUNDARY
  Sped *bedj[TWO];
  
  double alpha0 = 0.0; Vec eps0;
  if ( type == BOUNDARY ) {
    
    Sped *e1, *e2;
    Spvt *v1 = ppdedge_first_ring_vertex( ed, &e1 );
    Spvt *v2 = ppdedge_last_ring_vertex( ed, &e2 );
    bedj[0] = e1; bedj[1] = e2;

    // al0s1$B!$(Beps0s1 $B$N7W;;(B(v0 $B$K4X$9$kCM(B)
    double al0s1 = VAL68; Vec eps0s1;
    eps0s1.x = eps0s1.y = eps0s1.z = 0.0;
    eps0s1.x += ( VAL18 * v1->vec.x );
    eps0s1.y += ( VAL18 * v1->vec.y );
    eps0s1.z += ( VAL18 * v1->vec.z );
    eps0s1.x += ( VAL18 * v2->vec.x );
    eps0s1.y += ( VAL18 * v2->vec.y );
    eps0s1.z += ( VAL18 * v2->vec.z );

    // al0js1, eps0js1 $B$N7W;;(B
    double al0js1[TWO]; Vec eps0js1[TWO];
    al0js1[0] = VAL12;
    eps0js1[0].x = VAL12 * v1->vec.x;
    eps0js1[0].y = VAL12 * v1->vec.y;
    eps0js1[0].z = VAL12 * v1->vec.z;
    al0js1[1] = VAL12;
    eps0js1[1].x = VAL12 * v2->vec.x;
    eps0js1[1].y = VAL12 * v2->vec.y;
    eps0js1[1].z = VAL12 * v2->vec.z;

    // alpha0$B!$(Beps0 $B$N7W;;(B
    alpha0 = VAL68 * al0s1 + VAL18 * al0js1[0] + VAL18 * al0js1[1];
    eps0.x = VAL68 * eps0s1.x + VAL18 * eps0js1[0].x + VAL18 * eps0js1[1].x;
    eps0.y = VAL68 * eps0s1.y + VAL18 * eps0js1[0].y + VAL18 * eps0js1[1].y;
    eps0.z = VAL68 * eps0s1.z + VAL18 * eps0js1[0].z + VAL18 * eps0js1[1].z;

    // aljs1, epsj01 $B$N7W;;(B
    // $B6-3&$NNY@\E@$N;;=P(B
    double aljs1[TWO]; Vec epsjs1[TWO];
    Spvt *v1a = ppdvertex_another_boundaryvertex( v1, another_vt(e1, v1) );
    assert( v1a != NULL );
    aljs1[0] = VAL18;
    epsjs1[0].x = VAL68 * v1->vec.x + VAL18 * v1a->vec.x;
    epsjs1[0].y = VAL68 * v1->vec.y + VAL18 * v1a->vec.y;
    epsjs1[0].z = VAL68 * v1->vec.z + VAL18 * v1a->vec.z;
    Spvt *v2a = ppdvertex_another_boundaryvertex( v2, another_vt(e2, v2) );
    assert( v2a != NULL );
    aljs1[1] = VAL18;
    epsjs1[1].x = VAL68 * v2->vec.x + VAL18 * v2a->vec.x;
    epsjs1[1].y = VAL68 * v2->vec.y + VAL18 * v2a->vec.y;
    epsjs1[1].z = VAL68 * v2->vec.z + VAL18 * v2a->vec.z;

    // alpha0j, eps0j $B$N7W;;(B
    alpha0j = (double *) malloc( TWO * sizeof(double) );
    eps0j = (Vec *) malloc( TWO * sizeof(Vec) );
    
    alpha0j[0] = VAL68 * al0js1[0] + VAL18 * al0s1 + VAL18 * aljs1[0];
    eps0j[0].x = VAL68 * eps0js1[0].x + VAL18 * eps0s1.x + VAL18 * epsjs1[0].x;
    eps0j[0].y = VAL68 * eps0js1[0].y + VAL18 * eps0s1.y + VAL18 * epsjs1[0].y;
    eps0j[0].z = VAL68 * eps0js1[0].z + VAL18 * eps0s1.z + VAL18 * epsjs1[0].z;
    alpha0j[1] = VAL68 * al0js1[1] + VAL18 * al0s1 + VAL18 * aljs1[1];
    eps0j[1].x = VAL68 * eps0js1[1].x + VAL18 * eps0s1.x + VAL18 * epsjs1[1].x;
    eps0j[1].y = VAL68 * eps0js1[1].y + VAL18 * eps0s1.y + VAL18 * epsjs1[1].y;
    eps0j[1].z = VAL68 * eps0js1[1].z + VAL18 * eps0s1.z + VAL18 * epsjs1[1].z;
//      display("alpha0j[0] %g alpha0j[1] %g\n", alpha0j[0], alpha0j[1] );
    
  } else {
    
    //
    // k0, beta0, al0s1$B!$(Beps0s1 $B$N7W;;(B(v0 $B$K4X$9$kCM(B)
    //
    double beta0;
    double al0s1; Vec eps0s1;
    loop_even_vertex( ed, &k0, &beta0, &al0s1, &eps0s1, TRUE );
		   
    ////////////////////////////////////////////////////////////////////////
    //
    // alpha0$B!$(Beps0 $B$N7W;;(B
    //  (v0sub2 = alpha0 * v0 + eps0)
    //  - eps0js1 $B$N7W;;$b$3$3$G9T$J$&(B
    //   (v0s1 = al0s1 * v0 + eps0s1)
    //
    ////////////////////////////////////////////////////////////////////////

    // eps0 $B$N=i4|2=(B
    eps0.x = eps0.y = eps0.z = 0.0;

    // v0s2 $B$N9`$N78?t7W;;(B (al0s2)
    double dk0 = (double) k0;
    double al0s2 = 1.0 - dk0 * beta0;

  // al0s1 $B$H(B al0s2 $B$r9g$o$;$?$b$N(B
    alpha0 += (al0s1 * al0s2);

    // v0s1 $B$N9`$N78?t7W;;(B (eps0s1)
    eps0.x += (al0s2 * eps0s1.x);
    eps0.y += (al0s2 * eps0s1.y);
    eps0.z += (al0s2 * eps0s1.z);

    //
    // alpha0j$B!$(Beps0j $B$NNN0h3NJ](B
    //  
    alpha0j = (double *) malloc( k0 * sizeof(double) );
    eps0j = (Vec *) malloc( k0 * sizeof(Vec) );

  // vj $B$N3JG<G[Ns$NNN0h3NJ](B ($B8e$G;H$&$?$a(B)
    vjarray = (Spvt **) malloc( k0 * sizeof(Spvt *) );
  
    j = 0;
    edj = NULL; Sped *nedj = NULL; Sped *dedj = NULL;
    vj = ppdedge_first_ring_vertex( ed, &edj );
    do {

      // vj $B$N3JG<(B
      vjarray[j] = vj;
    
      ////////////////////////////////////////////////////////////////////////
      //
      // alpha0$B!$(Beps0 $B$N7W;;$NB3$-(B
      //  (v0sub2 = alpha0 * v0 + eps0)
      //  - eps0js1 $B$N7W;;$b$3$3$G9T$J$&(B
      //   (v0s1 = al0s1 * v0 + eps0s1)
      //
      ////////////////////////////////////////////////////////////////////////

      //
      // v0js1: v_{0j}^{1}
      //
    
      // vj+1 $B$N7W;;(B
      Sped *edjp1 = NULL;
      Spvt *vjp1 = ppdedge_next_ring_vertex( vj, ed, edj, &edjp1  );
      // vj-1 $B$N7W;;(B
      Sped *edjm1 = NULL;
      Spvt *vjm1 = ppdedge_prev_ring_vertex( vj, ed, edj, &edjm1  );
    
      // al0js1, eps0js1
      double al0js1; Vec eps0js1;
      loop_odd_vertex( NULL, vj, vjp1, vjm1, 1, &al0js1, &eps0js1 );
    
      // v0s2 $B$N78?t$r3]$1$k(B
      alpha0 += (al0js1 * beta0);
      eps0.x += (eps0js1.x * beta0);
      eps0.y += (eps0js1.y * beta0);
      eps0.z += (eps0js1.z * beta0);

      ////////////////////////////////////////////////////////////////////////
      //
      // alpha0j$B!$(Beps0j $B$N7W;;(B
      //  (v0jsub2 = alpha0j * v0 + eps0j)
      //
      ////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////
      // v0js1: v_{0j}^{1}
      // $B>e$G7W;;$7$?(B al0js1, eps0js1 $B$r;H$&(B
      //

      //////////////////////////////////////////////////////////////////////
      // v0s1: v_{0}^{1}
      // $B>e$G7W;;$7$?(B al0s1, eps0s1 $B$r;H$&(B
      //
    
      //////////////////////////////////////////////////////////////////////
      // vjs1:  v_{j}^{1}
      // aljs1, epsjs1
      // $B$3$l$O$A$H$a$s$I$/$5$$(B
      //

      double aljs1; Vec epsjs1;
      if ( vj->isBoundary == TRUE ) {

	// $B6-3&$N(B even vertex
	// v0 $B$O<0$NCf$KF~$i$J$$(B
	aljs1 = 0.0;
	epsjs1.x = VAL68 * vj->vec.x + VAL18 * (vjp1->vec.x + vjm1->vec.x);
	epsjs1.y = VAL68 * vj->vec.y + VAL18 * (vjp1->vec.y + vjm1->vec.y);
	epsjs1.z = VAL68 * vj->vec.z + VAL18 * (vjp1->vec.z + vjm1->vec.z);
	
      } else {
	
	int kj; double betaj;
	loop_even_vertex_vj( vj, ed, &kj, &betaj, &aljs1, &epsjs1 );
//  	display("kj %d aljs1 %g betaj %g\n", kj, aljs1, betaj );
	
      }

      //////////////////////////////////////////////////////////////////////
      // v0jp1s1: v_{0j+1}^{1}
      // v0, vjp1, vj, vjp2 (vj, vjp1 $B$O$9$G$K>e$G7W;;$7$F$$$k(B)
      //
    
      // vj+2 $B$N7W;;(B
      Sped *edjp2 = NULL;
      Spvt *vjp2 = ppdedge_next_ring_vertex( vjp1, ed, edjp1, &edjp2 );
    
      // al0jp1s1, eps0jp1s1
      double al0jp1s1; Vec eps0jp1s1;
      loop_odd_vertex( NULL, vjp1, vj, vjp2, 1, &al0jp1s1, &eps0jp1s1 );

      //////////////////////////////////////////////////////////////////////
      // v0jm1s1: v_{0j-1}^{1}
      // v0, vjm1, vj, vjm2 (vj, vjm1 $B$O$9$G$K>e$G7W;;$7$F$$$k(B)
    
      // vj-2 $B$N7W;;(B
      Sped *edjm2 = NULL;
      Spvt *vjm2 = ppdedge_prev_ring_vertex( vjm1, ed, edjm1, &edjm2 );
    
      // al0jm1s1, eps0jm1s1
      double al0jm1s1; Vec eps0jm1s1;
      loop_odd_vertex( NULL, vjm1, vj, vjm2, 1, &al0jm1s1, &eps0jm1s1 );
    
      //////////////////////////////////////////////////////////////////////
      // vjjp1s1: v_{jj+1}^{1}
      // v0, vjp1, vj, v0jjp1 (vj, vjp1 $B$O$9$G$K>e$G7W;;$7$F$$$k(B)
      //
    
      // aljjp1s1, epsjjp1s1
      double aljjp1s1; Vec epsjjp1s1;
      if ( (vj->isBoundary == TRUE) && (vjp1->isBoundary == TRUE) ) {

	// $B6-3&$N(B odd vertex
	// v0 $B$O<0$NCf$KF~$i$J$$(B
	aljjp1s1 = 0.0;
	epsjjp1s1.x = VAL12 * ( vj->vec.x + vjp1->vec.x );
	epsjjp1s1.y = VAL12 * ( vj->vec.y + vjp1->vec.y );
	epsjjp1s1.z = VAL12 * ( vj->vec.z + vjp1->vec.z );
	
      } else {
	
	// v0jj+1 $B$N7W;;(B
	Spvt *v0jjp1 = ppdedge_opposite_vertex( ed, vj, vjp1 );
	assert( v0jjp1 != NULL );
	// aljjp1s1, epsjjp1s1
	loop_odd_vertex( vj, vjp1, NULL, v0jjp1, 3, &aljjp1s1, &epsjjp1s1 );
	
      }
    
      //////////////////////////////////////////////////////////////////////
      // vjjm1s1: v_{jj-1}^{1}
      // v0, vjm1, vj, v0jjm1 (vj, vjm1 $B$O$9$G$K>e$G7W;;$7$F$$$k(B)
      //


      // aljjm1s1, epsjjm1s1
      double aljjm1s1; Vec epsjjm1s1;
      if ( (vj->isBoundary == TRUE) && (vjm1->isBoundary == TRUE) ) {
      
	// $B6-3&$N(B odd vertex
	// v0 $B$O<0$NCf$KF~$i$J$$(B
	aljjm1s1 = 0.0;
	epsjjm1s1.x = VAL12 * ( vj->vec.x + vjm1->vec.x );
	epsjjm1s1.y = VAL12 * ( vj->vec.y + vjm1->vec.y );
	epsjjm1s1.z = VAL12 * ( vj->vec.z + vjm1->vec.z );

      } else {      // v0jj-1 $B$N7W;;(B
	
	Spvt *v0jjm1 = ppdedge_opposite_vertex( ed, vj, vjm1 );
	assert( v0jjm1 != NULL );
	// aljjm1s1, epsjjm1s1
	loop_odd_vertex( vj, vjm1, NULL, v0jjm1, 3, &aljjm1s1, &epsjjm1s1 );

      }
    
      //
      // $B$^$H$a(B
      //
    
      // alpha0j
      alpha0j[j] = 0.0;
      alpha0j[j] += (( 10.0 * al0js1
		       + al0s1
		       + aljs1
		       + al0jp1s1
		       + al0jm1s1
		       + aljjp1s1
		       + aljjm1s1 ) / 16.0);
//        display("\talpha v0 %g vj %g v0j %g\n", al0s1, aljs1, al0js1 );
//        display("\talpha v0j+1 %g v0j-1 %g vjj+1 %g vjj-1 %g\n",
//  	      al0jp1s1, al0jm1s1, aljjp1s1, aljjm1s1 );

      // eps0j
      eps0j[j].x = eps0j[j].y = eps0j[j].z = 0.0;
      eps0j[j].x += (( 10.0 * eps0js1.x
		       + eps0s1.x
		       + epsjs1.x
		       + eps0jp1s1.x
		       + eps0jm1s1.x
		       + epsjjp1s1.x
		       + epsjjm1s1.x ) / 16.0);
      eps0j[j].y += (( 10.0 * eps0js1.y
		       + eps0s1.y
		       + epsjs1.y
		       + eps0jp1s1.y
		       + eps0jm1s1.y
		       + epsjjp1s1.y
		       + epsjjm1s1.y ) / 16.0);
      eps0j[j].z += (( 10.0 * eps0js1.z
		       + eps0s1.z
		       + epsjs1.z
		       + eps0jp1s1.z
		       + eps0jm1s1.z
		       + epsjjp1s1.z
		       + epsjjm1s1.z ) / 16.0);
    
      //      display("\teps v0 %g %g %g vj %g %g %g v0j %g %g %g\n",
      //  	    eps0s1.x, eps0s1.y, eps0s1.z, 
      //  	    epsjs1.x, epsjs1.y, epsjs1.z, 
      //  	    eps0js1.x, eps0js1.y, eps0js1.z );
      //      display("\teps v0j+1 %g %g %g v0j-1 %g %g %g\n",
      //  	    eps0jp1s1.x, eps0jp1s1.y, eps0jp1s1.z, 
      //  	    eps0jm1s1.x, eps0jm1s1.y, eps0jm1s1.z );
      //      display("\teps vjj+1 %g %g %g vjj-1 %g %g %g\n",
      //  	    epsjjp1s1.x, epsjjp1s1.y, epsjjp1s1.z, 
      //  	    epsjjm1s1.x, epsjjm1s1.y, epsjjm1s1.z );

      //
      // $B<!$NNY@\D:E@(B
      //
      vj = ppdedge_next_ring_vertex( vj, ed, edj, &nedj  );
      edj = nedj;
      ++j;
    
    } while ( (vj != ppdedge_first_ring_vertex( ed, &dedj )) && (vj != NULL) );
  }

  //
  // v0 , v0j $B$K$D$+$&%^%H%j%/%9$N7W;;(B
  //
  double mat0[10];
  initialize_matrix4( (double *) mat0 );

//  #ifdef _HYBRID_QEM
  // ed $B$N(B QEM $B$r(B mat0 $B$K2C;;(B
  add_matrix4( (double *) mat0, (double *) ed->mat, (double *) mat0 );
  // ed->sv, ed->ev $B$N(B QEM $B$r(B mat0 $B$K2C;;(B
  add_matrix4( (double *) mat0, (double *) ed->sv->mat, (double *) mat0 );
  add_matrix4( (double *) mat0, (double *) ed->ev->mat, (double *) mat0 );
//  #endif //_HYBRID_QEM
  
  if ( type == INTERNAL ) {
    
    mat0j = (double **) malloc( k0 * sizeof(double *) );
    for ( j = 0; j < k0; ++j ) {
      mat0j[j] = (double *) malloc( 10 * sizeof(double) );
      initialize_matrix4( (double *) mat0j[j] );
    }

    // ed $B$N6aK5$N%(%C%8$N(B mat $B$r=8$a$k(B
    edj = ppdedge_first_ring_edge( ed );
    do {
      vj = ppdedge_ring_another_vt( edj, ed );

      // v0 $B$N%^%H%j%/%9$NOB;;(B
//        if ( global_ppd->vn > (int) (global_ppd->nvt_org / 10) ) {
//  	add_matrix4( (double *) mat0, (double *) edj->mat, (double *) mat0 );
//        }
      
      // v0j $B$N%^%H%j%/%9$NOB;;(B
      add_matrix4( (double *) mat0j[vj->sid], (double *) edj->mat,
		   (double *) mat0j[vj->sid] );
    
      edj = ppdedge_next_ring_edge( edj, ed );
    } while ( (edj != ppdedge_first_ring_edge( ed )) && (edj != NULL) );
  
    //multivalue_matrix4( (double *) mat0, 0.5, (double *) mat0 );

  } else { // type == BOUNDARY

//      display("aa\n");
    mat0j = (double **) malloc( TWO * sizeof(double *) ); // $BFs$D$J$N$G(B
    for ( j = 0; j < TWO; ++j ) {
      mat0j[j] = (double *) malloc( 10 * sizeof(double) );
      initialize_matrix4( (double *) mat0j[j] );
//        display("edj %d boundary %d\n", bedj[j]->no, bedj[j]->isBoundary );
      add_matrix4( (double *) mat0j[j], (double *) bedj[j]->mat,
		   (double *) mat0j[j] );
    }
//      display("bb\n");
    
  }

  //////////////////////////////////////////////////////////////////////
  //
  // QEM 2sub Matrix (qsmat) $B$N3JG<(B
  //
  //////////////////////////////////////////////////////////////////////
  
  initialize_matrix4( (double *) qsmat );
  
  // A_{0} $B$N9`$NOB;;(B
  double qsmatA0[10];
  qem_matrix4( alpha0, &eps0, (double *) mat0, (double *) qsmatA0 );
  add_matrix4( (double *) qsmat, (double *) qsmatA0, (double *) qsmat );

  if ( type == INTERNAL ) {
    
    // A_{0j} $B$N9`$NOB;;(B
    for ( j = 0; j < k0; ++j ) {

      // A_{0j} $B$N9`$N%^%H%j%/%9$NOB;;(B
      double qsmatA0j[10];
      qem_matrix4( alpha0j[j], &(eps0j[j]), (double *) mat0j[j], (double *) qsmatA0j );
      add_matrix4( (double *) qsmat, (double *) qsmatA0j, (double *) qsmat );
    
    }
    
    // free $B$J$I$N8e;OKv(B
    for ( j = 0; j < k0; ++j ) {

      // sid $B$N(B id $B$N8e;OKv(B
      vjarray[j]->sid = SMDNULL;

      free( mat0j[j] );
    }

    free( vjarray );
    free( mat0j );
    free( alpha0j );
    free( eps0j );
    
  } else { // type == BOUNDARY

    // A_{0j} $B$N9`$NOB;;(B    
    for ( j = 0; j < TWO; ++j ) {
      display("j %d\n", j );
      // A_{0j} $B$N9`$N%^%H%j%/%9$NOB;;(B
      double qsmatA0j[10];
      qem_matrix4( alpha0j[j], &(eps0j[j]), (double *) mat0j[j], (double *) qsmatA0j );
      add_matrix4( (double *) qsmat, (double *) qsmatA0j, (double *) qsmat );
    
    }
  
    for ( j = 0; j < TWO; ++j ) {
      free( mat0j[j] );
    }
    free( mat0j );
    free( alpha0j );
    free( eps0j );

  }
}

//
// loop scheme $B$N(B odd vertex $B$N7W;;(B
//   (for make_eqem2submatrix, make_vqem2submatrix)
//
// alpha (v0 $B$N78?t(B), eps ($B;D$j$N%Y%/%H%k(B) $B$r5a$a$k(B
// int v0: v0 $B$,(B v1$B!A(Bv4 $B$N$&$A$N$I$ND:E@$G$"$k$+$r;XDj$9$k(B
//
void loop_odd_vertex( Spvt *v1, Spvt *v2, Spvt *v3, Spvt *v4, int v0,
		      double *alpha, Vec *eps )
{
  // alpha $B$N7W;;(B
  if ( (v0 == 1) || (v0 == 2) ) *alpha = VAL38;
  else *alpha = VAL18;

  // eps $B$N7W;;(B
  eps->x = eps->y = eps->z = 0.0;

  if ( v1 != NULL ) {
    eps->x += (VAL38 * v1->vec.x);
    eps->y += (VAL38 * v1->vec.y);
    eps->z += (VAL38 * v1->vec.z);
  } else {
    assert( v0 == 1 );
  }

  if ( v2 != NULL ) {
    eps->x += (VAL38 * v2->vec.x);
    eps->y += (VAL38 * v2->vec.y);
    eps->z += (VAL38 * v2->vec.z);
  } else {
    assert( v0 == 2 );
  }
    
  if ( v3 != NULL ) {
    eps->x += (VAL18 * v3->vec.x);
    eps->y += (VAL18 * v3->vec.y);
    eps->z += (VAL18 * v3->vec.z);
  } else {
    assert( v0 == 3 );
  }

  if ( v4 != NULL ) {
    eps->x += (VAL18 * v4->vec.x);
    eps->y += (VAL18 * v4->vec.y);
    eps->z += (VAL18 * v4->vec.z);
  } else {
    assert( v0 == 4 );
  }
}

//
// loop scheme $B$N(B even vertex $B$N7W;;(B $B$=$N(B1
//   (for make_eqem2submatrix, make_vqem2submatrix)
// v0 $B$N:YJ,3dE@$r5a$a$k$?$a$N4X?t(B
//
// alpha (v0 $B$N78?t(B), eps ($B;D$j$N%Y%/%H%k(B) $B$r5a$a$k(B
//
// isStoreID: $BNY@\D:E@$K(B id (v->sid) $B$r3d$jEv$F$k$+$I$&$+$N%U%i%C%0(B
//
void loop_even_vertex( Sped *ed, int *k, double *beta, double *alpha,
		       Vec *eps, BOOL isStoreID )
{

  // eps $B$N=i4|2=(B
  eps->x = eps->y = eps->z = 0.0;
  
  *k = 0;
  Sped *edj = NULL; Sped *nedj = NULL; Sped *dedj = NULL;
  Spvt *vj = ppdedge_first_ring_vertex( ed, &edj );
  do {

    // sid $B$X$N(B id $B$N3JG<(B
    if ( isStoreID == TRUE ) {
      vj->sid = *k;
    }
    
    // eps0 $B$X$NCM$N3JG<(B
    eps->x += vj->vec.x;
    eps->y += vj->vec.y;
    eps->z += vj->vec.z;

    ++(*k);
    
    vj = ppdedge_next_ring_vertex( vj, ed, edj, &nedj  );
    edj = nedj;
  } while ( (vj != ppdedge_first_ring_vertex( ed, &dedj )) && (vj != NULL) );
  
  //
  // k0, beta0 $B$N7W;;(B (v0 $B$K4X$9$kCM(B)
  //
  double dk = (double) (*k);
  *beta = calc_beta( *k );

  //
  // alpha$B!$(Beps $B$N7W;;(B
  //   (v0s1 = al0s1 * v0 + eps0s1)
  //
  *alpha = 1.0 - dk * (*beta);

  eps->x *= (*beta);
  eps->y *= (*beta);
  eps->z *= (*beta);
  
}

//
// loop scheme $B$N(B even vertex $B$N7W;;(B $B$=$N(B2
//   (for make_eqem2submatrix, make_vqem2submatrix)
// v0 $B$NNY@\D:E@(B vj $B$N:YJ,3dE@$r5a$a$k$?$a$N4X?t(B
//
// alpha (v0 $B$N78?t(B), eps ($B;D$j$N%Y%/%H%k(B) $B$r5a$a$k(B
//
void loop_even_vertex_vj( Spvt *vj, Sped *ed, int *k, double *beta, double *alpha,
			  Vec *eps )
{
  // eps $B$N=i4|2=(B
  eps->x = eps->y = eps->z = 0.0;

  // k $B$N=i4|2=(B
  *k = 0;
  
  //
  // vj $B$N(B 1-ring $B6aK5D:E@$NC5:w(B
  //
  // vk $B$,(B ed->sv, ed->ev $B$H0lCW$9$k$H$-$N?t$r?t$($k%U%i%C%0(B
  int vkcount = 0; 
  // vj $B$NNY@\%(%C%8$rC)$k(B
  Sped *edk = ppdvertex_first_edge( vj );
  do {

    Spvt *vk = another_vt( edk, vj );
    // vk = ed->sv, vk = ed->ev $B$H$J$k$H$-$O>J$/(B (alpha $B$NJ}$KF~$k$?$a(B)
    if ( (vk != ed->sv) && (vk != ed->ev)  ) { 
      eps->x += vk->vec.x;
      eps->y += vk->vec.y;
      eps->z += vk->vec.z;
      ++(*k);
    } else { // vk = ed->sv or vk = ed->ev
      ++vkcount;
    }

    edk = ppdvertex_next_edge( edk, vj );
  } while ( (edk != ppdvertex_first_edge( vj )) && (edk != NULL) );

  // vk = ed->sv or vk = ed->ev $B$H$J$k(B vk $B$,$"$k$H$-!$(Bkj $B$K(B1$B$r2C;;(B
  // $BFs$D$"$k$H$-$O!$(Bedge collapse $B$5$l$F0l$D$K$J$k(B
  //  display("vkcount %d\n", vkcount );
  if ( vkcount >= 1 ) ++(*k);

  //
  // dkj, betaj $B$N7W;;(B (vj $B$K4X$9$kCM(B)
  //
  double dk = (double) (*k);
  *beta = calc_beta( *k );

  // alpha (vj $B$N(B 1-ring $BD:E@(B (vk) $B$N$&$A0l$D$,(B v0 $B$H$J$k(B)
  *alpha = *beta;
    
  // eps $B$K(B beta $B$r3]$1$k(B
  eps->x *= (*beta);
  eps->y *= (*beta);
  eps->z *= (*beta);

  // vj $B$O(B eps $B$NJ}$KF~$k(B
  double altmp = 1.0 - dk * (*beta);
  eps->x += ( altmp * vj->vec.x );
  eps->y += ( altmp * vj->vec.y );
  eps->z += ( altmp * vj->vec.z );
  
}
    

////////////////////////////////////////////////////////////////////////
//
// ed $B$NN>C<E@$K@\B3$9$k%(%C%8(B (1-Ring Edge) $B$r=g$KC5:w$9$k4X?t(B
//
////////////////////////////////////////////////////////////////////////

//
// Edge Ring $B$N:G=i$ND:E@(B
//
Sped *ppdedge_first_ring_edge( Sped *ed )
{
  if ( ed == NULL ) return NULL;

  Spvt *vt;
  if ( ed->isBoundary == TRUE ) {
    vt = ed->ev;
  } else if ( (ed->sv->isBoundary == TRUE ) ) {
    vt = ed->sv;
  } else if ( (ed->ev->isBoundary == TRUE ) ) {
    vt = ed->ev;
  } else {
    vt = ed->sv;
  }
    
  Sped *red = ppdvertex_first_edge( vt );
  while ( TRUE ) {
    if ( red != ed ) return red;
    red = ppdvertex_next_edge( red, vt );
  }
  return NULL;
}

//
// Edge Ring $B$N<!$N%(%C%8(B
//
Sped *ppdedge_next_ring_edge( Sped *red, Sped *ed )
{
  Spvt *vt, *ovt;
  if ( (ed->sv == red->sv) || (ed->sv == red->ev) ) {
    vt = ed->sv;
    ovt = ed->ev;
  } else {
    vt = ed->ev;
    ovt = ed->sv;
  }

  Sped *ted = ppdvertex_next_edge( red, vt );
  if ( ted == NULL ) return NULL;

  if ( ted == ed ) {
    ted = ppdvertex_next_edge( ted, ovt );
  }
  return ted;
}

//
// Edge Ring $B$N<!$N%(%C%8(B
//
Sped *ppdedge_prev_ring_edge( Sped *red, Sped *ed )
{
  Spvt *vt, *ovt;
  if ( (ed->sv == red->sv) || (ed->sv == red->ev) ) {
    vt = ed->sv;
    ovt = ed->ev;
  } else {
    vt = ed->ev;
    ovt = ed->sv;
  }

  Sped *ted = ppdvertex_prev_edge( red, vt );
  if ( ted == NULL ) return NULL;

  if ( ted == ed ) {
    ted = ppdvertex_prev_edge( ted, ovt );
  }
  return ted;
}

//
// Edge Ring $B$N#1!]6aK5D:E@(B
//
Spvt *ppdedge_ring_another_vt( Sped *red, Sped *ed )
{
  Spvt *vt;
  if ( (ed->sv == red->sv) || (ed->sv == red->ev) ) {
    vt = ed->sv;
  } else {
    vt = ed->ev;
  }
  return another_vt( red, vt );
}

////////////////////////////////////////////////////////////////////////
//
// ed $B$NN>C<E@$K@\B3$9$kD:E@(B (1-Ring Edge Vertex) $B$r=g$KC5:w$9$k4X?t(B
//
////////////////////////////////////////////////////////////////////////

//
// Edge Ring $B$N:G=i$ND:E@(B
//
Spvt *ppdedge_first_ring_vertex( Sped *ed, Sped **red )
{
  Sped *ted = ppdedge_first_ring_edge( ed );
  if ( ted == NULL ) return NULL;
  Spvt *vt = ppdedge_ring_another_vt( ted, ed );
  *red = ted;
  return vt;
}

//
// Edge Ring $B$N:G8e$ND:E@(B
//
Spvt *ppdedge_last_ring_vertex( Sped *ed, Sped **red )
{
  if ( ed == NULL ) return NULL;
  assert( (ed->sv->isBoundary == TRUE) || (ed->ev->isBoundary == TRUE) );
  
  Sped *edj = NULL; Sped *nedj = NULL; Sped *dedj = NULL;
  Spvt *vj = ppdedge_first_ring_vertex( ed, &edj );
  do {

    Spvt *pvj = vj;
    *red = edj;
    
    vj = ppdedge_next_ring_vertex( vj, ed, edj, &nedj  );
    if ( vj == NULL ) return pvj;
    edj = nedj;
  } while ( (vj != ppdedge_first_ring_vertex( ed, &dedj )) && (vj != NULL) );
  
  return NULL;
}

//
// Edge Ring $B$N<!$ND:E@(B
//
Spvt *ppdedge_next_ring_vertex( Spvt *vt, Sped *ed, Sped *red, Sped **nred )
{
  if ( (vt == NULL) || (red == NULL) ) {
    *nred = NULL;
    return NULL;
  }

  Sped *ted = red;
  while ( TRUE ) {
    ted = ppdedge_next_ring_edge( ted, ed );
    if ( ted == NULL ) break;
    Spvt *tvt = ppdedge_ring_another_vt( ted, ed );
    if ( tvt != vt ) {
      *nred = ted;
      return tvt;
    }
  }
  
  *nred = NULL;
  return NULL;
}

//
// Edge Ring $B$NA0$ND:E@(B
//
Spvt *ppdedge_prev_ring_vertex( Spvt *vt, Sped *ed, Sped *red, Sped **pred )
{
  if ( (vt == NULL) || (red == NULL) ) {
    *pred = NULL;
    return NULL;
  }

  Sped *ted = red;
  while ( TRUE ) {
    ted = ppdedge_prev_ring_edge( ted, ed );
    if ( ted == NULL ) break;
    Spvt *tvt = ppdedge_ring_another_vt( ted, ed );
    if ( tvt != vt ) {
      *pred = ted;
      return tvt;
    }
  }
  
  *pred = NULL;
  return NULL;
}

//
// vi, vj $B$NNY@\LL$G!$(Bed->sv $B$b$7$/$O(B ed->ev $B$r4^$^$J$$LL$N(B
// $B$&$A!$(Bvi, vj $B0J30$ND:E@(B
//
Spvt *ppdedge_opposite_vertex( Sped *ed, Spvt *vi, Spvt *vj )
{
  if ( (vi == NULL) || (vj == NULL) ) return NULL;

  Sped *ned = find_ppdedge_new( vi, vj );
  if ( ned == NULL ) return NULL;

  if ( ned->lhe != NULL ) {
    Spfc *fc = ned->lhe->bpfc;
    Spvt *vt = ppdface_another_vertex( fc, vi, vj );
    if ( (vt != ed->sv) && (vt != ed->ev) ) return vt;
  }

  if ( ned->rhe != NULL ) {
    Spfc *fc = ned->rhe->bpfc;
    Spvt *vt = ppdface_another_vertex( fc, vi, vj );
    if ( (vt != ed->sv) && (vt != ed->ev) ) return vt;
  }

  return NULL;
}

//
// QEM $BMQ(B $B%^%H%j%/%94X?t(B
//
void qem_matrix4( double alpha, Vec *eps, double *mat, double *qsmat )
{
  //initialize_matrix4( qsmat );

//    display("(in) mat0 %g mat10 %g\n", mat[0], mat[9] );
//    display("(in) eps %g %g %g\n", eps->x, eps->y, eps->z );
  // $B%^%H%j%/%9ItJ,(B (A): alpha*alpha*A0
  double al2 = alpha * alpha;
  qsmat[0] = mat[0] * al2;
  qsmat[1] = mat[1] * al2;
  qsmat[2] = mat[2] * al2;
  qsmat[3] = mat[3] * al2;
  qsmat[4] = mat[4] * al2;
  qsmat[5] = mat[5] * al2;
  
  // $B%Y%/%H%kItJ,(B (b): -alpha*(Ae+b0)
  Vec Ae;
  Ae.x = mat[0] * eps->x + mat[3] * eps->y + mat[4] * eps->z;
  Ae.y = mat[3] * eps->x + mat[1] * eps->y + mat[5] * eps->z;
  Ae.z = mat[4] * eps->x + mat[5] * eps->y + mat[2] * eps->z;
  qsmat[6] = ( (Ae.x + mat[6]) * alpha );
  qsmat[7] = ( (Ae.y + mat[7]) * alpha );
  qsmat[8] = ( (Ae.z + mat[8]) * alpha );
  
  // $BDj?tItJ,(B (c): e0 A e0 +2 b0e0 + c
  qsmat[9] = ( eps->x * Ae.x
	       + eps->y * Ae.y
	       + eps->z * Ae.z
	       + 2.0 * mat[6] * eps->x
	       + 2.0 * mat[7] * eps->y
	       + 2.0 * mat[8] * eps->z
	       + mat[9] );
  
}

////////////////////////////////////////////////////////////////////////
//
// $B%F%9%H4X?t(B
//
////////////////////////////////////////////////////////////////////////

// for testing
void edge_ring_test( Sppd *ppd )
{
  for ( Sped *ed = ppd->sped; ed != NULL; ed = ed->nxt ) {

    display("edge %d (boundary %d) sv %d (boundary %d) ev %d (bondary %d)\n",
	    ed->no, ed->isBoundary,
	    ed->sv->no, ed->sv->isBoundary,
	    ed->ev->no, ed->ev->isBoundary );
    // ed->sv $B$NNY@\D:E@$NC5:w(B
    Spvt *vt = ed->sv;
    display("\tsv edge: ");
    Sped *ted = ppdvertex_first_edge( vt );
    do {
      display("%d ", ted->no );
      ted = ppdvertex_next_edge( ted, vt );
    } while ( (ted != ppdvertex_first_edge( vt )) && (ted != NULL) );
    display("\n");
    
    // ed->ev $B$NNY@\D:E@$NC5:w(B
    vt = ed->ev;
    display("\tev edge: ");
    ted = ppdvertex_first_edge( vt );
    do {
      display("%d ", ted->no );
      ted = ppdvertex_next_edge( ted, vt );
    } while ( (ted != ppdvertex_first_edge( vt )) && (ted != NULL) );
    display("\n");

    // edge ring $B$NC5:w(B
    display("\t(a) edge ring: ");
    ted = ppdedge_first_ring_edge( ed );
    do {
      display("%d ", ted->no );
      ted = ppdedge_next_ring_edge( ted, ed );
    } while ( (ted != ppdedge_first_ring_edge( ed )) && (ted != NULL) );
    display("\n");
    
  }
}

// for testing
void edge_ring_vertex_test( Sppd *ppd )
{
  for ( Sped *ed = ppd->sped; ed != NULL; ed = ed->nxt ) {
    
    display("edge %d (boundary %d) sv %d (boundary %d) ev %d (bondary %d)\n",
	    ed->no, ed->isBoundary,
	    ed->sv->no, ed->sv->isBoundary,
	    ed->ev->no, ed->ev->isBoundary );
    // ed->sv $B$NNY@\D:E@$NC5:w(B
    Spvt *vt = ed->sv;
    display("\tsv vertex: ");
    Sped *ted = ppdvertex_first_edge( vt );
    do {
      Spvt *tvt = another_vt( ted, vt );
      display("%d ", tvt->no );
      ted = ppdvertex_next_edge( ted, vt );
    } while ( (ted != ppdvertex_first_edge( vt )) && (ted != NULL) );
    display("\n");
    
    // ed->ev $B$NNY@\D:E@$NC5:w(B
    vt = ed->ev;
    display("\tev vertex: ");
    ted = ppdvertex_first_edge( vt );
    do {
      Spvt *tvt = another_vt( ted, vt );
      display("%d ", tvt->no );
      ted = ppdvertex_next_edge( ted, vt );
    } while ( (ted != ppdvertex_first_edge( vt )) && (ted != NULL) );
    display("\n");

    // edge ring $B$NC5:w(B
    display("\tedge ring vertex 1: ");
    ted = ppdedge_first_ring_edge( ed );
    do {
      Spvt *tvt = ppdedge_ring_another_vt( ted, ed );
      display("%d ", tvt->no );
      ted = ppdedge_next_ring_edge( ted, ed );
    } while ( (ted != ppdedge_first_ring_edge( ed )) && (ted != NULL) );
    display("\n");

    // edge ring vertex $B$NC5:w(B
    display("\tedge ring vertex 2: ");
    Spvt *tvt = ppdedge_first_ring_vertex( ed, &ted );
    Sped *nred = NULL;
    Sped *ded = NULL;
    do {
      display("%d ", tvt->no );
      tvt = ppdedge_next_ring_vertex( tvt, ed, ted, &nred  );
      ted = nred;
    } while ( (tvt != ppdedge_first_ring_vertex( ed, &ded )) && 
	      (tvt != NULL) );
    display("\n");
    
  }
}

