// Shared global `params` and params_init for GUI (meshtoss.cxx is not linked).

#include <cmath>

#include "../optmesh/smd.h"
#include "../optmesh/params.h"
#include "../optmesh/qem.h"
#include "../optmesh/simpmesh.h"

Params params;

void params_init(Params *p)
{
  p->nfile = 0;
  p->subdivision = EVAL_VERTEX;
  p->vnum = 0;
  p->num_specified = FALSE;
  p->pss_output = FALSE;
  p->save_files = FALSE;
  p->print = FALSE;
  p->pre_optimize = FALSE;

  p->discon_penalty = 1000.0;
  double tol_angle = 160.0;
  p->angle_tol = cos(tol_angle * SMDPI / 180.0);
  p->shape_tol = 0.01;

  p->qemmode = MODE_VQEM;
  p->gamma = 1.0;
}
