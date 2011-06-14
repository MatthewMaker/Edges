//--------------------------------------------------------------------------
// $Id: OctTree.h,v 1.1.1.1 2000/09/17 06:56:05 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __OCTTREE_H__
#define __OCTTREE_H__

#include "Common.h"
#include "Point.h"
#include "Vector.h"

class OctTreeNode;

class OctTree
{
public:

    static const int	 theCapacity;

    OctTree( Point &min, Point &max );
    ~OctTree();

    void	 query( Vector &newN, Point &newP,
	                Vector &oldN, Point &oldP ) const;

    void	 draw( int x ) const;

private:
    OctTreeNode 	*myRoot;

    int			 myNum;
    Point		 myMin;
    Point		 myMax;
};

#endif //__OCTTREE_H__
