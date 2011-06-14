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
// $Id: HalfEdge.h,v 1.4 2000/10/24 03:31:42 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __HALFEDGE_H__
#define __HALFEDGE_H__

class Vector;
class Vertex;
class Point;
class Face;
class Op;

#include "Common.h"
#include "utility.hpp"

// Half Edge:
// ----------->>*
// ^edge        ^myVertex

class HalfEdge : public Common, boost::noncopyable
{
public:

    // Iterator - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // lets you traverse the circular link list that is of
    // EVERY "HalfEdge" class as if it has an end.
    //
    // usage: [e is a HalfEdge pointer]
    // for ( HalfEdge::Iterator i(e) ; i ; ++i )
    // {
    //	  i->some_method();
    // }
    //
    class Iterator 
    {
	public:
	    Iterator( HalfEdge *e, bool lapped = false )
		: myHead(e), myCur(e), myLapped(lapped) {}
	    Iterator( const Iterator &i )
		: myHead(i.myHead), myCur(i.myCur), myLapped(i.myLapped) {}

	    // assignment
	    Iterator &operator=( const Iterator &i );

	    // equivalence
	    bool operator==( const Iterator &i ) const;

	    // prefix++
	    void operator++();

	    // provides access to the pointer we are at right now
	    HalfEdge	*operator*()	{ return myCur; }
	    HalfEdge	*operator->()	{ return myCur; }

	    // these are used in conditions, to check to see if we are
	    // done iterating.
	    operator void*() const { return ( myLapped ? 0 : (void *)this ); }
	    bool operator!() const { return ( myLapped ? true : false ); }

	private:
	    HalfEdge	*myHead;
	    HalfEdge	*myCur;
	    bool	 myLapped;
    };
    // Iterator - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    friend class HalfEdge::Iterator;

    // constructor with vertex and face , default prev = next = sym = this
    // This means every HalfEdge starts as a circular doubly-linked list.
    // This is an invariant, maintained by insertNext() and insertPrev()
    HalfEdge( Vertex &v, Face &f );
    virtual ~HalfEdge()				{}

    // Accept a visiting Op, ask it to operate on myself
    virtual bool	 acceptOp( Op &op );

    // insert Next edge, e->next will dup this->next
    // e better be a single edge (no prev or next)
    bool	 insertNext( HalfEdge *e );
    HalfEdge	&getNext( void ) const		{ return *myNext; }
    
    // set Next edge, e->next will dup this->next
    // e better be a single edge (no prev or next)
    bool	 insertPrev( HalfEdge *e );
    HalfEdge	&getPrev( void ) const		{ return *myPrev; }

    // set Sym edge e->sym = this if possible, else bad!
    void	 forceSym( HalfEdge *e )	{ mySym = e; e->mySym = this; }
    bool	 checkSym( void );

    HalfEdge	&getSym( void ) const		{ return *mySym; }

    Vertex	&getVertex( void ) const	{ return myVertex; }
    Face	&getFace( void ) const		{ return myFace; }

    // find the vector represented by this edge
    Vector	 edgeVector() const;

    // Edge Traversal - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    // Returns a reference to the adjacent edge that points to my vertex
    HalfEdge	&getNextEdgeSharingMyVertex() const;

    // true if edge has no sym (that's bad)
    bool	 isSingle() const		{ return (mySym==this); }

    // true if this edge is a seam
    void	 setSeam( bool yes );
    bool	 isSeam() const			{ return myFlags & IS_SEAM; }

    // toggle the "picked" statis of this edge
    void	 setPicked( bool yes );
    bool	 isPicked() const		{ return myFlags & IS_PICKED; }

private:
    HalfEdge();

    // true if edge is isolated (that's bad)
    bool	 isIsolated() const		{ return ( myPrev==this ||
	                                                   myNext==this ); }
    
    // checks to see if my Sym and I can be considered a seam edge.
    // I'm a seam if my vertex or my sym's has multiple normals/UVs.
    // ( *IMPORTANT* only called within checkSym() )
    void	 checkSeam();

    HalfEdge	*myNext;
    HalfEdge	*myPrev;
    HalfEdge	*mySym;
    Vertex	&myVertex;
    Face	&myFace;

    // Flags ...
    // TODO, me and my sym should really share the same "flag" char
    // so that we don't need to symo update both flags.. :(
    unsigned char myFlags;

    // Picked.. if this edge is selected
    // I'm a seam if my vertex or my sym's has multiple normals/UVs.
    enum
    {
	IS_PICKED = 1,	/* flag is on if this edge is picked */
	IS_SEAM	  = 2,  /* flag is on if I'm a seam edge */
    };

//    bool	 myIsPicked;
//    bool	 myIsSeam;
};

#endif //__HALFEDGE_H__
