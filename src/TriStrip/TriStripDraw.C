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
// $Id: TriStripDraw.C,v 1.2 2001/03/21 04:45:53 vchma Exp $
//--------------------------------------------------------------------------

#include "TriStrip.h"
#include "TriStripDraw.h"
//#include "Array.h"
//#include "Face.h"
//#include "HalfEdge.h"
#include "Vertex.h"
//#include "Vector.h"
//#include "Vector2.h"
//#include "PriQueue.h"
//#include "PQFace.h"

//#include "Timer.h"
//#include "ProgressBar.h"

//#include <cstdlib>
//#include <algorithm>
#include <iostream>
#include <vector>
//#include <iomanip>
//#include <fstream>
//#include <string>

#include <GL/gl.h>

// #define SMART

//#define NUM_VERT_PER_LINE 3
//
//using std::cout;
//using std::endl;
//
//enum
//{
//    RESTART,
//    LEFT,
//    RIGHT,
//};

TriStripDraw::TriStripDraw( const TriStrip &ts )
: myTS( ts ),
  myDL( glGenLists( 1 ) )
{
    assert( myDL );

    GLfloat color[3];
    TriStrip::Strip *curStrip;

    TriStrip::Strip::const_iterator jj;
    TriStrip::StripsArray::const_iterator ii;

    int i = 0, j = 0;

    glNewList( myDL, GL_COMPILE );

    // for each strip that I have generated
    for ( ii = ts.strip().begin(), i = 0 ; ii < ts.strip().end() ; ++ii, ++i )
    {
	curStrip = *ii;

//	cout << "[" << myDL << "," << ts.numStrips() << "] " << i << ":"; 

	// draw the strip
	color[0] = (rand()/(float)RAND_MAX);
	color[1] = (rand()/(float)RAND_MAX);
	color[2] = (rand()/(float)RAND_MAX);

	glColor3fv( color );

	glBegin( GL_TRIANGLE_STRIP );

	    for ( jj = curStrip->begin(), j = 0 ; jj < curStrip->end() ; ++jj, ++j )
	    {
		const Vector *v = (*jj)->getNormal();
		const Point &p = (*jj)->getPoint();

//		cout << j << "-";
		if ( v ) glNormal3d( v->x, v->y, v->z ); 
		glVertex3d( p.x, p.y, p.z ); 
	    }

	glEnd();

//	cout << "|" << endl;
    }
//    cout << "here" << endl;
    glEndList();
//    cout << "here 2" << endl;
}

TriStripDraw::~TriStripDraw()
{
    if ( myDL ) glDeleteLists( myDL, 1 );
}
