//
// simpcheck.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _SIMPCHECK_H
#define _SIMPCHECK_H

#ifdef __cplusplus
extern "C" {
#endif

extern void optcheck( Sppd * );
extern void boundary_check( Sppd * );
extern void isolate_vertex_check( Sppd * );
extern void vertex_neighborhood_check( Sppd * );
extern Sphe *create_vertex_halfedge_link( Spvt *, Sppd * );
extern void isolated_face_check( Sppd * );

#ifdef __cplusplus
}
#endif

#endif // _SIMPCHECK_H
