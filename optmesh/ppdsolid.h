//
// ppdsolid.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _PPDSOLID_H
#define _PPDSOLID_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Spso *create_ppdsolid(Sppd *);
extern void free_ppdsolid(Spso *, Sppd *);

#ifdef __cplusplus
}
#endif

#endif // _PPDSOLID_H
