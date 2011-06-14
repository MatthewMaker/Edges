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
// $Id: Vector.C,v 1.2 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Vector.h"
#include "Point.h"
#include "Op.h"

Vector ZeroVector;

// Constructor that takes a point ---------------------------------------------
Vector::Vector( const Point &p )
: x(p.x), y(p.y), z(p.z)
{
} 

bool
Vector::acceptOp( Op &op )
{
    return op( this );
}

// Hackish Vector-Point Dot product -------------------------------------------
double
Vector::operator*( const Point &p ) const
{
    return (p.x * x + p.y * y + p.z * z);
}

// normalize this vector and return length ------------------------------------
double
Vector::normalize()
{
    double denom = 0.0;

    double tx = ( x > 0.0 ? x : -x );
    double ty = ( y > 0.0 ? y : -y );
    double tz = ( z > 0.0 ? z : -z );

    if( tx > ty )
    {
	if( tx > tz )
	{
	    if( 1.0 + tx > 1.0 )
	    {
		ty = ty / tx;
		tz = tz / tx;
		denom = 1.0 / ( tx * sqrt( 1.0 + ty*ty + tz*tz ) );
	    }
	}
	else /* tz > tx > ty */
	{
	    if( 1.0 + tz > 1.0 )
	    {
		ty = ty / tz;
		tx = tx / tz;
		denom = 1.0 / ( tz * sqrt( 1.0 + ty*ty + tx*tx ) );
	    }
	}
    }
    else
    {
	if( ty > tz )
	{
	    if( 1.0 + ty > 1.0 )
	    {
		tz = tz / ty;
		tx = tx / ty;
		denom = 1.0 / ( ty * sqrt( 1.0 + tz*tz + tx*tx ) );
	    }
	}
	else /* tx < ty < tz */
	{
	    if( 1.0 + tz > 1.0 )
	    {
		ty = ty / tz;
		tx = tx / tz;
		denom = 1.0 / ( tz * sqrt( 1.0 + ty*ty + tx*tx ) );
	    }
	}
    }
    if( 1.0 + tx + ty + tz > 1.0 )
    {
	operator*=( denom );
	return 1.0/denom;
    }
    return 0.0;
}

// Output ---------------------------------------------------------------------
std::ostream &
operator<<( std::ostream &os, Vector &v )
{
    return os << v.x << " " << v.y << " " << v.z << "," << std::endl;
}
