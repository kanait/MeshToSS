//
// oppd.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _OPPD_H
#define _OPPD_H

#ifdef __cplusplus
extern "C" {
#endif

extern Oppd *create_oppd(void);
extern void free_oppd(Oppd *);
extern Oso *create_osolid( int );
extern Opt *create_opart( int );
extern Ofc *create_oface( int );
extern Onm *create_onormal( int );
extern Ovt *create_overtex( int );

#ifdef __cplusplus
}
#endif

#endif // _OPPD_H
