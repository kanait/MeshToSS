//
// meshtoss.cxx
// meshtoss main function
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "smd.h"
#include "params.h"
#include "ppd.h"
#include "ppdlist.h"

#include "qem.h"
#include "simpmesh.h"

Params params;

Sppd *global_ppd = NULL;

void params_init( Params *params )
{
  params->nfile = 0;
  params->subdivision = EVAL_VERTEX;
  params->vnum = 0;
  params->num_specified = FALSE;
  params->pss_output = FALSE;
  params->save_files = FALSE;
  params->print = FALSE;
  params->pre_optimize = FALSE;

  // penalties
  params->discon_penalty = 1000.0;
  double tol_angle = 160.0;
  params->angle_tol = cos( tol_angle * SMDPI / 180.0 );
  params->shape_tol = 0.01;

  // qem mode
  params->qemmode = MODE_VQEM;
  
  // parameters
  params->gamma = 1.0;
  
}

#if !defined(_LIB)

//void main( int argc, char *argv[] )
int main( int argc, char *argv[] )
{
  params_init( &params );
  
  int i = 1;
  int j = 0;
  while( i < argc ) {
    
    if ( !strcmp(argv[i], "-slp") ) {

      params.subdivision = EVAL_SLP;
      
    } else if ( !strcmp(argv[i], "-2sub") ) {

      params.subdivision = EVAL_2SUB;
      
    } else if ( !strcmp(argv[i], "-pss") ) {
      
      params.pss_output = TRUE;
      ++i;
      if ( argv[i] ) strcpy( params.pssfilename, argv[i] );
      
    } else if ( !strcmp(argv[i], "-gamma") ) {
      
      ++i;
      if ( argv[i] ) params.gamma = atof( argv[i] );
      
    } else if ( !strcmp(argv[i], "-vnum") ) {

      params.num_specified = TRUE;
      ++i;
      if ( argv[i] ) params.vnum = atoi(argv[i]);
      
    } else if ( !strcmp(argv[i], "-angle") ) {

      ++i;
      if ( argv[i] ) params.angle_tol = cos( atof(argv[i]) * SMDPI / 180.0 );
      
    } else if ( !strcmp(argv[i], "-shape") ) {

      ++i;
      if ( argv[i] ) params.shape_tol = atof(argv[i]);
      
    } else if ( !strcmp(argv[i], "-discon") ) {

      ++i;
      if ( argv[i] ) params.discon_penalty = atof(argv[i]);
      
    } else if ( !strcmp(argv[i], "-pre") ) {

      params.pre_optimize = TRUE;
      
    } else if ( !strcmp(argv[i], "-save_files") ) {

      params.save_files = TRUE;
      
    } else if ( !strcmp(argv[i], "-eqem") ) {

      params.qemmode = MODE_EQEM;
      
    } else if ( !strcmp(argv[i], "-print") ) {

      params.print = TRUE;
      
    } else {
      
      if ( j < 2 ) {
        strcpy( params.filename[j], argv[i] );
      }
      ++j;
      ++(params.nfile);

    }
    ++i;     
  }

  if ( params.nfile != 2 ) {
    fprintf( stderr, "meshtoss ver.1.01 (originally from optmesh) \n");
    fprintf( stderr, "Copyright (c) 1995-2000 by Takashi Kanai. All rights reserved.\n");
    fprintf( stderr, "usage: meshtoss(.exe) in.ppd out.ppd [-vnum num] [-lod out.pss] [-slp] [-2sub] [-eqem] [-pre] [-angle val] [-shape val] [-discon val] [-print]\n");
    fprintf( stderr, "options: \n");
    fprintf( stderr, "  -slp:\t\tgenerates subdivision control meshes (using SLP)\n");
    fprintf( stderr, "  -2sub:\tgenerates subdivision control meshes (using 2-subdivision) \n");
    fprintf( stderr, "  -eqem:\t\tedge-base QEM mode.\n");
    fprintf( stderr, "  -pss:\t\tprogressive subdivision surfaces format(.pss) output.\n");
    fprintf( stderr, "  -gamma:\tparameter used in 2-subdivision scheme. (0 < g < 1)\n");
    fprintf( stderr, "  -vnum:\tspecifies the number of vertices. (default: 0)\n");
    fprintf( stderr, "  -angle:\tspecifies angle tolerance. \n");
    fprintf( stderr, "  \t(default: 150.0. a value is from 0.0 to 180.0. 0.0 is the most strong.)\n");
    fprintf( stderr, "  -shape:\tspecify triangle shape tolerance. \n");
    fprintf( stderr, "  \t(default: 0.0. a value is from 0.0 to 1.0. 1.0 is the most strong.)\n");
    fprintf( stderr, "  -discon:\tpenalty for discontinuous edges such as creases. (default: 1000.0)\n");
    fprintf( stderr, "  -pre:\tglobal optimization as the pre-processing (available only if -subdiv is on).\n");
    fprintf( stderr, "  -save_files:\tsave ppd files which have various number of vertices (v100, v500, v1000, v3000, v5000, v10000).\n");
    fprintf( stderr, "  -print:\tprint status.\n");

    exit( 1 );
  }

  // open ppd
  Sppd *ppd = (Sppd *) NULL;
  if ( (ppd  = open_ppd( params.filename[0] )) == (Sppd *) NULL ) {
    fprintf( stderr, "Error: can't open %s.\n", params.filename[0] );
    exit( 1 );
  }

  global_ppd = ppd;
  
  // 
  simpmesh( ppd, params.vnum );
  
  write_ppd_file( params.filename[1], ppd );
  
  free_ppd( ppd );
}

#endif // #if !defined(_LIB)
