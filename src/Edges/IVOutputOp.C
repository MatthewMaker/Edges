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
// $Id: IVOutputOp.C,v 1.1 2000/10/24 03:31:42 vchma Exp $
//--------------------------------------------------------------------------

#include "IVOutputOp.h"
#include "HalfEdge.h"
#include "Face.h"
#include "Mesh.h"
#include "Point.h"
#include "Vertex.h"
#include "Vector.h"
#include "Vector2.h"

IVOutputOp::IVOutputOp( const char *filename )
:myOut( filename ? filename : "temp.iv" )
{
}

IVOutputOp::~IVOutputOp()
{
    myOut.flush();
    myOut.close();
}

bool
IVOutputOp::operator()( Mesh *m )
{
    unsigned int i, j;

    myOut << "#Inventor V2.0 ascii" << std::endl << std::endl;
    myOut << "Material {" << std::endl;
    myOut << "\tambientColor\t0.09 0.04 0.04" << std::endl;
    myOut << "\tdiffuseColor\t0.9 0.4 0.4" << std::endl;
    myOut << "\tspecularColor\t0.3 0.3 0.3" << std::endl;
    myOut << "\tshininess\t0.2" << std::endl;
    myOut << "\ttransparency\t0" << std::endl;
    myOut << "}" << std::endl << std::endl;

    myOut << "Separator {" << std::endl;
    myOut << "\tCoordinate3 {" << std::endl;
    myOut << "\t\tpoint   [" << std::endl;

    // output Vertices - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    j = m->numPoints();
    for ( i = 0 ; i < j ; i++ )
    {
	myOut << "\t\t\t";
	if ( !m->getPoint(i).acceptOp( *this ) )
	    return false;
    }

    myOut << "\t\t]" << std::endl;
    myOut << "\t}" << std::endl;

    // output Normals - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#if 0
    myOut << "\tNormal {" << endl;
    myOut << "\t\tvector   [" << endl;

    for ( i = 0 ; i < myVertices.size() ; i++ )
    {
	myOut << "\t\t\t";
	myVertices[i]->printNormalToFile( myOut );
    }

    myOut << "\t\t]" << endl;
    myOut << "\t}" << endl;
#endif

    myOut << "\tMaterialBinding { value PER_VERTEX_INDEXED }" << std::endl;
    myOut << "\tMaterial {" << std::endl;
    myOut << "\t\tdiffuseColor [" << std::endl;

    for ( i = m->numFaces() ; i --> 0 ; )
    {
	for ( j = m->getFace( i ).numEdges() ; j --> 0 ; )
	{
	    myOut << "\t\t\t0.9 0.4 0.4," << std::endl;
	}
	myOut << std::endl;
    }
	
    myOut << "\t\t]" << std::endl;
    myOut << "\t}" << std::endl;

    myOut << "\tIndexedFaceSet {" << std::endl;
    myOut << "\t\tmaterialIndex\t-1" << std::endl;
    myOut << "\t\tnormalIndex\t-1" << std::endl;
    myOut << "\t\ttextureCoordIndex\t-1" << std::endl;
    myOut << "\t\tcoordIndex\t[" << std::endl;

    for ( i = m->numFaces() ; i --> 0 ; )
    {
	myOut << "\t\t\t";
	if ( !m->getFace(i).acceptOp( *this ) )
	    return false;
    }

    myOut << "\t\t]" << std::endl;
    myOut << "\t}" << std::endl;
    myOut << "}" << std::endl;

    return true;
}

bool
IVOutputOp::operator()( HalfEdge *e )
{
    return false;
}

bool
IVOutputOp::operator()( Vector2 *v )
{
    myOut << v->x << " " << v->y << "," << std::endl; 
    return true;
}

bool
IVOutputOp::operator()( Vector *v )
{
    myOut << v->x << " " << v->y << " " << v->z << "," << std::endl;
    return true;
}

bool
IVOutputOp::operator()( Vertex *v )
{
    return false;
}

bool
IVOutputOp::operator()( Point *p )
{
    myOut << p->x << " " << p->y << " " << p->z << "," << std::endl;
    return true;
}

bool
IVOutputOp::operator()( Face *f )
{
    for ( HalfEdge::Iterator i( &(f->getEdge()) ) ; i ; ++i )
    {
	myOut << i->getVertex().getPoint().getMeshId() << ", ";
    }

    myOut << "-1, " << std::endl;

    return true;
}

