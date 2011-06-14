/**********************
 CS 488 Assignment 3
 Name:   Selina Siu
 ID:     94126614
 UserID: ssiu
**********************/

#include <cmath>

#include "gr.h"
#include "util.h"

/*
 * Some functions handling vectors
 */

/*
 * Creates a new vector and returns it
 */
GrVector3D mkVector3D( double x, double y, double z )
{
    GrVector3D result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

/*
 * Creates a new point and returns it
 */
GrPoint3D mkPoint3D( double x, double y, double z )
{
    GrPoint3D result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

/*
 * calculates the dot product of two vectors and returns it
 */
double vvDot( GrVector3D v1, GrVector3D v2 )
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/*
 * Multiplies a vector by a scaler
 */
GrVector3D svMpy( double s, GrVector3D v )
{
    GrVector3D rval;
    rval.x = s * v.x;
    rval.y = s * v.y;
    rval.z = s * v.z;
    return rval;
}

/*
 * Adds two vectors and returns the sum vector.
 */
GrVector3D vvAdd( GrVector3D v1, GrVector3D v2 )
{
    GrVector3D result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

/*
 * Adds a point and a vector.
 */
GrPoint3D pvAdd( GrPoint3D p1, GrVector3D v2 )
{
    GrPoint3D result;
    result.x = p1.x + v2.x;
    result.y = p1.y + v2.y;
    result.z = p1.z + v2.z;
    return result;
}

/*
 * subtracts point v2 from v1
 */
GrVector3D ppSub( GrPoint3D p1, GrPoint3D p2 )
{
    GrVector3D result;
    result.x = p1.x - p2.x;
    result.y = p1.y - p2.y;
    result.z = p1.z - p2.z;
    return result;
}

/*
 * subtracts vector v2 from v1
 */
GrVector3D vvSub( GrVector3D v1, GrVector3D v2 )
{
    GrVector3D result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

/*
 * subtracts vector v2 from point v1
 */
GrPoint3D pvSub( GrPoint3D p1, GrVector3D v2 )
{
    GrPoint3D result;
    result.x = p1.x - v2.x;
    result.y = p1.y - v2.y;
    result.z = p1.z - v2.z;
    return result;

}

/*
 * Normalizes vector v and returns the length before the
 * normalization
 */
double vNormalize( GrVector3D *v )
{
    double denom;
    double x = ( v->x > 0.0 ) ? v->x : - v->x;
    double y = ( v->y > 0.0 ) ? v->y : - v->y;
    double z = ( v->z > 0.0 ) ? v->z : - v->z;
    if( x > y )
        {
        if( x > z )
            {
            if( 1.0 + x > 1.0 )
                {
                y = y / x;
                z = z / x;
                denom = 1.0 / ( x * sqrt( 1.0 + y*y + z*z ) );
                }
            }
        else /* z > x > y */
            {
            if( 1.0 + z > 1.0 )
                {
                y = y / z;
                x = x / z;
                denom = 1.0 / ( z * sqrt( 1.0 + y*y + x*x ) );
                }
            }
        }
    else
        {
        if( y > z )
            {
            if( 1.0 + y > 1.0 )
                {
                z = z / y;
                x = x / y;
                denom = 1.0 / ( y * sqrt( 1.0 + z*z + x*x ) );
                }
            }
        else /* x < y < z */
            {
            if( 1.0 + z > 1.0 )
                {
                y = y / z;
                x = x / z;
                denom = 1.0 / ( z * sqrt( 1.0 + y*y + x*x ) );
                }
            }
        }
    if( 1.0 + x + y + z > 1.0 )
        {
        *v = svMpy( denom, *v );
	return 1.0/denom;
        }
    else
        {
        return 0.;
        }
}

/*
 * Calculates the crossproduct two vectors.
 */
GrVector3D crossProd(GrVector3D u, GrVector3D v)
{
  GrVector3D ret;
  ret.x = u.y * v.z - u.z * v.y;
  ret.y = u.z * v.x - u.x * v.z;
  ret.z = u.x * v.y - u.y * v.x;
  return ret;
}

/* 
 * multiplies a vector to a matrix form the right 
 */
GrVector3D transVec(GrMatrix4x4 mat, GrVector3D v)
{
  GrVector3D r;

  r.x = v.x * mat[0][0] + v.y * mat[0][1] + v.z * mat[0][2];
  r.y = v.x * mat[1][0] + v.y * mat[1][1] + v.z * mat[1][2];
  r.z = v.x * mat[2][0] + v.y * mat[2][1] + v.z * mat[2][2];
  return r;
}

/*
 * transform a point by multiplying it to a matrix from the right
 */
GrPoint3D transPoint(GrMatrix4x4 mat, GrPoint3D p)
{
  GrPoint3D r;
  r.x = p.x * mat[0][0] + p.y * mat[0][1] + p.z * mat[0][2];
  r.y = p.x * mat[1][0] + p.y * mat[1][1] + p.z * mat[1][2];
  r.z = p.x * mat[2][0] + p.y * mat[2][1] + p.z * mat[2][2];
  r.x += mat[0][3];
  r.y += mat[1][3];
  r.z += mat[2][3];
  return r;
}

/*
 * transforms a vector by multiplying it to a matrix from the left
 */
GrVector3D transNorm(GrVector3D n, GrMatrix4x4 mat)
{
  GrVector3D r;
  r.x = n.x * mat[0][0] + n.y * mat[1][0] + n.z * mat[2][0];
  r.y = n.x * mat[0][1] + n.y * mat[1][1] + n.z * mat[2][1];
  r.z = n.x * mat[0][2] + n.y * mat[1][2] + n.z * mat[2][2];
  return r;
}


/* *************************************************************************
 *
 * Handling matrices
 *
 * *************************************************************************/

/*
 * initialize a matrix to the identity matrix.
 */
void initMatrix(GrMatrix4x4 h)
{
  int i,j;
  
  for( i= 0 ; i< 4 ; i++ )
    for( j= 0 ; j< 4 ; j++ )
      h[i][j]= (i==j);
}

/*
 * Multiplying mat1*mat2
 * ret can be one of mat1 or mat2
 */
void multMatrix(GrMatrix4x4 ret, GrMatrix4x4 mat1, GrMatrix4x4 mat2)
{
  int i,j;
  GrMatrix4x4 r;

  for( i= 0 ; i< 4 ; i++ )
    for( j= 0 ; j< 4 ; j++ )
      r[i][j]= mat1[i][0] * mat2[0][j] +
                  mat1[i][1] * mat2[1][j] +
		  mat1[i][2] * mat2[2][j] +
		  mat1[i][3] * mat2[3][j];

  for( i= 0 ; i< 4 ; i++ )
    for( j= 0 ; j< 4 ; j++ )
      ret[i][j] = r[i][j];
}

/* 
 * transposeMatrix
 */
void transposeMatrix(GrMatrix4x4 ret, GrMatrix4x4 inMatrix)
{
  int i, j;

  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
      ret[i][j] = inMatrix[j][i];
      
}
