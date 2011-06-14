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
// $Id: ts.C,v 1.2 2001/02/08 02:19:56 vchma Exp $
//--------------------------------------------------------------------------

#include "Mesh.h"
#include "TriStrip.h"

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

int 
main( int argc, char **argv )
{
    std::cout << argv[0] << " & EDGES, Copyright (C) 1999-2000 Vincent C. H. Ma\n";
    std::cout << argv[0] << " & EDGES comes with ABSOLUTELY NO WARRANTY; for details see\n";
    std::cout << "www.gnu.org/copyleft/gpl.html\n";
    std::cout << "This is free software, and you are welcome to redistribute\n";
    std::cout << "it under certain conditions; see www.gnu.org/copyleft/gpl.html\n";
    std::cout << std::endl;

    if ( argc != 2 )
    {
	std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
	return 1;
    }

    Mesh *theMesh;
    TriStrip *theStrip;

    // sanity check
    if ( !argv[1] )
    {
	std::cerr << argv[0] << ": bad filename!" << std::endl;
	return 1;
    }

    // create a new control mesh, and read in the mesh definition
    if ( !( theMesh = new Mesh ) )
    {
	std::cerr << argv[0] << ": out of memory!" << std::endl;
	return 1;
    }

    if ( !theMesh->readFile( argv[1] ) )
    {
	delete theMesh;
	std::cerr << argv[0] << ": unable to read " << argv[1] << "!" << std::endl;
	return 1;
    }

    if ( !( theStrip = new TriStrip( *theMesh ) ) )
    {
	std::cerr << argv[0] << ": out of memory!" << std::endl;
	return 1;
    }
    theStrip->output( argv[1] );

    delete theStrip;
    delete theMesh;

    return TCL_OK;
}
