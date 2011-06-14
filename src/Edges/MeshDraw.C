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
// $Id: MeshDraw.C,v 1.1 2001/02/22 15:53:43 vchma Exp $
//--------------------------------------------------------------------------

#include "Mesh.h"
#include "MeshDraw.h"
#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"

#include <vector>

#include <GL/gl.h>

// Colour for drawing in the wireframe drawing.
GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat linemat[] = { 0.9f, 0.3f, 0.4f, 1.0f };
GLfloat plinemat[] = { 0.2f, 0.5f, 1.0f, 1.0f };

// Colour for drawing the solid drawing.
GLfloat amb[] = { 0.09, 0.04, 0.04, 1.0 };
GLfloat dif[] = { 0.9, 0.4, 0.4, 1.0 };
GLfloat spc[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat shine = 100.0F;

// create wireframe display list ----------------------------------------------
void
MeshDraw::createWireFrame( void )
{
    int i;
    Point *p, *q;

    myWireDL = glGenLists( 1 );
    assert( myWireDL );

    glNewList( myWireDL, GL_COMPILE );

	// draw each edge
	for ( i = myMesh.myOneOutOfTwo.size() ; i --> 0 ; )
	{
	    p = &( myMesh.myOneOutOfTwo[i]->getVertex().getPoint() );
	    q = &( myMesh.myOneOutOfTwo[i]->getSym().getVertex().getPoint() );

	    if ( myMesh.myOneOutOfTwo[i]->isSeam() )
		glColor3fv( linemat );
	    else
		glColor3fv( plinemat );

	    glBegin( GL_LINES );
		glVertex3d( p->x, p->y, p->z );
		glVertex3d( q->x, q->y, q->z );
	    glEnd();
	}

	// draw each normal
	for ( i = myMesh.myVertices.size() ; i --> 0 ; )
	{
	    Vertex *v = myMesh.myVertices[i];
	    const Vector *n = v->getNormal();

	    if ( n )
	    {
		const Point &pp = v->getPoint();

		glBegin( GL_LINES );
		    glVertex3d( pp.x, pp.y, pp.z );
		    glVertex3d( pp.x+n->x, pp.y+n->y, pp.z+n->z );
		glEnd();
	    }
	}

    glEndList();
}

// create solid display list ----------------------------------------------
void
MeshDraw::createSolid( void )
{
    int i, j;
    Point *p;
    HalfEdge *e;

    mySolidDL = glGenLists( 1 );
    assert( mySolidDL );

    glNewList( mySolidDL, GL_COMPILE );

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, amb );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, dif );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, spc );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shine );

	for ( i = myMesh.myFaces.size() ; i --> 0 ; )
	{
	    Face &f = *(myMesh.myFaces[i]);
	    Face::EdgeIterator fi( f );

	    glBegin( GL_POLYGON );

		Vector &v = f.getNormal();
		glNormal3d( v.x, v.y, v.z );

		do
		{
		    Point &p = fi.getNext()->getVertex().getPoint();
		    glVertex3d( p.x, p.y, p.z );
		} while ( fi.hasNext() );

	    glEnd();
	}

    glEndList();
}

// Draws this mesh as a wireframe ---------------------------------------------
void
MeshDraw::drawWireFrame( bool hidden, bool silhouette )
{
    int i;
    Face::EdgeIterator fi;
    Point *p, *q;
    HalfEdge *cur;

//    glEnable(GL_DEPTH_TEST);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Set colour of line
//    glColor3fv( plinemat );

    glDisable(GL_LIGHTING);
    glColorMaterial( GL_FRONT_AND_BACK, GL_SPECULAR );
    glEnable(GL_COLOR_MATERIAL);

//    glBegin( GL_LINES );
//	glVertex3d( myMin.x, myMin.y, myMin.z );
//	glVertex3d( myMax.x, myMax.y, myMax.z );
//    glEnd();

    // Render outline of polygons as lines
    glBegin( GL_LINES );
    for ( i = myMesh.myOneOutOfTwo.size() ; i --> 0 ; )
    {
	cur = myMesh.myOneOutOfTwo[i];
	if ( silhouette && cur->isPicked() )
	    glColor3fv( linemat );
	else
	    glColor3fv( plinemat );

	p = &( cur->getVertex().getPoint() );
	q = &( cur->getPrev().getVertex().getPoint() );

    if ( hidden || cur->isPicked() )
    {
	glVertex3d( p->x, p->y, p->z );
	glVertex3d( q->x, q->y, q->z );
    }
    }
    glEnd();

    // if we are doing hidden-line removal
    if ( hidden || 1 )
    {
	// now, render again, but draw filled polygons.  Also, shrink
	// the polygons using offset, and draw them black to match the
	// background.  This way, these black polys will "hide" the
	// lines that should be hidden.  Voila!
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );

	glEnable(GL_POLYGON_OFFSET_FILL);

 	glPolygonOffset( 1.0f, 1.0f );
	glColor3fv( black );

	for ( i = myMesh.myFaces.size() ; i --> 0 ; )
	{
	    fi.resetFace( *(myMesh.myFaces[i]) );

	    glBegin(GL_POLYGON);
	    do {
		Point &p = fi.getNext()->getVertex().getPoint();
		glVertex3d( p.x, p.y, p.z );
	    } while ( fi.hasNext() );
	    glEnd();
	}

	// now we are done, turn off the OFFSET mode
	glEnable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
    }
//    glEnable(GL_LIGHTING);
}
