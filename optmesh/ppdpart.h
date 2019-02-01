//
// ppdpart.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _PPDPART_H
#define _PPDPART_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Sppt *create_ppdpart(Sppd *);
extern void free_ppdpart(Sppt *, Sppd *);

#ifdef __cplusplus
}
#endif

#endif // _PPDPART_H
