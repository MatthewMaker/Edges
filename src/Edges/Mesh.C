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
// $Id: Mesh.C,v 1.9 2001/03/21 15:39:22 vchma Exp $
//--------------------------------------------------------------------------

#include "Mesh.h"
#include "Vertex.h"
#include "HalfEdge.h"
#include "Face.h"
#include "Vector2.h"
#include "DualEdge.h"
#include "DualVertex.h"

#include "InputFilter.h"
#include "InputFilterRegister.h"

#include "Op.h"

#include <vector>
#include <cstring>
#include <cstdio>

#define NUM_EDGES 100
#define NUM_VERTICES 100
#define NUM_FACES 100

// Constructor and Destructor -------------------------------------------------
Mesh::Mesh()
: myMax( 0, 0, 0 ),
  myMin( 0, 0, 0 ),
  myCenter( 0, 0, 0 ),
  myDMax( 0, 0, 0 ),
  myDMin( 0, 0, 0 )
{
    myVertices.init( NUM_VERTICES );
    myFaces.init( NUM_FACES );
    myEdges.init( NUM_EDGES );
    myOneOutOfTwo.init( NUM_EDGES );
    myDEdges.init( NUM_EDGES );
    myNormals.init( NUM_EDGES );
    myUVs.init( NUM_EDGES );

    Tcl_InitHashTable( &myEdgeEndPointLookUp, 2 );
}

Mesh::~Mesh()
{
    Tcl_DeleteHashTable( &myEdgeEndPointLookUp );
    cleanup();
}

void
Mesh::cleanup()
{
    int i;
    for ( i = myVertices.size() ; i --> 0 ; ) { delete myVertices[i]; }
    for ( i = myFaces.size() ; i --> 0 ; ) { delete myFaces[i]; }
    for ( i = myEdges.size() ; i --> 0 ; ) { delete myEdges[i]; }
    for ( i = myDEdges.size() ; i --> 0 ; ) { delete myDEdges[i]; }
    for ( i = myNormals.size() ; i --> 0 ; ) { delete myNormals[i]; }
    for ( i = myUVs.size() ; i --> 0 ; ) { delete myUVs[i]; }

    myVertices.init( NUM_VERTICES );
    myFaces.init( NUM_FACES );
    myEdges.init( NUM_EDGES );
    myDEdges.init( NUM_EDGES );
    myOneOutOfTwo.init( NUM_EDGES );
    myNormals.init( NUM_EDGES );
    myUVs.init( NUM_EDGES );

    DVList::getDVM().reset();
}

// Accept a visiting Op -------------------------------------------------------
bool
Mesh::acceptOp( Op &op )
{
    return op( this );
}

// Add a new vertex -----------------------------------------------------------
int
Mesh::addVertex( double x, double y, double z )
{
    Point *p = new Point( x, y, z );

    p->setMeshId( myPoints.append( p ) );

    // this will set up the bounding box incrementally
    if ( myPoints.size() == 1 )
    {
	myMax = *p;
	myMin = *p;
    }
    else 
    {
	if( p->x > myMax.x ) myMax.x = p->x;
	if( p->y > myMax.y ) myMax.y = p->y;
	if( p->z > myMax.z ) myMax.z = p->z;

	if( p->x < myMin.x ) myMin.x = p->x;
	if( p->y < myMin.y ) myMin.y = p->y;
	if( p->z < myMin.z ) myMin.z = p->z;
    }

    return myPoints.size() - 1;
}

// Add a new normal -----------------------------------------------------------
int
Mesh::addNormal( double x, double y, double z )
{
    Vector *v = new Vector( x, y, z );
    v->setMeshId( myNormals.append( v ) );
    return myNormals.size() - 1;
}

// Add a new UV ---------------------------------------------------------------
int
Mesh::addUV( double x, double y )
{
    Vector2 *v = new Vector2( x, y );
    v->setMeshId( myUVs.append( v ) );
    return myUVs.size() - 1;
}

// Attemp to match edges ------------------------------------------------------
// We use a Hash table to store edges, index by their (origin,destination)
// vertex pair.  Therefore, when given an edge, we ask for an edge
// in the hash table that is of the same endpoints, but backwards.  If
// found, that's our sym.  If not, add the current edge into the hash table.
void
Mesh::attemptMatchEdge( HalfEdge *e, int origin, int dest )
{
    HalfEdge		*aSym;
    Tcl_HashEntry	*entry;
    int			 eq[2] = { origin, dest };
    int			 qe[2] = { dest, origin };
    int			 ok;

    if ( ( entry = Tcl_FindHashEntry( &myEdgeEndPointLookUp, (char*)qe ) ) )
    {
	aSym = (HalfEdge *) Tcl_GetHashValue( entry );
	aSym->forceSym( e );
	Tcl_DeleteHashEntry( entry );
    }
    else
    {
	entry = Tcl_CreateHashEntry( &myEdgeEndPointLookUp, (char*)eq, &ok );
	Tcl_SetHashValue( entry, e );

	// only add to "myOneOutOfTwo" if it's a new edge with no
	// sym yet.  This edge's sym will not be added here.
	// this ensures that we will only get "One out of two" in
	// each pair, in addition to those edges that have no sym.
	myOneOutOfTwo.append( e );
    }
}

// Add a new face, given a list of vertex indices -----------------------------
// returns 0 if fail, otherwise non-zero for success
int
Mesh::addFace( int numVertices,
	       const unsigned int *vIdx,
	       const unsigned int *nIdx,
	       const unsigned int *tIdx )
{
    int i;
    HalfEdge *e, *last;
    Face *f;

    bool hasNormals	= ( nIdx != 0 );
    bool hasUVs		= ( tIdx != 0 );

    f = new Face( numVertices, 0 );
    f->setMeshId( myFaces.append( f ) );

    for ( i = 0 ; i < numVertices ; i++ )
    {
	Vector *n;
	Vector2 *t;

	// decide if the given Point index is correct.
	if ( vIdx[i] >= myPoints.size() )
	{
	    std::cerr << "Mesh::addFace: bad vertex " << vIdx[i] << std::endl;
	    return 0;
	}

	// if client specified normal indices, and it is valid,
	// then use it to retrieve the normal, otherwise, NULL
	if ( hasNormals )
	{
	    if ( nIdx[i] >= myNormals.size() )
	    {
		std::cerr << "Mesh::addFace: bad normal " << nIdx[i] << std::endl;
		return 0;
	    }
	    else
	    {
		n = myNormals[nIdx[i]];
	    }
	}
	else
	{
	    n = (Vector *)0;
	}

	// if client specified UV indices, and it is valid,
	// then use it to retrieve the UV, otherwise, NULL
	if ( hasUVs )
	{
	    if ( tIdx[i] >= myUVs.size() )
	    {
		std::cerr << "Mesh::addFace: bad UV " << tIdx[i] << std::endl;
		return 0;
	    }
	    else
	    {
		t = myUVs[tIdx[i]];
	    }
	}
	else
	{
	    t = (Vector2 *)0;
	}

	// create a new Vertex object for this vertex of this face
	// (so, yes, a physical vertex of the mesh is represented by N
	// Vertex objects, where N = number of incident faces )
	Vertex *v = new Vertex( *(myPoints[vIdx[i]]), n, t );

	// record the vertex into the mesh.
	v->setMeshId( myVertices.append( v ) );

	// create a new edge
	e = new HalfEdge( *v, *f );
	e->setMeshId( myEdges.append( e ) ); 

	if ( i == 0 )
	{
	    f->setEdge( e );
	    attemptMatchEdge( e, vIdx[numVertices-1], vIdx[0] );
	}
	else
	{
	    last->insertNext( e );
	    attemptMatchEdge( e, vIdx[i-1], vIdx[i] );
	}

	last = e;
    }

    // figure out the bounding box of the dual vertices
    Point &p = f->getDual()->getPoint();

    if( p.x > myDMax.x ) myDMax.x = p.x;
    if( p.y > myDMax.y ) myDMax.y = p.y;
    if( p.z > myDMax.z ) myDMax.z = p.z;

    if( p.x < myDMin.x ) myDMin.x = p.x;
    if( p.y < myDMin.y ) myDMin.y = p.y;
    if( p.z < myDMin.z ) myDMin.z = p.z;

    return 1;
}

void
Mesh::calcDualEdges()
{
    int i;
    HalfEdge *e;

    for ( i = myOneOutOfTwo.size() ; i --> 0 ; )
    {
	e = myOneOutOfTwo[i];
	if ( !e->isSingle() )
	{
	    DualVertex &p1 = *(e->getFace().getDual());
	    DualVertex &p2 = *(e->getSym().getFace().getDual());

	    DualEdge *de = new DualEdge( p1, p2, *e );

	    #if 0
	    // error?
	    if ( ((p1.getD()>0)!=(p2.getD()>0)) )
	    cerr << "urgh!" << endl;
	    #endif
	    de->setGoesThruInf( ((p1.getD()>0)!=(p2.getD()>0)) );
	    de->setMeshId( myDEdges.append( de ) );
	}
    }
}


// See if mesh is valid -------------------------------------------------------
bool
Mesh::isValid() const
{
    // If there are an odd number of edges that's bad
//    if ( myEdges.size() % 2 ) return false;

    bool ok = true;
    HalfEdge *cur;

    // if there are any edge that is single that's bad
    for ( int i = myOneOutOfTwo.size() ; i --> 0 ; )
    {
	cur = myOneOutOfTwo[i];
	if ( !cur->checkSym() )
	{
	    cur->setPicked( true );
	    ok = false;
	}
    }

    // it's ok then
    return ok;
}

#if 0
// Attempt to calculate all face normals --------------------------------------
bool
Mesh::calcFaceNormals()
{
    for ( int i = myFaces.size() ; i --> 0 ; )
	if ( myFaces[i]->getNormal().isZero() ) return false;

    return true;
}
#endif

// Read in a file -------------------------------------------------------------
bool
Mesh::readFile( char *filename )
{
    if ( myEdges.size() || myFaces.size() || myVertices.size() )
    {
	std::cerr << "Mesh::readfile: Mesh already defined" << std::endl;
	return false;
    }
    if ( !filename )
    {
	std::cerr << "Mesh::readfile: no filename!" << std::endl;
	return false;
    }

    InputFilter &ifr =
	InputFilterRegister::getInstance().getInputFilter( filename );
    if ( !ifr )
    {
	std::cerr << "Mesh::readFile: [FATAL] no input filter for ";
	std::cerr << filename << std::endl;
	return false;
    }

    cleanup();

    bool rc = ifr.readFile( filename, *this );
    if ( !rc )
    { 
	cleanup();
	return false;
    }

    if ( !isValid() )
    {
	std::cerr << "Mesh::readfile: mesh has unmatched half-edges!" << std::endl;
    }

    // calculate the center of the model
    myCenter.x = (myMax.x + myMin.x) / 2.;
    myCenter.y = (myMax.y + myMin.y) / 2.;
    myCenter.z = (myMax.z + myMin.z) / 2.;

    calcDualEdges();

    myDCenter.x = (myDMax.x + myDMin.x) / 2.;
    myDCenter.y = (myDMax.y + myDMin.y) / 2.;
    myDCenter.z = (myDMax.z + myDMin.z) / 2.;

    debugOutput( std::cout );

    return true;
}

void
Mesh::getLookAt( double *lookAt, double &maxDim )
{
    double x = ( myMax.x - myCenter.x );
    double y = ( myMax.y - myCenter.y );
    double z = ( myMax.z - myCenter.z );

    double d = ( x > y ? ( x > z ? x : z ) : ( y > z ? y : z ) );

    // the viewpoint location (eye)
    lookAt[0] = myCenter.x;
    lookAt[1] = myCenter.y;
//    lookAt[0] = myMax.x + 5 * d;
//    lookAt[1] = myMax.y + 5 * d;
    lookAt[2] = myMax.z + 5 * d;

    // the point you look at (target)
    lookAt[3] = myCenter.x;
    lookAt[4] = myCenter.y;
    lookAt[5] = myCenter.z;

    // the up vector 
    lookAt[6] = 0;
    lookAt[7] = 1;
    lookAt[8] = 0;

    // do maxDim
    x = ( myMax.x - myMin.x );
    y = ( myMax.y - myMin.y );
    z = ( myMax.z - myMin.z );

    maxDim = ( x > y ? ( x > z ? x : z ) : ( y > z ? y : z ) );
}

double 
Mesh::getMaxDimension() const
{
    double x = ( myMax.x - myMin.x );
    double y = ( myMax.y - myMin.y );
    double z = ( myMax.z - myMin.z );

    return ( x > y ? ( x > z ? x : z ) : ( y > z ? y : z ) );
}

// Output to a stream ---------------------------------------------------------
void
Mesh::debugOutput( std::ostream &os )
{
//    int i;

    os << "Mesh has:" << std::endl;
    os << "  " << myPoints.size() << " points" << std::endl;
    os << "  " << myVertices.size() << " vertices" << std::endl;
//     for ( i = 0 ; i < myVertices.size() ; i++ )
//     {
// 	os << "    Vertex " << i;
// 	os << " " << *(myVertices[i]);
//     }
    os << "  " << myEdges.size() << " edges" << std::endl;
    os << "  " << myFaces.size() << " faces" << std::endl;
    os << "  " << myNormals.size() << " normals" << std::endl;
    os << "  " << myUVs.size() << " UVs" << std::endl;
//     for ( i = 0 ; i < myFaces.size() ; i++ ) 
// 	os << *(myFaces[i]);

    os << "  " << myDEdges.size() << " dual edges" << std::endl << std::endl;

    os << "Bounding Box: \n" << myMin << myMax;
    os << "Centroid: " << myCenter << std::endl;

    os << "Bounding Box: \n" << myDMin << myDMax;
    os << "Centroid: " << myDCenter << std::endl;
}

std::ostream&
operator<<( std::ostream &os, Mesh &m )
{
    m.debugOutput( os );
    return os;
}

