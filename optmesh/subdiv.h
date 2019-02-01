//
// subdiv.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _SUBDIV_H
#define _SUBDIV_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Sppd* ppdsubdiv( Sppd*, int );
extern void ppdsubdiv_initialize( Sppd* );
extern void loop_calculation( Sppd*, Spvt**, Spvt**, Sppd* );
extern void loop_even_internal( Spvt*, Vec* );
extern void loop_odd_internal( Sped*, Vec* );
extern void loop_even_boundary( Spvt*, Vec* );
extern void loop_odd_boundary( Sped*, Vec* );
extern double calc_beta( int );
extern void set_subdiv_boundary( Sppd* );
extern Sppd* subdiv_top( Sppd* );

// subdivision type
#define SUBDIV_DOOSABIN 0
#define SUBDIV_LOOP 1
#define SUBDIV_CATMULL 2
#define SUBDIV_HOPPE 3
  
// beta value for valence 3
#define BETA3 0.1875
#define VAL68 0.75
#define VAL58 0.625
#define VAL12 0.5
#define VAL38 0.375
#define VAL18 0.125
#define VAL3164 

#ifdef __cplusplus
}
#endif

#endif // _SUBDIV_H
