//
// simpmesh.cxx
// simplifying mesh functions
// 
// Copyright (c) 1999 Takashi Kanai; All rights reserved. 
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>
#include "stdlib.h"
#include <assert.h>

#include "smd.h"

#include "params.h"
#include "veclib.h"
#include "matlib.h"

#include "ppd.h"
#include "ppdlist.h"
#include "ppdvertex.h"
#include "ppdface.h"
#include "ppdedge.h"

#include "simpcheck.h"

Params params;

int main(int argc, char *argv[])
{
  if ( argc != 2 ) {
    fprintf( stderr, "usage: %s in.ppd. \n", argv[0] );
    exit(1);
  }

  Sppd *ppd = open_ppd( argv[1] );
  optcheck( ppd );
  write_ppd_file( "check.ppd", ppd );
}

void optcheck( Sppd *ppd )
{
  if ( ppd == (Sppd *) NULL ) return;

  // $B!&%#!&%F!&%/!"%[%+%e%&%r!"b$D$+$I$&$(B  fprintf(stdout, "CHECK 1: edge boundary check...\n");
  boundary_check( ppd );
  fprintf(stdout, "\nCHECK 2: vertex neighborhood check...\n");
  vertex_neighborhood_check( ppd );
  fprintf(stdout, "\nCHECK 3: isolated vertex check...\n");
  isolate_vertex_check( ppd );
  fprintf(stdout, "\nCHECK 4: isolated face check...\n");
  isolated_face_check( ppd );
}

//
// boundary check
//
void boundary_check( Sppd *ppd )
{
  for (Sped *e = ppd->sped; e != NULL; e = e->nxt ) {
    if ( e->isBoundary == TRUE ) {
      fprintf(stdout, "Edge No.%d is boundary\n", e->no );
    }
  }
}

//
// isolated vertex check
//
void isolate_vertex_check( Sppd *ppd )
{
  int count = 0;
  Spvt *nvt;
  for ( Spvt *vt = ppd->spvt; vt != NULL; vt = nvt ) {
    nvt = vt->nxt;
    if ( vt->sphe == NULL ) {
      ++count;
      fprintf(stdout, "Vertex No.%d is isolated. remove ...\n", vt->no );
      free_ppdvertex( vt, ppd );
    }
  }
  
  fprintf(stdout, "done. vertex %d removed.\n", count );
}

//
// vertex neighborhood topology check
void vertex_neighborhood_check( Sppd *ppd )
{
  if ( ppd == NULL ) return;

  int i;
  int *fn = NULL;
  fn = (int *) malloc( ppd->vn * sizeof(int) );
  for ( i = 0; i < ppd->vn; ++i ) {
    fn[i] = 0;
  }
  for ( Spfc *fc = ppd->spfc; fc != NULL; fc = fc->nxt ) {
    Sphe *he = fc->sphe;
    do {
      ++( fn[he->vt->no] );
    } while ( (he = he->nxt) != fc->sphe );
  }

  int ppdvn = ppd->vn;

  Spvt *vt;
  for ( i = 0, vt = ppd->spvt; i < ppdvn; vt = vt->nxt, ++i ) {

    int cnt = 0;
    Sphe *he;
    he = vt->sphe;
    do {
      ++cnt;
      he = ppdvertex_ccwnext_halfedge( he );
    } while ( (he != vt->sphe) && (he != NULL) );

    if ( cnt < fn[vt->no] ) {

      // create new vertex
      Spvt *newvt = create_ppdvertex( ppd );
      newvt->bpso = ppd->spso;
      newvt->vec.x = vt->vec.x;
      newvt->vec.y = vt->vec.y;
      newvt->vec.z = vt->vec.z;

      fprintf(stdout, "vertex %d is not two manifold. new vertex %d geneated.\n",
	      vt->no, newvt->no );
      // face -> vertex update
      he = vt->sphe;
      do {
	he->vt = newvt;
	he = ppdvertex_ccwnext_halfedge( he );
      } while ( (he != vt->sphe) && (he != NULL) );

      // edge ->vertex update
      Sped *e = ppdvertex_first_edge( vt );
      do {
	if ( e->sv == vt ) e->sv = newvt;
	if ( e->ev == vt ) e->ev = newvt;
	e = ppdvertex_next_edge( e, vt );
      } while ( (e != ppdvertex_first_edge( vt )) && (e != NULL) );

      // vt->sphe $B$N%j%s%/$r%"%C%W%G!<%H(B
      newvt->sphe = vt->sphe;
      vt->sphe = create_vertex_halfedge_link( vt, ppd );
      
    }
  }
  
  free( fn );
}
    
Sphe *create_vertex_halfedge_link( Spvt *vt, Sppd *ppd )
{
  for ( Spfc *fc = ppd->spfc; fc != NULL; fc = fc->nxt ) {
    Sphe *he = fc->sphe;
    do {
      if ( he->vt == vt ) return he;
    } while ( (he = he->nxt) != fc->sphe );
  }
  return NULL;
}
      
void isolated_face_check( Sppd *ppd )
{
  Spfc *fc, *nfc;
  for ( fc = ppd->spfc; fc != NULL; fc = nfc ) {

    nfc = fc->nxt;
    
    Sphe *he = fc->sphe;

    BOOL flag = FALSE;
    do {
      // not boundary edge
      if ( he->ed->isBoundary != TRUE ) {
	flag = TRUE;
      }
    } while ( (he = he->nxt) != fc->sphe );

    if ( flag == FALSE ) {

      fprintf(stdout, "Face No.%d is isolated. remove...\n", fc->no );

      // delete edges
      do {
	Spvt *vt = he->vt; he->vt = NULL;
	free_ppdvertex( vt, ppd );
	
	Sped *ed = he->ed; he->ed = NULL;
	free_ppdedge( ed, ppd );

      } while ( (he = he->nxt) != fc->sphe );

      free_ppdface( fc, ppd );
      
    }
  }
}
      
    
