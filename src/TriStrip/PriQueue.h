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
// $Id: PriQueue.h,v 1.2 2000/09/17 22:54:59 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __PRI_QUEUE_H__
#define __PRI_QUEUE_H__

#include "PriQueueElem.h"


// perhaps the name is misleading.  But, to use this class as a priority
// queue you really need an array of N of these classes, one per possible
// key value that each element can have.  Then, when you wanted the min
// or max key element you can just ask the appropriate PriQueue list for
// its head.  The cool thing is that this, in cooperation with PriQueueElem,
// makes changing the key of an element (and the consequent heapify op) 
// very quick.  Removal of an element from one PriQueue object is constant
// time.  Then when you put the element with the new key into its
// new home, you just do an append (also constant time)
//
// yeah yeah, it's very specialized.. but it works.  sped up stripfying
// one particular model from 3 mins to < 1 sec. :)
class PriQueue
{
public:
    PriQueue() : myHead( 0 )			{}
    ~PriQueue()					{}

    // these all check to see if this is empty
    operator void*() const		{ return myHead ? (void*)this : 0; }
    bool operator!() const		{ return myHead ? 0 : 1; }
    bool		 isEmpty() const	{ return myHead == 0; }

    // append at the end.
    void		 append( PriQueueElem *p );

    // pop removes the the first element (pointed to by head)
    PriQueueElem	*pop();

    // remove just removes the given element from this.  If p turns out
    // to the pointed to by head, then pop will be called.
    void		 remove( PriQueueElem *p );

    // return the number of elements in this.
    unsigned int	 size();

private:
    PriQueueElem 	*myHead;
};

#endif // __PRI_QUEUE_H__
