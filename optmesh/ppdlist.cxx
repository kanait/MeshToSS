//
// ppdlist.cxx
//
// Copyright (c) 1997-2000 Takashi Kanai; All rights reserved. 
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//  #include <math.h>
//  #include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "smd.h"
#include "ppd.h"
#include "ppdsolid.h"
#include "ppdpart.h"
#include "ppdvertex.h"
#include "ppdface.h"
#include "ppdedge.h"
#include "ppdnormal.h"
//#include "ppdloop.h"
#include "oppd.h"
#include "file.h"

#include "ppdlist.h"

/***************************************
Write
****************************************/

void write_ppd_file( char *fname, Sppd *ppd )
{
  FILE *fp;
  Spso *s;
  Sppt *p;
  Spfc *f;
  Spvt *v;
  Spnm *nm;
  Sphe *he;
  //Splp *l;
  //Splv *lv;
  int  i, j, n;
//   Id	 a, b;
  //	Id	 id;
//   Vec  vec;
  //	Vec  vec1, vec2;
  int  solidnum, partnum, vertexnum, normalnum;
  int  facenum, fnodenum;
  int  bfc, efc, bpt, ept, bvt, evt, bnm, enm;
  Id	 vtx[128], nrm[128];
	
  if ((fp = fopen(fname, "w")) == NULL) {
    return;
  }
	
  // header
  for (s = ppd->spso, solidnum = 0; s != (Spso *) NULL; s = s->nxt) {
    ++solidnum;
  }
  for (p = ppd->sppt, partnum = 0; p != (Sppt *) NULL; p = p->nxt) {
    ++partnum;
  }
  for (v = ppd->spvt, vertexnum = 0; v != (Spvt *) NULL; v = v->nxt) {
    ++vertexnum;
  }
//   for (l = ppd->splp, loopnum = 0; l != (Splp *) NULL; l = l->nxt) {
//     if ( l->type != SHORTESTPATH ) ++loopnum;
//   }
  for (nm = ppd->spnm, normalnum = 0; nm != (Spnm *) NULL; nm = nm->nxt) {
    /*		 display("nm %g %g %g\n", nm->vec.x, nm->vec.y, nm->vec.z); */
    ++normalnum;
  }
  for (f = ppd->spfc, facenum = fnodenum = 0; f != (Spfc *) NULL; f = f->nxt) {
    ++facenum;
    fnodenum += f->hen;
  }
	
  fprintf(fp, "header\n");
  if (solidnum)  fprintf(fp, "\tsolid\t%d\n",  solidnum);
  if (partnum)	 fprintf(fp, "\tpart\t%d\n",	 partnum);
  if (normalnum) fprintf(fp, "\tnormal\t%d\n", normalnum);
  if (vertexnum) fprintf(fp, "\tvertex\t%d\n", vertexnum);
  if (facenum)	 {
    fprintf(fp, "\tface\t%d\n",  facenum);
    fprintf(fp, "\tfnode\t%d\n", fnodenum);
  }
  fprintf(fp, "end\n");
	
  /* solid */
	
  if (solidnum) {
    bfc = efc  = 0;
    bpt = ept  = 0;
    bvt = evt  = 0;
    bnm = enm  = 0;
    fprintf(fp, "solid\n");
    i = 1;
    for (s = ppd->spso; s != (Spso *) NULL; s = s->nxt) {
      fprintf(fp, "\t%d\t", i); 			/* print Id */
      if (partnum) {
	bpt = ept + 1;
	for (p = ppd->sppt; p != (Sppt *) NULL; p = p->nxt) {
	  if (p->bpso == s) {
	    ++ept;
	  }
	  fprintf(fp, "/p %d %d ", bpt, ept);
	}
	if (vertexnum) {
	  bvt = evt + 1;
	  for (v = ppd->spvt; v != (Spvt *) NULL; v = v->nxt) {
	    if (v->bpso == s) {
// 	      if ( v->sp_type != SP_VERTEX_STEINER ) {
	      ++evt;
// 	      }
	    }
	  }
	  fprintf(fp, "/v %d %d ", bvt, evt);
	}
	if (normalnum) {
	  bnm = enm + 1;
	  for (nm = ppd->spnm; nm != (Spnm *) NULL; nm = nm->nxt) {
	    if (nm->bpso == s) {
	      ++enm;
	    }
	  }
	  fprintf(fp, "/n %d %d ", bnm, enm);
	}
	if (facenum) {
	  bfc = efc + 1;
	  for (p = ppd->sppt; p != (Sppt *) NULL; p = p->nxt) {
	    for (f = ppd->spfc; f != (Spfc *) NULL; f = f->nxt) {
	      if ((f->bpso == s) && (f->bppt == p)) {
		++efc;
	      }
	    }
	  }
	  fprintf(fp, "/f %d %d ", bfc, efc);
	}
	++i;
      }
      fprintf(fp, "\n");
    }
    fprintf(fp, "end\n");
  }
	
  /* part */
  if (partnum) {
    bfc = efc  = 0;
    fprintf(fp, "part\n");
    i = 1;
    for (p = ppd->sppt; p != (Sppt *) NULL; p = p->nxt) {
      fprintf(fp, "\t%d\t", i); 			/* print Id */
      if (facenum) {
	bfc = efc + 1;
	for (f = ppd->spfc; f != (Spfc *) NULL; f = f->nxt) {
	  if (f->bppt == p) {
	    ++efc;
	  }
	}
	fprintf(fp, "/f %d %d ", bfc, efc);
      }
      ++i;
      fprintf(fp, "\n");
    }
    fprintf(fp, "end\n");
  }
	
  // vertex
  if (vertexnum) {
    fprintf(fp, "vertex\n");
    i = 1;
    for (s = ppd->spso; s != (Spso *) NULL; s = s->nxt) {
      for (v = ppd->spvt; v != (Spvt *) NULL; v = v->nxt) {
	if (v->bpso == s) {
	  v->sid = i;
	  fprintf(fp, "\t%d\t%g %g %g\n", i, v->vec.x, v->vec.y, v->vec.z);
	  ++i;
	}
      }
    }
    fprintf(fp, "end\n");
  }
	
  /* normal */
  if (normalnum) {
    fprintf(fp, "normal\n");
    i = 1;
    for (s = ppd->spso; s != (Spso *) NULL; s = s->nxt) {
      for (nm = ppd->spnm; nm != (Spnm *) NULL; nm = nm->nxt) {
	if (nm->bpso == s) {
	  nm->sid = i;
	  fprintf(fp, "\t%d\t%g %g %g\n", i, nm->vec.x, nm->vec.y, nm->vec.z);
	  ++i;
	}
      }
    }
    fprintf(fp, "end\n");
  }
	
  /* face */
  if (facenum) {
    fprintf(fp, "face\n");
    i = 1;
    for (s = ppd->spso; s != (Spso *) NULL; s = s->nxt) {
      for (p = ppd->sppt; p != (Sppt *) NULL; p = p->nxt) {
	for (f = ppd->spfc; f != (Spfc *) NULL; f = f->nxt) {
	  if ((f->bpso == s) && (f->bppt == p)) {
	    he = f->sphe;
	    n = 0;
						
	    do {
	      if (vertexnum) vtx[n] = he->vt->sid;
	      if (normalnum) nrm[n] = he->nm->sid;
	      ++n;
	    } while ((he = he->nxt) != f->sphe);
						
	    fprintf(fp, "\t%d\t", i);
	    /* vertex */
	    for (j = 0; j < n; ++j) {
	      fprintf(fp, "%d ", vtx[j]);
	    }
	    /* normal */
	    if (normalnum) { 
	      fprintf(fp, "\\\n");
	      fprintf(fp, "\t/n\t");
	      for (j = 0; j < n; ++j)
		fprintf(fp, "%d ", nrm[j]);
	    }
	    fprintf(fp, "\n");
	    ++i;
	  }
	}
      }
    }
    fprintf(fp, "end\n");
  }
	
  fclose(fp);
}

Sppd *open_ppd( char *fname )
{
  FILE *fp;
  if ( (fp = fopen(fname, "r")) == NULL ) return NULL;


  //
  // file variables
  //
  int mode = PPD_NONE;
  int i, n, done, ibuf[BUFSIZ];
  Id num, bn, en, id = 0;
  char	*bpt;
  char	fcmode, stc[BUFSIZ], buf[BUFSIZ];
  char	key[BUFSIZ], val[7][BUFSIZ];
  //
  // ppd elements
  //
  Spso *so = NULL;
  Sppt *pt = NULL;
  Spfc *fc = NULL;
  Spvt *vt = NULL;
  Spvt *sv = NULL;
  Spvt *ev = NULL;
  Spnm *nm = NULL;
  Sphe *he = NULL;
  Sped *ed = NULL;
  //
  // open ppd elements
  //
  Oso  *oso = NULL;
  Opt  *opt = NULL;
  Ofc  *ofc = NULL;
  Ovt  *ovt = NULL;
  Onm  *onm = NULL;
	
  Sppd *ppd  = create_ppd();
	
  sprintf( ppd->filename, "%s", fname );
	
  Oppd *oppd = create_oppd();
	
  while (fgets(buf, BUFSIZ, fp)) {
    
    sscanf(buf, "%s", key);
    if (comment(buf[0])) continue;
    
    else if (!strcmp(key, "end")) {
      mode = PPD_NONE;
    }
    else if (mode == PPD_HEADER) {	/* read "header" */
      sscanf(buf, "%s%s", key, val[0]);
      num = atoi(val[0]);
      if (!strcmp(key, "solid")) {
	oso = oppd->oso = create_osolid( num );
	for (i = 0; i < num; ++i) {
	  so = create_ppdsolid(ppd);
	  oso[i].so = so;
	}
      } else if (!strcmp(key, "part")) {
	opt = oppd->opt = create_opart( num );
	for (i = 0; i < num; ++i) {
	  pt = create_ppdpart(ppd);
	  opt[i].pt = pt;
	}
      } else if (!strcmp(key, "face")) {
	ofc = oppd->ofc = create_oface( num );
	for (i = 0; i < num; ++i) {
	  fc = create_ppdface(ppd);
	  ofc[i].fc = fc;
	}
      } else if (!strcmp(key, "vertex")) {
	ppd->nvt_org = num;
	ovt = oppd->ovt = create_overtex( num );
	for (i = 0; i < num; ++i) {
	  vt = create_ppdvertex(ppd);
	  ovt[i].vt = vt;
	}
      } else if (!strcmp(key, "normal")) {
	onm = oppd->onm = create_onormal( num );
	for (i = 0; i < num; ++i) {
	  nm = create_ppdnormal(ppd);
	  onm[i].nm = nm;
	}
      }
    } else if (mode == PPD_SOLID) {
      bpt = buf;
      bpt = cut(bpt, stc);
      fcmode = '\0';
      so = oso[atoi(stc) - 1].so;
      done = SMD_OFF;
      while (!done) {
	bpt = cut(bpt, stc);
	if (stc[0] == '/' && (stc[1] == 'f' || stc[1] == 'n'
			      || stc[1] == 'v' || stc[1] == 'p')) {
	  /* /f or /n or /v */
	  fcmode = stc[1];
	  bpt = cut(bpt, val[0]); bn = atoi(val[0]) - 1;
	  bpt = cut(bpt, val[1]); en = atoi(val[1]) - 1;
	  if (fcmode == 'f') {
	    for (i = bn; i <= en; ++i) {
	      fc = ofc[i].fc;
	      fc->bpso = so;
	    }
	  } else if (fcmode == 'n') {
	    for (i = bn; i <= en; ++i) {
	      nm = onm[i].nm;
	      nm->bpso = so;
	    }
	  } else if (fcmode == 'v') {
	    for (i = bn; i <= en; ++i) {
	      vt = ovt[i].vt;
	      vt->bpso = so;
	    }
	  } else if (fcmode == 'p') {
	    for (i = bn; i <= en; ++i) {
	      pt = opt[i].pt;
	      pt->bpso = so;
	    }
	  }
	}
	if (!bpt)  done = SMD_ON;
      }
    } else if (mode == PPD_PART) {
      bpt = buf;
      bpt = cut(bpt, stc);
      fcmode = '\0';
      pt = opt[atoi(stc) - 1].pt;
      done = SMD_OFF;
      while (!done) {
	if (bpt) {
	  bpt = cut(bpt, stc);
	  if (stc[0] == '/' && (stc[1] == 'f' || stc[1] == 'b')) {
	    /* /f or /b */
	    fcmode = stc[1];
	    bpt = cut(bpt, val[0]); bn = atoi(val[0]) - 1;
	    bpt = cut(bpt, val[1]); en = atoi(val[1]) - 1;
	    for (i = bn; i <= en; ++i) {
	      fc = ofc[i].fc;
	      fc->bppt = pt;
	    }
	  }
	} else done = SMD_ON;
      }
    } else if (mode == PPD_VERTEX) {
      
      sscanf(buf, "%s%s%s%s", val[0], val[1], val[2], val[3]);
      vt = ovt[id].vt;
      ++id;
      vt->vec.x = atof(val[1]);
      vt->vec.y = atof(val[2]);
      vt->vec.z = atof(val[3]);
      
    } else if (mode == PPD_NORMAL) {
			
      sscanf(buf, "%s%s%s%s", val[0], val[1], val[2], val[3]);
      nm = onm[id].nm; ++id;
      nm->vec.x = atof(val[1]);
      nm->vec.y = atof(val[2]);
      nm->vec.z = atof(val[3]);
			
    } else if (mode == PPD_FACE) {

      bpt = buf;
      bpt = cut(bpt, stc);
      fcmode = '\0';
      // /n or /u or /c or /h				
      if (stc[0] == '/' && (stc[1] == 'n' || stc[1] == 'u'
			    || stc[1] == 'c' || stc[1] == 'h')) {
	
	fcmode = stc[1];
      }
      n = 0;
      done = SMD_OFF;
      while (!done) {
	bpt = cut(bpt, stc);
	if (!bpt) {
	  if (strcmp(stc, "\\")) {
	    ibuf[n++] = atoi(stc);
	  }
	  done = SMD_ON;
	} else {
	  ibuf[n++] = atoi(stc);
	}
      }
      if (fcmode == '\0') {
				
	if ( n != TRIANGLE ) {
	  fprintf(stdout, "Warning: vertex number of Face No.%d is not a triangle.\n",
		  id+1 );
	}
	fc = ofc[id].fc; ++id;
//  	fprintf(stdout, "\t%d\t", id );
	for (i = 0; i < n; ++i) {
	  he = create_ppdhalfedge( fc );
	  he->vt = ovt[ibuf[i] - 1].vt;
	  //he->vt = ovt[ibuf[i]].vt;
//  	  fprintf(stdout, "%d ", ibuf[i]+1 );
	  // for evaluation
//  	  ++( he->vt->fn );
	}
//  	fprintf(stdout,"\n");
				
	// BEGIN: make Edge 
	he = fc->sphe;
	do {
					
	  sv = he->vt;
	  ev = he->nxt->vt;
	  
	  // vertex -> halfedge
	  // $B!"!V!"%M!"%L%+%e%&%r%H%3%J%?!"%[!"%1!"%k!"%K!"%[!&%^!#%7!&%f!&%#!&%F!&%/!"JC1$K%"%/%;%9$G$-$k$h$&$(B	  // $B:FG[Ns$9$k(B
	  if ( sv->sphe == (Sphe *) NULL ) {
	    sv->sphe = he;
	  }
					
	  if ( (ed = find_ppdedge(sv, ev)) == (Sped *) NULL ) {
	    // create an edge
	    ed = create_ppdedge( ppd );
	    // edge - vertex
	    ed->sv = sv;
	    ed->ev = ev;
	    // vertex - edge
	    (void) create_vtxed( sv, ed );
	    (void) create_vtxed( ev, ed );
	    // for evaluation
// 	    ++( sv->en );
// 	    ++( ev->en );
	    // edge - halfedge
	    he->ed = ed;
	    ed->lhe = he;
						
	  } else {
	    // an edge have already existed
	    he->ed = ed;
	    if ( (ed->sv == sv) && (ed->ev == ev) ) {
	      if ( ed->lhe != (Sphe *) NULL ) {
		fprintf(stdout, "Warning: Edge %d has more than three halfedges. \n",
			ed->no );
	      }
	      ed->lhe  = he;
	      // set mate
	      if ( ed->rhe != NULL ) {
		ed->rhe->mate = he;
		he->mate = ed->rhe;
	      }

	    } else {
							
	      if ( ed->rhe != (Sphe *) NULL ) {
		fprintf(stdout, "Warning: Edge %d has more than three halfedges. \n",
			ed->no);
	      }
	      ed->rhe  = he;
	      // set mate
	      if ( ed->lhe != NULL ) {
		ed->lhe->mate = he;
		he->mate = ed->lhe;
	      }
	    }
						
	  }
					
	} while ((he = he->nxt) != fc->sphe);
	
	// END: make Edge
				
	// normal
	calc_fnorm( fc );

      } else if (fcmode == 'n') {
	i = 0;
	he = fc->sphe;
	do {
	  he->nm = onm[ibuf[i++] - 1].nm;
	} while ((he = he->nxt) != fc->sphe);
      } else if (fcmode == 'u') {
      } else if (fcmode == 'c') {
      } else if (fcmode == 'h') {
      }
      
    } else if (mode == PPD_NONE) {
      if (!strcmp(key, "header")) {
	mode = PPD_HEADER;
      } else if (!strcmp(key, "solid")) {
	mode = PPD_SOLID;
	id = 0;
      } else if (!strcmp(key, "part")) {
	mode = PPD_PART;
	id = 0;
      } else if (!strcmp(key, "face")) {
	mode = PPD_FACE;
	id = 0;
      } else if (!strcmp(key, "vertex")) {
	mode = PPD_VERTEX;
	id = 0;
      } else if (!strcmp(key, "normal")) {
	mode = PPD_NORMAL;
	id = 0;
      }
    }
  }
	
  fclose( fp );
	
  free_oppd( oppd );

  //
  // isolated vertices check.
  free_ppdvertex_noedge( ppd );
  //ppdvertex_noedge_check( ppd );

  //
  // free vertex - edge links 
  FreePPDVertexEdge( ppd );
  
  //
  // needed for calculating shortest paths 
  //normalize_ppd( ppd );
	
  //
  // set boundary attributes
  calc_ppd_boundary( ppd );
	
  //
  // re-attaching ppdvertex to halfedge pointer.
  // This function must be executed with ``calc_ppd_boundary''.
  reattach_ppdvertex_halfedge( ppd );
	
  // 
  // calc edge length (for shortest path)
//    edge_length( ppd );

//   for ( vt = ppd->spvt; vt != (Spvt *) NULL; vt = vt->nxt ) {

//     display("vt %d atr %d\n", vt->no, vt->isBoundary );
//     // edge count
//     int ecount = 0;
//     ed = ppdvertex_first_edge( vt );
//     display("\t ed ");
//     do {
//       display("%d ", ed->no );
//       ++ecount;
//       ed = ppdvertex_next_edge( ed, vt );
//     } while ( (ed != ppdvertex_first_edge( vt )) && (ed != NULL) );
//     display("\n");

//     int fcount = 0;
//     Sphe *he = vt->sphe;
//     display("\t fc ");
//     do {
//       display("%d ", he->bpfc->no );
//       ++fcount;
//       he = ppdvertex_ccwnext_halfedge( he );
//     } while ( (he != vt->sphe) && (he != NULL) );
//     display("\n");
// //     display("vt %d atr %d en %d ecount %d fn %d fcount %d\n",
// // 	    vt->no, vt->atr, vt->en, ecount, vt->fn, fcount );
//   }
  
  return ppd;
}

