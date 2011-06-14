//--------------------------------------------------------------------------
// $Id: OBJFilter.h,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __OBJFILTER__
#define __OBJFILTER__

#include "InputFilter.h"

class OBJFilter : public InputFilter 
{
public:
    OBJFilter();
    virtual ~OBJFilter();

    virtual bool readFile( char *filename, Mesh &m );
    virtual const char *description() const	{ return "Wavefront OBJ"; }
    virtual const char *name() const		{ return "OBJFilter"; }
};

#endif // __OBJFILTER__
