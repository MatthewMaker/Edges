//--------------------------------------------------------------------------
// $Id: InputFilterRegister.h,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __INPUT_FILTER_REGISTER__
#define __INPUT_FILTER_REGISTER__

#include <string>
#include "InputFilter.h"
#include "utility.hpp"

#include <tcl.h>

class InputFilterRegister: public boost::noncopyable
{
public:
    static InputFilterRegister& getInstance();
    ~InputFilterRegister();

    bool registerInputFilter( char *ext, InputFilter *iftr );

    InputFilter &getInputFilter( const char *filename );

protected:
    InputFilterRegister();

private:
    Tcl_HashTable myHashTable;
};

#endif // __INPUT_FILTER_REGISTER__
