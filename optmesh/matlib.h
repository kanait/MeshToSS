//
// matlib.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _MATLIB_H
#define _MATLIB_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Vmtx NewMatrix(int, int);
extern void FreeMatrix(Vmtx, int);
extern void TransposeMatrix(Vmtx, Vmtx, int, int);
extern void MultMatrix(Vmtx, Vmtx, Vmtx, int, int, int);
extern void rot_x(double, Vec *);
extern void rot_y(double, Vec *);
extern void rot_z(double, Vec *);
extern void rot_inv_x(double, Vec *);
extern void rot_inv_y(double, Vec *);
extern void rot_inv_z(double, Vec *);
extern void rot_all(double, double, double, Vec *);
extern void rot_vec(double, Vec *, Vec *);
extern void MultMatrix4(double *, double *, double *);
extern void MakeRotXCoSi(double, double, double *);
extern void MakeRotX(double, double *);
extern void MakeRotY(double, double *);
extern void MakeRotZ(double, double *);
extern void MakeRotVec(double, double, Vec *, double *);
extern void MakeRotInvVec(double, double, Vec *, double *);
extern void MultMatrixVec(double *, Vec *, Vec *);
extern void MultMatrixVecUpdate(double *, Vec *);
extern void EqualMatrix4(double *, double *);
extern void MakeRotVecAngle(double, Vec *, double *);
extern void MakeRotInvVecAngle(double, Vec *, double *);
extern void iMakeRotVec(double, Vec *, double *);
extern void iMakeRotInvVec(double, Vec *, double *);
extern void iMultMatrixVec(double *, Vec *);
extern void initialize_matrix4(double *);
extern void add_matrix4(double *, double *, double *);
extern void equal_matrix4(double *, double *);
extern void multivalue_matrix4(double *, double, double *);

#ifdef __cplusplus
}
#endif

#endif // _MATLIB_H  
