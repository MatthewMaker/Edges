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
// $Id: HalfEdge.C,v 1.5 2000/10/29 20:58:38 vchma Exp $
//--------------------------------------------------------------------------

#include "Vector.h"
#include "Vector2.h"
#include "Vertex.h"
#include "Point.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Op.h"

HalfEdge::HalfEdge( Vertex &v, Face &f )
: myNext(this), myPrev(this), mySym(this),
  myVertex(v), myFace(f), myFlags(0)
{
    v.setEdge( this );
}

bool
HalfEdge::acceptOp( Op &op )
{
    return op( this );
}

bool
HalfEdge::insertNext( HalfEdge *e )
{
    // we share the same face, I am prev of my next
    if ( e->getFace() == myFace )
    {
	e->myPrev =  this;
	myNext->myPrev = e;
	e->myNext = myNext;
	myNext = e;
	return true;
    }

    return false;
}

bool
HalfEdge::insertPrev( HalfEdge *e )
{
    // we share the same face, I am next to my prev
    if ( e->getFace() == myFace )
    {
	e->myNext = this;
	myPrev->myNext = e;
	e->myPrev = myPrev;
	myPrev = e;
	return true;
    }

    return false;
}

void
HalfEdge::checkSeam()
{
    const Vector *v[4] = { mySym->getPrev().getVertex().getNormal(),
		           getVertex().getNormal(),
		           myPrev->getVertex().getNormal(),
		           mySym->getVertex().getNormal() };

    // check first pair of vertices.. if the pointers are non-null,
    // and they represent different normals, I'm a seam edge.
    if ( v[0] && v[1] && ( *(v[0]) != *(v[1]) ) )
    {
	setSeam( true );
	
	// If I'm a seam edge because of that, I don't have to check
	// anymore..
	return;
    }

    // otherwise, do the same check for the 2nd pair.
    if ( v[2] && v[3] && ( *(v[2]) != *(v[3]) ) )
    {
	setSeam( true );
	return;
    }

    // now check texture coordinates.
    const Vector2 *t[4] = { mySym->getPrev().getVertex().getUV(),
		            getVertex().getUV(),
		            myPrev->getVertex().getUV(),
		            mySym->getVertex().getUV() };

    // check first pair of vertices.. if the pointers are non-null,
    // and they represent different UVs, I'm a seam edge.
    if ( t[0] && t[1] && ( *(t[0]) != *(t[1]) ) )
    {
	setSeam( true );
	return;
    }

    // otherwise, do the same check for the 2nd pair.
    if ( t[2] && t[3] && ( *(t[2]) != *(t[3]) ) )
    {
	setSeam( true );
    }
}

bool
HalfEdge::checkSym( void ) 
{
    // if mySym points to my origin, and I point to his origin, that's good.
    // (i.e. this will automatically fail if mySym==this
    if ( mySym->getPrev().getVertex() == getVertex() &&
	 myPrev->getVertex() == mySym->getVertex() )
    {
	checkSeam();
	return true;
    }

    // otherwise that's bad.
    return false;
}

Vector
HalfEdge::edgeVector() const
{
    if ( !myPrev )
    {
	return ZeroVector;
    }
    return myVertex.getPoint() - myPrev->myVertex.getPoint();
}

// Returns a reference to the adjacent edge that points to my vertex --------
HalfEdge&
HalfEdge::getNextEdgeSharingMyVertex() const
{
    // check to see if the prev edge of my sym edge points to the same
    // vertex as I do.. If not, the mesh is fucked.
    assert( mySym->getPrev().getVertex() == myVertex );

    // return the prev of my sym
    return mySym->getPrev();
}

void
HalfEdge::setSeam( bool yes )	
{
    if ( yes )
    {
	myFlags |= IS_SEAM;
	mySym->myFlags |= IS_SEAM;
    }
    else
    {
	myFlags &= ~IS_SEAM;
	mySym->myFlags &= ~IS_SEAM;
    }
}

void
HalfEdge::setPicked( bool yes )	
{
    if ( yes )
    {
	myFlags |= IS_PICKED;
	mySym->myFlags |= IS_PICKED;
    }
    else
    {
	myFlags &= ~IS_PICKED;
	mySym->myFlags &= ~IS_PICKED;
    }
}

// Iterator ------------------------------------------------------------------
// assignment
HalfEdge::Iterator&
HalfEdge::Iterator::operator=( const HalfEdge::Iterator &i )
{
    myHead = i.myHead;
    myCur = i.myCur;
    myLapped = i.myLapped;
    return *this;
}

// equivalence
bool
HalfEdge::Iterator::operator==( const HalfEdge::Iterator &i ) const
{ 
    return ( myHead==i.myHead && myCur==i.myCur && myLapped==i.myLapped );
}

// prefix++
void
HalfEdge::Iterator::operator++()
{
    if ( !myLapped )
    {
	myCur = myCur->myNext;
	if ( myCur == myHead )
	{
	    myLapped = true;
	}
    }
}


