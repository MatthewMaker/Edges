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
// $Id: Matrix.C,v 1.1 2001/03/21 04:45:53 vchma Exp $
//--------------------------------------------------------------------------

#include "Matrix.h"

static double id[4][4] = { { 1., 0., 0., 0. }, { 0., 1., 0., 0. },
                           { 0., 0., 1., 0. }, { 0., 0., 0., 1. } };

void
Matrix::makeIdentity()
{
    memcpy( mat, id, sizeof(double)*16 );
}

bool
Matrix::isIdentity() const
{
    return !memcmp( mat, id, sizeof(double)*16 );
}

const Matrix
Matrix::operator*( const Matrix &m )
{
    // use private constructor.
    Matrix tmp( 0 );

    for( int i = 4 ; i --> 0 ; )
	for( int j = 4 ; j --> 0 ; )
	    tmp.mat[i][j]= mat[i][0] * m.mat[0][j] +
		           mat[i][1] * m.mat[1][j] +
		           mat[i][2] * m.mat[2][j] +
		           mat[i][3] * m.mat[3][j];
    
    return tmp;
}

const Matrix
Matrix::transpose() const
{
    // use private constructor.
    Matrix tmp( 0 );

    for( int i = 4 ; i --> 0 ; )
	for( int j = 4 ; j --> 0 ; )
	    tmp.mat[i][j] = mat[j][i];
    
    return tmp;
}
