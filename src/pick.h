//
// pick.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _PICK_H
#define _PICK_H

extern BOOL edit_pick( int, int, int, ScreenAtr * );
extern BOOL edit_pick_area( int, int, int, int, int, ScreenAtr * );
extern Spvt *pick_ppdvertex_screen( int, int, ScreenAtr *, Sppd * );
extern Spvt *pick_ppdvertex_spath_screen( int, int, ScreenAtr *, Sppd * );
extern Spvt *pick_ppdvertex_screen_area( int, int, int, int, ScreenAtr *, Sppd * );
/*  extern Splp *pick_ppdloop_screen( int, int, ScreenAtr *, Sppd * ); */
extern Spfc *pick_ppdface_screen( int, int, ScreenAtr *, Sppd * );
/*  extern TEdge *pick_tedge_screen( int, int, ScreenAtr *, Tile * ); */
/*  extern int pick_tface_ppd_screen( int, int, ScreenAtr *, Sppd * ); */
//extern HVertex *pick_hvertex_screen( int, int, ScreenAtr *, HPpd * );
//extern HVertex *pick_hvertex_screen_area( int, int, ScreenAtr *, HPpd * );
//extern HFace *pick_hface_screen( int, int, ScreenAtr *, HPpd * );
//extern Splp *pick_splp_screen( int, int, ScreenAtr *, HPpd * );

#endif // _PICK_H
