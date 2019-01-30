//
// time.cxx
// time measurement functions
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
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
#include <limits.h>
#include <time.h>
#include <sys/times.h>
#endif

#include "smd.h"

#include "mytime.h"

#ifdef WIN32
struct tms {
  time_t tms_utime;
  time_t tms_stime;
  time_t tms_cutime;
  time_t tms_cstime;
};
#endif

#ifndef WIN32
static struct tms tbuf1;
static long real1;
#endif

// start timer
void time_start(void)
{				
#if !defined(WIN32)
  real1 = times(&tbuf1);
#endif
}

// stop timer 
void time_stop( void )
{
#if !defined(WIN32)
  struct tms tbuf2;
  long  real2;
  double realtime, usertime, systime;

  real2 = times(&tbuf2);

  //  double ct = (double) CLK_TCK;
  double ct = (double) CLOCKS_PER_SEC;
  
  realtime = (real2 - real1) / ct;
  usertime = (tbuf2.tms_utime - tbuf1.tms_utime) / ct;
  systime  = (tbuf2.tms_stime - tbuf1.tms_stime) / ct;
  fprintf(stdout,"processed time: \n");
  fprintf(stdout,"\treal:\t%.2f (s)\n", realtime);
  fprintf(stdout,"\tuser:\t%.2f (s)\n", usertime);
  fprintf(stdout,"\tsys: \t%.2f (s)\n", systime);
#endif
}

// stop timer 
void time_stop_value( double *realtime, double *usertime, double *systime )
{				
#ifndef WIN32
  struct tms tbuf2;
  long  real2;

  // double ct = (double) CLK_TCK;
  double ct = (double) CLOCKS_PER_SEC;
  
  real2 = times(&tbuf2);
  *realtime = (real2 - real1) / ct;
  *usertime = (tbuf2.tms_utime - tbuf1.tms_utime) / ct;
  *systime  = (tbuf2.tms_stime - tbuf1.tms_stime) / ct;
//   fprintf(stdout,"processed time: \n");
//   fprintf(stdout,"\treal:\t%.2f (s)\n", *realtime);
//   fprintf(stdout,"\tuser:\t%.2f (s)\n", *usertime);
//   fprintf(stdout,"\tsys: \t%.2f (s)\n", *systime);
#endif
}

