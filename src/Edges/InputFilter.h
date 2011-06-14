//--------------------------------------------------------------------------
// $Id: InputFilter.h,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __INPUTFILTER__
#define __INPUTFILTER__

#include <climits>	// for ignoreLine()
#include <strstream>	// for ourSs
#include <fstream>	// for ourIfs
#include <string>	// for ourCurFilename

#include "utility.hpp"

class Mesh;

class InputFilter : public boost::noncopyable 
{
public:
    virtual ~InputFilter()				{}

    // subclass-customizable methods --------------------------------------
    
    // different subclass can implement different versions of readFile
    // to handle different 3d file formats.
    virtual bool readFile( char *filename, Mesh &m )	= 0;

    // name() and description() allows subclasses to provide 
    // customizations to error messages.  printError() uses these
    virtual const char *name() const			= 0;	
    virtual const char *description() const		= 0;	

    // boolean conditions -------------------------------------------------
    // these allow clients to take an instance of a subclass of InputFilter
    // and stick it into an if statement, like
    // if ( InputFilter )
    // if ( !InputFilter )
    // to check input filter validity
    //
    // here they are default to "valid".. but since this is a pure virutal
    // class, this really is so that subclasses have a default :)
    //
    // the "positive" check method is a cast to void* because this is
    // probably safer than casting to a bool or an int.. because they
    // may be abused in implicit casts.
    virtual bool operator!() const			{ return false; }
    virtual operator void*() const			{ return (void*)1; }

protected:
    std::ifstream	 ourIfs; //??
    int		 ourLineNum;
    std::strstream	 ourSs;
    std::string	 ourCurFilename;

    // a bunch of common error messages.
    static const char* FAIL_READ_CMD;
    static const char* FAIL_READ_VID;
    static const char* FAIL_READ_VERT;
    static const char* FAIL_ADD_VERT;
    static const char* FAIL_READ_FID;
    static const char* FAIL_READ_FACE;
    static const char* FAIL_ADD_FACE;
    static const char* IGNORE;

    InputFilter()					{}

    // opens a file for reading.  This version opens it in text mode.
    virtual bool openFile( char *filename );

    // reads one line (one line is defined as from start of line (column 0)
    // to the first EOL (\n) character..  since "string" is being used
    // to read, there is no limit on how long the line is.  the result
    // is stuck into InputFilter::ourSs
    bool	 readLine();

    // output an error message containing the filename and line number.
    void printError( const char *errmsg ) const;

    // verify the state of the stream.
    virtual bool check( const char *errmsg )
    {
	if ( ourSs.eof() || ourSs.fail() )
	{
	    // clear the error bits, spew, and return false
	    ourSs.clear();
	    printError( errmsg );
	    return false;
	}

	// if there are no problems, return true;
	return true;
    }

private:
};

#endif // __INPUTFILTER__
