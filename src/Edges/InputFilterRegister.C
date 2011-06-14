#include <cstring>
#include "InputFilterRegister.h"

class dummyInputFilter : public InputFilter
{
public:

    static dummyInputFilter &getInstance()
    {
	static dummyInputFilter theDIF;
	return theDIF;
    }

    virtual ~dummyInputFilter() {}

    virtual bool readFile( char *filename, Mesh &m )	{ return false; }
    virtual const char *name() const			{ return "dummy"; }
    virtual const char *description() const		{ return "dummy"; }
    virtual bool operator!() const			{ return true; }
    virtual operator void*() const			{ return 0; }

protected:
    dummyInputFilter() {}
};

InputFilterRegister::InputFilterRegister()
{
    Tcl_InitHashTable( &myHashTable, TCL_STRING_KEYS );
}

InputFilterRegister::~InputFilterRegister()
{
//    // first go thru each entry and free the string key.
//    Tcl_HashSearch s;
//    for ( Tcl_HashEntry *e = Tcl_FirstHashEntry( &myHashTable, &s ) ;
//	    e ; e = Tcl_NextHashEntry( &s ) )
//    {
//	free( Tcl_GetHashKey( &myHashTable, e ) );
//    }

    // blow away the table.
    Tcl_DeleteHashTable( &myHashTable );
}

InputFilterRegister&
InputFilterRegister::getInstance()
{
    static InputFilterRegister theIFR;
    return theIFR;
}

bool
InputFilterRegister::registerInputFilter( char *ext, InputFilter *iftr )
{
    int ok;
    Tcl_HashEntry *e = Tcl_CreateHashEntry( &myHashTable, ext, &ok );
    if ( !ok ) return false;
    Tcl_SetHashValue( e, iftr ); 
    return true;
}

InputFilter &
InputFilterRegister::getInputFilter( const char *fn )
{
    std::string filename( fn );

    // find the last period.
    std::string::size_type idx = filename.rfind( '.' );

    // if no period is found, no extension = no idea. too bad.
    // otherwise, key
    if ( idx == std::string::npos ) return dummyInputFilter::getInstance();

    const char *ext = filename.substr( idx+1 ).c_str();

    // attempt to find an inputFilter by the extension.
    Tcl_HashEntry *e = Tcl_FindHashEntry( &myHashTable, ext );
    if ( !e ) return dummyInputFilter::getInstance();

    return *(static_cast<InputFilter *>( Tcl_GetHashValue( e ) ));
}
