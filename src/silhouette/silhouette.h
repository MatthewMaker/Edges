//--------------------------------------------------------------------------
// $Id: silhouette.h,v 1.1.1.1 2000/09/17 06:56:05 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __SILHOUETTE_H__
#define __SILHOUETTE_H__

#include "Point.h"
#include "Vector.h"

class Mesh;
class PMRTree;
class OctTree;

class Silhouette
{
public:
    Silhouette();
    ~Silhouette();

    // This gives Silhouette the mesh to operate on.  Silhouette
    // will access the data in mesh to build up structures to
    // find silhouette edges.  It will also modify mesh: it will
    // "pick" edges in the mesh.
    //
    // NOTE: this does not give Silhouette ownership of mesh. 
    //       Silhouette won't delete the mesh when it's destroyed.
    void	 useThisMesh( Mesh *m );

    // Given a viewpoint, Silhouette will markup the mesh those
    // edges that are silhouettes.
    //
    // opt1 is to use PMRTree to optimize initial SE generation
    // opt2 is to use conherency optimization
    bool	 markSilhouetteEdges( Point &vp, bool opt1, bool opt2 );

    void	 drawTree( int x ) const;

    void	 getTiming( char *buf ) const;

private:
    Silhouette( Silhouette & );
    void	 operator=( Silhouette & );

    Mesh	*myMesh;
    Vector	 myLastPoint;
    Point	 myLastPop;
    bool	 myHasLastPoint;

    PMRTree	*myTree;
    OctTree	*myOtherTree;

    char	 myTiming[80];
};

#endif //__SILHOUETTE_H__
