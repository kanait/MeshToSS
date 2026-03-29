//
// MaterialDialog.cpp
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../optmesh/smd.h"
#include "../optmesh/file.h"

#include "MeshToSS.h"
#include "screen.h"
#include "mtl.h"
#include "MaterialDialog.h"

extern CMaterialDialog *md;

/////////////////////////////////////////////////////////////////////////////
// CMaterialDialog dialog

CMaterialDialog::CMaterialDialog(CWnd* pParent /*=NULL*/)
  : CDialog(CMaterialDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(CMaterialDialog)
  m_matl_ppd = FALSE;
  //}}AFX_DATA_INIT
}


void CMaterialDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMaterialDialog)
	DDX_Control(pDX, IDC_MATERIAL_LIST, m_material_list);
	DDX_Check(pDX, IDC_MATL_PPD_CHECK, m_matl_ppd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaterialDialog, CDialog)
	//{{AFX_MSG_MAP(CMaterialDialog)
	ON_BN_CLICKED(IDC_MATL_PPD_CHECK, OnMatlPpdCheck)
	ON_LBN_DBLCLK(IDC_MATERIAL_LIST, OnDblclkMaterialList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaterialDialog message handlers

BOOL CMaterialDialog::Create()
{
  // TODO: Add specialized code here and/or call the base class.
  return CDialog::Create( CMaterialDialog::IDD );
}

BOOL CMaterialDialog::OnInitDialog() 
{
  CDialog::OnInitDialog();
	
  // TODO: Add extra initialization here
	
  for ( int i = MTLNUM - 1; i >= 0; --i ) 
    m_material_list.InsertString( 0, (LPCTSTR) mtltxt[i] );

  return TRUE;  // Return TRUE unless you set the focus to a control
  // NOTE: OCX property pages should return FALSE.
}

void CMaterialDialog::OnCancel() 
{
  // TODO: Add special cleanup code here.
  // CDialog::OnCancel();

  DestroyWindow();
}

void CMaterialDialog::PostNcDestroy() 
{
  // TODO: Add specialized code here and/or call the base class.
  //CDialog::PostNcDestroy();
  
  md = NULL;
  delete this;
}

void CMaterialDialog::OnMatlPpdCheck()
{
  // TODO: Add control notification handler code here.
  //display("matl ppd %d\n", m_matl_ppd );
  if ( m_matl_ppd == TRUE ) {
    m_matl_ppd = FALSE;
  } else {
    m_matl_ppd = TRUE;
  }
  //display("matl ppd %d\n", m_matl_ppd );
}

void CMaterialDialog::OnDblclkMaterialList()
{
  // TODO: Add control notification handler code here.
  int sel = m_material_list.GetCurSel();

  if ( m_matl_ppd == TRUE ) {
    Sppd *ppd = swin->screenatr.current_ppd;
    copy_material( &(ppd->matl), sel, (float *) mtlall );
  }

  GetTopLevelParent()->Invalidate();
  
}


