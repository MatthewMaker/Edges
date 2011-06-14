//--------------------------------------------------------------------------
// $Id: DualVertex.h,v 1.4 2001/03/21 15:39:22 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __DUAL_VERTEX_H__
#define __DUAL_VERTEX_H__

#include "Common.h"
#include "Array.h"
#include "Point.h"

#include <iosfwd>

class DualEdge;

class DualVertex : public Common
{
public:

    // gives DV ownership of Point p 
    DualVertex( double x, double y, double z, double D );
    virtual ~DualVertex()			{ delete myPoint; }

    // Accept a visiting Op, ask it to operate on myself
    virtual bool	 acceptOp( Op &op )	{ return false; }

    void		 addDualEdge( DualEdge *e ) { myEdges.append( e ); }
    int	 		 getNumDE( void ) const	{ return myEdges.size(); }
    DualEdge		&getDE( int i )		{ return *(myEdges[i]); }

    Point		&getPoint() const	{ return *myPoint; }
    double		 getD() const		{ return myHomo[3]; }

    friend std::ostream	&operator<<( std::ostream &os, DualVertex &v );

private:
    DualVertex();
    DualVertex( DualVertex & );
    void		 operator=( DualVertex & );

    Point		*myPoint;
    double		 myHomo[4];
    Array<DualEdge *>	 myEdges;
};

typedef enum
{
    DIM_X,
    DIM_Y,
    DIM_Z,
} DVDim;

class DVList
{
public:
    static DVList	&getDVM();
    ~DVList();

    int			 addDV( DualVertex *dv )
			 { return myDV.append( dv ); }
    int			 numDV()			{ return myDV.size(); }
    void		 reset()			{ myDV.init( 100 ); }

    void		 sort( DVDim dim, int start, int end );

    DualVertex**	 getArray()			{ return myDV.dupeGut();} 

    DualVertex		*operator[]( const int index )	{ return myDV[index]; }
    friend std::ostream	&operator<<( std::ostream &os, DVList &v );

private:
    DVList()						{ myDV.init( 100 ); }
    DVList( DVList &dvm )				{}
    void		 operator=( DVList &dvm )	{}

    Array<DualVertex *>	 myDV;
};

#endif //__DUAL_VERTEX_H__
