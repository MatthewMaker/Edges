//--------------------------------------------------------------------------
// $Id: Timer.h,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:

    // Default Constructor, will "restart" the timer.
    Timer()					{ restart(); }

    // Copy Constructor and assignment.  
    // Post: *this will have the same start time as "t" does.
    Timer( Timer &t ): myTime( t.myTime )	{}
    Timer &operator=( Timer &t )		{ myTime = t.myTime;
						  return *this; }

    ~Timer()					{}

    // restart the timer.
    void restart();

    // returns seconds elasped between (re)start and stop
    double stop();

private:

    double		 myTime;
    
};

#endif //__TIMER_H__
