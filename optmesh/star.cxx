//
// star.cxx
// functions for star structure
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "smd.h"
#include "ppdvertex.h"
#include "veclib.h"

#include "star.h"

// create star structure
Star *create_star( Sped *ed )
{
  Star *star = init_star();

  //
  // star vertex 、ヒ、ェ、、、ニ
  // ed->sv 、0 ネヨフワ。、ed->ev 、1 ネヨフワ、ヒウハヌシ
  //
  // ed->sv
  Starvt *starvt = NULL;
  starvt = create_starvertex( star );
  starvt->vt = ed->sv;
  ed->sv->starvt = starvt;
  starvt->vec.x = ed->sv->vec.x;
  starvt->vec.y = ed->sv->vec.y;
  starvt->vec.z = ed->sv->vec.z;
  // ed->ev
  starvt = create_starvertex( star );
  starvt->vt = ed->ev;
  ed->ev->starvt = starvt;
  starvt->vec.x = ed->ev->vec.x;
  starvt->vec.y = ed->ev->vec.y;
  starvt->vec.z = ed->ev->vec.z;

  CreateStarVertex( star, ed->sv );
//    display("aa\n");
  CreateStarVertex( star, ed->ev );
//    display("bb\n");
  CreateStarFace( star, ed->sv );
//    display("cc\n");
  CreateStarFace( star, ed->ev );
//    display("dd\n");
  CreateStarEdge( star );

  star->stare  = find_staredge( star, ed );
  star->isBoundary = ed->isBoundary;
  
  star->starsv = find_starvertex( star, ed->sv );
  star->svec.x = ed->sv->vec.x;
  star->svec.y = ed->sv->vec.y;
  star->svec.z = ed->sv->vec.z;
  star->starev = find_starvertex( star, ed->ev );
  star->evec.x = ed->ev->vec.x;
  star->evec.y = ed->ev->vec.y;
  star->evec.z = ed->ev->vec.z;

  delete_starlink( star );

  // temp
//    display("before:\n");
//    print_star( star );
  
  return star;
}

// create star vertex
void CreateStarVertex( Star *star, Spvt *sv )
{
  
  Sped *ed = ppdvertex_first_edge( sv );
  do {
    
    Spvt *vt = another_vt( ed, sv );

//      display("(create) vt %d\n", vt->no );
    Starvt *starvt;
    if ( (starvt = vt->starvt) == (Starvt *) NULL ) {
      starvt = create_starvertex( star );
      starvt->vt = vt;
      vt->starvt = starvt;
      starvt->vec.x = vt->vec.x;
      starvt->vec.y = vt->vec.y;
      starvt->vec.z = vt->vec.z;
    }
    
    ed = ppdvertex_next_edge( ed, sv );
  } while ( (ed != ppdvertex_first_edge( sv )) && (ed != NULL) );
    
}

// create star face
void CreateStarFace( Star *star, Spvt *sv )
{
  Sphe *he = sv->sphe;
  do { 
    Spfc *fc = he->bpfc;
    Starfc *starfc;
//      display("(create) fc %d\n", fc->no );
    if ( (starfc = (fc->starfc)) == (Starfc *) NULL ) {
      
      starfc = create_starface( star );
      starfc->fc = fc;
      fc->starfc = starfc;

      // face -> vertex links
      int i = 0;
      Sphe *ppdhe = fc->sphe;
      do {
	starfc->vt[i] = ppdhe->vt->starvt;
	++i;
      } while ( ( ppdhe = ppdhe->nxt ) != fc->sphe );
      
    }
    
    he = ppdvertex_ccwnext_halfedge( he );
  } while ( (he != sv->sphe) && (he != NULL) );
}

// create star edge
void CreateStarEdge( Star *star )
{
  for ( Starfc *sfc = star->spfc; sfc != (Starfc *) NULL; sfc = sfc->nxt ) {

    Spfc *fc = sfc->fc;
//     display("fc %d\n", fc->no );
    Stared *stared = NULL;

    int i = 0;
    Sphe *he = fc->sphe;
    do {
      
      Sped *ed = he->ed;
//       display("\ted %d\n", ed->no );
      if (  (stared = ed->stared) == (Stared *) NULL ) {
	stared = create_staredge( star );
	stared->ed = ed;
	ed->stared = stared;

	// edge -> vertex links
	stared->sv = ed->sv->starvt;
	stared->ev = ed->ev->starvt;

	// edge -> face links
	stared->lf = sfc;
	
	// face -> edge links
	sfc->ed[i] = stared; ++i;
	
      } else {
	// edge -> face links
	stared->rf = sfc;
	
	// face ->edge links
	sfc->ed[i] = stared; ++i;
      }
	
    } while ( ( he = he->nxt ) != fc->sphe );
  }

}

void delete_starlink( Star *star )
{
  // vertex
  for ( Starvt *svt = star->spvt; svt != (Starvt *) NULL; svt = svt->nxt ) {
    svt->vt->starvt = (Starvt *) NULL;
  }
  // face
  for ( Starfc *sfc = star->spfc; sfc != (Starfc *) NULL; sfc = sfc->nxt ) {
    sfc->fc->starfc = (Starfc *) NULL;
  }
  // edge
  for ( Stared *sed = star->sped; sed != (Stared *) NULL; sed = sed->nxt ) {
    sed->ed->stared = (Stared *) NULL;
  }
}

Star *init_star( void )
{
  Star *star = (Star *) malloc( sizeof( Star ) );
  star->stare  = NULL;
  star->starsv = NULL;
  star->starev = NULL;
  star->vn = 0; star->spvt = star->epvt = NULL;
  star->en = 0; star->sped = star->eped = NULL;
  star->fn = 0; star->spfc = star->epfc = NULL;
  
  star->isBoundary = FALSE;

  return star;
}

// vertex
Starvt *create_starvertex( Star *star )
{
  Starvt *vt = (Starvt *) malloc( sizeof(Starvt) );

  vt->nxt = (Starvt *) NULL;
  if (star->spvt == (Starvt *) NULL) {
    vt->prv   = (Starvt *) NULL;
    star->epvt = star->spvt = vt;
  } else {
    vt->prv   = star->epvt;
    star->epvt = star->epvt->nxt = vt;
  }
  vt->vt = (Spvt *) NULL;

  vt->no = star->vn;
  ++( star->vn );

  return vt;
}

Starvt *find_starvertex( Star *star, Spvt *vt )
{
  for ( Starvt *sv = star->spvt; sv != (Starvt *) NULL; sv = sv->nxt ) {
    if ( sv->vt == vt ) return sv;
  }
  return NULL;
}

void free_starvertex( Starvt *vt, Star *star )
{
  if ( vt == NULL ) return;
  
  if ( star->spvt == vt ) {
    if ( (star->spvt = vt->nxt) != (Starvt *) NULL )
      vt->nxt->prv = (Starvt *) NULL;
    else {
      star->epvt = (Starvt *) NULL;
    }
  } else if (star->epvt == vt) {
    vt->prv->nxt = (Starvt *) NULL;
    star->epvt = vt->prv;
  } else {
    vt->prv->nxt = vt->nxt;
    vt->nxt->prv = vt->prv;
  }

  free( vt );
  
  --( star->vn );
}

// edge
Stared *create_staredge( Star *star )
{
  Stared *ed = (Stared *) malloc( sizeof(Stared) );

  ed->nxt = (Stared *) NULL;
  if (star->sped == (Stared *) NULL) {
    ed->prv   = (Stared *) NULL;
    star->eped = star->sped = ed;
  } else {
    ed->prv   = star->eped;
    star->eped = star->eped->nxt = ed;
  }
  ed->ed = (Sped *) NULL;

  ed->rf = NULL;
  ed->lf = NULL;
  ed->sv = NULL;
  ed->ev = NULL;
  
  ed->no = star->en;
  ++( star->en );

  return ed;
}

Stared *find_staredge( Star *star, Sped *ed )
{
  for ( Stared *se = star->sped; se != (Stared *) NULL; se = se->nxt ) {
    if ( se->ed == ed ) return se;
  }
  return (Stared *) NULL;
}

void free_staredge( Stared *ed, Star *star )
{
  if ( ed == NULL ) return;
  
  if ( star->sped == ed ) {
    if ( (star->sped = ed->nxt) != (Stared *) NULL )
      ed->nxt->prv = (Stared *) NULL;
    else {
      star->eped = (Stared *) NULL;
    }
  } else if (star->eped == ed) {
    ed->prv->nxt = (Stared *) NULL;
    star->eped = ed->prv;
  } else {
    ed->prv->nxt = ed->nxt;
    ed->nxt->prv = ed->prv;
  }

  free( ed );
  
  --( star->en );
}

// face
Starfc *create_starface( Star *star )
{
  Starfc *fc = (Starfc *) malloc( sizeof(Starfc) );

  fc->nxt = (Starfc *) NULL;
  if (star->spfc == (Starfc *) NULL) {
    fc->prv   = (Starfc *) NULL;
    star->epfc = star->spfc = fc;
  } else {
    fc->prv   = star->epfc;
    star->epfc = star->epfc->nxt = fc;
  }
  fc->fc = (Spfc *) NULL;

  fc->vt[0] = (Starvt *) NULL;
  fc->vt[1] = (Starvt *) NULL;
  fc->vt[2] = (Starvt *) NULL;
  fc->ed[0] = (Stared *) NULL;
  fc->ed[1] = (Stared *) NULL;
  fc->ed[2] = (Stared *) NULL;
  
  fc->no = star->fn;
  ++( star->fn );

  return fc;
}

void free_starface( Starfc *fc, Star *star )
{
  if (fc == NULL) return;
  
  if ( star->spfc == fc ) {
    if ( (star->spfc = fc->nxt) != (Starfc *) NULL )
      fc->nxt->prv = (Starfc *) NULL;
    else {
      star->epfc = (Starfc *) NULL;
    }
  } else if (star->epfc == fc) {
    fc->prv->nxt = (Starfc *) NULL;
    star->epfc = fc->prv;
  } else {
    fc->prv->nxt = fc->nxt;
    fc->nxt->prv = fc->prv;
  }

  free( fc );
  
  --( star->fn );
}

void free_star( Star *star )
{
  // vertex
  Starvt *vt, *nvt;
  for ( vt = star->spvt; vt != (Starvt *) NULL; vt = nvt ) {
    nvt = vt->nxt;
    free_starvertex(vt, star);
  }

  // edge
  Stared *ed, *ned;
  for ( ed = star->sped; ed != (Stared *) NULL; ed = ned ) {
    ned = ed->nxt;
    free_staredge(ed, star);
  }

  // face
  Starfc *fc, *nfc;
  for ( fc = star->spfc; fc != (Starfc *) NULL; fc = nfc ) {
    nfc = fc->nxt;
    free_starface(fc, star);
  }

  free( star );
}

//
// star operation functions 
//
void staredge_collapse( Star *star )
{
  if ( star == (Star *) NULL ) return;

  Stared *e = star->stare;
  
  // edge collapse driver

  Starvt *vi = e->sv;
  Starvt *vj = e->ev;
  
  int  side;
  Starfc *f[2];
  if ( star->isBoundary != TRUE ) {
    side = EDGEINTERNAL;
    f[0] = e->lf;
    f[1] = e->rf;
  } else {
    side = EDGEBOUNDARY;
    f[0] = ( e->lf != NULL ) ? e->lf : e->rf;
  }
//    display("side %d\n", side );

  int  i;
  Stared *ej[2];
  for ( i = 0; i < side; ++i ) {
    
    Starvt *vk = get_lm_starvertex( f[i], vi, vj );
    Stared *ei = get_lm_staredge( f[i], vi, vk );
    ej[i] = get_lm_staredge( f[i], vj, vk );
    //Starfc *fi = get_lm_starface_rev( f[i], ei );
    Starfc *fj = get_lm_starface_rev( f[i], ej[i] );
    
    change_staredgefacelink( ei, f[i], fj );
    change_starfaceedgelink( fj, ej[i], ei );
    
  }
  
  // change links
  change_staredgevertexlink( star, vj, vi );
  change_starfacevertexlink( star, vj, vi );

  // frees
  for ( i = 0; i < side; ++i ) {
    free_starface( f[i], star );
    free_staredge( ej[i], star );
  }
  free_starvertex( vj, star );
  free_staredge( e, star );

//    display("after:\n");
//    print_star( star );

}

Starvt *get_lm_starvertex( Starfc *f, Starvt *v1, Starvt *v2 )
{
  if ( f == NULL ) return NULL;
  
  for ( int i = 0; i < TRIANGLE; ++i ) {
    Starvt *vt = f->vt[i];
    if ( ( vt != v1 ) && ( vt != v2 ) ) return vt;
  }
  return NULL;
}

Stared *get_lm_staredge( Starfc *f, Starvt *v1, Starvt *v2 )
{
  if ( f == NULL ) return NULL;

  for ( int i = 0; i < TRIANGLE; ++i ) {
    
    Stared *e = f->ed[i];
    if ( ((e->sv == v1) && (e->ev == v2)) ||
	 ((e->ev == v1) && (e->sv == v2)) )
      return e;
    
  }
  
  return (Stared *) NULL;
}

Starfc *get_lm_starface_rev( Starfc *f, Stared *e )
{
  if ( e == NULL ) return NULL;
  
  Starfc *rf = e->rf;
  Starfc *lf = e->lf;

  if ( rf == f ) return lf;
  if ( lf == f ) return rf;
  
  return (Starfc *) NULL;
}

void change_staredgefacelink( Stared *ei, Starfc *f, Starfc *fj )
{
  if ( ei == (Stared *) NULL ) return;

  if ( ei->rf == f ) {
    ei->rf = fj; 
    return;
  }

  if ( ei->lf == f ) {
    ei->lf = fj;
    return;
  }

}

void change_starfaceedgelink( Starfc *f, Stared *e1, Stared *e2 )
{
  int i;
  
  if ( f == (Starfc *) NULL ) return;

  for ( i = 0; i < TRIANGLE; ++i ) {
    
    if ( f->ed[i] == e1 ) {
      f->ed[i] = e2;
      return;
    }
  }
  
}

// edge -> vertex links
void change_staredgevertexlink( Star *star, Starvt *ev, Starvt *sv )
{
  if ( ev == NULL ) return;

  for ( Stared *e = star->sped; e != (Stared *) NULL; e = e->nxt ) {
    if ( e->sv == ev ) e->sv = sv;
    if ( e->ev == ev ) e->ev = sv;
  }
}

// face -> vertex links
void change_starfacevertexlink( Star *star, Starvt *ev, Starvt *sv )
{
  int     i;
  Starfc  *f;
  
  for ( f = star->spfc; f != (Starfc *) NULL; f = f->nxt ) {

    for ( i = 0; i < TRIANGLE; ++i ) {
      if ( f->vt[i] == ev ) f->vt[i] = sv;
    }

  }
  
}

void starnorm( Star *star )
{
  for ( Starfc *fc = star->spfc; fc != NULL; fc = fc->nxt ) {
    calc_starfacenorm( fc );
  }
}

void calc_starfacenorm( Starfc *fc )
{

  Vec *vec1 = &( fc->vt[0]->vec );
  Vec *vec2 = &( fc->vt[1]->vec );
  Vec *vec3 = &( fc->vt[2]->vec );
  Vec vec4, vec5;
  V3Sub( vec2, vec1, &vec4 );
  V3Sub( vec3, vec1, &vec5 );
  V3Cross( &vec4, &vec5, &(fc->nrm) );
  V3Normalize( &(fc->nrm) );
}

BOOL geom_check( Star *star, double angle_tol, double shape_tol )
{
  // angle check
  for ( Stared *ed = star->sped; ed != NULL; ed = ed->nxt ) {

    if ( (ed->rf == NULL) || (ed->lf == NULL) ) continue;

    double angle = V3Cosign( &(ed->rf->nrm), &(ed->lf->nrm) );
//      display(" rf %d lf %d angle %g\n", ed->rf->no, ed->lf->no, angle);
    if ( angle < angle_tol ) {
      return FALSE;
    }
  }

  // triangle shape check
  for ( Starfc *fc = star->spfc; fc != NULL; fc = fc->nxt ) {

    double comp = calc_starfcompactness( fc );
//      display("fc %d comp %g\n", fc->no, comp );
    if (  comp < shape_tol ) {
      return FALSE;
    }
    
  }
    
  return TRUE;
}

const double FOUR_ROOT3 = 6.928203230275509;

// compactness
double calc_starfcompactness( Starfc *fc )
{
  Stared *ed0 = fc->ed[0];
  Stared *ed1 = fc->ed[1];
  Stared *ed2 = fc->ed[2];
  double d0 = V3SquaredDistanceBetween2Points( &(ed0->sv->vec), &(ed0->ev->vec) );
  double d1 = V3SquaredDistanceBetween2Points( &(ed1->sv->vec), &(ed1->ev->vec) );
  double d2 = V3SquaredDistanceBetween2Points( &(ed2->sv->vec), &(ed2->ev->vec) );
  
  return FOUR_ROOT3 * calc_starfacearea( fc ) / (d0+d1+d2);
}

double calc_starfacearea( Starfc *fc )
{
  Vec    vec1, vec2, vec3, vec4, vec5, nrm;

  Starvt *v1 = fc->vt[0];
  Starvt *v2 = fc->vt[1];
  Starvt *v3 = fc->vt[2];
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
  
Starfc *find_starface( Star *star, Starvt *vi, Starvt *vj, Starvt *vk )
{
  for ( Starfc *fc = star->spfc; fc != NULL; fc = fc->nxt ) {
    if ( (fc->vt[0] == vi) && (fc->vt[1] == vj) && (fc->vt[2] == vk) ) {
      return fc;
    }
    if ( (fc->vt[0] == vi) && (fc->vt[2] == vj) && (fc->vt[1] == vk) ) {
      return fc;
    }
    if ( (fc->vt[1] == vi) && (fc->vt[2] == vj) && (fc->vt[0] == vk) ) {
      return fc;
    }
    if ( (fc->vt[1] == vi) && (fc->vt[0] == vj) && (fc->vt[2] == vk) ) {
      return fc;
    }
    if ( (fc->vt[2] == vi) && (fc->vt[0] == vj) && (fc->vt[1] == vk) ) {
      return fc;
    }
    if ( (fc->vt[2] == vi) && (fc->vt[1] == vj) && (fc->vt[0] == vk) ) {
      return fc;
    }
  }
  return NULL;
}

//
// for debugging
// 
void write_star_ppd_file( char *fname, Star *star )
{
  FILE *fp;
  if ((fp = fopen(fname, "w")) == NULL) {
    return;
  }
	
  // header
  int solidnum = 1;
  int partnum = 1;
  int vertexnum = star->vn;
  int facenum = star->fn;
  int fnodenum = star->fn * TRIANGLE;

  fprintf(fp, "header\n");
  fprintf(fp, "\tsolid\t%d\n",	solidnum);
  fprintf(fp, "\tpart\t%d\n",	partnum);
  fprintf(fp, "\tvertex\t%d\n",	vertexnum);
  fprintf(fp, "\tface\t%d\n",  facenum);
  fprintf(fp, "\tfnode\t%d\n", fnodenum);
  fprintf(fp, "end\n");
	
  //
  // solid
  //
  fprintf(fp, "solid\n");
  fprintf(fp, "\t1\t/p 1 1 /v 1 %d /f 1 %d\n", star->vn, star->fn );
  fprintf(fp, "end\n");

  //
  // part
  //
  fprintf(fp, "part\n");
  fprintf(fp, "\t1\t/f 1 %d\n", star->fn);
  fprintf(fp, "end\n");

  //
  // vertex
  //
  fprintf(fp, "vertex\n");
  int i = 1;
  Spvt *vt = NULL;
  for ( Starvt *starv = star->spvt; starv != NULL; starv = starv->nxt ) {
    vt = starv->vt;
    vt->sid = i;
    fprintf( fp, "\t%d\t%g %g %g\n", i, vt->vec.x, vt->vec.y, vt->vec.z );
    ++i;
  }
  fprintf(fp, "end\n");
	
  //
  // face
  //
  fprintf(fp, "face\n");
  i = 1;
  for ( Starfc *starf = star->spfc; starf != NULL; starf = starf->nxt ) {
    fprintf(fp, "\t%d\t", i );
    for ( int j = 0; j < TRIANGLE; ++j ) {
      fprintf(fp, "%d ", starf->vt[j]->vt->sid );
    }
    fprintf(fp, "\n");
    ++i;
  }
  fprintf(fp, "end\n");
	
  fclose(fp);
}

void print_star( Star *star )
{
  Starvt *starv;
  for ( starv = star->spvt; starv != (Starvt *) NULL; starv = starv->nxt ) {
    Spvt *vt = starv->vt;
    display("(starvt) vt id %d no %d\n", starv->no, vt->no );
    // edge
//      display("\tppded ");
//      Sped *ed = ppdvertex_first_edge( vt );
//      do {
//        display("%d ", ed->no );
//        ed = ppdvertex_next_edge( ed, vt );
//      } while ( (ed != ppdvertex_first_edge( vt )) && (ed != NULL) );
//      // face
//      display("\n");
//      display("\tppdfc ");
//      Sphe *ppdhe = starv->vt->sphe;
//      do {
//        display("%d ", ppdhe->bpfc->no );
//        ppdhe = ppdvertex_ccwnext_halfedge( ppdhe );
//      } while ( (ppdhe != starv->vt->sphe) && (ppdhe != NULL) );
//      display("\n");
  }

  // edge
//    for ( Stared *sed = star->sped; sed != NULL; sed = sed->nxt ) {
//      display("(stared) ed id %d no %d ppdsv %d ppdev %d\n", sed->no, sed->ed->no,
//  	    sed->ed->sv->no, sed->ed->ev->no );
//    }

  // face
  Starfc *starfc;
  for ( starfc = star->spfc; starfc != NULL; starfc = starfc->nxt ) {
    display("(starfc) fc id %d no %d ", starfc->no, starfc->fc->no );
    display("vt ");
    for ( int i = 0; i < TRIANGLE; ++i ) {
      display("%d ", starfc->vt[i]->no );
    }
    display("ppdvt ");
    Sphe *ppdhe = starfc->fc->sphe;
    do {
      display("%d ", ppdhe->vt->no );
    } while ( ( ppdhe = ppdhe->nxt ) != starfc->fc->sphe );
    display("\n");
  }
}

