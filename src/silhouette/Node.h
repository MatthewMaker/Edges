//--------------------------------------------------------------------------
// $Id: Node.h,v 1.2 2000/10/24 03:35:03 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __NODE_H__
#define __NODE_H__

#include "Common.h"
#include "Point.h"
#include "Vector.h"

class Node // : public Common
{
public:
    const Point	&getMin() { return myMin; }
    const Point	&getMax() { return myMax; }
    const Point	&getLoc() { return myLoc; }

protected:

    static char	 theVerts[8][3];
    static char	 theOrder[8][8];
    bool	 myIsLeaf;
    Point	 myMin;
    Point	 myMax;
    Point	 myLoc;

    // I'll use the index of children pointer as a 3-bit bit-field:
    //   bit 0 = x, bit 1 = y, bit 2 = z.
    //   the bit is ON if this block is on the > side of the split point.
    Node	*myChild[8];
};

#endif //__NODE_H__
