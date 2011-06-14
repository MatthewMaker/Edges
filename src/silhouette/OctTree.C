//--------------------------------------------------------------------------
// $Id: OctTree.C,v 1.1.1.1 2000/09/17 06:56:05 vchma Exp $
//--------------------------------------------------------------------------

#include "OctTree.h"
#include "OctTreeNode.h"
#include "Vector.h"
#include "Point.h"
#include "DualVertex.h"

const int OctTree::theCapacity = ENOUGH; 

OctTree::OctTree( Point &min, Point &max )
:myMin( min ),
 myMax( max )
{
    DVList &dvm = DVList::getDVM();
    myRoot = new OctTreeNode( min.x, min.y, min.z,
			      max.x, max.y, max.z,
			      dvm.getArray(), dvm.numDV() );
}

OctTree::~OctTree()
{
    delete myRoot;
}

void
OctTree::query( Vector &newN, Point &newP,
	        Vector &oldN, Point &oldP ) const
{
    int newChk = myRoot->intersect( newN, newP );
    int oldChk = myRoot->intersect( oldN, oldP );
    int x, y, z, newI, oldI;

    if ( newChk == 0 || oldChk == 0 || newChk != oldChk )
    {
//	cout << "going in!" << endl;

	// figure out the orientation number.
	x = ( newN.x < 0 ? 0 : 1 );
	y = ( newN.y < 0 ? 0 : 2 );
	z = ( newN.z < 0 ? 0 : 4 );
	newI = z + y + x;
	x = ( oldN.x < 0 ? 0 : 1 );
	y = ( oldN.y < 0 ? 0 : 2 );
	z = ( oldN.z < 0 ? 0 : 4 );
	oldI = z + y + x;

	myRoot->query( newN, newP, newI, oldN, oldP, oldI ); 
    }
}

void
OctTree::draw( int x ) const
{
}
