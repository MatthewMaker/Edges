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
// $Id: Mesh.h,v 1.7 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __MESH_H__
#define __MESH_H__

class Vertex;
class HalfEdge;
class DualEdge;
class Face;
class Vector2;
class Op;

#include "Common.h"
#include "utility.hpp"
#include "Array.h"	// for the arrays in the Mesh
#include "Vector.h"	// for using ZeroVector
#include "Point.h"

class MeshDraw;

class Mesh : public Common, boost::noncopyable
{
public:
    Mesh();
    virtual ~Mesh();

    friend class MeshDraw;

    // Add a new vertex/normal/UV, returns -1 if fail, otherwise returns index
    int		 addVertex( double x, double y, double z );
    int		 addNormal( double x, double y, double z );
    int		 addUV( double x, double y );

    // Add a new face, given a list of vertex indices, plus
    // optional normal and UVs
    // the arrays must be of the same length, i.e. numVertices.
    // o/w, undefined behaviour results.
    //
    // returns 0 if fail, otherwise non-zero for success
    int		 addFace( int numVertices,
	                  const unsigned int *vertexIndices,
			  const unsigned int *normalIndices = 0,
			  const unsigned int *UVIndices = 0 );

    // Return the number of registered Vertices, Faces and Edges
    int		 numPoints()		const	{ return myPoints.size(); }
    int		 numNormals()		const	{ return myNormals.size(); }
    int		 numUVs()		const	{ return myUVs.size(); }
    int		 numVertices()		const	{ return myVertices.size(); }
    int		 numFaces()		const	{ return myFaces.size(); }
    int		 numEdges()		const	{ return myEdges.size(); }
    int		 numDEdges()		const	{ return myDEdges.size(); }

    const Face		&getFace( int i )	const	{ return *(myFaces[i]); }
    const HalfEdge	&getEdge( int i )	const	{ return *(myEdges[i]); }
    const Vertex	&getVertex( int i )	const	{ return *(myVertices[i]); }
    const Point		&getPoint( int i )	const	{ return *(myPoints[i]); }
    const Vector	&getNormal( int i )	const	{ return *(myNormals[i]); }
    const Vector2	&getUV( int i )		const	{ return *(myUVs[i]); }

    Face	&getFace( int i )		{ return *(myFaces[i]); }
    HalfEdge	&getEdge( int i )		{ return *(myEdges[i]); }
    Vertex	&getVertex( int i )		{ return *(myVertices[i]); }
    Point	&getPoint( int i )		{ return *(myPoints[i]); }
    Vector	&getNormal( int i )		{ return *(myNormals[i]); }
    Vector2	&getUV( int i )			{ return *(myUVs[i]); }

    // See if mesh is valid i.e. all edges are paired, etc
    bool	 isValid() const;

    // Read in a file
    bool	 readFile( char *filename );

    // Debug output
    void	 debugOutput( std::ostream &os );

    // Accept a visiting Op, ask it to operate on myself
    virtual bool	 acceptOp( Op &op );

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ?!
    void	 getLookAt( double *lookAt, double &maxDim );
    Point	&getCentroid() 	{ return myCenter; }
    Point	&getDMin()	{ return myDMin; }
    Point	&getDMax()	{ return myDMax; }

    double	 getMaxDimension() const;
    double	 getBoundingSphereRadius() const
		    { return (myMax-myCenter).normalize(); }

    // TEMP
    DualEdge	**getDualEdges() { return myDEdges.dupeGut(); }
    const Array<Face *> &getFaceArray() const { return myFaces; }

    void	 calcDualEdges();
    
private:
    // Release all resources that are taken up by vertices/edges/faces
    void	 cleanup();

    // the EdgeEndPointLookUp is used to store the edges by their endpoint
    // indices.  Then, as edges are added, one can look for it's sym by
    // querying with the endpoints.
    void	 attemptMatchEdge( HalfEdge *e, int origin, int dest );

    Array<Point *>	myPoints;
    Array<Vertex *>	myVertices;
    Array<Face *>	myFaces;
    Array<HalfEdge *>	myEdges;
    Array<HalfEdge *>	myOneOutOfTwo;
    Array<DualEdge *>	myDEdges;
    Array<Vector *>	myNormals;
    Array<Vector2 *>	myUVs;

    Tcl_HashTable	myEdgeEndPointLookUp;

    Point		myMax;
    Point		myMin;
    Point		myCenter;

    Point		myDMax;
    Point		myDMin;
    Point		myDCenter;
};

#endif // __MESH_H__
