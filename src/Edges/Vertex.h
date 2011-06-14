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
// $Id: Vertex.h,v 1.4 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "utility.hpp"
#include "Common.h"
#include "Point.h"

#include <iosfwd>

class HalfEdge;
class Vector2;
class Op;

class Vertex : public Common, boost::noncopyable
{
public:

    Vertex( Point &p, Vector *normal = 0, Vector2 *uv = 0 )
    : myPoint( p ), myNormal( normal ), myUV( uv ) {}

    virtual ~Vertex()				{}

    // Accept a visiting Op, ask it to operate on myself
    virtual bool	 acceptOp( Op &op );

    // a bunch of get/sets - - - - - - - - - - - - - - - - - - - - - - - - 
    Point		&getPoint( void ) const	{ return myPoint; }

    void		 setNormal( Vector *n )	{ myNormal = n; }
    const Vector	*getNormal() const	{ return myNormal; }

    void		 setUV( Vector2 *UV ) 	{ myUV = UV; }
    const Vector2	*getUV() const		{ return myUV; }

    void		 setEdge( HalfEdge *e )	{ myEdge = e; }
    HalfEdge		&getEdge() const	{ return *myEdge; }

    // equivalence - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // yes, this is a funny definition of equivalent.  We only care
    // if two unique vertices point to the same point.  If they
    // do, they represent the same point in space, and so they are
    // equivalent in that sense.  Yes, that means two vertices
    // that represent the same point can have different normals and
    // UVs.. that's fine.
    bool	 	 operator==( Vertex &v )
			 { return myPoint == v.myPoint; }
    bool	 	 operator!=( Vertex &v )
			 { return myPoint != v.myPoint; }

    // output - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    friend std::ostream	&operator<<( std::ostream &os, Vertex &v );

private:
    Vertex();

    Point		&myPoint;
    HalfEdge		*myEdge;

    Vector		*myNormal;
    Vector2		*myUV;
};

#endif //__VERTEX_H__
