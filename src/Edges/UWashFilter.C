//--------------------------------------------------------------------------
// $Id: UWashFilter.C,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

// #include <cstring>
// #include <cstdio>

#include <string>
#include <strstream>
#include <vector>

#include "UWashFilter.h"
#include "InputFilterRegister.h"
#include "Mesh.h"

#define FACE_ALREADY	"all vertex specification must occur "\
			"before any face description"

UWashFilter::UWashFilter()
{
    InputFilterRegister &ifr = InputFilterRegister::getInstance();
    ifr.registerInputFilter( "m", this );
}

UWashFilter::~UWashFilter()
{
}

bool
UWashFilter::readFile( char *filename, Mesh &m )
{
    if ( !openFile( filename ) ) return false;

    std::string		 line, cmd, data;
    float		 d[3];
    int			 f[3];
    int			 maxVid = 0;
    int			 curVid = 0;
    std::vector<int>		 origVid;
    bool		 faceAlready = false;
    int			*vmap;

    // while there are still more to read
    while ( readLine() )
    {
	ourSs >> cmd;

	// here we check only for failure to read as string.
	// since, there are lines with one one command string,
	// and ourSs.eof() will be flagged even tho it's not
	// an error for this case.
	if ( ourSs.fail() )
	{
	    printError( FAIL_READ_CMD );
	    return false;
	}

	// skip blank lines or comment lines
	if ( cmd[0] == '#' ) continue;

	// vertex normal
	else if ( cmd == "Vertex" )
	{
	    if ( faceAlready )
	    {
		printError( FACE_ALREADY );
		return false;
	    }

	    // read in vertex ID
	    ourSs >> curVid;
	    if ( !check( FAIL_READ_VID ) ) return false; 

	    // read in vertex coordinates
	    ourSs >> d[0] >> d[1] >> d[2];
	    if ( !check( FAIL_READ_VERT ) ) break;

	    // add a new vertex in the mesh.
	    if ( m.addVertex( d[0], d[1], d[2] ) == -1 )
	    {
		printError( FAIL_ADD_VERT );
		return false;
	    }
	    origVid.push_back( curVid );
	    
	    if ( curVid > maxVid ) maxVid = curVid;
	}

	// face
	else if ( cmd == "Face" )
	{
	    if ( !faceAlready )
	    {
		vmap = new int[ maxVid + 1 ];

		for ( int i = origVid.size() ; i --> 0 ; )
		{
		    vmap[origVid[i]] = i;
		}

		origVid.clear();
		faceAlready = true;
	    }


	    // grab the unused face id (at least I don't care)
	    ourSs >> curVid;
	    if ( !check( FAIL_READ_FID ) ) return false; 

	    // grab the vertex ids that maks up the current face.
	    ourSs >> f[0] >> f[1] >> f[2];
	    if ( !check( FAIL_READ_FACE ) ) return false;

	    f[0] = vmap[f[0]];
	    f[1] = vmap[f[1]];
	    f[2] = vmap[f[2]];

	    if ( !m.addFace( 3, (const unsigned int *)f ) )
	    {
		printError( FAIL_ADD_FACE );
		return false;
	    }
	}

	// the rest we do not support.  we ignore them.
	else 
	{
	    printError( IGNORE );
	}
    }

    return true;
}
