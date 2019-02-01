//
// intsec.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _INTSEC_H
#define _INTSEC_H

#ifdef __cplusplus
extern "C" {
#endif

extern BOOL isLeftSide(Vec2d *, Vec2d *, Vec2d *);
extern BOOL isLeftSidePivot(Vec2d *, Vec2d *, Vec2d *);
extern BOOL isLeftSide3D( Vec *, Vec *, Vec *, Vec * );
extern BOOL isOnLine3D( Vec *, Vec *, Vec *, Vec * );
extern void swap_sign(int *, int *);
extern void swap_vec2d(Vec2d *, Vec2d *);
extern double isLeftVal(Vec2d *, Vec2d *, Vec2d *);
extern BOOL isLineSegmentCrossing(Vec2d *, Vec2d *, Vec2d *, Vec2d *);
extern void LineIntersectionPoint(Vec2d *, Vec2d *, Vec2d *, Vec2d *, Vec2d *);
extern int IntsecLines(Vec2d *, Vec2d *, Vec2d *, Vec2d *);
extern BOOL IntsecLinesParam(Vec2d *, Vec2d *, Vec2d *, Vec2d *, double *, double * );
extern BOOL IntsecLinesParam3D(Vec *, Vec *, Vec *, Vec *, double *, double * );
extern BOOL isParamsInFace( Spfc *, double, double );
extern double point_line_distance_param(Vec2d *, Vec2d *, Vec2d *, double *);
extern double point_line_distance(Vec2d *, Vec2d *, Vec2d *);
extern int vertex_line(Vec2d *, Vec2d *, Vec2d *);
extern BOOL isParallelLine( Vec *, Vec *, Vec *, Vec * );
extern BOOL isParamsOnLine( Vec2d *, Vec2d *, Vec2d * );

#ifdef __cplusplus
}
#endif

#endif // _INTSEC_H
