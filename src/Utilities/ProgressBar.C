//--------------------------------------------------------------------------
// $Id: ProgressBar.C,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#include <iostream>
#include "ProgressBar.h"

void
ProgressBar::restart( ULong goal )
{
  myCount = myNextTic = myTic = 0;
  myGoal = goal;

  myOS << "\n|  0%";
  if ( !myGoal ) myGoal = 1;  // prevent divide by zero
}

void
ProgressBar::draw()
{
    UInt tics_needed = static_cast<UInt>( ( myCount * 20.0 ) / myGoal );

    myOS << "\b\b\b\b";

    do {
	myOS << '=' << std::flush;
    } while ( ++myTic < tics_needed );

    // use fp so large counts work
    myNextTic = static_cast<ULong>( ( myTic / 20.0 ) * myGoal );

    if ( myCount == myGoal )
    {
	if ( myTic < 21 ) myOS << '=';
	myOS << "| 100%" << std::endl;
    }
    else
    {
	myOS << " " << ( myTic < 20 ? myTic * 5 : 99 ) << "%";
    }

}

