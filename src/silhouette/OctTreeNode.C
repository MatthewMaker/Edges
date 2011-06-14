//--------------------------------------------------------------------------
// $Id: OctTreeNode.C,v 1.3 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#include <GL/gl.h>
#include <iostream>

#include "OctTreeNode.h"
#include "Array.h"
#include "DualVertex.h"
#include "DualEdge.h"

static int Xcomp( const void *one, const void *two )
{
    Point &a = (*((DualVertex **)one))->getPoint();
    Point &b = (*((DualVertex **)two))->getPoint();

    if ( a.x < b.x )		return -1;
    else if ( a.x == b.x )	return 0;
    else 			return 1;
}

static int Ycomp( const void *one, const void *two )
{
    Point &a = (*((DualVertex **)one))->getPoint();
    Point &b = (*((DualVertex **)two))->getPoint();

    if ( a.y < b.y )		return -1;
    else if ( a.y == b.y )	return 0;
    else 			return 1;
}

static int Zcomp( const void *one, const void *two )
{
    Point &a = (*((DualVertex **)one))->getPoint();
    Point &b = (*((DualVertex **)two))->getPoint();

    if ( a.z < b.z )		return -1;
    else if ( a.z == b.z )	return 0;
    else 			return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

OctTreeNode::OctTreeNode( double nx, double ny, double nz,
			  double xx, double xy, double xz,
			  DualVertex **dv, int num )
{
    myMin.x = nx; myMin.y = ny; myMin.z = nz;
    myMax.x = xx; myMax.y = xy; myMax.z = xz;

    // if there are few enough points given to me, I won't split,
    // and keep the points in me.
    if ( num <= ENOUGH )
    {
	std::cout << "OctTreeNode " << ": keeping " << num << std::endl;
	myIsLeaf = true;
	memset( myChild, 0, 8 * sizeof( OctTreeNode * ) );
	myDV = dv;
	myNum = num; 
    }

    // if there are too many points, I should split.
    // since we are given all the points for this block (and my subblocks)
    // I'll determine the optimal split point using algo from the
    // OVERMARS-1983 book.
    else
    {
	std::cout << "OctTreeNode " << ": splitting " << num << std::endl;

	myIsLeaf = false;
	myDV = NULL;
	myNum = 0;

	int xpos, ypos, zpos, elems; //, j ;

	// sort all vertices by the x-dimension - - - - - - - - - - - - - - -
	qsort( (void *)dv, num, sizeof( DualVertex * ), Xcomp ); 

	// find the 3/4 point 
	elems = (int)ceil((double)num/4.);
	xpos = num - elems;

	// discover the x-value of the DVs before and after xpos.
	// take their average and call that myLoc.x
	myLoc.x = ( dv[xpos]->getPoint().x + dv[xpos+1]->getPoint().x ) * 0.5;

	// sort the remaining 3/4 points by their y-coord - - - - - - - - - -
	qsort( (void *)dv, xpos, sizeof( DualVertex * ), Ycomp ); 

	// find the 1/2 point of the whole array = 2/3 point of the remaining
	ypos = xpos - elems;

	// discover the y-value of the DVs before and after ypos.
	// take their average and call that myLoc.x
	myLoc.y = ( dv[ypos]->getPoint().y + dv[ypos+1]->getPoint().y ) * 0.5;

	// sort the remaining 1/2 points by their z-coord - - - - - - - - - -
	qsort( (void *)dv, ypos, sizeof( DualVertex * ), Zcomp ); 

	// find the 1/4 point of the whole array = 1/2 point of the remaining
	zpos = ypos - elems;

	// discover the y-value of the DVs before and after ypos.
	// take their average and call that myLoc.x
	myLoc.z = ( dv[zpos]->getPoint().z + dv[zpos+1]->getPoint().z ) * 0.5;

	Array<DualVertex *> ar[8];
	DualVertex** aa[8];
	int nar[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	int i;
	for ( i = 8 ; i --> 0 ; )
	{
//	    ar[i] = (DualVertex**)calloc( elems+(elems/4), sizeof(DualVertex*) );
	    ar[i].init( elems + ( elems / 4 ) );
	}

	int x, y, z, idx;

	for ( i = num ; i --> 0 ; )
	{
	    Point &p = dv[i]->getPoint();
	    x = ( p.x < myLoc.x ? 0 : 1 );
	    y = ( p.y < myLoc.y ? 0 : 2 );
	    z = ( p.z < myLoc.z ? 0 : 4 );

	    idx = z + y + x;

//	    ar[idx][nar[idx]++] = dv[i];
	    ar[idx].append( dv[i] );
	}

	for ( i = 8 ; i --> 0 ; )
	{
	    aa[i] = ar[i].dupeGut();
	    nar[i] = ar[i].size();
	    ar[i].init(0);
	}

	// debug
//	for ( i = 8 ; i --> 0 ; )
//	{
//	    cout << endl;
//	    if( !nar[i] ) cout << i << " is empty" << endl;
//	    else
//	    {
//		cout << i << endl;
//		for ( j = nar[i] ; j --> 0 ; )
//		{
//		    cout << ar[i][j]->getPoint();
//		}
//	    }
//	}

	free( dv );

	myChild[0]= ( nar[0] ? new OctTreeNode(myMin.x,myMin.y,myMin.z,myLoc.x,myLoc.y,myLoc.z,aa[0],nar[0]) : NULL );
	myChild[1]= ( nar[1] ? new OctTreeNode(myLoc.x,myMin.y,myMin.z,myMax.x,myLoc.y,myLoc.z,aa[1],nar[1]) : NULL );
	myChild[2]= ( nar[2] ? new OctTreeNode(myMin.x,myLoc.y,myMin.z,myLoc.x,myMax.y,myLoc.z,aa[2],nar[2]) : NULL );
	myChild[3]= ( nar[3] ? new OctTreeNode(myLoc.x,myLoc.y,myMin.z,myMax.x,myMax.y,myLoc.z,aa[3],nar[3]) : NULL );
	myChild[4]= ( nar[4] ? new OctTreeNode(myMin.x,myMin.y,myLoc.z,myLoc.x,myLoc.y,myMax.z,aa[4],nar[4]) : NULL );
	myChild[5]= ( nar[5] ? new OctTreeNode(myLoc.x,myMin.y,myLoc.z,myMax.x,myLoc.y,myMax.z,aa[5],nar[5]) : NULL );
	myChild[6]= ( nar[6] ? new OctTreeNode(myMin.x,myLoc.y,myLoc.z,myLoc.x,myMax.y,myMax.z,aa[6],nar[6]) : NULL );
	myChild[7]= ( nar[7] ? new OctTreeNode(myLoc.x,myLoc.y,myLoc.z,myMax.x,myMax.y,myMax.z,aa[7],nar[7]) : NULL );
    }
}

OctTreeNode::~OctTreeNode()
{
    for ( int i = 8 ; i --> 0 ; )
	if ( myChild[i] )
	    delete( myChild[i] );
}

void
OctTreeNode::query( Vector &newN )
{
    int i, j;

    // if I am a leaf node, check all dual edges
    // connected to my dual vertices
    if ( myIsLeaf )
	for ( i = myNum ; i --> 0 ; )
	    for ( j = myDV[i]->getNumDE() ; j --> 0 ; )
		myDV[i]->getDE(j).pickIfIntersect( newN, 1 );

    // otherwise, recursively check my children
    else
    {
	for ( i = 8 ; i --> 0 ; )
	    if ( myChild[i] )
		((OctTreeNode *)myChild[i])->query( newN );
    }
}

void
OctTreeNode::query( Vector &newN, Point &newP, int &newI,
	            Vector &oldN, Point &oldP, int &oldI )
{
    int i, j;
    int oldChk, newChk;

    // if I am a leaf node, check all dual edges
    // connected to my dual vertices
    if ( myIsLeaf )
    {
	for ( i = myNum ; i --> 0 ; )
	{
	    for ( j = myDV[i]->getNumDE() ; j --> 0 ; )
	    {
		myDV[i]->getDE(j).pickIfIntersect( newN, 1 );
	    }
	}
    }

    // otherwise, recursively check my children
    else
    {
	for ( i = 8 ; i --> 0 ; )
	{
	    if ( myChild[i] )
	    {
		// grab intersect code for the child block with
		// the 2 planes
		newChk = ((OctTreeNode *)myChild[i])->intersect( newN, newP );
		if ( newChk != 0 )
		    oldChk = ((OctTreeNode *)myChild[i])->intersect( oldN, oldP );

		// if the child block intersects with either planes,
		// or if the block is on different sides of them
		// query it.
		if ( newChk == 0 || oldChk == 0 ) // || newChk != oldChk )
		{
		    ((OctTreeNode *)myChild[i])->query( newN, newP, newI,
		                                        oldN, oldP, oldI ); 
		}
		else if ( newChk != oldChk )
		{
		    ((OctTreeNode *)myChild[i])->query( newN );
		}
	    }
	}
    }
}

// returns:
//  0 = intersects
//  1 = this box completely above
// -1 = this box completely below
int
OctTreeNode::intersect( Vector &N, Point &pop )
{
    double	 check;
    bool	 isAbove;
    double p[2][3] = { { (myMin.x-pop.x)*N.x, 
                         (myMin.y-pop.y)*N.y, 
                         (myMin.z-pop.z)*N.z },
                       { (myMax.x-pop.x)*N.x, 
                         (myMax.y-pop.y)*N.y, 
                         (myMax.z-pop.z)*N.z } };

    if ( ( check = p[1][0] + p[1][1] + p[1][2] ) == 0 ) return 0;
    isAbove = ( check > 0 );

    for ( int i = 7 ; i --> 0 ; )
    {
	check = p[((i&4)!=0)][0] + p[((i&2)!=0)][1] + p[((i&1)!=0)][2]; 
	if ( check == 0 ) return 0;
	else if ( ( check > 0 ) != isAbove ) return 0;
    }

    // otherwise, they are all on the same side of the plane, 
    return ( isAbove ? 1 : -1 );
}

// static GLfloat one[] = { 0.9f, 0.3f, 0.4f, 1.0f };
// static GLfloat two[] = { 0.2f, 0.5f, 1.0f, 1.0f };
// static GLfloat thr[] = { 0.5f, 0.5f, 1.0f, 1.0f };

void
OctTreeNode::draw( int x, int level ) const
{
//     if ( level == 0 )
//     {
// 	drawMe();
// 	glColor3fv( one );
// 	glBegin( GL_POINTS );
// 	for ( int i = myNum ; i --> 0 ; )
// 	{
// 	    Point &p = myDV[i]->getPoint();
// 	    glVertex3d( p.x, p.y, p.z );
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
OctTreeNode::drawMe() const
{
//    glColor3fv( thr );
//    glBegin( GL_LINE_LOOP );
//	for ( int i = 8 ; i --> 0 ; )	
//	    glVertex3d( myVerts[i].x, myVerts[i].y, myVerts[i].z );
//    glEnd();
}
