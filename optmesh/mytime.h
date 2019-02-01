//
// time.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
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
