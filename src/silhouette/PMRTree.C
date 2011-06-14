//--------------------------------------------------------------------------
// $Id: PMRTree.C,v 1.1.1.1 2000/09/17 06:56:05 vchma Exp $
//--------------------------------------------------------------------------

#include "GL/gl.h"
#include "PMRTree.h"

static GLfloat two[] = { 0.2f, 0.5f, 1.0f, 1.0f };

PMRTree::PMRTree( Point &min, Point &max, int num )
: myRoot( new PMRTreeNode( min.x, min.y, min.z, max.x, max.y, max.z ) ),
  myNum( num ),
  myMin( min ),
  myMax( max ),
  myDimension( max - min )
{
    myEdges.init( 100 );
    flags = (bool*) calloc( num, sizeof ( bool ) );
}

bool
PMRTree::add( DualEdge **de )
{
    for ( int i = myNum ; i --> 0 ; )
    {
	myRoot->add( de[i] );
    }
//    myRoot->add( de );
//    myEdges.append( de );

    return true;
}

void
PMRTree::query( Vector &N, Point &pop ) const
{
    int dir, x, y, z;
    memset( flags, 0, sizeof(bool)*myNum ); 

    x = ( N.x < 0 ? 0 : 1 );
    y = ( N.y < 0 ? 0 : 2 );
    z = ( N.z < 0 ? 0 : 4 );
    dir = z + y + x;
    myRoot->query( N, pop, dir, flags );
}

void
PMRTree::draw( int x ) const
{
    glDisable(GL_LIGHTING);
    glColorMaterial( GL_FRONT_AND_BACK, GL_SPECULAR );
    glEnable(GL_COLOR_MATERIAL);

    myRoot->draw( x, 1 );

    glColor3fv( two );
    glBegin( GL_LINES );
    for ( int i = myEdges.size() ; i --> 0 ; )
    {
	Point &p = myEdges[i]->getDualVertex(0).getPoint();
	Point &q = myEdges[i]->getDualVertex(1).getPoint();

	glVertex3d( p.x, p.y, p.z );
	glVertex3d( q.x, q.y, q.z );
    }
    glEnd();
}
