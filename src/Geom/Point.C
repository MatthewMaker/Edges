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
// $Id: Point.C,v 1.2 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#include <cstdlib>
#include <iostream>

#include "Point.h"
#include "Vector.h"
#include "Op.h"

bool
Point::acceptOp( Op &op )
{
    return op( this );
}

// Point-Vector Addition --------------------------------------------------
Point&
Point::operator+=( const Vector &v )
{
    x+=v.x;
    y+=v.y;
    z+=v.z;
    return *this;
}

// Point-Point substraction -----------------------------------------------
Vector
Point::operator-( const Point &p ) const
{
    return Vector( x-p.x, y-p.y, z-p.z );
}

// Point above plane ------------------------------------------------------
bool
Point::isAbove( const Vector &normal, const Point &p, bool on ) const
{
    Vector V( x - p.x, y - p.y, z - p.z );
    return ( on ? V * normal >= 0 : V * normal > 0 );
}

int
Point::isAbove( const Vector &normal, const Point &p ) const
{
    Vector V( x - p.x, y - p.y, z - p.z );
    double d = V * normal;
    return ( d > 0 ? 1 : ( d == 0 ) ? 0 : -1 );
}

// Output functions -------------------------------------------------------
std::ostream &
operator<<( std::ostream &os, Point &p )
{
    return os << "(" << p.x << "," << p.y << "," << p.z << ")" << std::endl;
}
