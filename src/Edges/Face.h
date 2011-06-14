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
// $Id: Face.h,v 1.3 2000/10/24 03:31:42 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __FACE_H__
#define __FACE_H__

#include "Common.h"
#include "utility.hpp"

#include <fstream>

class HalfEdge;
class Vertex;
class Vector;
class DualVertex;

class Face : public Common, boost::noncopyable
{
public:

    // nested iterator class ------------------------------------------------
    class EdgeIterator
    {
	public:
	    EdgeIterator() : head(NULL), cur(NULL) {}
	    EdgeIterator( const Face &f ): head(f.myEdge), cur(f.myEdge) {}
	    ~EdgeIterator() {}

	    void	 resetFace( Face &f )
			 { head = f.myEdge; cur = f.myEdge; }

	    bool	 hasNext() const
			 { return head && cur ?
			          head!=cur :
				  false; }

	    HalfEdge	*getNext();

	private:
	    EdgeIterator( EdgeIterator & );

	    HalfEdge	*head;
	    HalfEdge	*cur;
    };
    friend class Face::EdgeIterator;
    // nested iterator class ------------------------------------------------

    Face( int numEdges, HalfEdge *e );
    Face();

    virtual ~Face();

    // Accept a visiting Op, ask it to operate on myself
    virtual bool	 acceptOp( Op &op );

    // equivalence
    bool 		 operator==( Face &f )	{ return ( this == &f ); }
    bool 		 operator!=( Face &f )	{ return ( this != &f ); }

    // returns the normal of this face
    Vector		&getNormal();

    // get the dual point representation of this face.
    DualVertex		*getDual();

    int			 numEdges() const	{ return myNumEdges; }
    void		 setEdge( HalfEdge *e ) { myEdge = e; }
    HalfEdge		&getEdge() const	{ return *myEdge; }

    friend std::ostream	&operator<<( std::ostream &os, Face &f );

private:

    // calculate normal
    Vector			&calcNormal();
    void			 calcDual();

    HalfEdge			*myEdge;
    int				 myNumEdges;
    Vector			*myNormal;

    DualVertex			*myDual;
};

#endif //__FACE_H__

