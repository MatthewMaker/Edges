//--------------------------------------------------------------------------
// $Id: InputFilter.C,v 1.2 2000/10/29 17:49:27 vchma Exp $
//--------------------------------------------------------------------------
#include <iostream>

#include "InputFilter.h"
#include "UWashFilter.h"
#include "OBJFilter.h"

const char* InputFilter::FAIL_READ_CMD	= "failed to read in command" ;
const char* InputFilter::FAIL_READ_VID	= "cannot read vertex id";
const char* InputFilter::FAIL_READ_VERT	= "cannot read vertex coord";
const char* InputFilter::FAIL_ADD_VERT	= "failed to add vertex to mesh";
const char* InputFilter::FAIL_READ_FID	= "cannot read face id";
const char* InputFilter::FAIL_READ_FACE = "cannot read face specification";
const char* InputFilter::FAIL_ADD_FACE	= "failed to add face to mesh";
const char* InputFilter::IGNORE		= "Ignoring unhandled command";

static UWashFilter theUW;
static OBJFilter theOBJ;

void
InputFilter::printError( const char *errmsg ) const
{
    std::cerr << name() << ": " << errmsg;
    std::cerr << " in file " << ourCurFilename << " (" << description() << ")";
    std::cerr << " on line " << ourLineNum << std::endl;
}

bool
InputFilter::openFile( char *filename )
{
    // make sure the last file is closed.
    ourIfs.close();

    // reset the line number count
    ourLineNum = 0;

    // attempt to open the file in text mode
    ourIfs.open( filename );
    if ( ourIfs.fail() )
    {
	std::cerr << "InputFilter::openFile: failed to open " << filename << std::endl;
	return false;
    }

    // remember our filename (for error message output purposes)
    ourCurFilename = filename;

    std::cout << "InputFilter::openFile: loading file " << filename << std::endl;
//    cout << "InputFilter::openFile: Progress:";

    return true;
}

bool
InputFilter::readLine()
{
    std::string line;

    // blow away what's left in the stream
    // clear all flags on ourSs
    ourSs.ignore( INT_MAX );
    ourSs.clear();

    // only stop trying to read when a line with at least one
    // character is read
    do 
    {
	// read an entire line from the file
	std::getline( ourIfs, line );

	// if no more lines to be read, too bad.
	if ( ourIfs.eof() )
	{
//	    cout << endl;
	    return false;
	}

	// if for some reason an error occured..
	else if ( ourIfs.fail() )
	{
	    printError( "reading failed" );
	    ourIfs.clear();
//	    cout << endl;
	    return false;
	}

	// otherwise, increment the number of lines read.
	ourLineNum++;

    } while ( line == "" || line[0] == '\n' || line[0] == '\r' );

//    // little progress indicator for every hundred line read.
//    if ( ourLineNum % 100 == 0 )
//    {
//	cout << ".";
//	if ( ourLineNum % 300 == 0 ) cout.flush();
//    }

    // stick the line read into a strstream, for parsing later.
    ourSs << line.c_str();

    return true;
}
