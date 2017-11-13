#if !defined(ARIS_WINC_TIME_TIME_H_)
#define ARIS_WINC_TIME_TIME_H_

#include <config.h>

#if defined (__STRICT_ANSI__)
# undef __STRICT_ANSI__
# include <time.h>
# define __STRICT_ANSI__
#else
# include <time.h>
# include <sys/time.h>
#endif

#if defined(_TIMEVAL_DEFINED)
//#include <nxd_bsd.h>
#else
# include <sys/time.h>
#endif
#include <platforms/default/time/time.h>
int stime(time_t *timer);



#endif // ARIS_WINC_TIME_TIME_H_
