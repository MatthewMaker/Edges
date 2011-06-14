//--------------------------------------------------------------------------
// $Id: Timer.C,v 1.1.1.1 2000/09/17 06:56:07 vchma Exp $
//--------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/param.h>
#include <sys/times.h>
#include <sys/time.h>

#include "Timer.h"

void
Timer::restart()
{
    struct timeval	 Tm;
    struct timezone	 Tz;

    gettimeofday(&Tm,&Tz);
    myTime = Tm.tv_sec + ( 0.000001 * Tm.tv_usec );
}

double
Timer::stop()
{
    struct timeval	 Tm;
    struct timezone	 Tz;

    gettimeofday(&Tm,&Tz);
    return Tm.tv_sec + ( 0.000001 * Tm.tv_usec ) - myTime;
}
