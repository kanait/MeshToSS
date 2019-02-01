//
// swin.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _SWIN_H
#define _SWIN_H

extern Swin *create_swin(void);
extern void display3d_initialize(Disp3D *);
extern void screenatr_initialize(ScreenAtr *, int, int);
extern Material *material_initialize(void);
extern void swin_free_ppd(Swin *);
extern void free_swin(Swin *);

#endif // _SWIN_H
