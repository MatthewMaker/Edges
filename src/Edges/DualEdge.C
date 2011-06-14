//--------------------------------------------------------------------------
// $Id: DualEdge.C,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#include "Common.h"
#include "DualVertex.h"
#include "DualEdge.h"
#include "HalfEdge.h"

DualEdge::DualEdge( DualVertex &p1, DualVertex &p2, HalfEdge &e )
:myDV1( p1 ),
 myDV2( p2 ),
 myHE( e ),
 myVec( p2.getPoint() - p1.getPoint() ),
 myGoesThruInf( false ),
 myChecked( false )
{
    p1.addDualEdge( this );
    p2.addDualEdge( this );
}

double
DualEdge::intersect( const Vector &N, const double D ) const
{
    return ( D + ( N * myDV2.getPoint() ) ) / ( N * myVec ) ;
}

Point
DualEdge::param( const double t, const bool dir ) const
{
    return ( dir ? myDV1.getPoint() : myDV2.getPoint() ) + ( myVec * t );
}

void
DualEdge::pickIfIntersect( const Vector &N, const double D )
{
    double t = ( D + ( N * myDV2.getPoint() ) ) / ( N * myVec ) ;
    if ( myGoesThruInf ) myHE.setPicked( !( t > 0 && t < 1 ) );
    else                 myHE.setPicked( ( t >= 0 && t <= 1 ) );

//    myHE.setPicked( ( myGoesThruInf != ( t >= 0 && t <= 1 ) ) );
    myChecked = true;
}
