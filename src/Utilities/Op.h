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
// $Id: Op.h,v 1.1 2001/03/21 04:23:38 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __OP_H__
#define __OP_H__

#include "utility.hpp"

// Implements a "Visitor" class pattern.  This will be an operation
// that traverses a mesh and operates on each element.  How the op
// traverses the Mesh depends on the the individual Op itself.
class Mesh;
class Face;
class Point;
class Vertex;
class Vector;
class Vector2;
class HalfEdge;

class Op : public boost::noncopyable
{
public:
	   		 Op()				{}
    virtual		~Op()				{}

    virtual bool	 operator==( Op &o ) const	{ return false; }

    virtual bool	 operator()( Mesh *m )		= 0;
    virtual bool	 operator()( Face *f )		= 0;
    virtual bool	 operator()( Point *v )		= 0;
    virtual bool	 operator()( Vertex *v )	= 0;
    virtual bool	 operator()( Vector *v )	= 0;
    virtual bool	 operator()( Vector2 *v )	= 0;
    virtual bool	 operator()( HalfEdge *e )	= 0;

};

#endif //__OP_H__

