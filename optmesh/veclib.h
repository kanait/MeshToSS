//
// veclib.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _VECLIB_H
#define _VECLIB_H

#ifdef __cplusplus
extern "C" {
#endif

extern double V2SquaredLength(Vec2d *);
extern double V2Length(Vec2d *);
extern Vec2d *V2Negate(Vec2d *);
extern Vec2d *V2Normalize(Vec2d *);
extern Vec2d *V2Scale(Vec2d *, double);
extern Vec2d *V2Add(Vec2d *, Vec2d *, Vec2d *);
extern Vec2d *V2Sub(Vec2d *, Vec2d *, Vec2d *);
extern double V2Dot(Vec2d *, Vec2d *);
extern Vec2d *V2Combine(Vec2d *, Vec2d *, Vec2d *, double, double);
extern Vec2d *V2Mul(Vec2d *, Vec2d *, Vec2d *);
extern double V2DistanceBetween2Points(Vec2d *, Vec2d *);
extern Vec2d *V2MakePerpendicular(Vec2d *, Vec2d *);
extern Vec2d *V2MulPointByProjMatrix(Vec2d *, Matrix3 *, Vec2d *);
extern double V2TriArea( Vec2d *, Vec2d *, Vec2d * );
extern Matrix3 *V2MatMul(Matrix3 *, Matrix3 *, Matrix3 *);
extern Matrix3 *TransposeMatrix3(Matrix3 *, Matrix3 *);
extern double V2Cosign(Vec2d *, Vec2d *);
extern double V2Sign(Vec2d *, Vec2d *);
extern int V2Signi(Vec2d *, Vec2d *);
extern void V2IntersectParam( Vec2d *, Vec2d *, Vec2d *, Vec2d *, double *, double * );
extern double V3SquaredLength(Vec *);
extern double V3Length(Vec *);
extern Vec *V3Negate(Vec *);
extern Vec *V3Equal(Vec *, Vec *);
extern Vec *V3Normalize(Vec *);
extern Vec *V3Scale(Vec *, double);
extern Vec *V3Scr(double, Vec *, Vec *);
extern Vec *V3Add(Vec *, Vec *, Vec *);
extern Vec *OV3Sub(Vec *, Vec *, Vec *);
extern Vec *V3Sub(Vec *, Vec *, Vec *);
extern double V3Dot(Vec *, Vec *);
extern double OV3Dot(Vec *, Vec *);
extern Vec *V3Combine(Vec *, Vec *, Vec *, double, double);
extern Vec *V3Mul(Vec *, Vec *, Vec *);
extern double V3DistanceBetween2Points(Vec *, Vec *);
extern double OV3DistanceBetween2Points(Vec *, Vec *);
extern Vec *V3Cross(Vec *, Vec *, Vec *);
extern double V3Triple(Vec *, Vec *, Vec *);
extern double V3Cosign(Vec *, Vec *);
extern double V3SquaredDistanceBetween2Points(Vec *, Vec *);
extern Vec *V3MulPointByMatrix(Vec *, Matrix3 *, Vec *);
extern Vec *V3MulPointByProjMatrix(Vec *, Matrix4 *, Vec *);
extern Matrix4 *V3MatMul(Matrix4 *, Matrix4 *, Matrix4 *);

#ifdef __cplusplus
}
#endif

#endif // _VECLIB_H
