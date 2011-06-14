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
// $Id: PriQueue.C,v 1.2 2000/09/17 22:54:59 vchma Exp $
//--------------------------------------------------------------------------

#include "PriQueue.h"

void
PriQueue::append( PriQueueElem *p )
{
    // normal case of appending at the back
    if ( myHead )  myHead->prev()->insertNext( p );

    // special case: this list is empty
    else           myHead = p;
}

PriQueueElem*
PriQueue::pop()
{
    PriQueueElem *rc = 0;
    if ( myHead )
    {
	rc = myHead;
	if ( !rc->isAlone() )
	{
	    myHead = rc->next();
	    rc->removeMyself();
	}
	else
	{
	    myHead = 0;
	}
    }

    return rc;
}

void
PriQueue::remove( PriQueueElem *p )
{
    // if p is the head of this list just do a pop()
    if ( myHead && myHead == p )	pop();

    // o/w extract this element from its neighbours
    else if ( !p->isAlone() )	p->removeMyself();
}

unsigned int
PriQueue::size()
{
    unsigned int size = 0;

    if ( myHead )
    {
	if ( myHead->isAlone() ) size = 1;
	else
	{
	    PriQueueElem *p = myHead->next();
	    size = 1;
	    while ( p != myHead )
	    {
		++size;
		p = p->next();
	    }
	}
    }

    return size;
}
