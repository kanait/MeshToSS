//
// draw.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _DRAW_H
#define _DRAW_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

extern void init_gl3d(void);
extern void light_init(void);
extern void clear_gl3d(ScreenAtr *);
extern void set_default_view(ScreenAtr *);
extern void calc_rotate_matrix(ScreenAtr *);
extern void view_init( ScreenAtr * );
extern void get_GL_attributes(ScreenAtr *);
extern void world_to_win(Vec *, Vec2d *, double [16], double [16], int [4]);
extern void draws3d(ScreenAtr *);
extern void draw_string(ScreenAtr *, char *);
extern void draw_gradient_background(void);
extern void redraw(void);

#endif // _DRAW_H
