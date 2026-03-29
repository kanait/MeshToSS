//
// QuitDialog.cpp
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#include "stdafx.h"
#include "MeshToSS.h"
#include "QuitDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuitDialog dialog


CQuitDialog::CQuitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQuitDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuitDialog)
		// NOTE: ClassWizard adds or removes mapping macros here.
	//}}AFX_DATA_INIT
}


void CQuitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuitDialog)
		// NOTE: ClassWizard adds or removes mapping macros here.
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuitDialog, CDialog)
	//{{AFX_MSG_MAP(CQuitDialog)
		// NOTE: ClassWizard adds or removes mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuitDialog message handlers
