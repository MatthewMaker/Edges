//--------------------------------------------------------------------------
// $Id: OBJFilter.C,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

// #include <cstring>
// #include <cstdio>

#include <string>
#include <strstream>

#include "OBJFilter.h"
#include "InputFilterRegister.h"
#include "Mesh.h"

#define FAIL_READ_NORMAL	"failed to read vertex normal specification"
#define FAIL_READ_UV		"failed to read vertex UV specification"
#define FAIL_ADD_NORMAL		"failed to add normal to mesh"
#define FAIL_ADD_UV		"failed to add UV to mesh"

OBJFilter::OBJFilter()
{
    InputFilterRegister &ifr = InputFilterRegister::getInstance();
    ifr.registerInputFilter( "obj", this );
    ifr.registerInputFilter( "smf", this );
}

OBJFilter::~OBJFilter()
{
}

bool
OBJFilter::readFile( char *filename, Mesh &m )
{
    if ( !openFile( filename ) ) return false;

    std::string		 line, cmd, data;
    float		 f[3];

    // while we have not exhasusted input yet.
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

	// now we read the command to see what's up.
	// vertex normal
	else if ( cmd == "vn" )
	{
	    // attempt to read three float values.
	    ourSs >> f[0] >> f[1] >> f[2];
	    if ( !check( FAIL_READ_NORMAL ) ) return false;

	    // add a normal vector to the mesh;
	    if ( m.addNormal( f[0], f[1], f[2] ) == -1 )
	    {
		printError( FAIL_ADD_NORMAL );
		return false;
	    }
	}

	// vertex texUV
	else if ( cmd == "vt" )
	{
	    // attempt to read two float values.
	    ourSs >> f[0] >> f[1];
	    if ( !check( FAIL_READ_UV ) ) return false;

	    // add a UV coordinate to the mesh;
	    if ( m.addUV( f[0], f[1] ) == -1 )
	    {
		printError( FAIL_ADD_UV );
		return false;
	    }
	}

	// vertex
	else if ( cmd == "v" )
	{
	    // read in vertex coordinates
	    ourSs >> f[0] >> f[1] >> f[2];
	    if ( !check( FAIL_READ_VERT ) ) break;

	    // add a new vertex in the mesh.
	    if ( m.addVertex( f[0], f[1], f[2] ) == -1 )
	    {
		printError( FAIL_ADD_VERT );
		return false;
	    }
	}

	// face
	else if ( cmd == "f" || cmd == "t" )
	{
	    int			 num = 0;
	    unsigned int	 d[3], nn[3], tt[3];
	    std::string::size_type	 slash1, slash2;
	    std::string		 v, n, t;

	    bool 		 notdone = true;

	    bool		 hasNormal = false;
	    bool		 hasUV = false;

	    while ( notdone )
	    {
		ourSs >> data;
		
		if ( ourSs.fail() )
		{
		    // if we didn't even read a complete face yet
		    if ( num < 2 )
		    {
			printError( FAIL_READ_FACE );
			return false;
		    }
		    // o/w break out of the "notdone" loop
		    break;
		}
		else if ( ourSs.eof() )
		{
		    notdone = false;
		}

		slash1 = data.find( '/' ); 
		slash2 = data.find( '/', slash1+1 ); 

		// if there are no / in the token, that's great.
		if ( slash1 == std::string::npos )
		{
		    d[num] = atoi( data.c_str() );
		    d[num]--;

		    hasNormal = false;
		    hasUV = false;

//		    cout << data << endl;
		}
		
		// if there is only one / in the token, that means
		// only texture UV and no normal
		else if ( slash2 == std::string::npos )
		{
		    // we use "slash1 to split token
		    // into the two integers representing 
		    // the vertex and textureUV indices
		    v = data.substr( 0, slash1 );
		    d[num] = atoi( v.c_str() );
		    d[num]--;

		    t = data.substr( slash1 + 1 );
		    tt[num] = atoi( t.c_str() );
		    tt[num]--;

		    hasUV = true;
		    hasNormal = false;
		}
		else
		{
		    // o/w, we use "slash[1/2] to split the string we
		    // just read into three integers representing 
		    // the vertex, texture and normal indices.
		    v = data.substr( 0, slash1 );
		    d[num] = atoi( v.c_str() );
		    d[num]--;

		    // of course it is possible to have only
		    // normal index and not texture index.
		    hasUV = ( slash2 - slash1 > 1 );
		    if ( hasUV )
		    {
			t = data.substr( slash1 + 1, slash2 - slash1 - 1 );
			tt[num] = atoi( t.c_str() );
			tt[num]--;
		    }

		    n = data.substr( slash2 + 1 );
		    nn[num] = atoi( n.c_str() );
		    nn[num]--;
		    hasNormal = true;

//		    cout << data << " " << d[num];
//		    cout << " " << nn[num] << " " << tt[num] << endl;
		}

		if ( num == 2 )
		{
		    // add a face
		    if ( !m.addFace( 3, d,
				     ( hasNormal ? nn : 0 ),
				     ( hasUV ? tt : 0 ) ) )
		    {
			printError( FAIL_ADD_FACE );
			return false;
		    }

		    // move the 3rd vertex to the 2nd slot, so
		    // next time the next vertex we read will
		    // be the third again.
		    d[num-1] = d[num];
		    nn[num-1] = nn[num];
		    tt[num-1] = tt[num];
		}
		else ++num;
	    }
	}

	// TODO usemtl
	// TODO texture
	
	// the rest we do not support.  we ignore them.
	else 
	{
//	    printError( IGNORE );
//	    cerr << "\"" << cmd << "\""<< endl;
	}
    }

    return true;
}
