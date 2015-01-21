//
// tricube.cxx
// Triangle-Cube intersection functions
// 
// Copyright (c) 2000 Takashi Kanai; All rights reserved. 
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>
#include "smd.h"
#include "octree.h"
#include "tricube.h"

#define SIGN3( A ) (((A).x<0)?4:0 | ((A).y<0)?2:0 | ((A).z<0)?1:0)
#define CROSS( A, B, C ) { \
			     (C).x =  (A).y * (B).z - (A).z * (B).y; \
			     (C).y = -(A).x * (B).z + (A).z * (B).x; \
			     (C).z =  (A).x * (B).y - (A).y * (B).x; \
			 }
#define SUB( A, B, C ) { \
			   (C).x =  (A).x - (B).x; \
			   (C).y =  (A).y - (B).y; \
			   (C).z =  (A).z - (B).z; \
		       }
#define LERP( A, B, C) ((B)+(A)*((C)-(B)))
#define MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))

//___________________________________________________________________________

// Which of the six face-plane(s) is point P TCOUTSIDE of?

long face_plane( Vec p )
{
  long outcode;

  outcode = 0;
  if (p.x >  .5) outcode |= 0x01;
  if (p.x < -.5) outcode |= 0x02;
  if (p.y >  .5) outcode |= 0x04;
  if (p.y < -.5) outcode |= 0x08;
  if (p.z >  .5) outcode |= 0x10;
  if (p.z < -.5) outcode |= 0x20;
  return outcode;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Which of the twelve edge plane(s) is point P TCOUTSIDE of?

long bevel_2d( Vec p )
{
  long outcode;

  outcode = 0;
  if ( p.x + p.y > 1.0) outcode |= 0x001;
  if ( p.x - p.y > 1.0) outcode |= 0x002;
  if (-p.x + p.y > 1.0) outcode |= 0x004;
  if (-p.x - p.y > 1.0) outcode |= 0x008;
  if ( p.x + p.z > 1.0) outcode |= 0x010;
  if ( p.x - p.z > 1.0) outcode |= 0x020;
  if (-p.x + p.z > 1.0) outcode |= 0x040;
  if (-p.x - p.z > 1.0) outcode |= 0x080;
  if ( p.y + p.z > 1.0) outcode |= 0x100;
  if ( p.y - p.z > 1.0) outcode |= 0x200;
  if (-p.y + p.z > 1.0) outcode |= 0x400;
  if (-p.y - p.z > 1.0) outcode |= 0x800;

  return outcode;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Which of the eight corner plane(s) is point P TCOUTSIDE of?

long bevel_3d( Vec p )
{
  long outcode;

  outcode = 0;
  if (( p.x + p.y + p.z) > 1.5) outcode |= 0x01;
  if (( p.x + p.y - p.z) > 1.5) outcode |= 0x02;
  if (( p.x - p.y + p.z) > 1.5) outcode |= 0x04;
  if (( p.x - p.y - p.z) > 1.5) outcode |= 0x08;
  if ((-p.x + p.y + p.z) > 1.5) outcode |= 0x10;
  if ((-p.x + p.y - p.z) > 1.5) outcode |= 0x20;
  if ((-p.x - p.y + p.z) > 1.5) outcode |= 0x40;
  if ((-p.x - p.y - p.z) > 1.5) outcode |= 0x80;

  return outcode;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Test the point "alpha" of the way from P1 to P2
// See if it is on a face of the cube             
// Consider only faces in "mask"                  

long check_point(Vec p1, Vec p2, float alpha, long mask)
{
  Vec plane_point;

  plane_point.x = LERP(alpha, p1.x, p2.x);
  plane_point.y = LERP(alpha, p1.y, p2.y);
  plane_point.z = LERP(alpha, p1.z, p2.z);
  
  return (face_plane(plane_point) & mask);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Compute intersection of P1 --> P2 line segment with face planes
// Then test intersection point to see if it is on cube face       
// Consider only face planes in "outcode_diff"                     
// Note: Zero bits in "outcode_diff" means face line is TCOUTSIDE of 

long check_line( Vec p1, Vec p2, long outcode_diff )
{

  if ((0x01 & outcode_diff) != 0)
    if (check_point(p1,p2,( .5-p2.x)/(p1.x-p2.x),0x3e) == TCINSIDE) return TCINSIDE;
  if ((0x02 & outcode_diff) != 0)
    if (check_point(p1,p2,(-.5-p2.x)/(p1.x-p2.x),0x3d) == TCINSIDE) return TCINSIDE;
  if ((0x04 & outcode_diff) != 0) 
    if (check_point(p1,p2,( .5-p2.y)/(p1.y-p2.y),0x3b) == TCINSIDE) return TCINSIDE;
  if ((0x08 & outcode_diff) != 0) 
    if (check_point(p1,p2,(-.5-p2.y)/(p1.y-p2.y),0x37) == TCINSIDE) return TCINSIDE;
  if ((0x10 & outcode_diff) != 0) 
    if (check_point(p1,p2,( .5-p2.z)/(p1.z-p2.z),0x2f) == TCINSIDE) return TCINSIDE;
  if ((0x20 & outcode_diff) != 0) 
    if (check_point(p1,p2,(-.5-p2.z)/(p1.z-p2.z),0x1f) == TCINSIDE) return TCINSIDE;

  return TCOUTSIDE;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Test if 3D point is TCINSIDE 3D triangle

long point_triangle_intersection( Vec p, Spfc *fc )
{
  Vec fvec[3];
  int i = 0;
  Sphe *he = fc->sphe;
  do {
    fvec[i].x = he->vt->vec.x;
    fvec[i].y = he->vt->vec.y;
    fvec[i].z = he->vt->vec.z;
    ++i;
  } while ( (he = he->nxt) != fc->sphe );
  
  // First, a quick bounding-box test:                               
  // If P is TCOUTSIDE triangle bbox, there cannot be an intersection.

  if ( p.x > MAX3(fvec[0].x, fvec[1].x, fvec[2].x) ) return TCOUTSIDE;
  if ( p.y > MAX3(fvec[0].y, fvec[1].y, fvec[2].y) ) return TCOUTSIDE;
  if ( p.z > MAX3(fvec[0].z, fvec[1].z, fvec[2].z) ) return TCOUTSIDE;
  if ( p.x < MIN3(fvec[0].x, fvec[1].x, fvec[2].x) ) return TCOUTSIDE;
  if ( p.y < MIN3(fvec[0].y, fvec[1].y, fvec[2].y) ) return TCOUTSIDE;
  if ( p.z < MIN3(fvec[0].z, fvec[1].z, fvec[2].z) ) return TCOUTSIDE;

  // For each triangle side, make a vector out of it by subtracting vertexes;
  // make another vector from one vertex to point P.                         
  // The crossproduct of these two vectors is orthogonal to both and the     
  // signs of its X,Y,Z components indicate whether P was to the TCINSIDE or   
  // to the TCOUTSIDE of this triangle side.                                   

  long sign12,sign23,sign31;
  Vec vect12,vect23,vect31,vect1h,vect2h,vect3h;
  Vec cross12_1p,cross23_2p,cross31_3p;
  SUB(fvec[0], fvec[1], vect12);
  SUB(fvec[0],    p, vect1h);
  CROSS(vect12, vect1h, cross12_1p);
  sign12 = SIGN3(cross12_1p);	// Extract X,Y,Z signs as 0...7 integer

  SUB(fvec[1], fvec[2], vect23);
  SUB(fvec[1], p, vect2h);
  CROSS(vect23, vect2h, cross23_2p);
  sign23 = SIGN3(cross23_2p);

  SUB(fvec[2], fvec[0], vect31);
  SUB(fvec[2], p, vect3h);
  CROSS(vect31, vect3h, cross31_3p);
  sign31 = SIGN3(cross31_3p);

  // If all three crossproduct vectors agree in their component signs,
  // then the point must be TCINSIDE all three.                         
  // P cannot be TCOUTSIDE all three sides simultaneously.              

  if ( (sign12 == sign23) && (sign23 == sign31) )
    return TCINSIDE;
  else
    return TCOUTSIDE;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// ********************************************
// This is the main algorithm procedure.      
// Triangle t is compared with a unit cube,   
// centered on the origin.                    
// It returns TCINSIDE (0) or TCOUTSIDE(1) if t   
// intersects or does not intersect the cube. 
// ********************************************

int t_c_intersection( Spfc *fc, OTree *ot )
{
  long  v1_test,v2_test,v3_test;
  float d;
  Vec   vect12,vect13,norm;
  Vec   hitpp,hitpn,hitnp,hitnn;
  
  Vec   center;
  center.x = (ot->or_.xmax + ot->or_.xmin) / 2.0;
  center.y = (ot->or_.ymax + ot->or_.ymin) / 2.0;
  center.z = (ot->or_.zmax + ot->or_.zmin) / 2.0;
  double scalex = 1.0 / (ot->or_.xmax - ot->or_.xmin);
  double scaley = 1.0 / (ot->or_.ymax - ot->or_.ymin);
  double scalez = 1.0 / (ot->or_.zmax - ot->or_.zmin);
  
  Vec fvec[3];
  int i = 0;
  Sphe *he = fc->sphe;
  do {
    fvec[i].x = (he->vt->vec.x - center.x) * scalex;
    fvec[i].y = (he->vt->vec.y - center.y) * scaley;
    fvec[i].z = (he->vt->vec.z - center.z) * scalez;
    ++i;
  } while ( (he = he->nxt) != fc->sphe );
  
  // First compare all three vertexes with all six face-planes
  // If any vertex is TCINSIDE the cube, return immediately!    

  if ( (v1_test = face_plane(fvec[0])) == TCINSIDE ) return TCINSIDE;
  if ( (v2_test = face_plane(fvec[1])) == TCINSIDE ) return TCINSIDE;
  if ( (v3_test = face_plane(fvec[2])) == TCINSIDE ) return TCINSIDE;

  // If all three vertexes were TCOUTSIDE of one or more face-planes,
  // return immediately with a trivial rejection!                  

  if ( (v1_test & v2_test & v3_test) != 0 ) return TCOUTSIDE;

  // Now do the same trivial rejection test for the 12 edge planes

  v1_test |= bevel_2d(fvec[0]) << 8; 
  v2_test |= bevel_2d(fvec[1]) << 8; 
  v3_test |= bevel_2d(fvec[2]) << 8;
  if ( (v1_test & v2_test & v3_test) != 0 ) return TCOUTSIDE;

  // Now do the same trivial rejection test for the 8 corner planes

  v1_test |= bevel_3d(fvec[0]) << 24; 
  v2_test |= bevel_3d(fvec[1]) << 24; 
  v3_test |= bevel_3d(fvec[2]) << 24; 
  if ( (v1_test & v2_test & v3_test) != 0 ) return TCOUTSIDE;

  // If vertex 1 and 2, as a pair, cannot be trivially rejected
  // by the above tests, then see if the v1-->v2 triangle edge 
  // intersects the cube.  Do the same for v1-->v3 and v2-->v3.
  // Pass to the intersection algorithm the "OR" of the outcode
  // bits, so that only those cube faces which are spanned by  
  // each triangle edge need be tested.                        

  if ( (v1_test & v2_test) == 0 )
    if ( check_line(fvec[0],fvec[1],v1_test|v2_test) == TCINSIDE )
      return TCINSIDE;
  if ( (v1_test & v3_test) == 0 )
    if ( check_line(fvec[0],fvec[2],v1_test|v3_test) == TCINSIDE )
      return TCINSIDE;
  if ( (v2_test & v3_test) == 0 )
    if ( check_line(fvec[1],fvec[2],v2_test|v3_test) == TCINSIDE )
      return TCINSIDE;

  // By now, we know that the triangle is not off to any side,     
  // and that its sides do not penetrate the cube.  We must now    
  // test for the cube intersecting the interior of the triangle.  
  // We do this by looking for intersections between the cube      
  // diagonals and the triangle...first finding the intersection   
  // of the four diagonals with the plane of the triangle, and     
  // then if that intersection is TCINSIDE the cube, pursuing        
  // whether the intersection point is TCINSIDE the triangle itself. 

  // To find plane of the triangle, first perform crossproduct on
  // two triangle side vectors to compute the normal vector.     
                                
  SUB(fvec[0],fvec[1],vect12);
  SUB(fvec[0],fvec[2],vect13);
  CROSS(vect12,vect13,norm);

  // The normal vector "norm" X,Y,Z components are the coefficients
  // of the triangles AX + BY + CZ + D = 0 plane equation.  If we  
  // solve the plane equation for X=Y=Z (a diagonal), we get       
  // -D/(A+B+C) as a metric of the distance from cube center to the
  // diagonal/plane intersection.  If this is between -0.5 and 0.5,
  // the intersection is TCINSIDE the cube.  If so, we continue by   
  // doing a point/triangle intersection.                          
  // Do this for all four diagonals.                               

  d = norm.x * fvec[0].x + norm.y * fvec[0].y + norm.z * fvec[0].z;
  hitpp.x = hitpp.y = hitpp.z = d / (norm.x + norm.y + norm.z);
  if (fabs(hitpp.x) <= 0.5)
    if (point_triangle_intersection(hitpp, fc) == TCINSIDE) return TCINSIDE;
  hitpn.z = -(hitpn.x = hitpn.y = d / (norm.x + norm.y - norm.z));
  if (fabs(hitpn.x) <= 0.5)
    if (point_triangle_intersection(hitpn, fc) == TCINSIDE) return TCINSIDE;
  hitnp.y = -(hitnp.x = hitnp.z = d / (norm.x - norm.y + norm.z));
  if (fabs(hitnp.x) <= 0.5)
    if (point_triangle_intersection(hitnp, fc) == TCINSIDE) return TCINSIDE;
  hitnn.y = hitnn.z = -(hitnn.x = d / (norm.x - norm.y - norm.z));
  if (fabs(hitnn.x) <= 0.5)
    if (point_triangle_intersection(hitnn, fc) == TCINSIDE) return TCINSIDE;

  // No edge touched the cube; no cube diagonal touched the triangle.
  // We're done...there was no intersection.                         

  return OUTSIDE;

}
