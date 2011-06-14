//--------------------------------------------------------------------------
// $Id: PMRTreeNode.h,v 1.1.1.1 2000/09/17 06:56:05 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __PMRTREE_NODE_H__
#define __PMRTREE_NODE_H__

#include "Node.h"
#include "Array.h"
#include "DualEdge.h"

#define theSplittingThreshold 5000

class PMRTreeNode : public Node
{
public:

    PMRTreeNode( Point &min, Point &max )
    { PMRTreeNode( min.x, min.y, min.z, max.x, max.y, max.z ); }

    PMRTreeNode( double nX, double ny, double nz,
                 double xX, double xy, double xz );
		
    ~PMRTreeNode();

    void	 add( DualEdge *de );
    void	 query( const Vector &N, const Point &pop,
	                const int &dir, bool *flag );
    Point	 getLocation() const		{ return myLoc; }

    void	 draw( int x, int level ) const;

private:

    void	 splitMe();
    void	 drawMe() const;
    
    bool	 intersect( DualEdge &de );

    bool	 isInside( Point p ) const
		 { return( p.x <= myMax.x &&
		           p.y <= myMax.y &&
		           p.z <= myMax.z &&
		           p.x >= myMin.x &&
		           p.y >= myMin.y &&
		           p.z >= myMin.z  );
		 }

    bool	 checkFace( const Vector &N, double D, DualEdge &de ) const;

    Array<DualEdge*>	 myEdges;
};

#endif //__PMRTREE_NODE_H__
