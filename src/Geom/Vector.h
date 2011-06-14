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
// $Id: Vector.h,v 1.2 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iosfwd>

#include "Common.h"

class Point;
class Op;

class Vector : public Common
{
public:
    double x;
    double y;
    double z;

    Vector( void ) : x(0.0), y(0.0), z(0.0) {}
    Vector( const Vector &v ) : x(v.x), y(v.y), z(v.z) {} 
    Vector( const Point &p );
    Vector( double x1, double y1, double z1 ) : x(x1), y(y1), z(z1) {}

    // Accept a visiting Op, ask it to operate on myself
    virtual bool	 acceptOp( Op &op );


    void		 makeZero()
			 { x = 0.; y = 0.; z = 0.; }
    bool		 isZero() const
			 { return ( x == 0. && y == 0. && z == 0. ); }

    // Assignment
    Vector		&operator=( const Vector &v )
			 { x=v.x; y=v.y; z=v.z; return *this; }

    // Equivalence
    bool		 operator==( const Vector &v ) const
			 { return ( x==v.x && y==v.y && z==v.z ); }
    bool		 operator!=( const Vector &v ) const
			 { return ( x!=v.x || y!=v.y || z!=v.z ); }

    // Vector negation
    const Vector	 operator-( void ) const
			 { return Vector( -x, -y, -z ); }

    // Vector-Vector addition
    const Vector	&operator+=( const Vector &v ) 
			 { x+=v.x, y+=v.y, z+=v.z; return *this; }
    friend const Vector	 operator+( const Vector &p, const Vector &q )
			 { return Vector( p ) += q; }

    // Vector-Vector substraction
    const Vector	&operator-=( const Vector &v ) 
			 { x-=v.x, y-=v.y, z-=v.z; return *this; }
    friend const Vector	 operator-( const Vector &p, const Vector &q )
			 { return Vector( p ) -= q; }

    // scalar multiple
    const Vector	&operator*=( const double s )
			 { x*=s; y*=s; z*=s; return *this; }
    friend const Vector	 operator*( const Vector &v, const double s )
			 { return Vector( v ) *= s; }
    friend const Vector	 operator*( const double s, const Vector &v )
			 { return Vector( v ) *= s; }

    // dot product
		double	 operator*( const Vector &v ) const
			 { return (v.x * x + v.y * y + v.z * z); }
		double	 dot( const Vector &v ) const
			 { return (v.x * x + v.y * y + v.z * z); }
		double	 operator*( const Point &p ) const;

    // cross product
    Vector		 operator^( const Vector &v ) const
			 { return Vector( y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x ); }
    Vector		 cross( const Vector &v ) const
			 { return Vector( y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x ); }

    // in-place unitization, returning the length before normalization
    double		 normalize();

    friend std::ostream &operator<<( std::ostream &os, Vector &v );
};

extern Vector ZeroVector;

#define NEW_ZERO_VECTOR new Vector()

#endif //__VECTOR_H__
