//
// time.cxx
// time measurement functions
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#if defined(_DEBUG) && defined(_WINDOWS)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <stdio.h>
#include <sys/types.h>

#if defined(_WIN32) || defined(_WIN64)
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include "smd.h"

#include "mytime.h"

static double g_last_real;
static double g_last_user;
static double g_last_sys;

void mytime_get_last( double *realtime, double *usertime, double *systime )
{
  *realtime = g_last_real;
  *usertime = g_last_user;
  *systime = g_last_sys;
}

#ifdef WIN32
struct tms {
  time_t tms_utime;
  time_t tms_stime;
  time_t tms_cutime;
  time_t tms_cstime;
};
#endif

#if !defined(_WIN32) && !defined(_WIN64)

static struct timeval wall_start;
static struct rusage ru_start;

static double timeval_diff_sec( const struct timeval *later, const struct timeval *earlier )
{
  return (double)( later->tv_sec - earlier->tv_sec )
         + (double)( later->tv_usec - earlier->tv_usec ) * 1.0e-6;
}

#endif

// start timer
void time_start(void)
{
#if !defined(_WIN32) && !defined(_WIN64)
  (void) gettimeofday( &wall_start, NULL );
  (void) getrusage( RUSAGE_SELF, &ru_start );
#endif
}

// stop timer
void time_stop( void )
{
#if !defined(_WIN32) && !defined(_WIN64)
  struct timeval wall_end;
  struct rusage ru_end;
  double realtime, usertime, systime;

  (void) gettimeofday( &wall_end, NULL );
  (void) getrusage( RUSAGE_SELF, &ru_end );

  realtime = timeval_diff_sec( &wall_end, &wall_start );
  usertime = timeval_diff_sec( &ru_end.ru_utime, &ru_start.ru_utime );
  systime  = timeval_diff_sec( &ru_end.ru_stime, &ru_start.ru_stime );

  g_last_real = realtime;
  g_last_user = usertime;
  g_last_sys = systime;

  /* stderr: visible when the binary is run attached to a terminal (not with `open *.app`). */
  fprintf( stderr, "processed time: \n" );
  fprintf( stderr, "\treal:\t%.2f (s)\n", realtime );
  fprintf( stderr, "\tuser:\t%.2f (s)\n", usertime );
  fprintf( stderr, "\tsys: \t%.2f (s)\n", systime );
  (void) fflush( stderr );
#endif
}

// stop timer (values only)
void time_stop_value( double *realtime, double *usertime, double *systime )
{
#if !defined(_WIN32) && !defined(_WIN64)
  struct timeval wall_end;
  struct rusage ru_end;

  (void) gettimeofday( &wall_end, NULL );
  (void) getrusage( RUSAGE_SELF, &ru_end );

  *realtime = timeval_diff_sec( &wall_end, &wall_start );
  *usertime = timeval_diff_sec( &ru_end.ru_utime, &ru_start.ru_utime );
  *systime  = timeval_diff_sec( &ru_end.ru_stime, &ru_start.ru_stime );

  g_last_real = *realtime;
  g_last_user = *usertime;
  g_last_sys = *systime;
#endif
}
