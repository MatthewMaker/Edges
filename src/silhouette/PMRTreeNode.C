//--------------------------------------------------------------------------
// $Id: PMRTreeNode.C,v 1.2 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#include <GL/gl.h>
#include <iostream>
#include <cstdlib>

#include "PMRTreeNode.h"
    
static const Vector posX(  1,  0,  0 );
static const Vector posY(  0,  1,  0 );
static const Vector posZ(  0,  0,  1 );
static const Vector negX( -1,  0,  0 );
static const Vector negY(  0, -1,  0 );
static const Vector negZ(  0,  0, -1 );

PMRTreeNode::PMRTreeNode( double nx, double ny, double nz,
			  double xx, double xy, double xz )
{
    myIsLeaf = true;

    myMin.x = nx; myMin.y = ny; myMin.z = nz;
    myMax.x = xx; myMax.y = xy; myMax.z = xz;
    myLoc.x = (xx+nx)*0.5; myLoc.y = (xy+ny)*0.5; myLoc.z = (xy+nz)*0.5;

    memset( myChild, 0, 8 * sizeof( PMRTreeNode * ) );

    myEdges.init( theSplittingThreshold );
}

PMRTreeNode::~PMRTreeNode()
{
    for ( int i = 8 ; i --> 0 ; )
	if ( myChild[i] )
	    delete( myChild[i] );
}

void
PMRTreeNode::splitMe()
{
    std::cout << "PMRTreeNode:: Splitting" << std::endl;
    myChild[4]=new PMRTreeNode(myMin.x,myMin.y,myLoc.z,myLoc.x,myLoc.y,myMax.z);
    myChild[5]=new PMRTreeNode(myLoc.x,myMin.y,myLoc.z,myMax.x,myLoc.y,myMax.z);
    myChild[6]=new PMRTreeNode(myMin.x,myLoc.y,myLoc.z,myLoc.x,myMax.y,myMax.z);
    myChild[7]=new PMRTreeNode(myLoc.x,myLoc.y,myLoc.z,myMax.x,myMax.y,myMax.z);

    myChild[0]=new PMRTreeNode(myMin.x,myMin.y,myMin.z,myLoc.x,myLoc.y,myLoc.z);
    myChild[1]=new PMRTreeNode(myLoc.x,myMin.y,myMin.z,myMax.x,myLoc.y,myLoc.z);
    myChild[2]=new PMRTreeNode(myMin.x,myLoc.y,myMin.z,myLoc.x,myMax.y,myLoc.z);
    myChild[3]=new PMRTreeNode(myLoc.x,myLoc.y,myMin.z,myMax.x,myMax.y,myLoc.z);

    myIsLeaf = false;
}

void
PMRTreeNode::add( DualEdge *de )
{
    // if this line intersects the block of this node
    if ( intersect( *de ) )
    {
	// if I am a leaf
	if ( myIsLeaf )
	{
	    myEdges.append( de );

	    // if we have to split on this insertion.
	    if ( myEdges.size() > theSplittingThreshold )
	    {
		splitMe();

		for ( int i = myEdges.size() ; i --> 0 ; )
		{
		    DualEdge *e = myEdges[i];
		    for ( int j = 8 ; j --> 0 ; )
			((PMRTreeNode *)myChild[j])->add( e );
		}

		myEdges.init( 0 );
	    }
	}

	// o/w if I am not a leaf,
	else
	{
	    for ( int j = 8 ; j --> 0 ; )
		((PMRTreeNode *)myChild[j])->add( de );
	}
    }
}

void
PMRTreeNode::query( const Vector &N,
                    const Point &pop,
		    const int &dir,
		    bool *flag )
{
    int i;

    // if I am a leaf node, check all my dual edges
    if ( myIsLeaf )
    {
	for ( i = myEdges.size() ; i --> 0 ; )
	{
	    int id = myEdges[i]->getMeshId();
	    if ( !flag[id] )
	    {
		myEdges[i]->pickIfIntersect( N, 1 );
		flag[id] = true;
	    }
	}
    }

    // otherwise, recursively check my children
    else
    {
	int above, d = 0;
	above = myLoc.isAbove( N, pop );
	if ( above == 0 )
	{
	    for ( i = 8 ; i --> 0 ; )
	    {
		((PMRTreeNode *)myChild[i])->query( N, pop, dir, flag );
	    }

	    return;
	}

	else if ( above > 0 )
	    d = 7 - dir;
 	else if ( above < 0 )
 	    d = dir;

	const Point &n = myChild[d]->getMin(); 
	const Point &x = myChild[d]->getMax(); 

	double p[2][3] = { { (n.x-pop.x)*N.x, 
			     (n.y-pop.y)*N.y, 
			     (n.z-pop.z)*N.z },
			   { (x.x-pop.x)*N.x, 
			     (x.y-pop.y)*N.y, 
			     (x.z-pop.z)*N.z } };
	double check;

	for ( i = 8 ; i --> 0 ; )
	{
 	    if ( i == 7-d ) continue;

	    check = p[theVerts[i][0]][0]+
	            p[theVerts[i][1]][1]+
		    p[theVerts[i][2]][2];

// 	    if ( ( above > 0 && check <= 0 ) || ( above < 0 && check >= 0 ) )
	    if ( check == 0 || ( ( above > 0 ) != ( check < 0 ) ) )
		((PMRTreeNode *)myChild[i])->query( N, pop, dir, flag );
	}
    }
}

bool
PMRTreeNode::intersect( DualEdge &de )
{
    Point p0 = de.getDualVertex(0).getPoint();
    Point p1 = de.getDualVertex(1).getPoint();

    // if either endpoint is inside, the DE intersects me.
    if ( isInside( p0 ) || isInside( p1 ) ) return true;

    // check the supporting plane of each face for intersection with DE
    if ( checkFace( posX, -myMax.x, de ) ) return true;
    if ( checkFace( posY, -myMax.y, de ) ) return true;
    if ( checkFace( posZ, -myMax.z, de ) ) return true;

    if ( checkFace( negX, -myMin.x, de ) ) return true;
    if ( checkFace( negY, -myMin.y, de ) ) return true;
    if ( checkFace( negZ, -myMin.z, de ) ) return true;

    return false;
}

bool
PMRTreeNode::checkFace( const Vector &N, double D, DualEdge &de ) const
{
    double t = de.intersect( N, D );
    if ( de.goesThruInf() ? !( t > 0 && t < 1 ) : ( t >= 0 && t <= 1 ) )
	if ( isInside( de.param( t ) ) )
	    return true;

    return false;
}

// static GLfloat one[] = { 0.9f, 0.3f, 0.4f, 1.0f };
// static GLfloat two[] = { 0.2f, 0.5f, 1.0f, 1.0f };
// static GLfloat thr[] = { 0.5f, 0.5f, 1.0f, 1.0f };

void
PMRTreeNode::draw( int x, int level ) const
{
//     if ( level == 0 )
//     {
// 	drawMe();
// 	glColor3fv( one );
// 	glBegin( GL_LINES );
// 	for ( int i = myEdges.size() ; i --> 0 ; )
// 	{
// 	    Point &p = myEdges[i]->getDualVertex(0).getPoint();
// 	    Point &q = myEdges[i]->getDualVertex(1).getPoint();
// 
// 	    glVertex3d( p.x, p.y, p.z );
// 	    glVertex3d( q.x, q.y, q.z );
// 	}
// 	glEnd();
//     }
//     else
//     {
// 	myChild[x]->draw( 0, 0 );
//     }
}

// draws our box.
void
PMRTreeNode::drawMe() const
{
//    glColor3fv( thr );
//    glBegin( GL_LINE_LOOP );
//	for ( int i = 8 ; i --> 0 ; )	
//	    glVertex3d( myVerts[i].x, myVerts[i].y, myVerts[i].z );
//    glEnd();
}
