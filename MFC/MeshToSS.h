//
// MeshToSS.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#if !defined(AFX_OPENGL_H__11D0ECD4_2844_11D2_AB51_00E029217E15__INCLUDED_)
#define AFX_OPENGL_H__11D0ECD4_2844_11D2_AB51_00E029217E15__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSApp:
// See OpenGL.cpp for the behavior of this class.
//

class CMeshToSSApp : public CWinApp
{
public:
	CMeshToSSApp();

// Overrides
	// ClassWizard generates virtual function overrides.
	//{{AFX_VIRTUAL(CMeshToSSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMeshToSSApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGL_H__11D0ECD4_2844_11D2_AB51_00E029217E15__INCLUDED_)
