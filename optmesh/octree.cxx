//
// octree.cxx
// Octree data structure functions
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

//  #include "tricube.h"
#include "veclib.h"

#include "tricube.h"

#include "octree.h"

//
// create octree functions 
//
OTree *create_otreechild( OTree *parent, int level, int id,
			  double xmin, double xmax,
			  double ymin, double ymax,
			  double zmin, double zmax )
{
  OTree *otree = (OTree *) malloc(sizeof(OTree));

  otree->parent   = parent;
  otree->level    = level;
  otree->id       = id;

  // top
  if ( parent == NULL ) otree->top = NULL;
  else if ( parent->top == NULL ) otree->top = parent;
  else otree->top = parent->top;

  otree->on = 0;
  otree->maxotlevel = 0;
  otree->child[0] = NULL;
  otree->child[1] = NULL;
  otree->child[2] = NULL;
  otree->child[3] = NULL;
  otree->child[4] = NULL;
  otree->child[5] = NULL;
  otree->child[6] = NULL;
  otree->child[7] = NULL;
  otree->or_.xmin  = xmin;
  otree->or_.xmax  = xmax;
  otree->or_.ymin  = ymin;
  otree->or_.ymax  = ymax;
  otree->or_.zmin  = zmin;
  otree->or_.zmax  = zmax;
  otree->opn      = 0;
  otree->sopnt    = NULL;
  otree->eopnt    = NULL;
  otree->ofn      = 0;
  otree->soface   = NULL;
  otree->eoface   = NULL;

  return otree;
}  

OTree *create_otreechild_or( OTree *parent, int level, int id,
			     ORange *or_ )
{
  OTree *otree = (OTree *) malloc( sizeof(OTree) );

  otree->parent   = parent;
  otree->level    = level;
  otree->id       = id;

  // top
  if ( parent == NULL ) otree->top = NULL;
  else if ( parent->top == NULL ) otree->top = parent;
  else otree->top = parent->top;
  
  otree->on = 0;
  otree->maxotlevel = 0;
  otree->child[0] = NULL;
  otree->child[1] = NULL;
  otree->child[2] = NULL;
  otree->child[3] = NULL;
  otree->child[4] = NULL;
  otree->child[5] = NULL;
  otree->child[6] = NULL;
  otree->child[7] = NULL;
  otree->or_.xmin  = or_->xmin;
  otree->or_.xmax  = or_->xmax;
  otree->or_.ymin  = or_->ymin;
  otree->or_.ymax  = or_->ymax;
  otree->or_.zmin  = or_->zmin;
  otree->or_.zmax  = or_->zmax;
  otree->opn      = 0;
  otree->sopnt    = NULL;
  otree->eopnt    = NULL;
  otree->ofn      = 0;
  otree->soface   = NULL;
  otree->eoface   = NULL;

  return otree;
}  

OPoint *insert_otree_point( Spvt *vt, OTree *otree )
{
  OPoint *op = (OPoint *) malloc( sizeof(OPoint) );
  op->vt  = vt;
  op->ot  = otree;

  op->nxt = NULL;
  if ( otree->sopnt == NULL ) {
    op->prv = NULL;
    otree->eopnt = otree->sopnt = op;
  } else {
    op->prv = otree->eopnt;
    otree->eopnt = otree->eopnt->nxt = op;
  }

  ++(otree->opn);

  return op;
}
    
OFace *insert_otree_face( Spfc *fc, OTree *otree )
{
  OFace *of = (OFace *) malloc( sizeof(OFace) );
  of->fc  = fc;
  of->ot  = otree;

  of->nxt = NULL;
  if ( otree->soface == NULL ) {
    of->prv = NULL;
    otree->eoface = otree->soface = of;
  } else {
    of->prv = otree->eoface;
    otree->eoface = otree->eoface->nxt = of;
  }

  ++(otree->ofn);

  return of;
}

// free octree functions

void free_ppdotree( OTree *octree )
{
  free_ppdotree_child( octree );
}

void free_ppdotree_child( OTree *otree )
{
  if ( otree == NULL ) return;

  OTree *top;
  if ( otree->top != NULL ) top = otree->top;
  else top = otree;
    
  for ( int i = 0; i < 8; ++i ) {
    free_ppdotree_child( otree->child[i] );
    otree->child[i] = NULL;
  }
  
  --(top->on);
  
  free_ppdopoint( otree );
  free_ppdoface( otree );
  free( otree );
  
}

void free_ppdopoint( OTree *otree )
{
  if ( otree == (OTree *) NULL ) return;

  OPoint *op, *nop;
  for ( op = otree->sopnt; op != (OPoint *) NULL; op = nop ) {
    nop = op->nxt;
    delete_otree_point( op, otree );
  }
  otree->sopnt = otree->eopnt = NULL;
  otree->opn = 0;
}

void delete_otree_point(OPoint *op, OTree *otree)
{
  if ( otree->sopnt == op ) {
    if ((otree->sopnt = op->nxt) != (OPoint *) NULL)
      op->nxt->prv = (OPoint *) NULL;
    else {
      otree->eopnt = (OPoint *) NULL;
    }
  } else if ( otree->eopnt == op ) {
    op->prv->nxt = (OPoint *) NULL;
    otree->eopnt = op->prv;
  } else {
    op->prv->nxt = op->nxt;
    op->nxt->prv = op->prv;
  }
  
  free(op);
  --( otree->opn );
}

void free_ppdoface(OTree *otree)
{
  if ( otree == (OTree *) NULL ) return;
  
  OFace *of, *nof;
  for (of = otree->soface; of != (OFace *) NULL; of = nof) {
    nof = of->nxt;
    delete_otree_face(of, otree);
  }
  otree->soface = otree->eoface = (OFace *) NULL;
  otree->ofn = 0;
}

void delete_otree_face(OFace *of, OTree *otree)
{
  if (otree->soface == of) {
    if ((otree->soface = of->nxt) != (OFace *) NULL)
      of->nxt->prv = (OFace *) NULL;
    else {
      otree->eoface = (OFace *) NULL;
    }
  } else if (otree->eoface == of) {
    of->prv->nxt = (OFace *) NULL;
    otree->eoface = of->prv;
    
  } else {
    of->prv->nxt = of->nxt;
    of->nxt->prv = of->prv;
  }
  
  free(of);
  --( otree->ofn );
}

int decide_orange_child_number(Vec *vec, ORange *or_, ORange *ornew)
{
  double hx = (or_->xmin + or_->xmax) / 2.0;
  double hy = (or_->ymin + or_->ymax) / 2.0;
  double hz = (or_->zmin + or_->zmax) / 2.0;
  
  /*printf("(parent or) %f %f %f %f %f %f\n", 
	 or_->xmin, or_->xmax, 
	 or_->ymin, or_->ymax, 
	 or_->zmin, or_->zmax);
  printf("(vec) %f %f %f\n", vec->x, vec->y, vec->z);*/
  
  if ((vec->x > or_->xmin) && 
      ((vec->x < hx) || (fabs(vec->x - hx) < SMDZEROEPS)) &&
      (vec->y > or_->ymin) && 
      ((vec->y < hy) || (fabs(vec->y - hy) < SMDZEROEPS)) &&
      (vec->z > or_->zmin) && 
      ((vec->z < hz) || (fabs(vec->z - hz) < SMDZEROEPS))) {
    ornew->xmin = or_->xmin;
    ornew->xmax = hx;
    ornew->ymin = or_->ymin;
    ornew->ymax = hy;
    ornew->zmin = or_->zmin;
    ornew->zmax = hz;
    return 0;
  } else if ((vec->x > hx) && 
	     ((vec->x < or_->xmax) || (fabs(vec->x - or_->xmax) < SMDZEROEPS)) &&
	     (vec->y > or_->ymin) && 
	     ((vec->y < hy) || (fabs(vec->y - hy) < SMDZEROEPS)) &&
	     (vec->z > or_->zmin) && 
	     ((vec->z < hz) || (fabs(vec->z - hz) < SMDZEROEPS))) {
    ornew->xmin = hx;
    ornew->xmax = or_->xmax;
    ornew->ymin = or_->ymin;
    ornew->ymax = hy;
    ornew->zmin = or_->zmin;
    ornew->zmax = hz;
    return 1;
  } else if ((vec->x > or_->xmin) && 
	     ((vec->x < hx) || (fabs(vec->x - hx) < SMDZEROEPS)) &&
	     (vec->y > hy) && 
	     ((vec->y < or_->ymax) || (fabs(vec->y - or_->ymax) < SMDZEROEPS)) &&
	     (vec->z > or_->zmin) && 
	     ((vec->z < hz) || (fabs(vec->z - hz) < SMDZEROEPS))) {
    ornew->xmin = or_->xmin;
    ornew->xmax = hx;
    ornew->ymin = hy;
    ornew->ymax = or_->ymax;
    ornew->zmin = or_->zmin;
    ornew->zmax = hz;
    return 2;
  } else if ((vec->x > hx) && 
	     ((vec->x < or_->xmax) || (fabs(vec->x - or_->xmax) < SMDZEROEPS)) &&
	     (vec->y > hy) && 
	     ((vec->y < or_->ymax) || (fabs(vec->y - or_->ymax) < SMDZEROEPS)) &&
	     (vec->z > or_->zmin) && 
	     ((vec->z < hz) || (fabs(vec->z - hz) < SMDZEROEPS))) {
    ornew->xmin = hx;
    ornew->xmax = or_->xmax;
    ornew->ymin = hy;
    ornew->ymax = or_->ymax;
    ornew->zmin = or_->zmin;
    ornew->zmax = hz;
    return 3;
  } else if ((vec->x > or_->xmin) && 
	     ((vec->x < hx) || (fabs(vec->x - hx) < SMDZEROEPS)) &&
	     (vec->y > or_->ymin) && 
	     ((vec->y < hy) || (fabs(vec->y - hy) < SMDZEROEPS)) &&
	     (vec->z > hz) && 
	     ((vec->z < or_->zmax) || (fabs(vec->z - or_->zmax) < SMDZEROEPS))) {
    ornew->xmin = or_->xmin;
    ornew->xmax = hx;
    ornew->ymin = or_->ymin;
    ornew->ymax = hy;
    ornew->zmin = hz;
    ornew->zmax = or_->zmax;
    return 4;
  } else if ((vec->x > hx) && 
	     ((vec->x < or_->xmax) || (fabs(vec->x - or_->xmax) < SMDZEROEPS)) &&
	     (vec->y > or_->ymin) && 
	     ((vec->y < hy) || (fabs(vec->y - hy) < SMDZEROEPS)) &&
	     (vec->z > hz) && 
	     ((vec->z < or_->zmax) || (fabs(vec->z - or_->zmax) < SMDZEROEPS))) {
    ornew->xmin = hx;
    ornew->xmax = or_->xmax;
    ornew->ymin = or_->ymin;
    ornew->ymax = hy;
    ornew->zmin = hz;
    ornew->zmax = or_->zmax;
    return 5;
  } else if ((vec->x > or_->xmin) && 
	     ((vec->x < hx) || (fabs(vec->x - hx) < SMDZEROEPS)) &&
	     (vec->y > hy) && 
	     ((vec->y < or_->ymax) || (fabs(vec->y - or_->ymax) < SMDZEROEPS)) &&
	     (vec->z > hz) && 
	     ((vec->z < or_->zmax) || (fabs(vec->z - or_->zmax) < SMDZEROEPS))) {
    ornew->xmin = or_->xmin;
    ornew->xmax = hx;
    ornew->ymin = hy;
    ornew->ymax = or_->ymax;
    ornew->zmin = hz;
    ornew->zmax = or_->zmax;
    return 6;
  } else if ((vec->x > hx) && 
	     ((vec->x < or_->xmax) || (fabs(vec->x - or_->xmax) < SMDZEROEPS)) &&
	     (vec->y > hy) && 
	     ((vec->y < or_->ymax) || (fabs(vec->y - or_->ymax) < SMDZEROEPS)) &&
	     (vec->z > hz) && 
	     ((vec->z < or_->zmax) || (fabs(vec->z - or_->zmax) < SMDZEROEPS))) {
    ornew->xmin = hx;
    ornew->xmax = or_->xmax;
    ornew->ymin = hy;
    ornew->ymax = or_->ymax;
    ornew->zmin = hz;
    ornew->zmax = or_->zmax;
    return 7;
  } else {
    return 8;
  }
}

void subdivide_orange( ORange *or_, int i, ORange *ornew )
{
  double hx = (or_->xmin + or_->xmax) / 2.0;
  double hy = (or_->ymin + or_->ymax) / 2.0;
  double hz = (or_->zmin + or_->zmax) / 2.0;
  
  if (i == 0) {
    ornew->xmin = or_->xmin;
    ornew->xmax = hx;
    ornew->ymin = or_->ymin;
    ornew->ymax = hy;
    ornew->zmin = or_->zmin;
    ornew->zmax = hz;
  } else if (i == 1) {
    ornew->xmin = hx;
    ornew->xmax = or_->xmax;
    ornew->ymin = or_->ymin;
    ornew->ymax = hy;
    ornew->zmin = or_->zmin;
    ornew->zmax = hz;
  } else if (i == 2) {
    ornew->xmin = or_->xmin;
    ornew->xmax = hx;
    ornew->ymin = hy;
    ornew->ymax = or_->ymax;
    ornew->zmin = or_->zmin;
    ornew->zmax = hz;
  } else if (i == 3) {
    ornew->xmin = hx;
    ornew->xmax = or_->xmax;
    ornew->ymin = hy;
    ornew->ymax = or_->ymax;
    ornew->zmin = or_->zmin;
    ornew->zmax = hz;
  } else if (i == 4) {
    ornew->xmin = or_->xmin;
    ornew->xmax = hx;
    ornew->ymin = or_->ymin;
    ornew->ymax = hy;
    ornew->zmin = hz;
    ornew->zmax = or_->zmax;
  } else if (i == 5) {
    ornew->xmin = hx;
    ornew->xmax = or_->xmax;
    ornew->ymin = or_->ymin;
    ornew->ymax = hy;
    ornew->zmin = hz;
    ornew->zmax = or_->zmax;
  } else if (i == 6) {
    ornew->xmin = or_->xmin;
    ornew->xmax = hx;
    ornew->ymin = hy;
    ornew->ymax = or_->ymax;
    ornew->zmin = hz;
    ornew->zmax = or_->zmax;
  } else if (i == 7) {
    ornew->xmin = hx;
    ornew->xmax = or_->xmax;
    ornew->ymin = hy;
    ornew->ymax = or_->ymax;
    ornew->zmin = hz;
    ornew->zmax = or_->zmax;
  }
}

#define VOXOFFSET 1.01

OTree *init_otree( double xmax, double xmin, 
		   double ymax, double ymin, 
		   double zmax, double zmin )
{
  int level = 0;
  int id = 0;
  double xmid = (xmin + xmax) / 2.0; 
  double ymid = (ymin + ymax) / 2.0; 
  double zmid = (zmin + zmax) / 2.0; 
  double xlen = (xmax - xmin) / 2.0;
  double ylen = (ymax - ymin) / 2.0;
  double zlen = (zmax - zmin) / 2.0;
  OTree *head  = create_otreechild( NULL, level, id, 
				    xmid - xlen * VOXOFFSET,
				    xmid + xlen * VOXOFFSET,
				    ymid - ylen * VOXOFFSET,
				    ymid + ylen * VOXOFFSET,
				    zmid - zlen * VOXOFFSET,
				    zmid + zlen * VOXOFFSET );
  ++( head->on );
  head->maxotlevel = 5;
  
  return head;
}

//  void normalize_ppdpoint(Sppd *ppd)
//  {
//    int   i;
//    Sppnt *p;
  
//    display("normalize point ... ");
  
//    for (i = 0, p = (ppd->pnt + i); i < ppd->tptn; ++i, ++p) {
//      p->vec->x = (p->vec->x - ppd->center.x) * ppd->scale;
//      p->vec->y = (p->vec->y - ppd->center.y) * ppd->scale;
//      p->vec->z = (p->vec->z - ppd->center.z) * ppd->scale;
//    }
//    display("done.\n");
//  }

//
// vertex 、マ。、point 、ヒツミ、ケ、OPoint 、緲僂靴討い//
void ppdvertex_to_ppdotree( Sppd *ppd, OTree *octree )
{
  display("create vertex -> octree link: ...\n");

  //
  // level of octree subdivision
  //
  int i = 0; Spvt *vt;
  for ( i = 0, vt = ppd->spvt; vt != NULL; vt = vt->nxt, ++i ) {
    // vt がどこのセルに入るかを決める
    OTree *ot = octree;
    while ( ot->level < octree->maxotlevel ) {
      ORange or_;
      int num = decide_orange_child_number( &(vt->vec), &(ot->or_), &(or_) );
      if ( ot->child[num] == NULL ) {
	ot->child[num] = create_otreechild_or( ot, ot->level + 1,
					       octree->on++, &(or_) );
      }
      ot = ot->child[num];
    }
    // ot に vt を入れる
    (void) insert_otree_point( vt, ot );
  }

  display("create vertex -> octree link: done.\n");
}

void ppdface_to_ppdotree( Sppd *ppd, OTree *octree )
{
  display("create face -> octree link: ...\n");

  int i = 0; Spfc *fc;
  for ( i = 0, fc = ppd->spfc; fc != NULL; fc = fc->nxt, ++i ) {
    if ( !(i % 1000) ) display("face:\t %d/%d\n", i, ppd->fn );
    decide_oface_child( octree, fc );
  }
  display("\n");
  display("\toctree: %d\n", octree->on );
  display("create face -> octree link: done.\n");
}

void decide_oface_child( OTree *ot, Spfc *fc ) 
{
  OTree *top;
  if ( ot->top != NULL ) top = ot->top;
  else top = ot;
  if ( ot->level == top->maxotlevel ) {
    (void) insert_otree_face( fc, ot );
    return;
  }

  for ( int i = 0; i < 8; ++i ) {
    if ( ot->child[i] == NULL ) {
      ORange newor;
      subdivide_orange( &(ot->or_), i, &(newor) );
      ot->child[i] = create_otreechild_or( ot, ot->level + 1, 
					   top->on++, &(newor) );
      
    }
    // ちょっとでも cube にひっかかっていれば，子供を探索する
    if ( t_c_intersection( fc, ot->child[i] ) == INSIDE ) {
      decide_oface_child( ot->child[i], fc );
    }
  }    
}

void calc_point_to_face_distance( OTree *octree, Sppd *ppd )
{
  display("calc point->face distance: ...\n");

  point_to_face_distance_child( octree, ppd );
  
  display("calc point->face distance: done.\n");
}

void point_to_face_distance_child( OTree *octree, Sppd *ppd )
{
  OTree *top;
  if ( octree->top != NULL ) top = octree->top;
  else top = octree;
  
  if ( octree->level == top->maxotlevel ) {

    for ( OPoint *op = octree->sopnt; op != NULL; op = op->nxt ) {

      double min_dis = 0.0;
      int i = 0;

#if 0
      if ( octree->ofn != 0 ) {
	for ( OFace *of = octree->soface; of != NULL; of = of->nxt ) {

	  //display("\tfc %d\n", of->fc->no );
	  double dis = point_face_distance( op->vt, of->fc );

	  if ( i ) {
	    if ( dis < min_dis ) min_dis = dis;
	  } else min_dis = dis;

	  ++i;
	}
	display("vt %d fn %d mininum distance: %g\n",
		op->vt->no, octree->ofn, min_dis );
      } else {
#endif
	for ( Spfc *fc = ppd->spfc; fc != NULL; fc = fc->nxt ) {
	  double dis = point_face_distance( op->vt, fc );

	  if ( i ) {
	    if ( dis < min_dis ) min_dis = dis;
	  } else min_dis = dis;

	  ++i;
	}

	fprintf(stdout, "%d %g\n", op->vt->no, min_dis );
	display("vt %d fn %d mininum distance: %g\n",
		op->vt->no, octree->ofn, min_dis );

#if 0	
      }
#endif      
      
    }
  }

  for ( int i = 0; i < 8; ++i ) {
    if ( octree->child[i] != NULL ) {
      point_to_face_distance_child( octree->child[i], ppd );
    }
  }

}

// point to face distance
double point_face_distance( Spvt *vt, Spfc *fc )
{
  Vec *v0 = &(fc->sphe->vt->vec);
  Vec *v1 = &(fc->sphe->nxt->vt->vec);
  Vec *v2 = &(fc->sphe->nxt->nxt->vt->vec);
  //display("v0 %g %g %g\n", v0->x, v0->y, v0->z );
  
  Vec v0v2, v0v1, v1v2, pv1, pv2;
  V3Sub( v0, v2, &v0v2 );
  V3Sub( v0, v1, &v0v1 );
  V3Sub( v1, v2, &v1v2 );
  V3Sub( &(vt->vec), v1, &pv1 );
  V3Sub( &(vt->vec), v2, &pv2 );
  //display("pv1 %g %g %g\n", pv1.x, pv1.y, pv1.z );
  double X = MULTI * V3Dot( &v0v2, &v0v1);
  double Z = MULTI * V3Dot( &v0v2, &v1v2); 
  double Y = MULTI * V3Dot(&v1v2, &v0v1);
  double W = MULTI * V3Dot(&v1v2, &v1v2);
  double A = MULTI * V3Dot(&pv2, &v0v1);
  double B = MULTI * V3Dot(&pv2, &v1v2);
  //display("X %g Z %g Y %g W %g \n", X, Z, Y, W );
  
  // 垂線の足を求める
  double den = X * W - Z * Y;
  //display("den %g\n", den );
  if ( fabs(den) < SMDZEROEPS ) return 9999.9;

  double a = (W * A - Y * B) / den;
  double b = (X * B - Z * A) / den;
  double c = 1.0 - a - b;
  Vec vec;
  vec.x = a * v0->x + b * v1->x + c * v2->x;
  vec.y = a * v0->y + b * v1->y + c * v2->y;
  vec.z = a * v0->z + b * v1->z + c * v2->z;
  
  double dis = V3DistanceBetween2Points( &vec, &(vt->vec) );
  
  if (up(a) && up(b) && up(c)) { // (1)
    
    return dis;
    
  } else if (zero(a) && up(b) && up(c)) { // (2)
    
    a = 0.0;
    c = 1.0 - b;
    vec.x = a * v0->x + b * v1->x + c * v2->x;
    vec.y = a * v0->y + b * v1->y + c * v2->y;
    vec.z = a * v0->z + b * v1->z + c * v2->z;
    return V3DistanceBetween2Points( &vec, &(vt->vec) );
    
    
  } else if (up(a) && zero(b) && up(c)) { // (3)

    b = 0.0;
    a = 1.0 - c;
    vec.x = a * v0->x + b * v1->x + c * v2->x;
    vec.y = a * v0->y + b * v1->y + c * v2->y;
    vec.z = a * v0->z + b * v1->z + c * v2->z;
    return V3DistanceBetween2Points(&vec, &(vt->vec));
    
    
  } else if (up(a) && up(b) && zero(c)) { // (4)

    c = 0.0;
    b = 1.0 - a;
    vec.x = a * v0->x + b * v1->x + c * v2->x;
    vec.y = a * v0->y + b * v1->y + c * v2->y;
    vec.z = a * v0->z + b * v1->z + c * v2->z;
    return V3DistanceBetween2Points(&vec, &(vt->vec));
    
  } else if (zdown(a) && zdown(b) && up(c)) { // (8)

    return V3DistanceBetween2Points(v2, &(vt->vec));
    
  } else if (up(a) && zdown(b) && zdown(c)) { // (9)

    return V3DistanceBetween2Points(v0, &(vt->vec));
    
  } else if (zdown(a) && up(b) && zdown(c)) { // (10)

    return V3DistanceBetween2Points(v1, &(vt->vec));
    
  } else if (down(a) && up(b) && up(c)) { // (5)

    a = 0.0;
    b = B / W;
    c = 1.0 - b;
    
    if (down(c)) {
      
      return V3DistanceBetween2Points(v1, &(vt->vec));
      
    } else if (down(b)) {
      
      return V3DistanceBetween2Points(v2, &(vt->vec));
      
    } else {
      
      vec.x = b * v1->x + c * v2->x;
      vec.y = b * v1->y + c * v2->y;
      vec.z = b * v1->z + c * v2->z;
      return V3DistanceBetween2Points(&vec, &(vt->vec));
      
    }
    
  } else if (up(a) && down(b) && up(c)) { // (6)
    
    b = 0.0;
    a = V3Dot(&v0v2, &v0v2) / V3Dot(&pv2, &v0v2);
    c = 1.0 - a;

    if (down(c)) {

      return V3DistanceBetween2Points(v0, &(vt->vec));
      
    } else if (down(a)) {
      
      return V3DistanceBetween2Points(v2, &(vt->vec));
      
    } else {

      vec.x = a * v0->x + c * v2->x;
      vec.y = a * v0->y + c * v2->y;
      vec.z = a * v0->z + c * v2->z;
      return V3DistanceBetween2Points(&vec, &(vt->vec));
      
    }
  } else if (up(a) && up(b) && down(c)) { // (7)
    
    c = 0.0;
    a = V3Dot(&v0v1, &v0v1) / V3Dot(&pv1, &v0v1);
    b = 1.0 - a;

    if (down(a)) {

      return V3DistanceBetween2Points(v1, &(vt->vec));
      
    } else if (down(b)) {

      return V3DistanceBetween2Points(v0, &(vt->vec));
      
    } else {
      
      vec.x = a * v0->x + b * v1->x;
      vec.y = a * v0->y + b * v1->y;
      vec.z = a * v0->z + b * v1->z;
      return V3DistanceBetween2Points(&vec, &(vt->vec));
    }
  }
  
  return 9999.9;
}

BOOL zup( double a )
{
  if ( (fabs(a-0.0) < SMDZEROEPS) || (a > 0.0) )
    return TRUE;
  else
    return FALSE;
}

BOOL zdown( double a )
{
  if ( (fabs(a-0.0) < SMDZEROEPS) || (a < 0.0) )
    return TRUE;
  else
    return FALSE;
}

BOOL zero( double a )
{
  if ( fabs(a-0.0) < SMDZEROEPS )
    return TRUE;
  else
    return FALSE;
}

BOOL up( double a )
{
  if ( a > 0.0 )
    return TRUE;
  else
    return FALSE;
}

BOOL down( double a )
{
  if ( a < 0.0 )
    return TRUE;
  else
    return FALSE;
}

