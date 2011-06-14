//--------------------------------------------------------------------------
// $Id: DualVertex.C,v 1.3 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#include "DualEdge.h"
#include "DualVertex.h"
#include "Point.h"

#include <cstdlib>
#include <iostream>

DualVertex::DualVertex( double x, double y, double z, double D )
: myPoint( new Point( x/D, y/D, z/D ) )
{
    myHomo[0] = x;
    myHomo[1] = y;
    myHomo[2] = z;
    myHomo[3] = D;

    myMId = DVList::getDVM().addDV( this );
}

std::ostream&
operator<<( std::ostream &os, DualVertex &v ) 
{
    return os << "DualVertex " << v.getMeshId() << " " << v.getPoint();
}

//============================================================================

static int Xcomp( const void *one, const void *two )
{
    Point &a = ((DualVertex *)one)->getPoint();
    Point &b = ((DualVertex *)two)->getPoint();

    if ( a.x < b.x )		return -1;
    else if ( a.x == b.x )	return 0;
    else 			return 1;
}

static int Ycomp( const void *one, const void *two )
{
    Point &a = ((DualVertex *)one)->getPoint();
    Point &b = ((DualVertex *)two)->getPoint();

    if ( a.y < b.y )		return -1;
    else if ( a.y == b.y )	return 0;
    else 			return 1;
}

static int Zcomp( const void *one, const void *two )
{
    Point &a = ((DualVertex *)one)->getPoint();
    Point &b = ((DualVertex *)two)->getPoint();

    if ( a.z < b.z )		return -1;
    else if ( a.z == b.z )	return 0;
    else 			return 1;
}

DVList&
DVList::getDVM()
{
    static DVList *dvm = NULL;

    if ( !dvm ) dvm = new DVList();

    return *dvm;
}

void
DVList::sort( DVDim dim, int s, int e )
{
    switch( dim )
    {
	case DIM_X: myDV.sort( s, e, Xcomp ); break;
	case DIM_Y: myDV.sort( s, e, Ycomp ); break;
	case DIM_Z: myDV.sort( s, e, Zcomp ); break;
    }
}

std::ostream&
operator<<( std::ostream &os, DVList &v )
{
    DualVertex **a = v.myDV.dupeGut();

    for ( unsigned int i = 0 ; i < v.myDV.size() ; i++ )
    {
	os << a[i]->getPoint();
    }

    free( (void *)a );

    return os;
}
