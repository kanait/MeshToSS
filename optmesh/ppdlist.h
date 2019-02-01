//
// ppdlist.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _PPDLIST_H
#define _PPDLIST_H

#ifdef __cplusplus
extern "C" {
#endif

extern void write_ppd_file(char *, Sppd *);
extern Sppd *open_ppd(char *);

#ifdef __cplusplus
}
#endif

#endif // _PPDLIST_H
