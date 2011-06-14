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
// $Id: MeshDraw.h,v 1.1 2001/02/22 15:53:43 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __MESHDRAW_H__
#define __MESHDRAW_H__

#include "Common.h"
#include "utility.hpp"
#include "Mesh.h"

#include <GL/gl.h>

class MeshDraw : public boost::noncopyable
{
public:
		 MeshDraw( const Mesh &m )
		    : myMesh( m ), myWireDL( 0 ), mySolidDL( 0 ) {}

    virtual	~MeshDraw()
		    {
			if ( myWireDL ) glDeleteLists( myWireDL, 1 );
			if ( mySolidDL ) glDeleteLists( mySolidDL, 1 );
		    }

    // Draws this mesh as a wireframe
    void	 createWireFrame( void );
    void	 drawWireFrame()
		    {
			if ( !myWireDL ) createWireFrame();
			glCallList( myWireDL );
		    }

    void	 drawWireFrame( bool hidden, bool silhouette );

    // Draws this mesh as a solid
    void	 createSolid( void );
    void	 drawSolid( void )
		    {
			if ( mySolidDL ) createSolid();
			glCallList( mySolidDL );
		    }

private:
    const Mesh	&myMesh;
    GLuint	 myWireDL;
    GLuint	 mySolidDL;
};

#endif // __MESHDRAW_H__
