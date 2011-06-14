//--------------------------------------------------------------------------
// $Id: UWashFilter.h,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __UWashFILTER__
#define __UWashFILTER__

#include "InputFilter.h"

class UWashFilter : public InputFilter 
{
public:

    UWashFilter();
    virtual ~UWashFilter();

    virtual bool readFile( char *filename, Mesh &m );
    virtual const char *description()	const { return "U(Wash) Meshes"; }
    virtual const char *name()		const { return "UWashFilter"; }
};

#endif // __UWashFILTER__
