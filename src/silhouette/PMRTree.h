//--------------------------------------------------------------------------
// $Id: PMRTree.h,v 1.1.1.1 2000/09/17 06:56:05 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __PMRTREE_H__
#define __PMRTREE_H__

#include "Array.h"
#include "PMRTreeNode.h"

class PMRTree
{
public:

    PMRTree( Point &min, Point &max, int num );
    ~PMRTree()				{ delete myRoot; }

//    bool	 add( DualEdge *de );
    bool	 add( DualEdge **de );
    void	 query( Vector &N, Point &pop ) const;

    bool	 isEmpty() const	{ return !myEdges.size(); }
    int		 size() const		{ return myEdges.size(); }

    void	 draw( int x ) const;

private:
    PMRTreeNode 	*myRoot;

    int			 myNum;
    Point		 myMin;
    Point		 myMax;
    Vector		 myDimension; 

    bool		*flags;

    Array<DualEdge*>	 myEdges;
};

#endif //__PMRTREE_H__
