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
// $Id: Face.C,v 1.5 2001/03/21 15:39:22 vchma Exp $
//--------------------------------------------------------------------------

#include "Point.h"
#include "Vertex.h"
#include "Vector.h"
#include "DualVertex.h"
#include "HalfEdge.h"
#include "Face.h"
#include "Op.h"

#define EPSILON 1.0e-30

Face::Face( int numEdges, HalfEdge *e )
: myEdge(e),
  myNumEdges(numEdges),
  myNormal(0),
  myDual(0)
{
}

Face::Face()
: myEdge(0),
  myNumEdges(0),
  myNormal(0),
  myDual(0)
{
}

Face::~Face()
{
    delete myNormal;
    delete myDual;
}

bool
Face::acceptOp( Op &op )
{
    return op( this );
}

// calculate the dual ---------------------------------------------------------
void
Face::calcDual()
{
    double D;

    // calculate the normal of this face if necessary
    getNormal();

    D = -( *myNormal * myEdge->getVertex().getPoint() );  
    if ( D >= 0 && D < EPSILON ) D = EPSILON;
    else if ( D < 0 && D > -EPSILON ) D = -EPSILON;

    myDual = new DualVertex( myNormal->x, myNormal->y, myNormal->z, D );
}

DualVertex*
Face::getDual() 
{
    if ( !myDual ) calcDual();
    return myDual;
}

// Calculate the normal -------------------------------------------------------
Vector &
Face::calcNormal()
{
    if ( !myNormal ) myNormal = new Vector;

    if ( !myEdge )
	myNormal->makeZero();
    else if ( &(myEdge->getPrev()) != myEdge )
	*myNormal = myEdge->getPrev().edgeVector() ^ myEdge->edgeVector();
    else if ( ( &(myEdge->getNext()) != myEdge ) )
	*myNormal = myEdge->edgeVector() ^ myEdge->getNext().edgeVector();
    else
	myNormal->makeZero();

//    myNormal.normalize();

    return *myNormal;
}

Vector &
Face::getNormal()
{
    return ( myNormal ?
	     ( myNormal->isZero() ? calcNormal() : *myNormal ) :
	     calcNormal() );
}

// other operators ------------------------------------------------------------
std::ostream	&
operator<<( std::ostream &os, Face &f ) 
{
    HalfEdge *e = &(f.myEdge->getNext());
    os << "    Face " << f.myMId << ":" << std::endl;
    os << "      " << f.getEdge().getVertex();

    while ( e != f.myEdge )
    {
	os << "      " << e->getVertex();
	e = &(e->getNext());
    }

    return os;
}

// Face_EdgeIterator ==========================================================
HalfEdge*
Face::EdgeIterator::getNext()
{
    HalfEdge *e = cur;
    cur = &(cur->getNext());

    return e;
}

