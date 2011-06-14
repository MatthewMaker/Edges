//--------------------------------------------------------------------------
// $Id: ProgressBar.h,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

#include <iosfwd>
#include "utility.hpp"

//  ProgressBar displays a progress bar.
//
//  This class will draw something like this:
//  start:		|  0%
//  in progress:	|===== 40%
//  end:		|==================|

typedef unsigned long ULong;
typedef unsigned int  UInt;

class ProgressBar : boost::noncopyable {

public:
   ProgressBar( ULong goal, std::ostream& os = std::cout ) : myOS(os)
		 { restart(goal); }

   //  Effects: display appropriate scale
   //  Postconditions: count()==0, goal()==goal
   void          restart( ULong goal );

   //  Effects: Display appropriate progress tic if needed.
   //  Postconditions: count()== original count() + increment
   //  Returns: count().
   ULong	 operator+=( ULong increment )
		 {
		     if ( (myCount += increment) >= myNextTic ) draw();
		     return myCount;
		 }

   ULong  operator++()		{ return operator+=( 1 ); }
   ULong  count() const		{ return myCount; }
   ULong  goal() const		{ return myGoal; }

private:

   std::ostream	&myOS;
   ULong	 myCount;
   ULong	 myGoal;
   ULong	 myNextTic;
   UInt		 myTic;

   void draw();
};

#endif // __PROGRESS_BAR_H__
