//
// meshtoss_gui_prefix.h — non-MFC preamble for Qt / cross-platform GUI builds.
//

#ifndef MESHTOSS_GUI_PREFIX_H
#define MESHTOSS_GUI_PREFIX_H

#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#endif
