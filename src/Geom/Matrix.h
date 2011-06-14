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
// $Id: Matrix.h,v 1.1 2001/03/21 04:45:53 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iosfwd>
#include <cstring>

#include "Vector.h"
#include "Point.h"

class Matrix
{
public:
    explicit	 Matrix( void )
		    { makeIdentity(); }
			
		 Matrix( const Matrix &m )
		    { memcpy( mat, m.mat, sizeof(double)*16 ); }

    void	 makeIdentity();
    bool	 isIdentity() const;

    // Assignment
    Matrix	&operator=( const Matrix &m )
		    {
			memcpy( mat, m.mat, sizeof(double)*16 );
			return *this;
		    }

    // Equivalence
    bool	 operator==( const Matrix &m ) const
		    { return !memcmp( mat, m.mat, sizeof(double)*16 ); }

    bool	 operator!=( const Matrix &m ) const
		    { return memcmp( mat, m.mat, sizeof(double)*16 ); }

    // M * M -> Matrix multiplication
    const
    Matrix	 operator*( const Matrix &m ); 
    const
    Matrix	&operator*=( const Matrix &m )
		    { return (*this) = (*this) * m; }

    // M * s -> Matrix/Scalar multiplication
    const
    Matrix	&operator*=( const double s )
		    {
			for ( int i = 4 ; i --> 0 ; )
			    for ( int j = 4 ; j --> 0 ; )
				mat[j][i] *= s;

			return *this;
		    }
    friend const
    Matrix	 operator*( const Matrix &m, const double s )
		    { return Matrix( m ) *= s; }


    // M * P -> Point transformation
    const Point		 operator*( Point &p ) const
			{ return Point(
			    p.x*mat[0][0]+p.y*mat[0][1]+p.z*mat[0][2]+mat[0][3],
			    p.x*mat[1][0]+p.y*mat[1][1]+p.z*mat[1][2]+mat[1][3],
			    p.x*mat[2][0]+p.y*mat[2][1]+p.z*mat[2][2]+mat[2][3] ); }

    // M * V -> vector transformation
    const Vector	 operator*( Vector &v ) const
			{ return Vector(
			    v.x*mat[0][0]+v.y*mat[0][1]+v.z*mat[0][2],
			    v.x*mat[1][0]+v.y*mat[1][1]+v.z*mat[1][2],
			    v.x*mat[2][0]+v.y*mat[2][1]+v.z*mat[2][2] ); }

    // V * M -> normal vector transformation
    // this is left mult -> different from M*V.
    friend
    const Vector	 operator*( Vector &n, Matrix &m ) 
			{ return Vector(
			    n.x*m.mat[0][0]+n.y*m.mat[1][0]+n.z*m.mat[2][0],
			    n.x*m.mat[0][1]+n.y*m.mat[1][1]+n.z*m.mat[2][1],
			    n.x*m.mat[0][2]+n.y*m.mat[1][2]+n.z*m.mat[2][2] ); }

    // transpose
    const
    Matrix		 transpose() const;

    operator double*()	{ return (double*)mat; }

private:
    // constructor for internal useage.. one that doesn't bother initializing
    explicit		Matrix( char ) {}

    double mat[4][4];
};

#endif //__MATRIX_H__
