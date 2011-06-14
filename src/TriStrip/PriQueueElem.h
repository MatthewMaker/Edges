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
// $Id: PriQueueElem.h,v 1.2 2000/09/17 22:54:59 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __PRI_QUEUE_ELEM_H__
#define __PRI_QUEUE_ELEM_H__

class PriQueueElem
{
public:
    PriQueueElem() : myLoc(-1), myPrev(this), myNext(this)	{}
    virtual ~PriQueueElem()					{}

    virtual int	 key()				 const = 0;
    virtual void changeKey( int by = 1 )	       = 0;

    int		 loc()				 const	{ return myLoc; }
    void	 setLoc( const int l )			{ myLoc = l; }

    bool	 isAlone()	const	{ return myNext==this&&myPrev==this; }

    PriQueueElem        *next() const	{ return myNext; }
    PriQueueElem        *prev() const	{ return myPrev; }

    // insert p after me.
    void         insertNext( PriQueueElem *p )
    {
	p->myPrev =  this;
	myNext->myPrev = p;
	p->myNext = myNext;
	myNext = p;
    }

    void         removeMyself()
    {
	myPrev->myNext = myNext;
	myNext->myPrev = myPrev;
	myNext = this;
	myPrev = this;
    }

protected:

    int			 myLoc;
    PriQueueElem        *myPrev;
    PriQueueElem        *myNext;

};

#endif // __PRI_QUEUE_ELEM_H__
