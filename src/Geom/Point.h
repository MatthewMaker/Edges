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
// $Id: Point.h,v 1.2 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __POINT_H__
#define __POINT_H__

#include <iosfwd>

#include "Common.h"

class Vector;
class Op;

class Point : public Common
{
public:

    double	x;
    double	y;
    double	z;

    Point( void ) : x(0.), y(0.), z(0.) {}
    Point( const Point &p ) : x(p.x), y(p.y), z(p.z) {} 
    Point( double x1, double y1, double z1 ) : x(x1), y(y1), z(z1) {}

    virtual ~Point()	 {}

    // Accept a visiting Op, ask it to operate on myself
    virtual bool	 acceptOp( Op &op );

    void		 makeZero()
			 { x = 0.; y = 0.; z = 0.; }

    // assignment
    Point		&operator=( const Point &p )
			 { x = p.x; y = p.y; z = p.z; return *this; }

    // equivalence
    virtual bool 	 operator==( const Point &p ) const
			 { return ( x==p.x && y==p.y && z==p.z ); }
    virtual bool 	 operator!=( const Point &p ) const
			 { return ( x!=p.x || y!=p.y || z!=p.z ); }

    // scalar multiplication (hack! not defined mathematically) 
    Point		&operator*=( const double d )
			 { x*=d; y*=d; z*=d; return *this; }
    friend const Point	 operator*( const double s, const Point &p )
			 { return Point( p ) *= s; }
    friend const Point	 operator*( const Point &p, const double s )
			 { return Point( p ) *= s; }


    // Point-Point addition (hack! not defined mathematically) 
    Point		&operator+=( const Point &p )
			 { x+=p.x; y+=p.y; z+=p.z; return *this; }
    friend const Point	 operator+( const Point &p, const Point &q )
			 { return Point( p ) += q; }

    // Point-Vector addition
    Point		&operator+=( const Vector &v );
    friend const Point	 operator+( const Vector &v, const Point &p )
			 { return Point( p ) += v; }
    friend const Point	 operator+( const Point &p, const Vector &v )
			 { return Point( p ) += v; }
			     
    // Point-Point subtraction
    Vector		 operator-( const Point &p ) const;

    friend std::ostream	&operator<<( std::ostream &os, Point &p );

    // checks to see if the this point is above the plane
    // represented by normalRep and pointOnPlane
    // if "on", then true if this point is on the plane.
    // o/w, only strictly above.
    bool		 isAbove( const Vector &normal,
	                          const Point &pointOnPlane,
				  bool on ) const;

    // same as above, but -1 = below, 0 = on, 1 = above
    int			 isAbove( const Vector &normal,
	                          const Point &pointOnPlane ) const;
};


#endif // __POINT_H__
