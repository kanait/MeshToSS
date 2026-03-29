//
// SRDialog.cpp
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

#include "screen.h"

#include "MeshToSS.h"
#include "SRDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CSRDialog dialog


CSRDialog::CSRDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSRDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSRDialog)
  //}}AFX_DATA_INIT
}

BOOL CSRDialog::Create( void )
{
  return CDialog::Create( CSRDialog::IDD );
}

BOOL CSRDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  // TODO: Add extra initialization here
  ScreenAtr *screen = &(swin->screenatr);
  m_radius_cyl = screen->rad_cyl;
  m_radius_sph = screen->rad_sph;
  
  return TRUE;
  // Return TRUE unless you set the focus to a control
  // NOTE: OCX property pages should return FALSE.
}

void CSRDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CSRDialog)
  DDX_Text(pDX, IDC_RADIUS_CYL, m_radius_cyl);
  DDX_Text(pDX, IDC_RADIUS_SPH, m_radius_sph);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSRDialog, CDialog)
  //{{AFX_MSG_MAP(CSRDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRDialog message handlers

void CSRDialog::OnOK() 
{
  // TODO: Add validation code here.
	
  //	CDialog::OnOK();
  UpdateData(TRUE);

  ScreenAtr *screen;
  screen = &(swin->screenatr);
  screen->wnd = GetTopLevelParent();
  screen->rad_sph = m_radius_sph;
  screen->rad_cyl = m_radius_cyl;
  screen->wnd->RedrawWindow();

}

void CSRDialog::OnCancel() 
{
  // TODO: Add special cleanup code here.
	
  //	CDialog::OnCancel();
  DestroyWindow();
}

void CSRDialog::PostNcDestroy() 
{
  // TODO: Add specialized code here and/or call the base class.
	
  //	CDialog::PostNcDestroy();
  delete this;
}

