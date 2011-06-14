//--------------------------------------------------------------------------
// $Id: gr.h,v 1.1.1.1 2000/09/17 06:56:05 vchma Exp $
//--------------------------------------------------------------------------

#ifndef _GR_
#define _GR_

#include "togl.h"

/* Gr Types */
typedef struct {
   double    x,y;      /* XY */
} GrPoint2D;

typedef struct { 
   double    x,y,z;    /* XYZ */
} GrPoint3D;

typedef struct { 
   double    x,y,z;    /* XYZ-vector */
} GrVector3D;

typedef double    GrMatrix4x4[4][4];         /* [row][column] */

extern int gr_identity( void );
extern int gr_rotate( char axis, double angle );
extern int gr_translate( GrVector3D trans );
extern int gr_trackball( GrPoint2D start, GrPoint2D end, double scale );
extern int load( char *name );

extern void gr_initialize(struct Togl* togl);
extern void gr_reshape(struct Togl* togl);
extern void gr_render(struct Togl* togl);

extern void gr_setHidden( unsigned int yes );
extern void gr_setSilhouette( unsigned int yes );
extern void gr_setStrip( unsigned int yes );
extern void gr_setOptimization( unsigned int yes );

#endif /* _GR_ */


