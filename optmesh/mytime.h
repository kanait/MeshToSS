//
// time.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _MYTIME_H
#define _MYTIME_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern void time_start( void );
extern void time_stop( void );
extern void time_stop_value( double *, double *, double * );

#ifdef __cplusplus
}
#endif

#endif // _MYTIME_H
