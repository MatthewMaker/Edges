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
// $Id: Common.h,v 1.1 2001/03/21 04:23:38 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __COMMON_H__
#define __COMMON_H__

#include <cassert>
#include <cstdlib>
#include <cmath>

#include <tcl.h>

class Op;

class Common
{
public:

	   		 Common();
    virtual		~Common()		{}
    
    int			 getMeshId() const	{ return myMId; }
    void		 setMeshId( int id )	{ myMId = id; }

    virtual bool	 acceptOp( Op & ) = 0;

protected:

    int myMId;	// id given by the mesh to each object in the mesh
};

// Macro-o-matic!
#define IS_BETWEEN( v, a, b ) ( (v) >= (a) && (v) < (b) ) 
#define IS_NOT_BETWEEN( v, a, b ) ( (v) >= (b) || (v) < (a) ) 

#endif //__COMMON_H__

