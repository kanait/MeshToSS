//
// simperror.cxx
// simplifying mesh functions
// 
// Copyright (c) 2000 Takashi Kanai; All rights reserved. 
//

// #include "StdAfx.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "smd.h"

#include "veclib.h"
#include "matlib.h"

#include "params.h"
#include "ppd.h"
#include "ppdlist.h"
#include "octree.h"

Params params;

void params_init( Params *params )
{
  params->print = TRUE;
}
  
#ifdef WIN32
//void main( int argc, char *argv[] )
int main( int argc, char *argv[] )
#else
int main( int argc, char *argv[] )
#endif
{
  if ( argc != 3 ) {
    fprintf(stderr, "Usage: %s in.ppd in.xyz(ppd)\n", argv[0]);
    exit( 1 );
  }

  params_init( &params );
  
  // open ppd
  Sppd *ppd = (Sppd *) NULL;
  if ( (ppd  = open_ppd( argv[1] )) == (Sppd *) NULL ) {
    fprintf( stderr, "Error: can't open %s.\n", argv[1] );
    exit( 1 );
  }

  // open ppd
  Sppd *xyz = (Sppd *) NULL;
  if ( (xyz  = open_ppd( argv[2] )) == (Sppd *) NULL ) {
    fprintf( stderr, "Error: can't open %s.\n", argv[2] );
    exit( 1 );
  }

  int j = 0;
  double max_dis = 0.0;
  double avr_dis = 0.0;
  for ( Spvt *vt = xyz->spvt; vt != NULL; vt = vt->nxt ) {

    int i = 0;
    double min_dis = 0.0;
    for ( Spfc *fc = ppd->spfc; fc != NULL; fc = fc->nxt ) {
      double dis = point_face_distance( vt, fc );

      if ( i ) {
	if ( dis < min_dis ) min_dis = dis;
      } else min_dis = dis;
      ++i;
    }

    fprintf(stdout, "%d %g\n", vt->no, min_dis );
    
    if ( j ) {
      if ( min_dis > max_dis ) max_dis = min_dis;
    } else max_dis = min_dis;
    ++j;

    avr_dis += min_dis;
  }

  avr_dis /= (double) xyz->vn;
  
  fprintf(stdout, "avr %g max %g\n", avr_dis, max_dis );

  // octree 、ホス魘//    double xmax, xmin, ymax, ymin, zmax, zmin;
//    ppd_size( ppd, &xmax, &xmin, &ymax, &ymin, &zmax, &zmin );
//    double txmax, txmin, tymax, tymin, tzmax, tzmin;
//    ppd_size( xyz, &txmax, &txmin, &tymax, &tymin, &tzmax, &tzmin );
  
//    if ( txmax > xmax ) xmax = txmax;
//    if ( txmin < xmin ) xmin = txmin;
//    if ( tymax > ymax ) ymax = tymax;
//    if ( tymin < ymin ) ymin = tymin;
//    if ( tzmax > zmax ) ymax = tzmax;
//    if ( tzmin < zmin ) ymin = tzmin;
  
//    OTree *octree = init_otree( xmax, xmin, ymax, ymin, zmax, zmin );
//    ppdface_to_ppdotree( ppd, octree );
//    ppdvertex_to_ppdotree( xyz, octree );
//    calc_point_to_face_distance( octree, ppd ); // ppd: face の方
  
  // ppd の face を Octree に格納
  
  
  free_ppd( ppd );
  free_ppd( xyz );
}

