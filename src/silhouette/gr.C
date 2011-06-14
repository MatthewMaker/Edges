//--------------------------------------------------------------------------
// Edges
// Copyright (C) 1999-2000 Vincent C. H. Ma
// http://edges.sourceforge.net
// vchma@users.sourceforge.net
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// $Id: gr.C,v 1.8 2001/03/21 15:39:22 vchma Exp $
//--------------------------------------------------------------------------

#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <tcl.h>

#include "util.h"
#include "Mesh.h"
#include "MeshDraw.h"
#include "gr.h"
#include "silhouette.h"
#include "TriStrip.h"
#include "TriStripDraw.h"
#include "IVOutputOp.h"

/*************************************************************************
* Constants
**************************************************************************/
const double NEAR = 0.1;        /* near plane */
const double FAR = 100000.0;      /* far plane */

/*************************************************************************
* Globals 
**************************************************************************/
GrPoint2D screenSize;               /* display window size */
Mesh *theMesh = NULL;
MeshDraw *theMeshDraw = NULL;
Silhouette theSil;
TriStrip *theStrip = NULL;
TriStripDraw *theStripDraw = NULL;

// Global transformations
GrVector3D      rotate;

GrVector3D      translate;
GrMatrix4x4	trackball;
GrMatrix4x4	accum;

GrMatrix4x4	i_trackball;
GrMatrix4x4	i_accum;

// Current filename
char		*filename = NULL;

double		 lookAt[9] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
double		 maxDim = 0;

static bool hidden = true;
static bool sil = false;
static bool strip = false;
static int opt = 2;

static GLfloat light_specular[] = {0.5, 0.5, 0.5, 1.0};
static GLfloat light_diffuse[] = {0.6, 0.6, 0.6, 0.7};
static GLfloat light_position[] = {0.0, 0.0, 10.0, 1.0};
static GLfloat light_position2[] = {0.0, 0.0, 10.0, 1.0};
static GLfloat constAtten = 1.0;
static GLfloat linearAtten = 0.0;
static GLfloat quadAtten = 0.0;

/*************************************************************************
* functions to access the dag hash table 
**************************************************************************/

void gr_setHidden( unsigned int yes ) { hidden = yes; }
void gr_setSilhouette( unsigned int yes ) { sil = yes; }
void gr_setOptimization( unsigned int yes ) { opt = yes; }

void gr_setStrip( unsigned int yes )
{
    strip = yes;
    if ( yes )
    {
// 	glEnable( GL_LIGHT0 );
// 	glEnable( GL_LIGHTING );
#ifndef CULL
  	glDisable( GL_CULL_FACE );
#endif
    }
    else
    {
	glEnable( GL_CULL_FACE );
// 	glDisable( GL_LIGHT0 );
// 	glDisable( GL_LIGHTING );
    }
}

// Calculate the accumulated transformation matrix ----------------------------
void calcAccum()
{
    Point p( 0, 0, 0 );

    if ( theMesh ) p = theMesh->getCentroid();

    glPushMatrix();
    glLoadIdentity();
    glTranslated(translate.x * maxDim, translate.y * maxDim, translate.z * maxDim);

    // this makes sure that we are always using trackball to rotate
    // about the center of the model, not origin of world.
    if ( theMesh )
    {
	glTranslated( p.x, p.y, p.z );
	glMultMatrixd((GLdouble *)trackball);
	glTranslated( -p.x, -p.y, -p.z );
    }
    else
    {
	glMultMatrixd((GLdouble *)trackball);
    }
//    glRotated(rotate.x, 1.0, 0.0, 0.0);
//    glRotated(rotate.y, 0.0, 1.0, 0.0);
//    glRotated(rotate.z, 0.0, 0.0, 1.0);  
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *)accum);
    glPopMatrix();

    glPushMatrix();
    glLoadIdentity();
    // this makes sure that we are always using trackball to rotate
    // about the center of the model, not origin of world.
    if ( theMesh )
    {
	glTranslated( p.x, p.y, p.z );
	glMultMatrixd((GLdouble *)i_trackball);
	glTranslated( -p.x, -p.y, -p.z );
    }
    else
    {
	glMultMatrixd((GLdouble *)i_trackball);
    }
    glTranslated(-translate.x, -translate.y, -translate.z);
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *)i_accum);
    glPopMatrix();
}

// Reset transformation -------------------------------------------------------
int gr_identity()
{  
    initMatrix(trackball);
    initMatrix(i_trackball);
    calcAccum();
    return TCL_OK;
}

// gr_rotate ------------------------------------------------------------------
int gr_rotate( char axis, double in_angle )
{
    switch (axis)
    {
	case 'x': 
	case 'X': 
	    rotate.x = in_angle;
	    break;

	case 'y': 
	case 'Y': 
	    rotate.y = in_angle;
	    break;

	case 'z': 
	case 'Z': 
	    rotate.z = in_angle;
	    break;
    }

    calcAccum();

    return TCL_OK;
}

// gr_translate ---------------------------------------------------------------
int gr_translate( GrVector3D trans )
{
    translate = trans;
    calcAccum();
    return TCL_OK;
}

/******************************************
* gr_initialize - init data structures
******************************************/
void gr_initialize( struct Togl *togl )
{
    screenSize.x = 0;
    screenSize.y = 0;

    /**** initialize open gl stuff ****/
    glShadeModel(GL_SMOOTH);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /**** initialize lighting *****/
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constAtten);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAtten);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadAtten);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    /**** culling *****/
    glCullFace(GL_BACK);
#ifdef CULL
    glEnable(GL_CULL_FACE);
#else
    glDisable(GL_CULL_FACE);
#endif

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    /**** enable normalizing ****/
    glEnable(GL_NORMALIZE);

    // enable blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ZERO );

//    glEnable( GL_LINE_SMOOTH );
//    glEnable( GL_POLYGON_SMOOTH );

    glPolygonMode( GL_FRONT, GL_FILL );

    // Initialize global model transformation stuff
    rotate    = mkVector3D( 0.0, 0.0, 0.0 );
    translate = mkVector3D( 0.0, 0.0, 0.0 );
    initMatrix( accum );
    initMatrix( trackball );

    initMatrix( i_accum );
    initMatrix( i_trackball );
}

/**********************************************************************
* Creating new nodes
**********************************************************************/

// gr_trackball ---------------------------------------------------------------
Vector
calcVector(GrPoint2D p, GrPoint2D origin)
{
    Vector newV;
    double length;

    /* calculate components */
    /* make the diameter 1/2 the size of the screen width */
    newV.x = (p.x - origin.x) *2.0 / (screenSize.x/2);
    newV.y = (p.y - origin.y) *2.0 / (screenSize.x/2);
    newV.z = (1.0 - newV.x * newV.x - newV.y * newV.y);

    if (newV.z < 0.0)
    { 
	length = sqrt(1.0 - newV.z);
	newV.z = 0.0;
	newV.x /= length;
	newV.y /= length;
    }
    else
    {
	newV.z = sqrt(newV.z);
    }

    return newV;
}

int gr_trackball( GrPoint2D old_p, GrPoint2D new_p, double scale )
{
    GrPoint2D origin;
    Vector newV, oldV, vec;
    double radians;

    /*
       printf("trackball: node %s old: %.2lf %.2lf new: %.2lf %.2lf s: %.2lf\n", 
       node, old_p.x, old_p.y, new_p.x, new_p.y, scale);
     */

    /* calculate middle of the screen */
    origin.x = screenSize.x /2;
    origin.y = screenSize.y /2;

    newV = calcVector(new_p, origin);
    oldV = calcVector(old_p, origin);

    /* generate rotation vector */
    vec.x = oldV.y * newV.z - newV.y * oldV.z;
    vec.y = oldV.z * newV.x - newV.z * oldV.x;
    vec.z = -(oldV.x * newV.y - newV.x * oldV.y);
//    vec = oldV ^ newV;

    vec.x *= -1.0;
    vec.y *= -1.0;

    /* find length of vector */
    radians = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    if (radians > -0.000001 && radians < 0.000001)
	return TCL_OK;

    /* normalize rotation vector */
    vec.x = vec.x / radians;
    vec.y = - vec.y / radians;
    vec.z = vec.z / radians;
    radians *= scale;

    /* use OpenGL to rotate about an arbitrary axis */
    glPushMatrix();
    glLoadIdentity();
    glRotated(radians, vec.x, vec.y, vec.z);
    glMultMatrixd((GLdouble *) trackball);
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *)trackball);
    glPopMatrix();

    glPushMatrix();
//    glLoadIdentity();
    glLoadMatrixd((GLdouble *) i_trackball);
    glRotated(-radians, vec.x, vec.y, vec.z);
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *)i_trackball);
    glPopMatrix();

    calcAccum();

    return TCL_OK;
}

// gr_render ------------------------------------------------------------------
void
gr_reshape( struct Togl *togl )
{
    screenSize.x = Togl_Width(togl);
    screenSize.y = Togl_Height(togl);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei)screenSize.x, (GLsizei)screenSize.y);
    gluPerspective(40.0, (GLfloat)screenSize.x/(GLfloat)screenSize.y, NEAR, FAR);

    glMatrixMode(GL_MODELVIEW);
}

void
gr_render( struct Togl *togl )
{
    GLenum errCode;

    screenSize.x = Togl_Width(togl);
    screenSize.y = Togl_Height(togl);

    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // if no mesh has been loaded yet then don't continue
    if ( !theMesh ) return;
    else if ( !theStrip ) return;

    assert( theMeshDraw );
    assert( theStripDraw );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei)screenSize.x, (GLsizei)screenSize.y);
    gluPerspective(40.0, (GLfloat)screenSize.x/(GLfloat)screenSize.y, NEAR, FAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt( lookAt[0], lookAt[1], lookAt[2], 
	       lookAt[3], lookAt[4], lookAt[5], 
	       lookAt[6], lookAt[7], lookAt[8] ); 

    // here we figure out which edges are the silhouette edges
    // TODO check to see if i_accum * accum = I

    // first, figure out the current world-space position
    // of the viewpoint
    double *lo = (double *)i_accum;
    Point p(
	    lo[0]*lookAt[0]+lo[4]*lookAt[1]+lo[8] *lookAt[2]+lo[12],
	    lo[1]*lookAt[0]+lo[5]*lookAt[1]+lo[9] *lookAt[2]+lo[13],
	    lo[2]*lookAt[0]+lo[6]*lookAt[1]+lo[10]*lookAt[2]+lo[14] );
//	Point q = theMesh->getCentroid();
//
//	Point r(
//	      lo[0]*q.x+lo[4]*q.y+lo[8] *q.z+lo[12],
//	      lo[1]*q.x+lo[5]*q.y+lo[9] *q.z+lo[13],
//	      lo[2]*q.x+lo[6]*q.y+lo[10]*q.z+lo[14] );
//
//	cout << "--current eye:" << p;
//
//	Vector v = p - r;

//	p.x = v.x;
//	p.y = v.y;
//	p.z = v.z;

    // now markup the silhouette edges
    if ( !strip )
	theSil.markSilhouetteEdges( p, ( opt == 1 ), ( opt == 2 ) );

    glMultMatrixd((GLdouble *)accum);

    if ( !strip )
	theMeshDraw->drawWireFrame( hidden, sil );
    else
    {
	if ( !hidden )
	{
	    glDisable( GL_LIGHT0 );
	    glDisable( GL_LIGHTING );
	    theMeshDraw->drawWireFrame();
	}
	else
	{
	    glEnable( GL_LIGHT0 );
	    glEnable( GL_LIGHTING );
	    light_position2[0] = p.x;
	    light_position2[1] = p.y;
	    light_position2[2] = p.z;
	    glLightfv(GL_LIGHT0, GL_POSITION, light_position2);
	}
	theStripDraw->draw();
    }

    glFlush();

    if ( ( errCode = glGetError() ) != GL_NO_ERROR )
	fprintf( stderr, "OpenGL Error: %s\n", gluErrorString( errCode ) );

    Togl_SwapBuffers(togl);

    #if 0
    // debug
    numR++;
    if ( numR >= 1000 )
    {
	numR = 0;
	theChild++;
	if ( theChild >= 8 ) theChild = 0;
	cout << theChild << endl;
    }
    #endif
}

// Load a mesh ----------------------------------------------------------------
int
load( char *name )
{
    // sanity check
    if ( !name ) return TCL_ERROR;

    // destroy the old mesh
    if ( theMesh )
    {
	delete theMesh;
	theMesh = NULL;
    }
    if ( theStrip )
    {
	delete theStrip;
	theStrip = NULL;
    }

    if ( theMeshDraw )
    {
	delete theMeshDraw;
	theMeshDraw = NULL;
    }
    if ( theStripDraw )
    {
	delete theStripDraw;
	theStripDraw = NULL;
    }

    // create a new control mesh, and read in the mesh definition
    if ( !( theMesh = new Mesh ) ) return TCL_ERROR;
    if ( !theMesh->readFile( name ) ) return TCL_ERROR;

    assert( theMesh );
    theMeshDraw = new MeshDraw( *theMesh );
    assert( theMeshDraw );

//    IVOutputOp op( "abcd.iv" );
//
//    theMesh->acceptOp( op );

    theMesh->getLookAt( lookAt, maxDim );

    maxDim /= 10;
    if ( maxDim < 1 ) maxDim = 1;
    std::cout << "MaxDim = " << maxDim << std::endl;

    theSil.useThisMesh( theMesh );
    theStrip = new TriStrip( *theMesh );
    theStrip->output( name );

    theStripDraw = new TriStripDraw( *theStrip );
    assert( theStripDraw );

    std::cout << "done" << std::endl;

    filename = strdup( name );

    return TCL_OK;
}

