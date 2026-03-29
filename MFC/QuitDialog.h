//
// QuitDialog.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#if !defined(AFX_QUITDIALOG_H__AC3B06D0_3282_11D2_AB58_00E029217E15__INCLUDED_)
#define AFX_QUITDIALOG_H__AC3B06D0_3282_11D2_AB58_00E029217E15__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CQuitDialog dialog

class CQuitDialog : public CDialog
{
// Construction
public:
	CQuitDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog data
	//{{AFX_DATA(CQuitDialog)
	enum { IDD = IDD_QUIT_DIALOG };
		// NOTE: ClassWizard adds data members here.
	//}}AFX_DATA


// Overrides
	// ClassWizard generates virtual function overrides.

	//{{AFX_VIRTUAL(CQuitDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuitDialog)
		// NOTE: ClassWizard adds member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_QUITDIALOG_H__AC3B06D0_3282_11D2_AB58_00E029217E15__INCLUDED_)
