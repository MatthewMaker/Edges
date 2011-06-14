//--------------------------------------------------------------------------
// $Id: OctTreeNode.h,v 1.1.1.1 2000/09/17 06:56:05 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __OCTTREE_NODE_H__
#define __OCTTREE_NODE_H__

#include "Node.h"

class DualVertex;

#define ENOUGH 512	// multiples of 8
    
class OctTreeNode : public Node
{
public:
		
    OctTreeNode( double nx, double ny, double nz,
		 double xx, double xy, double xz,
		 DualVertex **dv, int num );

    ~OctTreeNode();

    void	 query( Vector &newN, Point &newP, int &newI,
	                Vector &oldN, Point &oldP, int &oldI );

    void	 query( Vector &newN );

    void	 draw( int x, int level ) const;

    int		 intersect( Vector &N, Point &pop );

private:
    void	 drawMe() const;
    
    int		 myNum;
    DualVertex** myDV;

};

#endif //__OCTTREE_NODE_H__
