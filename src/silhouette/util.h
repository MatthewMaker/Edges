#ifndef _UTIL_
#define _UTIL_

#include "gr.h"

/* points and vectors */
extern GrVector3D	mkVector3D( double x, double y, double z );
extern GrPoint3D	mkPoint3D( double x, double y, double z );

extern double		vvDot( GrVector3D v1, GrVector3D v2 );
extern GrVector3D	svMpy( double s, GrVector3D v );

extern GrVector3D	vvAdd( GrVector3D v1, GrVector3D v2 );
extern GrPoint3D	pvAdd( GrPoint3D v1, GrVector3D v2 );

extern GrVector3D	ppSub( GrPoint3D v1, GrPoint3D v2 );
extern GrVector3D	vvSub( GrVector3D v1, GrVector3D v2 );
extern GrPoint3D	pvSub( GrPoint3D v1, GrVector3D v2 );

extern double		vNormalize( GrVector3D *v );

extern GrVector3D	crossProd(GrVector3D u, GrVector3D v);
extern GrVector3D	transVec(GrMatrix4x4 mat, GrVector3D v);
extern GrPoint3D	transPoint(GrMatrix4x4 mat, GrPoint3D p);
extern GrVector3D	transNorm(GrVector3D p, GrMatrix4x4 mat);

/* matrices */
extern void		initMatrix(GrMatrix4x4 h);
extern void		multMatrix(GrMatrix4x4 ret, 
                                   GrMatrix4x4 mat1, GrMatrix4x4 mat2);
extern void             transposeMatrix(GrMatrix4x4 ret, GrMatrix4x4 inMatrix);

#endif /* _UTIL_ */
