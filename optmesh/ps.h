//
// ps.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _PS_H
#define _PS_H

#ifdef __cplusplus
extern "C" {
#endif

#define WINDOWCENTERX 250.0
#define WINDOWCENTERY 400.0
#define PARA_WINDOWCENTERX 100
#define PARA_WINDOWCENTERY 450

#define RADIUS 100.0

extern void pstran(Vec2d *, Vec2d *);
extern void pstran_p(Vec2d *, Vec2d *);
extern void harmonicmap_to_ps(char *, Sppd *);
extern void psharmonicppdedge( FILE *, Sppd * );

#ifdef __cplusplus
}
#endif

#endif // _PS_H
