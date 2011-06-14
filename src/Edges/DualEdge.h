//--------------------------------------------------------------------------
// $Id: DualEdge.h,v 1.2 2001/03/21 15:39:22 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __DUALEDGE_H__
#define __DUALEDGE_H__

#include "Common.h"
#include "Vector.h"
#include "DualVertex.h"

class HalfEdge;

class DualEdge : public Common
{
public:
    
    // constructor with 2 points as endpoints
    DualEdge( DualVertex &p1, DualVertex &p2, HalfEdge &e );

    // destructor
    virtual ~DualEdge()				{}

    // Accept a visiting Op, ask it to operate on myself
    virtual bool	 acceptOp( Op &op )	{ return false; }

    DualVertex	&getDualVertex( int i ) const	{ return (i==1?myDV1:myDV2); }

    bool	 goesThruInf() const		{ return myGoesThruInf; }
    void	 setGoesThruInf( const bool b )	{ myGoesThruInf = b; }

    HalfEdge	&getHalfEdge() const		{ return myHE; }

    Vector	 getVector( const bool dir = false ) const
		 { return ( dir ? myVec * -1 : myVec ); }

    double	 intersect( const Vector &N, const double D ) const;
    Point	 param( const double t, const bool dir = false ) const;

    void	 pickIfIntersect( const Vector &N, const double D );

    bool	 checked() const		{ return myChecked; }
    void	 setChecked( const bool y )	{ myChecked = y; }
    
private:
    DualEdge( );
    DualEdge( DualEdge & );

    DualVertex	&myDV1;
    DualVertex	&myDV2;
    HalfEdge	&myHE;

    Vector	 myVec; 

    bool	 myGoesThruInf;
    bool	 myChecked;
};

#endif //__DUALEDGE_H__
