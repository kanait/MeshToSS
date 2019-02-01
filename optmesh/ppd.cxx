//
// ppd.cxx
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

#include <math.h>
#include <stdlib.h>
#include "smd.h"

#include "ppdsolid.h"
#include "ppdpart.h"
#include "ppdvertex.h"
#include "ppdedge.h"
#include "ppdface.h"
#include "ppdnormal.h"
#include "oppd.h"
#include "veclib.h"

#include "ppd.h"

/***************************************
  Ppd functions
****************************************/

Sppd *create_ppd( void )
{
  double angle;
  Sppd *ppd;
  
  if ((ppd = (Sppd *) malloc(sizeof(Sppd))) == (Sppd *) NULL) {
    return (Sppd *) NULL;
  }
  
  // solid
  ppd->sn   = 0;
  ppd->sid  = 0;
  ppd->spso = (Spso *) NULL;
  ppd->epso = (Spso *) NULL;
  
  // part
  ppd->pn   = 0;
  ppd->pid  = 0;
  ppd->sppt = (Sppt *) NULL;
  ppd->eppt = (Sppt *) NULL;
  
  // face
  ppd->fn   = 0;
  ppd->fid  = 0;
  ppd->spfc = (Spfc *) NULL;
  ppd->epfc = (Spfc *) NULL;
  
  // normal
  ppd->nn   = 0;
  ppd->nid  = 0;
  ppd->spnm = (Spnm *) NULL;
  ppd->epnm = (Spnm *) NULL;
  
  // vertex
  ppd->vn   = 0;
  ppd->vid  = 0;
  ppd->spvt = (Spvt *) NULL;
  ppd->epvt = (Spvt *) NULL;

  // new
  ppd->nvt_org = 0;
  
  // edge
  ppd->en   = 0;
  ppd->eid  = 0;
  ppd->sped = NULL;
  ppd->eped = NULL;
  
  // scale, center
  ppd->scale = 0.0;
  ppd->center.x = 0.0;
  ppd->center.y = 0.0;
  ppd->center.z = 0.0;

  // for lod
  ppd->lodpqh = NULL;

  // for subdivision
  ppd->parent = NULL;
  ppd->child = NULL;
  ppd->sub_level = 0;
  
  angle = 45.0;
  ppd->smooth_angle = cos( angle * SMDPI / 180.0 );

  // file type
  ppd->file_type = FILE_NONE;
  
  // conv dis
  ppd->dis = 0.0;

  // bounding box length
  ppd->length = 0.0;

  // error
  ppd->err = 0.0;
  
  return ppd;
}

Sppd* copy_ppd( Sppd* ppd )
{
  Sppd* newppd = create_ppd();
  newppd->spso = create_ppdsolid( newppd );
  newppd->sppt = create_ppdpart( newppd );

  // for simpmesh_l2norm
  newppd->length = ppd->length;
  newppd->err = ppd->err;
  
  Ovt *ovt = create_overtex( ppd->vn );
  int i = 0;
  for ( Spvt* vt = ppd->spvt; vt != NULL; vt = vt->nxt )
    {
      vt->sid = i; ++i;

      Spvt* nvt = create_ppdvertex( newppd );
      nvt->bpso = newppd->spso;
      ovt[vt->sid].vt = nvt;
      V3Equal( &(nvt->vec), &(vt->vec) );
    }

  for ( Spfc* fc = ppd->spfc; fc != NULL; fc = fc->nxt )
    {
      Spfc* nfc = create_ppdface( newppd );
      nfc->bpso = newppd->spso;
      nfc->bppt = newppd->sppt;
      
      Sphe* he = fc->sphe;
      do {
	Sphe* nhe = create_ppdhalfedge( nfc );
	nhe->vt = ovt[he->vt->sid].vt;
      } while ( (he = he->nxt) != fc->sphe );

      calc_fnorm( nfc );
    }
  ppd_make_edges( newppd );
  calc_ppd_boundary( newppd );
  reattach_ppdvertex_halfedge( newppd );
  
  free( ovt );

  return newppd;
  
}
    

void free_ppd( Sppd *ppd )
{
  Spso *solid, *nsolid;
  Sppt *part, *npart;
  Spfc *face, *nface;
  Spnm *normal, *nnormal;
  Spvt *vertex, *nvertex;
  Sped *edge, *nedge;

  if ( ppd == (Sppd *) NULL ) return;

  ppd->parent = NULL;
  
  // for subdivision
  if ( ppd->child != NULL )
    {
      free_ppd( ppd->child );
      ppd->child = NULL;
    }

  // edge
  for (edge = ppd->sped; edge != (Sped *) NULL; edge = nedge) {
    nedge = edge->nxt;
    free_ppdedge(edge, ppd);
  }
  ppd->sped = ppd->eped = NULL;

  // face
  for (face = ppd->spfc; face != (Spfc *) NULL; face = nface) {
    nface = face->nxt;
    free_ppdface(face, ppd);
  }
  ppd->spfc = ppd->epfc = NULL;

  // normal
  for (normal = ppd->spnm; normal != (Spnm *) NULL; normal = nnormal) {
    nnormal = normal->nxt;
    free_ppdnormal(normal, ppd);
  }
  ppd->spnm = ppd->epnm = NULL;

  // vertex
  for ( vertex = ppd->spvt; vertex != (Spvt *) NULL; vertex = nvertex ) {
    nvertex = vertex->nxt;
    free_ppdvertex( vertex, ppd );
  }
  ppd->spvt = ppd->epvt = NULL;

  // part
  for (part = ppd->sppt; part != (Sppt *) NULL; part = npart) {
    npart = part->nxt;
    free_ppdpart(part, ppd);
  }
  ppd->sppt = ppd->eppt = NULL;

  // solid
  for (solid = ppd->spso; solid != (Spso *) NULL; solid = nsolid) {
    nsolid = solid->nxt;
    free_ppdsolid(solid, ppd);
  }
  ppd->spso = ppd->epso = NULL;

  // ppd
  free( ppd );
}

void calc_ppd_boundary( Sppd *ppd )
{
  for ( Sped *e = ppd->sped; e != NULL; e = e->nxt ) {
    if ( (e->lhe == NULL) || (e->rhe == NULL) ) {
      e->isBoundary  = TRUE;
      e->sv->isBoundary = TRUE;
      e->ev->isBoundary = TRUE;
    } else {
      e->isBoundary = FALSE;
    }
  }
}

void normalize_ppd( Sppd *ppd )
{
  int  i;
  Spvt *v;
  double xlength, ylength, zlength;
  double maxx, minx, maxy, miny, maxz, minz;
  
  maxx=minx=maxy=miny=maxz=minz=0.0;
  for (i = 0, v = ppd->spvt; v != (Spvt *) NULL; ++i, v = v->nxt) {
    if (i) {
      if (v->vec.x > maxx) maxx = v->vec.x;
      if (v->vec.x < minx) minx = v->vec.x;
      if (v->vec.y > maxy) maxy = v->vec.y;
      if (v->vec.y < miny) miny = v->vec.y;
      if (v->vec.z > maxz) maxz = v->vec.z;
      if (v->vec.z < minz) minz = v->vec.z;
    }	else {
      maxx = minx = v->vec.x;
      maxy = miny = v->vec.y;
      maxz = minz = v->vec.z;
    }
  }

  xlength = maxx - minx;
  ylength = maxy - miny;
  zlength = maxz - minz;

  ppd->length = sqrt( xlength*xlength
		      + ylength*ylength
		      + zlength*zlength );
//    if (xlength > ylength)   maxlength = xlength;
//    else                     maxlength = ylength;
//    if (zlength > maxlength) maxlength = zlength;

//    ppd->scale = maxlength / 1.5;
//    ppd->center.x = (maxx + minx) / 2.0;
//    ppd->center.y = (maxy + miny) / 2.0;
//    ppd->center.z = (maxz + minz) / 2.0;

//    for (i = 0, v = ppd->spvt; v != (Spvt *) NULL; ++i, v = v->nxt) {
//      v->vec.x = (v->vec.x - ppd->center.x) / ppd->scale;
//      v->vec.y = (v->vec.y - ppd->center.y) / ppd->scale;
//      v->vec.z = (v->vec.z - ppd->center.z) / ppd->scale;
//    }
}

void ppd_size( Sppd *ppd,
	       double *maxx, double *minx,
	       double *maxy, double *miny,
	       double *maxz, double *minz )
{
  *maxx=*minx=*maxy=*miny=*maxz=*minz=0.0;
  int  i;
  Spvt *v;
  for (i = 0, v = ppd->spvt; v != NULL; ++i, v = v->nxt) {
    if (i) {
      if (v->vec.x > *maxx) *maxx = v->vec.x;
      if (v->vec.x < *minx) *minx = v->vec.x;
      if (v->vec.y > *maxy) *maxy = v->vec.y;
      if (v->vec.y < *miny) *miny = v->vec.y;
      if (v->vec.z > *maxz) *maxz = v->vec.z;
      if (v->vec.z < *minz) *minz = v->vec.z;
    }	else {
      *maxx = *minx = v->vec.x;
      *maxy = *miny = v->vec.y;
      *maxz = *minz = v->vec.z;
    }
  }
}

void unnormalize_ppd( Sppd *ppd )
{
  Spvt *v;
  
  for ( v = ppd->spvt; v != (Spvt *) NULL; v = v->nxt ) {
    v->vec.x = v->vec.x * ppd->scale +  ppd->center.x;
    v->vec.y = v->vec.y * ppd->scale +  ppd->center.y;
    v->vec.z = v->vec.z * ppd->scale +  ppd->center.z;
  }
  
}

void ppd_make_edges( Sppd* ppd )
{
  for ( Spfc *fc = ppd->spfc; fc != NULL; fc = fc->nxt ) {

    Sphe *he = fc->sphe;
    do {

      Spvt *sv = he->vt;
      Spvt *ev = he->nxt->vt;
      
      Sped *ed;
      if ((ed = find_ppdedge(sv, ev)) == (Sped *) NULL) {
	// create an edge
	ed = create_ppdedge( ppd );
	// edge - vertex
	ed->sv = sv;
	ed->ev = ev;
	//ed->length = V3DistanceBetween2Points( &(sv->vec), &(ev->vec) );
	// vertex - edge
	(void) create_vtxed( sv, ed );
	(void) create_vtxed( ev, ed );
	// edge - halfedge
	he->ed = ed;
	ed->lhe = he;

      } else {
	// an edge have already existed
	he->ed = ed;
	if ( (ed->sv == sv) && (ed->ev == ev) ) {
	  if ( ed->lhe != NULL ) {
	    display("Warning. Edge %d has more than three halfedges. \n",
		    ed->no );
	  }
	  ed->lhe  = he;
	  // set mate
	  if ( ed->rhe != NULL ) {
	    ed->rhe->mate = he;
	    he->mate = ed->rhe;
	  }
	      
	} else {
	  if ( ed->rhe != NULL ) {
	    display("Warning. Edge %d has more than three halfedges. \n",
		    ed->no);
	  }
	  ed->rhe  = he;
	  // set mate
	  if ( ed->lhe != NULL ) {
	    ed->lhe->mate = he;
	    he->mate = ed->lhe;
	  }
	}
	
      }

      // vertex -> halfedge link
      sv->sphe = he;

    } while ( (he = he->nxt) != fc->sphe );
    
  }
}

//
// サーウムキチイス
//
void ppd_triangulation( Sppd* ppd )
{
  int i = 0;
  int n = ppd->fn;
  Spfc* fc = ppd->spfc;
	
  while ( i < n ) {
		
    if ( fc->hen == TRIANGLE )
      {
	fc = fc->nxt; i++;
	continue;
      }
    Spfc* nfc = fc->nxt;
    ppdface_triangulation( fc, ppd );
    free_ppdface( fc, ppd );
    fc = nfc; ++i;
  }
}

//
// フフ、エ、ネ、ホサーウムキチイス
//
void ppdface_triangulation( Spfc* fc, Sppd* ppd )
{
  int n = fc->hen;
	
  Spvt** vtarray = (Spvt **) malloc( n * sizeof( Spvt* ) );

  int i;
  Sphe* he;
  for( i = 0, he = fc->sphe; i < n; ++i, he = he->nxt ) {
    vtarray[i] = he->vt;
  }
	
  // create new faces
  Spfc** fcarray = (Spfc **) malloc( (n - 2) * sizeof( Spfc * ) );
  for ( i = 0; i < n - 2; ++i ) {
    fcarray[i] = create_ppdface( ppd );
  }
  Spvt** newvtarray = (Spvt **) malloc( (n - 1) * sizeof( Spvt * ) );
  for ( i = 0; i < n - 1; ++i ) {
    newvtarray[i] = vtarray[i+1];
  }
  for ( i = 0; i < n - 2; ++i ) {
		
    Sphe* he0 = create_ppdhalfedge( fcarray[i] );
    he0->vt = vtarray[0];
    Sphe* he1 = create_ppdhalfedge( fcarray[i] );
    he1->vt = newvtarray[i];
    Sphe* he2 = create_ppdhalfedge( fcarray[i] );
    he2->vt = newvtarray[i+1];
		
    calc_fnorm( fcarray[i] );
  }
	
  free(vtarray); free(newvtarray);
  free(fcarray);
	
}  

