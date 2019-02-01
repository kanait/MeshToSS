//
// ppd.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _PPD_H
#define _PPD_H

#ifdef __cplusplus
extern "C" {
#endif

// functions
extern Sppd *create_ppd(void);
extern Sppd* copy_ppd( Sppd* );
extern void free_ppd(Sppd *);
extern void calc_ppd_boundary(Sppd *);
extern void normalize_ppd(Sppd *);
extern void ppd_size(Sppd *, double *, double *, double *, double *, double *, double *);
extern void unnormalize_ppd(Sppd *);
extern void ppd_make_edges( Sppd* );
extern void ppd_triangulation( Sppd* );
extern void ppdface_triangulation( Spfc*, Sppd* );

#ifdef __cplusplus
}
#endif

#endif // _PPD_H  
