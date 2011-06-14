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
// $Id: Vertex.C,v 1.4 2001/03/21 06:34:17 vchma Exp $
//--------------------------------------------------------------------------

#include "Vertex.h"
#include "Op.h"

#include <iostream>

std::ostream&
operator<<( std::ostream &os, Vertex &v )
{
    return os << "Vertex " << v.getMeshId() << " " << v.getPoint();
}

bool
Vertex::acceptOp( Op &op )
{
    return op( this );
}

