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
// $Id: IVOutputOp.h,v 1.1 2000/10/24 03:31:42 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __IVOoutputOp_H__
#define __IVOoutputOp_H__

#include "Op.h"
#include <fstream>

class IVOutputOp : public Op
{
public:
			 IVOutputOp( const char *filename );
    virtual		~IVOutputOp();

    virtual bool	 operator()( Mesh *m );
    virtual bool	 operator()( Face *f );
    virtual bool	 operator()( Point *v );
    virtual bool	 operator()( Vertex *v );
    virtual bool	 operator()( Vector *v );
    virtual bool	 operator()( Vector2 *v );
    virtual bool	 operator()( HalfEdge *e );

private:
			 IVOutputOp();

    std::ofstream		 myOut;
};

#endif //__IVOoutputOp_H__
