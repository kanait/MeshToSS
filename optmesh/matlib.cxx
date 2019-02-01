//
// matlib.cxx
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

// #include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#ifndef WIN32
//#include <stdio.h>
//#endif

#include <math.h>
#include <stdlib.h>
#include "smd.h"
#include "veclib.h"

#include "matlib.h"

Vmtx NewMatrix(int cols, int rows)
{
  int   i;
  Vmtx  newM;
  newM = (double **) malloc(rows * sizeof(double *));
  for (i = 0; i < rows; i++)
    newM[i] = (double *) malloc(cols * sizeof(double));
  return newM;
}

void FreeMatrix(Vmtx mat, int rows)
{
  int   i;
  for (i = 0; i < rows; i++)
    free(mat[i]);
  free(mat);
}

void TransposeMatrix(Vmtx inM, Vmtx outM, int cols, int rows)
{
  int   tempI, tempJ;
  for (tempI = 0; tempI < rows; tempI++)
    for (tempJ = 0; tempJ < cols; tempJ++)
      outM[tempI][tempJ] = inM[tempJ][tempI];
}

void MultMatrix(Vmtx firstM, Vmtx secondM, Vmtx outM, int firstrows,
	   int cols, int secondcols)
{
  int   i, j, k;
  double sum;

  for (i = 0; i < secondcols; i++)
    for (j = 0; j < firstrows; j++) {
      sum = 0.0;
      for (k = 0; k < cols; k++)
	sum += firstM[j][k] * secondM[k][i];
      outM[j][i] = sum;
    }
}

void rot_x(double val, Vec *vec)
{
  double v[2];
  double rd, co, si;
  
  v[0] = vec->y;
  v[1] = vec->z;

  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  vec->y =  co * v[0] + si * v[1];
  vec->z = -si * v[0] + co * v[1];
}

void rot_y(double val, Vec *vec)
{
  double v[2];
  double rd, co, si;
  
  v[0] = vec->z;
  v[1] = vec->x;

  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  vec->z =  co * v[0] + si * v[1];
  vec->x = -si * v[0] + co * v[1];
}

void rot_z(double val, Vec *vec)
{
  double v[2];
  double rd, co, si;
  
  v[0] = vec->x;
  v[1] = vec->y;

  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  vec->x =  co * v[0] + si * v[1];
  vec->y = -si * v[0] + co * v[1];
}

void rot_inv_x(double val, Vec *vec)
{
  double v[2];
  double rd, co, si;
  
  v[0] = vec->y;
  v[1] = vec->z;

  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  vec->y = co * v[0] - si * v[1];
  vec->z = si * v[0] + co * v[1];
}

void rot_inv_y(double val, Vec *vec)
{
  double v[2];
  double rd, co, si;
  
  v[0] = vec->z;
  v[1] = vec->x;

  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  vec->z = co * v[0] - si * v[1];
  vec->x = si * v[0] + co * v[1];
}

void rot_inv_z(double val, Vec *vec)
{
  double v[2];
  double rd, co, si;
  
  v[0] = vec->x;
  v[1] = vec->y;

  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  vec->x = co * v[0] - si * v[1];
  vec->y = si * v[0] + co * v[1];
}

void rot_all(double xval, double yval, double zval, Vec *vec)
{
  double v[3];
  double xrd, yrd, zrd;
  double cx, cy, cz, sx, sy, sz;
  
  v[0] = vec->x;
  v[1] = vec->y;
  v[2] = vec->z;

  xrd = xval * SMDPI / 180.0;
  yrd = yval * SMDPI / 180.0;
  zrd = zval * SMDPI / 180.0;
  cx = cos(xrd); sx = sin(xrd);
  cy = cos(yrd); sy = sin(yrd);
  cz = cos(zrd); sz = sin(zrd); 

  vec->x = cy * cz * v[0] - cy * sz * v[1] + sy * v[2];
  vec->y = (sx * sy * cz + cx * sz) * v[0] + (-1.0 * sx * sy * sz + cx * cz) * v[1]
    - sx * cy * v[2];
  vec->z = (-1.0 * cx * sy * cz + sx * sz) * v[0] + (cx * sy * sz + sx * cz) * v[1]
    + cx * cy * v[2];
}

void rot_vec(double angle, Vec *axis, Vec *vec)
{
  double v[3], m[9];
  double rd, co, si, mco;
  double ax2, ay2, az2, axay, axaz, ayaz;
  Vec    a;
  
  v[0] = vec->x;
  v[1] = vec->y;
  v[2] = vec->z;

  rd = angle * SMDPI / 180.0;
  co = cos(rd); si = sin(rd); mco = 1.0 - co;
  a.x = axis->x; a.y = axis->y; a.z = axis->z;
  V3Normalize(&a);

  ax2 = a.x * a.x;
  ay2 = a.y * a.y;
  az2 = a.z * a.z;
  axay = a.x * a.y;
  axaz = a.x * a.z;
  ayaz = a.y * a.z;
  
  m[0] = ax2 + (1.0 - ax2) * co;
  m[1] = axay * mco + a.z * si;
  m[2] = axaz * mco - a.y * si;
  m[3] = axay * mco - a.z * si;
  m[4] = ay2 + (1.0 - ay2) * co;
  m[5] = ayaz * mco + a.x * si;
  m[6] = axaz * mco + a.y * si;
  m[7] = ayaz * mco - a.x * si;
  m[8] = az2 + (1.0 - az2) * co;
  
  vec->x = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
  vec->y = m[3] * v[0] + m[4] * v[1] + m[5] * v[2];
  vec->z = m[6] * v[0] + m[7] * v[1] + m[8] * v[2];
}

void MultMatrix4(double *m, double *mul, double *sum)
{
  int   i;
  
  for (i = 0; i < 4; i++) {
    sum[4*i]   = m[0] * mul[4*i] + m[4] * mul[4*i+1] + m[8]  * mul[4*i+2] + m[12] * mul[4*i+3];
    sum[4*i+1] = m[1] * mul[4*i] + m[5] * mul[4*i+1] + m[9]  * mul[4*i+2] + m[13] * mul[4*i+3];
    sum[4*i+2] = m[2] * mul[4*i] + m[6] * mul[4*i+1] + m[10] * mul[4*i+2] + m[14] * mul[4*i+3];
    sum[4*i+3] = m[3] * mul[4*i] + m[7] * mul[4*i+1] + m[11] * mul[4*i+2] + m[15] * mul[4*i+3];
  }
}

void MakeRotXCoSi(double co, double si, double *m)
{
  m[0] = 1.0; m[4] = 0.0; m[8]  = 0.0; m[12] = 0.0;
  m[1] = 0.0; m[5] = co;  m[9]  = -si; m[13] = 0.0;
  m[2] = 0.0; m[6] = si;  m[10] =  co; m[14] = 0.0;
  m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
}

void MakeRotX(double val, double *m)
{
  double rd, co, si;
  
  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  // display("(matx) rad %g co %g si %g\n", rd, co, si );
  m[0] = 1.0; m[4] = 0.0; m[8]  = 0.0; m[12] = 0.0;
  m[1] = 0.0; m[5] = co;  m[9]  = -si; m[13] = 0.0;
  m[2] = 0.0; m[6] = si;  m[10] =  co; m[14] = 0.0;
  m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
//   for ( int i = 0; i < 16; i+=4 ) 
//     display("mx(b) %g %g %g %g\n", m[i], m[i+1], m[i+2], m[i+3] );
}

void MakeRotY(double val, double *m)
{
  double rd, co, si;
  
  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  m[0] = co ; m[4] = 0.0; m[8]  = si;  m[12] = 0.0;
  m[1] = 0.0; m[5] = 1.0; m[9]  = 0.0; m[13] = 0.0;
  m[2] = -si; m[6] = 0.0; m[10] =  co; m[14] = 0.0;
  m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
}

void MakeRotZ(double val, double *m)
{
  double rd, co, si;
  
  rd = val * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  m[0] = co;  m[4] = -si; m[8]  = 0.0; m[12] = 0.0;
  m[1] = si;  m[5] =  co; m[9]  = 0.0; m[13] = 0.0;
  m[2] = 0.0; m[6] = 0.0; m[10] = 1.0; m[14] = 0.0;
  m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
}

// make rotation matrix according to an axis
// instead an angle, we input cos(angle), sin(angle) for efficiency
// reference:
//   "Computer Graphics" by D. F. Rogers and J. A. Adams, 
//  , translated in Japanese by F. Yamaguchi, pp.60-64
// CATION: the rotation matrix described below is different from
//         the original book. It is really R^{T}.
// 
void MakeRotVec( double co, double si, Vec *axis, double *m )
{
  double mco;
  double n1, n2, n3, n1_2, n2_2, n3_2, n1n2, n2n3, n1n3;
  Vec    a;
  
  // rd = angle * SMDPI / 180.0;
  // co = cos(rd); si = sin(rd);
  mco = 1.0 - co;
  a.x = axis->x; a.y = axis->y; a.z = axis->z;
  V3Normalize( &a );

  n1 = a.x;
  n2 = a.y;
  n3 = a.z;
  n1_2 = n1 * n1;
  n2_2 = n2 * n2;
  n3_2 = n3 * n3;
  n1n2 = n1 * n2;
  n2n3 = n2 * n3;
  n1n3 = n1 * n3;
  
  m[0]  = n1_2 + (1.0 - n1_2) * co;
  m[4]  = n1n2 * mco + n3 * si;
  m[8]  = n1n3 * mco - n2 * si;
  m[12]  = 0.0;
  
  m[1]  = n1n2 * mco - n3 * si;
  m[5]  = n2_2 + (1.0 - n2_2) * co;
  m[9]  = n2n3 * mco + n1 * si;
  m[13]  = 0.0;
  
  m[2]  = n1n3 * mco + n2 * si;
  m[6]  = n2n3 * mco - n1 * si;
  m[10] = n3_2 + (1.0 - n3_2) * co;
  m[14] = 0.0;
  
  m[3] = 0.0;
  m[7] = 0.0;
  m[11] = 0.0;
  m[15] = 1.0;
}

void MakeRotInvVec( double co, double si, Vec *axis, double *m )
{
  double mco;
  double ax2, ay2, az2, axay, axaz, ayaz;
  Vec    a;
  
  mco = 1.0 - co;
  a.x = axis->x; a.y = axis->y; a.z = axis->z;
  V3Normalize( &a );

  ax2  = a.x * a.x;
  ay2  = a.y * a.y;
  az2  = a.z * a.z;
  axay = a.x * a.y;
  axaz = a.x * a.z;
  ayaz = a.y * a.z;
  
  m[0]  = ax2 + (1.0 - ax2) * co; 
  m[1]  = axay * mco + a.z * si;
  m[2]  = axaz * mco - a.y * si;
  m[3]  = 0.0;
  m[4]  = axay * mco - a.z * si;  
  m[5]  = ay2 + (1.0 - ay2) * co; 
  m[6]  = ayaz * mco + a.x * si;
  m[7]  = 0.0;
  m[8]  = axaz * mco + a.y * si;
  m[9]  = ayaz * mco - a.x * si;
  m[10] = az2 + (1.0 - az2) * co;
  m[11] = 0.0;
  m[12] = 0.0;
  m[13] = 0.0;
  m[14] = 0.0;
  m[15] = 1.0;
}

void MultMatrixVec(double *mat, Vec *vec, Vec *nvec)
{
  int   i, j;
  double a[4], b[4];

  a[0] = vec->x; a[1] = vec->y; a[2] = vec->z; a[3] = 1.0;
  for ( i = 0, j = 0; i < 16; i += 4, ++j ) {
    b[j] = ( mat[i] * a[0] + mat[i+1] * a[1]
	     + mat[i+2] * a[2] + mat[i+3] * a[3] );
  }
  nvec->x = b[0];
  nvec->y = b[1];
  nvec->z = b[2];
}

void MultMatrixVecUpdate(double *mat, Vec* vec)
{
  int   i;
  double a[4], b[4];

  a[0] = vec->x; a[1] = vec->y; a[2] = vec->z; a[3] = 1.0;
  for (i = 0; i < 4; ++i) 
    b[i] = (mat[i] * a[0] + mat[4 + i] * a[1] +  mat[8 + i] * a[2] + mat[12 + i] * a[3]);
  vec->x = b[0];
  vec->y = b[1];
  vec->z = b[2];
}

void EqualMatrix4(double *mat, double* mat1)
{
  int  i;

  for (i = 0; i < 16; ++i) mat1[i] = mat[i];
}

void MakeRotVecAngle( double angle, Vec *axis, double *m )
{
  double rd, co, si;
  double mco;
  double n1, n2, n3, n1_2, n2_2, n3_2, n1n2, n2n3, n1n3;
  Vec    a;
  
  rd = angle * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  mco = 1.0 - co;
  a.x = axis->x; a.y = axis->y; a.z = axis->z;
  V3Normalize( &a );

  n1 = a.x;
  n2 = a.y;
  n3 = a.z;
  n1_2 = n1 * n1;
  n2_2 = n2 * n2;
  n3_2 = n3 * n3;
  n1n2 = n1 * n2;
  n2n3 = n2 * n3;
  n1n3 = n1 * n3;
  
  m[0]  = n1_2 + (1.0 - n1_2) * co;
  m[4]  = n1n2 * mco + n3 * si;
  m[8]  = n1n3 * mco - n2 * si;
  m[12]  = 0.0;
  
  m[1]  = n1n2 * mco - n3 * si;
  m[5]  = n2_2 + (1.0 - n2_2) * co;
  m[9]  = n2n3 * mco + n1 * si;
  m[13]  = 0.0;
  
  m[2]  = n1n3 * mco + n2 * si;
  m[6]  = n2n3 * mco - n1 * si;
  m[10] = n3_2 + (1.0 - n3_2) * co;
  m[14] = 0.0;
  
  m[3] = 0.0;
  m[7] = 0.0;
  m[11] = 0.0;
  m[15] = 1.0;
}

void MakeRotInvVecAngle( double angle, Vec *axis, double *m )
{
  double rd, co, si;
  double mco;
  double ax2, ay2, az2, axay, axaz, ayaz;
  Vec    a;
  
  rd = angle * SMDPI / 180.0;
  co = cos(rd); si = sin(rd);
  mco = 1.0 - co;
  a.x = axis->x; a.y = axis->y; a.z = axis->z;
  V3Normalize( &a );

  ax2  = a.x * a.x;
  ay2  = a.y * a.y;
  az2  = a.z * a.z;
  axay = a.x * a.y;
  axaz = a.x * a.z;
  ayaz = a.y * a.z;
  
  m[0]  = ax2 + (1.0 - ax2) * co; 
  m[1]  = axay * mco + a.z * si;
  m[2]  = axaz * mco - a.y * si;
  m[3]  = 0.0;
  m[4]  = axay * mco - a.z * si;  
  m[5]  = ay2 + (1.0 - ay2) * co; 
  m[6]  = ayaz * mco + a.x * si;
  m[7]  = 0.0;
  m[8]  = axaz * mco + a.y * si;
  m[9]  = ayaz * mco - a.x * si;
  m[10] = az2 + (1.0 - az2) * co;
  m[11] = 0.0;
  m[12] = 0.0;
  m[13] = 0.0;
  m[14] = 0.0;
  m[15] = 1.0;
}

// make rotation matrix according to an axis
// instead an angle, we input cos(angle), sin(angle) for efficiency
// reference:
//   "Computer Graphics" by D. F. Rogers and J. A. Adams, 
//  , translated in Japanese by F. Yamaguchi, pp.60-64
// CATION: the rotation matrix described below is different from
//         the original book. It is really R^{T}.
// input: angle and vector 
// for rotation 
void iMakeRotVec(double angle, Vec *axis, double *m)
{
  double rd, co, si, mco;
  double ax2, ay2, az2, axay, axaz, ayaz;
  Vec    a;
  
  rd = angle * SMDPI / 180.0;
  co = cos(rd); si = sin(rd); mco = 1.0 - co;
  a.x = axis->x; a.y = axis->y; a.z = axis->z;
  V3Normalize(&a);

  ax2  = a.x * a.x;
  ay2  = a.y * a.y;
  az2  = a.z * a.z;
  axay = a.x * a.y;
  axaz = a.x * a.z;
  ayaz = a.y * a.z;
  
  m[0]  = ax2 + (1.0 - ax2) * co;
  m[1]  = axay * mco - a.z * si;
  m[2]  = axaz * mco + a.y * si;
  m[3]  = 0.0;
  m[4]  = axay * mco + a.z * si;
  m[5]  = ay2 + (1.0 - ay2) * co;
  m[6]  = ayaz * mco - a.x * si;
  m[7]  = 0.0;
  m[8]  = axaz * mco - a.y * si;
  m[9]  = ayaz * mco + a.x * si;
  m[10] = az2 + (1.0 - az2) * co;
  m[11] = 0.0;
  m[12] = 0.0;
  m[13] = 0.0;
  m[14] = 0.0;
  m[15] = 1.0;
}

void iMakeRotInvVec(double angle, Vec *axis, double *m)
{
  double rd, co, si, mco;
  double ax2, ay2, az2, axay, axaz, ayaz;
  Vec    a;
  
  rd = angle * SMDPI / 180.0;
  co = cos(rd); si = sin(rd); mco = 1.0 - co;
  a.x = axis->x; a.y = axis->y; a.z = axis->z;
  V3Normalize(&a);

  ax2  = a.x * a.x;
  ay2  = a.y * a.y;
  az2  = a.z * a.z;
  axay = a.x * a.y;
  axaz = a.x * a.z;
  ayaz = a.y * a.z;
  
  m[0]  = ax2 + (1.0 - ax2) * co; 
  m[1]  = axay * mco + a.z * si;
  m[2]  = axaz * mco - a.y * si;
  m[3]  = 0.0;
  m[4]  = axay * mco - a.z * si;  
  m[5]  = ay2 + (1.0 - ay2) * co; 
  m[6]  = ayaz * mco + a.x * si;
  m[7]  = 0.0;
  m[8]  = axaz * mco + a.y * si;
  m[9]  = ayaz * mco - a.x * si;
  m[10] = az2 + (1.0 - az2) * co;
  m[11] = 0.0;
  m[12] = 0.0;
  m[13] = 0.0;
  m[14] = 0.0;
  m[15] = 1.0;
}

void iMultMatrixVec(double *mat, Vec* vec)
{
  int   i;
  double a[4], b[4];

  a[0] = vec->x; a[1] = vec->y; a[2] = vec->z; a[3] = 1.0;
  for (i = 0; i < 4; ++i) 
    b[i] = (mat[i] * a[0] + mat[4 + i] * a[1] +  mat[8 + i] * a[2] + mat[12 + i] * a[3]);
  vec->x = b[0];
  vec->y = b[1];
  vec->z = b[2];
}

void initialize_matrix4( double *mat )
{
  for ( int i = 0; i < 10; ++i ) mat[i] = 0.0;
}

void add_matrix4( double *a, double *b, double *c )
{
  for ( int i = 0; i < 10; ++i ) c[i] = a[i] + b[i];
}

void equal_matrix4( double *a, double *b )
{
  for ( int i = 0; i < 10; ++i ) b[i] = a[i];
}

void multivalue_matrix4( double *a, double val, double *b )
{
  for ( int i = 0; i < 10; ++i ) b[i] = val * a[i];
}

