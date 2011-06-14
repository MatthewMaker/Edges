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
// $Id: TriStrip.h,v 1.4 2001/02/08 02:19:56 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __TRI_STRIP_H__
#define __TRI_STRIP_H__

#include "Mesh.h"

#include <vector>

class TriStrip 
{
public:

    // type definition
    typedef std::vector<Vertex *> Strip;
    typedef std::vector<Strip *> StripsArray;

			 TriStrip( Mesh &m );
			~TriStrip();

    // retrives the number of strips in this model
    unsigned int	 numStrips() const	{ return myNumStrips; }

    // retrives the array of strips.
    const StripsArray	&strip() const		{ return myStrips; }

    // output the strips data to a file.
    bool		 output( const char *filename );

private:
    Mesh		&myMesh;

    unsigned int	 myNumSingleton;
    unsigned int	 myNumPoints;
    unsigned int	 myNumStrips;

    StripsArray		 myStrips;
};

#endif // __TRI_STRIP_H__
