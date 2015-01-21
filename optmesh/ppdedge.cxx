//
// ppdedge.cxx
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <stdlib.h>
#include "smd.h"
#include "ppdvertex.h"
#include "veclib.h"

#include "ppdedge.h"

//
// Edge
//

Sped *create_ppdedge( Sppd *ppd )
{
  Sped *ed;

  ed = (Sped *) malloc( sizeof(Sped) );

  ed->nxt = (Sped *) NULL;
  if (ppd->sped == (Sped *) NULL) {
    ed->prv   = (Sped *) NULL;
    ppd->eped = ppd->sped = ed;
  } else {
    ed->prv   = ppd->eped;
    ppd->eped = ppd->eped->nxt = ed;
  }

  // edge color
  ed->col = EDGEWHITE;
  
  // IDs
  ed->no  = ppd->eid;

  // sid
  ed->sid = SMDNULL;
  
  // links
  ed->sv  = NULL;
  ed->ev  = NULL;
  ed->lhe = NULL;
  ed->rhe = NULL;

  // for creating hppdlod
  //ed->atr = EDGEINTERNAL;
  ed->isBoundary = FALSE;

  // star
  ed->stared = NULL;

  // lodprique
  ed->pqc = NULL;
  ed->pq_type = EMPTY;
  
  // subdiv boundary
  ed->isSubdivBoundary = FALSE;
  
  ++( ppd->en );
  ++( ppd->eid );
  
  return ed;
}

void free_ppdedge( Sped *ed, Sppd *ppd )
{
  if (ed == (Sped *) NULL) return;

  //free_ppdedgevertex( ed );

  // kill edge - halfedge links
  //if ( ed->lhe != NULL ) {
  //    ed->lhe->ed = NULL;
  //    ed->lhe = NULL;
  //}
  //if ( ed->rhe != NULL ) {
  //ed->rhe->ed = NULL;
  //ed->rhe = NULL;
  //}

  if ( ppd->sped == ed ) {
    if ((ppd->sped = ed->nxt) != (Sped *) NULL)
      ed->nxt->prv = (Sped *) NULL;
    else {
      ppd->eped = (Sped *) NULL;
    }
  } else if (ppd->eped == ed) {
    ed->prv->nxt = (Sped *) NULL;
    ppd->eped = ed->prv;
  } else {
    ed->prv->nxt = ed->nxt;
    ed->nxt->prv = ed->prv;
  }

  free( ed );
  --( ppd->en );
}

void FreeSped( Sped *ed, Sppd *ppd )
{
  if ( ed == NULL ) return;
  free_ppdedge( ed, ppd );
}

void edge_length( Sppd *ppd )
{
  int  i;
  Sped *ed;
  
  for ( i = 0, ed = ppd->sped; ed != (Sped *) NULL; ed = ed->nxt, ++i ) {

    ed->length = V3DistanceBetween2Points( &(ed->sv->vec), &(ed->ev->vec) );
//     if ( i ) {
      
//       if ( ed->length > ppd->max_edge_length ) {
// 	ppd->max_edge_length = ed->length;
//       }
      
//     } else {
//       ppd->max_edge_length = ed->length;
//     }
    
  }
}

Sped *find_ppdedge(Spvt *sv, Spvt *ev)
{
  Vted *ve;
  
  ve = sv->svted;
  while (ve != (Vted *) NULL) {
    if ( ( (ve->ed->sv == sv) && (ve->ed->ev == ev) ) ||
	 ( (ve->ed->sv == ev) && (ve->ed->ev == sv) ) ) {
      return ve->ed;
    }
    ve = ve->nxt;
  }
  return (Sped *) NULL;
}

Sped *find_ppdedge_new( Spvt *sv, Spvt *ev )
{
  Sped *ed, *fed;
  //display("sv %d ven %d\n", sv->no, sv->ven );
  ed = fed = ppdvertex_first_edge( sv );
  do {
    //display("ed %d\n", ed->no );
    if ( ( (ed->sv == sv) && (ed->ev == ev) ) ||
	 ( (ed->sv == ev) && (ed->ev == sv) ) ) 
      return ed;
    
    ed = ppdvertex_next_edge( ed, sv );
  } while ( (ed != NULL) && (ed != fed) );

  return NULL;
}

double normalize_ppdedge( Sppd *ppd )
{
  int  i;
  Spvt *v;
  double maxlength, scale;
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

  if (xlength > ylength)   maxlength = xlength;
  else                     maxlength = ylength;
  if (zlength > maxlength) maxlength = zlength;

  scale = 1.0 / maxlength;

  return scale;
}

void ppdedge_to_vec( Sped *ed, Vec *vec )
{
  V3Sub( &(ed->ev->vec), &(ed->sv->vec), vec );
  //V3Normalize( vec );
}

Spfc *ppdedge_another_fc( Sped *ed, Spfc *fc )
{
  if ( ed->lhe != NULL ) {
    if ( ed->lhe->bpfc != fc ) return ed->lhe->bpfc;
  }
  if ( ed->rhe != NULL ) {
    if ( ed->rhe->bpfc != fc ) return ed->rhe->bpfc;
  }
  return NULL;
}

