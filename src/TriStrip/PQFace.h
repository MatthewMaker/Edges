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
// $Id: PQFace.h,v 1.3 2000/09/18 14:39:32 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __PQ_FACE_H__
#define __PQ_FACE_H__

#include "PriQueueElem.h"
#include "Face.h"
#include "utility.hpp"

class PQFace : public PriQueueElem, boost::noncopyable
{
public:
    PQFace( const Face &f );
    virtual ~PQFace()						{}

    virtual int	 key()				 const	{ return myKey; }
    virtual void changeKey( int by = 1 )		{ myKey += by; }

    const Face	&getFace()			 const 	{ return myFace; }

private:
    const Face	&myFace;
    int		 myKey;
};

#endif // __PQ_FACE_H__
