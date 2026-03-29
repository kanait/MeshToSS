//
// MeshInfoDialog.cpp
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#include "stdafx.h"

#include "../optmesh/smd.h"

#include "screen.h"
#include "MeshToSS.h"

#include "MeshInfoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMeshInfoDialog *midlg = NULL;

/////////////////////////////////////////////////////////////////////////////
// CMeshInfoDialog dialog


CMeshInfoDialog::CMeshInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMeshInfoDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(CMeshInfoDialog)
  //}}AFX_DATA_INIT

}


void CMeshInfoDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMeshInfoDialog)
  DDX_Control(pDX, IDC_MESH_INFO_EDIT, m_mesh_info);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMeshInfoDialog, CDialog)
	//{{AFX_MSG_MAP(CMeshInfoDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshInfoDialog message handlers

BOOL CMeshInfoDialog::Create( void )
{
  // TODO: Add specialized code here and/or call the base class.
  return CDialog::Create( CMeshInfoDialog::IDD );
}

BOOL CMeshInfoDialog::OnInitDialog() 
{
  CDialog::OnInitDialog();
	
  // TODO: Add extra initialization here

  Sppd *ppd = swin->screenatr.current_ppd;
  CString string;
  string.Format(_T("ppd\n\nvertices:\t%d\nfaces:\t%d\nedges:\t%d\n"),
		ppd->vn, ppd->fn, ppd->en); 
  m_mesh_info.FmtLines( TRUE );
  m_mesh_info.SetWindowText( string );
  

  return TRUE;
  // Return TRUE unless you set the focus to a control
  // NOTE: OCX property pages should return FALSE.
}

void CMeshInfoDialog::PostNcDestroy() 
{
  // TODO: Add specialized code here and/or call the base class.
	
  //CDialog::PostNcDestroy();

  midlg = NULL;
  delete this;
}

void CMeshInfoDialog::OnOK() 
{
  // TODO: Add validation code here.
	
  //CDialog::OnOK();
  DestroyWindow();
}

