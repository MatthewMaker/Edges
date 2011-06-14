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
// $Id: Vector2.h,v 1.2 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <iostream>

#include "Common.h"

class Op;

class Vector2 : public Common
{
public:
    double x;
    double y;

    Vector2( void ) : x(0.0), y(0.0) {}
    Vector2( const Vector2 &v ) : x(v.x), y(v.y) {} 
    Vector2( double x1, double y1 ) : x(x1), y(y1) {}

    // Accept a visiting Op, ask it to operate on myself
    virtual	bool	acceptOp( Op &op );

		void	makeZero()	
			{ x = 0; y = 0; }

    const	Vector2	&operator=( const Vector2 &v )
			{ x=v.x; y=v.y; return *this; }
	    	bool	operator==( const Vector2 &v ) const
			{ return ( x==v.x && y==v.y ); }
	    	bool	operator!=( const Vector2 &v ) const
			{ return ( x!=v.x || y!=v.y ); }

		Vector2	operator+( const Vector2 &v ) const
			{ return Vector2( v.x+x, v.y+y ); }

    // scalar multiple, non-destructive
		Vector2	operator*( const double s ) const
			{ return Vector2( s*x, s*y ); }
    friend	Vector2	operator*( const double s, const Vector2 &v )
			{ return Vector2( s*v.x, s*v.y ); }

    // scalar multiple, destructive
		Vector2	&operator*=( const double s )
			{ x*=s; y*=s; return *this; }

    // dot product
		double	operator*( const Vector2 &v ) const
			{ return ( v.x * x + v.y * y ); }

		bool	isZero() const
			{ return ( x==0 && y==0 ); }

    friend	std::ostream &operator<<( std::ostream &os, Vector2 &v );

};

extern Vector2 ZeroVector2;

#define NEW_ZERO_VECTOR2 new Vector2()

#endif //__VECTOR_2_H__
