//
// vrml.cxx
//
// Copyright (c) 2000 Takashi Kanai; All rights reserved. 
//

#include "StdAfx.h"

//  #if defined(_DEBUG) && defined(_WINDOWS)
//  #define new DEBUG_NEW
//  #undef THIS_FILE
//  static char THIS_FILE[] = __FILE__;
//  #endif

#if 0
#ifdef WIN32
#include <string.h>
#endif
#endif

#include <math.h>
#include <assert.h>
#include "../optmesh/smd.h"
#include "../optmesh/ppd.h"
#include "../optmesh/ppdsolid.h"
#include "../optmesh/ppdpart.h"
#include "../optmesh/ppdvertex.h"
#include "../optmesh/ppdface.h"
#include "../optmesh/oppd.h"

#if 0
#include "ppdedge.h"
#include "ppdnormal.h"
#include "ppdloop.h"
#include "veclib.h"
#endif

#ifdef WITH_LIBVRML97 // libvrml97
//  #include <vrml97/Doc.h>
//  #include <vrml97/System.h>
#include "vrml97/VrmlNode.h"
#include "vrml97/VrmlNodeCoordinate.h"
#include "vrml97/VrmlNodeIFaceSet.h"
#include "vrml97/VrmlNodeShape.h"
#include "vrml97/VrmlScene.h"
#endif

#if 0 // openvrml
#include "openVRML/vrml97node.h"
#include "openVRML/VrmlScene.h"
#endif

#include "vrml.h"

#ifdef WITH_LIBVRML97 // libvrml97
static Sppd* vrmlSceneToPpd( VrmlScene* );
static Sppd* ShapeToPpd( VrmlNodeShape* );
static void traverse( VrmlNode* );
#endif

Sppd* open_vrml( char* fname )
{
#ifdef WITH_LIBVRML97 // libvrml97
  VrmlScene vrmlScene( fname, NULL );
  
  // Scene to ppd
  Sppd* ppd = vrmlSceneToPpd( &vrmlScene );

  vrmlScene.destroyWorld();

  return ppd;
  
#endif
  return NULL;
}

#ifdef WITH_LIBVRML97 // libvrml97

static Sppd* out_ppd;

//
// Shape が入っている Node を探す
//
void traverse( VrmlNode* node )
{
//    if ( out_ppd != NULL ) return;
  
  VrmlNodeShape* shape = node->toShape();
  if ( shape )
    {
//        display("A shape!\n");
      out_ppd = ShapeToPpd( shape );
//        return;
    }
  else {
//      display("dame.\n");
  }
//    else
//      {
  VrmlNodeGroup *group = node->toGroup();
  if (group) {
//      display("child %d\n", group->size() );
    for ( int i = group->size(); i > 0; --i ) {
//        display("i %d\n", i );
      traverse( group->child(i-1) );
    }
  }
      //    }
}
  
static Sppd* vrmlSceneToPpd( VrmlScene* vrmlScene )
{
  VrmlNode* node = vrmlScene->getRoot();

  out_ppd = NULL;
  traverse( node );
  Sppd* ppd = out_ppd;
  if ( ppd != NULL )
    {
      ppd_triangulation( ppd );
      ppd_make_edges( ppd );
      free_ppdvertex_noedge( ppd );
      FreePPDVertexEdge( ppd );
      calc_ppd_boundary( ppd );
      reattach_ppdvertex_halfedge( ppd );
      normalize_ppd( ppd );
//        display("vn %d fn %d\n", ppd->vn, ppd->fn );
    }
  out_ppd = NULL;
  return ppd;
}

//
// out_ppd にすべての Shape を追加する
//
Sppd* ShapeToPpd( VrmlNodeShape* shape )
{
  // ppd の生成
  Sppd* ppd;
  if ( out_ppd == NULL )
    {
      ppd = create_ppd();
      ppd->spso = create_ppdsolid( ppd );
      ppd->sppt = create_ppdpart( ppd );
      ppd->sppt->bpso = ppd->spso;
    }
  else
    {
      ppd = out_ppd;
    }
  
  // IndexedFaceSet の取得
  VrmlNodeIFaceSet* ifaceset = shape->getGeometry()->toIFaceSet();
//    if ( ifaceset ) display("ifaceset.\n");

  // 座標値の取得
  // coord が座標値列
  if ( ifaceset == NULL ) return ppd;

  VrmlMFVec3f& coord =
    ((VrmlNodeCoordinate*) ifaceset->getCoordinate())->coordinate();
  int nvert = coord.size();
  //    display("n %d\n", coord.size() );

  // oppd の生成
  Oppd* oppd = create_oppd();
  // overtex の生成
  Ovt* ovt = create_overtex( nvert );
  oppd->ovt = ovt;
  
  float* points = &coord[0][0];
  int i;
  for ( i = 0; i < coord.size(); ++i ) {

    float* v = &(points[3*i]);

    // vt の生成
    Spvt* vt = create_ppdvertex( ppd );
    vt->bpso = ppd->spso;
    ovt[i].vt = vt;
    vt->vec.x = (double) v[0];
    vt->vec.y = (double) v[1];
    vt->vec.z = (double) v[2];
    
//      display("point %d %g %g %g\n", i, v[0], v[1], v[2] );
  }

  // 面の座標列の取得
  const VrmlMFInt &coordIndex = ifaceset->getCoordIndex();
  display("n %d\n", coordIndex.size() );
  int fid = 0;
//    display("face %d\t", fid );
  int* faces = &coordIndex[0];
  
  Spfc* fc = create_ppdface( ppd );
  fc->bpso = ppd->spso;
  fc->bppt = ppd->sppt;

#if 1
  // he の生成
//    for ( i = 0; i < 3; ++i )
  for ( i = 0; i < coordIndex.size()-1; ++i )
    {
      if ( faces[i] == -1 ) {
	calc_fnorm( fc );
	if ( i == coordIndex.size()-2 ) break;
	// new face
	++fid;
	fc = create_ppdface( ppd );
	fc->bpso = ppd->spso;
	fc->bppt = ppd->sppt;
	
//  	display("\nface %d\t", fid);
      } else {
	Sphe* he = create_ppdhalfedge( fc );
	he->vt = ovt[faces[i]].vt;
//  	display("%d ", faces[i]);
      }
    }

#endif
  
  free_oppd( oppd );

  
  return ppd;
}
#endif // WITH_LIBVRML97

void write_vrml( char* fname, Sppd* ppd, // ppd is top-level ppd
		 int type )
{
  if ( ppd == NULL ) return;

  FILE *fp;
  if ( (fp = fopen(fname, "w")) == NULL ) return;
	
  // header
  fprintf(fp, "#VRML V2.0 utf8\n");
  if ( type == SAVE_VRML_SUBDIV )
    {
      fprintf(fp, "#EVRML_SUBDIV\n");
    }
  else
    {
      fprintf(fp, "#EVRML_POLY\n");
    }
  fprintf(fp, "\n");
  fprintf(fp, "DEF Home Viewpoint {\n");
  fprintf(fp, "    position	0 0 1.931\n");
  fprintf(fp, "    description	\"Home\"\n");
  fprintf(fp, "}\n");
  fprintf(fp, "NavigationInfo {\n");
  fprintf(fp, "    type	[ \"EXAMINE\", \"ANY\" ]\n");
  fprintf(fp, "    avatarSize 0\n");
  fprintf(fp, "}\n");
  fprintf(fp, "Collision {\n");
  fprintf(fp, "    collide FALSE\n");
  fprintf(fp, "    children [\n");
  fprintf(fp, "	Group {\n");
  fprintf(fp, "	    children [\n");
  fprintf(fp, "		DEF Cameras Switch {\n");
  fprintf(fp, "		    whichChoice	-1\n");
  fprintf(fp, "		},\n");
  fprintf(fp, "		Group {\n");
  fprintf(fp, "		    children [\n");
  fprintf(fp, "			Shape {\n");
  fprintf(fp, "			    appearance\n");
  fprintf(fp, "				Appearance {\n");
  fprintf(fp, "				    material\n");
  fprintf(fp, "					DEF _DefMat Material {\n");
  fprintf(fp, "					}\n");
  fprintf(fp, "				}\n");
  fprintf(fp, "			    geometry\n");
  fprintf(fp, "				IndexedFaceSet {\n");
  fprintf(fp, "				    coord\n");
  fprintf(fp, "					Coordinate {\n");  

  // vertex
  fprintf(fp, "					    point	[ ");
  int i = 0;
  for ( Spvt* vt = ppd->spvt; vt != NULL; vt = vt->nxt )
    {
      vt->sid = i; ++i;

      if ( vt == ppd->spvt )
	{
	  fprintf(fp, "%g %g %g,\n", vt->vec.x, vt->vec.y, vt->vec.z );
	}
      else if ( vt == ppd->epvt )
	{
	  fprintf(fp, "\t\t\t\t\t\t\t");
	  fprintf(fp, "%g %g %g ]\n", vt->vec.x, vt->vec.y, vt->vec.z );
	}
      else
	{
	  fprintf(fp, "\t\t\t\t\t\t\t");
	  fprintf(fp, "%g %g %g,\n", vt->vec.x, vt->vec.y, vt->vec.z );
	  
	}
    }
  fprintf(fp, "					}\n");
  fprintf(fp, "				    solid	FALSE\n");
  fprintf(fp, "				    creaseAngle	0.5\n");

  // face
  fprintf(fp, "				    coordIndex	[ ");
  for ( Spfc* fc = ppd->spfc; fc != NULL; fc = fc->nxt )
    {
      if ( fc != ppd->spfc ) fprintf(fp, "\t\t\t\t\t\t\t");
      Sphe* he = fc->sphe;
      do {
	fprintf(fp, "%d, ", he->vt->sid );
      } while ( (he = he->nxt) != fc->sphe );
      if ( fc != ppd->epfc ) fprintf(fp, "-1,\n");
      else fprintf(fp, "-1 ]\n");
    }
  fprintf(fp, "				}\n");
  fprintf(fp, "			}\n");
  fprintf(fp, "		    ]\n");
  fprintf(fp, "		}\n");
  fprintf(fp, "	    ]\n");
  fprintf(fp, "	}\n");
  fprintf(fp, "    ]\n");
 fprintf(fp, "}\n");
  
  fclose(fp);
}
  
int check_vrml( char* fname )
{
  FILE *fp;
  if ( (fp = fopen(fname, "r")) == NULL ) return FILE_NONE;

  int file_type = FILE_VRML;
  char buf[BUFSIZ], key[BUFSIZ];
  while ( fgets(buf, BUFSIZ, fp) ) {
    sscanf(buf, "%s", key);
    if ( key[0] != '#' ) continue;

    if ( !strcmp(key, "#EVRML_POLY") )
      file_type = FILE_EVRML_POLY;
    if ( !strcmp(key, "#EVRML_SUBDIV") )
      file_type = FILE_EVRML_SUBDIV;
  }
  fclose( fp );

  return file_type;
}
