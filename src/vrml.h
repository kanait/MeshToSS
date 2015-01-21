//
// vrml.h
//
// Copyright (c) 2000 Takashi Kanai; All rights reserved. 
//

#ifndef _VRML_H
#define _VRML_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef __cplusplus
extern "C" {
#endif
  
extern Sppd* open_vrml( char* );
extern void write_vrml( char*, Sppd*, int );
extern int check_vrml( char* );
  
#ifdef __cplusplus
}
#endif
#endif // _VRML_H

