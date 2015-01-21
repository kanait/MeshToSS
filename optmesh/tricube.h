//
// tricube.h
// Triangle-Cube intersection functions
// 
// Copyright (c) 2000 Takashi Kanai; All rights reserved. 
//

#ifndef _TRICUBE_H
#define _TRICUBE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "octree.h"
  
extern long face_plane(Vec);
extern long bevel_2d(Vec);
extern long bevel_3d(Vec);
extern long check_point(Vec, Vec, float, long);
extern long check_line(Vec, Vec, long);
extern long point_triangle_intersection(Vec, Spfc *);
extern int t_c_intersection(Spfc *, OTree *);

// defines
#define TCINSIDE      0
#define TCOUTSIDE     1

#ifdef __cplusplus
}
#endif

#endif // _TRICUBE_H
  
