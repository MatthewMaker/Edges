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
// $Id: TriStripDraw.h,v 1.1 2001/02/22 15:53:43 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __TRI_STRIP_DRAW_H__
#define __TRI_STRIP_DRAW_H__

#include "TriStrip.h"
#include "utility.hpp"

#include <GL/gl.h>

class TriStripDraw : public boost::noncopyable
{
public:

		 TriStripDraw( const TriStrip & );
		~TriStripDraw();

    // output the strips data to a file.
    void	 draw() const		{ glCallList( myDL ); }

private:
    GLuint		 myDL;
    const TriStrip	&myTS;
};

#endif // __TRI_STRIP_DRAW_H__
