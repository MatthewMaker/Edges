//--------------------------------------------------------------------------
// $Id: silhouette.C,v 1.5 2001/03/21 15:39:22 vchma Exp $
//--------------------------------------------------------------------------

#include "silhouette.h"
#include "Mesh.h"
#include "Point.h"
#include "DualEdge.h"
#include "DualVertex.h"
#include "HalfEdge.h"
#include "Vector.h"
#include "PMRTree.h"
#include "OctTree.h"

#include <cstring>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <ctime>
#include <sys/types.h>

Silhouette::Silhouette()
: myMesh( NULL ),
  myHasLastPoint( false ),
  myTree( NULL ),
  myOtherTree( NULL )
{
}

Silhouette::~Silhouette()
{
}

void
Silhouette::drawTree( int x ) const
{
    myTree->draw( x );
}

void
Silhouette::useThisMesh( Mesh *m )
{
    myMesh = m;
    myHasLastPoint = false;
    myLastPoint.makeZero();

    DVList &dvm = DVList::getDVM();

    if ( myTree ) delete myTree;
    if ( myOtherTree ) delete myOtherTree;

    if ( myMesh->numDEdges() > theSplittingThreshold )
    {
	myTree = new PMRTree( myMesh->getDMin(),
	                      myMesh->getDMax(),
			      myMesh->numDEdges() );

	myTree->add( myMesh->getDualEdges() );

	std::cout << "Using PMRTree Optimization" << std::endl;
    }
    else
    {
	myTree = NULL;
	std::cout << "Small enough to brute force" << std::endl;
    }

    if ( dvm.numDV() > OctTree::theCapacity )
    {
	myOtherTree = new OctTree( myMesh->getDMin(), myMesh->getDMax() );
	std::cout << "Using OctTree Optimization" << std::endl;
    }
    else
    {
	myOtherTree = NULL;
	std::cout << "Small enough to not care about coherency" << std::endl;
    }
}

#if 0
static bool
isOnTop( Point &normalRep, Point &pointOnPlane, Point &query )
{
    Vector V = query - pointOnPlane;
    return ( V * normalRep >= 0 );
}
#endif

bool
Silhouette::markSilhouetteEdges( Point &vp, bool opt1, bool opt2 )
{
    Point	 pop;	 // point on plane
    DualEdge	**de;
    double	 x = fabs(vp.x);
    double	 y = fabs(vp.y);
    double	 z = fabs(vp.z);
    Vector N( vp );

    int dir = 0;
    int i;

    char opt = ' ';

    // given viewpoint v = ( a, b, c )
    // the planar equation of the dual "viewplane" is
    //   ax + by + cz + 1 = 0

    // now, figure out a point on the plane
    dir = ( x > y ? ( x > z ? 0 : 2 ) : ( y > z ? 1 : 2 ) );
    switch ( dir )
    {
	case 0:	pop.x = -1.0 / vp.x; break;
	case 1:	pop.y = -1.0 / vp.y; break;
	case 2:	pop.z = -1.0 / vp.z; break;
    }

    clock_t before = clock();
    if ( myHasLastPoint && myOtherTree && opt2 )
    {
	opt = 'O';
	myOtherTree->query( N, pop, myLastPoint, myLastPop );
    }
    else
    {
	if ( myTree && opt1 )
	{
	    opt = 'P';
	    myTree->query( N, pop );
	}
	else
	{
	    de = myMesh->getDualEdges();

	    for ( i = myMesh->numDEdges() ; i --> 0 ; )
	    {
		Vector pp = de[i]->getVector();

		// if the viewplane is not parallel to the line segment
		if ( fabs(N * pp) > DBL_EPSILON*1.0e6 )
		    de[i]->pickIfIntersect( N, 1 );
		else
		    de[i]->getHalfEdge().setPicked( false );
	    }

	    free( (void *)de );
	}
    }
    clock_t after = clock();
    std::cout << opt << " timing: ";
    std::cout << ( (double)(after-before) / CLOCKS_PER_SEC );
    std::cout << "s" << std::endl;

    myLastPoint = N;
    myLastPop = pop;
    myHasLastPoint = true;

    return true;
}

void
Silhouette::getTiming( char *buf ) const
{
    strcpy( buf, myTiming );
}
