//
// oppd.cxx
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

// #include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <stdlib.h>

#include "smd.h"

#include "oppd.h"

/***************************************
  Open Ppd Functions
****************************************/

Oppd *create_oppd(void)
{
  Oppd *oppd;
  
  oppd = (Oppd *) malloc(sizeof(Oppd));

  oppd->oso = (Oso *) NULL;
  oppd->opt = (Opt *) NULL;
  oppd->onm = (Onm *) NULL;
  oppd->ovt = (Ovt *) NULL;
  oppd->ofc = (Ofc *) NULL;

  return oppd;
}
  
void free_oppd( Oppd *oppd )
{
  if ( oppd == NULL ) return;
  
  if ( oppd->oso ) free(oppd->oso);
  if ( oppd->opt ) free(oppd->opt);
  if ( oppd->onm ) free(oppd->onm);
  if ( oppd->ovt ) free(oppd->ovt);
  if ( oppd->ofc ) free(oppd->ofc);

  free(oppd);
}
  
Oso *create_osolid( int num )
{
  Oso *oso;
  
  oso = (Oso *) malloc(num * sizeof(Oso));
  
  return oso;
}

Opt *create_opart( int num )
{
  Opt *opt;
  
  opt = (Opt *) malloc(num * sizeof(Opt));

  return opt;
}

Ofc *create_oface( int num )
{
  Ofc *ofc;
  
  ofc = (Ofc *) malloc(num * sizeof(Ofc));

  return ofc;
}

Onm *create_onormal( int num )
{
  Onm *onm;
  
  onm = (Onm *) malloc(num * sizeof(Onm));

  return onm;
}

Ovt *create_overtex( int num )
{
  Ovt *ovt;
  
  ovt = (Ovt *) malloc(num * sizeof(Ovt));

  return ovt;
}

