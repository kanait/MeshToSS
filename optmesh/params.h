//
// params.h
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifndef _PARAMS_H
#define _PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

// structures for options
typedef struct _params {

  // input, output filename
  int  nfile;
  char filename[2][BUFSIZ];

  // TRUE: subdivision, FALSE: mesh simplification
  unsigned short subdivision;

  BOOL num_specified;
  int  vnum;
  
  // Progressive Subdivision Surfaces output
  BOOL pss_output;  
  char pssfilename[BUFSIZ]; 

  // save files
  BOOL save_files;

  // print
  BOOL print;

  // global optimization as a pre-processing
  // (available only if -subdiv is on.)
  BOOL pre_optimize;

  // Penalties
  double discon_penalty;
  double angle_tol;
  double shape_tol;

  // QEM mode (MODE_VQEM or MODE_EQEM default: MODE_VQEM)
  unsigned short qemmode;
  
  // parameter for 2-sub evaluation
  double gamma;
  
  
} Params;

void params_init( Params * );

extern Params params;

#ifdef __cplusplus
}
#endif

#endif // _PARAMS_H
