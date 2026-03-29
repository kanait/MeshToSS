//
// SRDialog.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#if !defined(AFX_SRDIALOG_H__A20A96A2_496A_11D2_AB68_00E029217E15__INCLUDED_)
#define AFX_SRDIALOG_H__A20A96A2_496A_11D2_AB68_00E029217E15__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SRDialog.h : header
//

/////////////////////////////////////////////////////////////////////////////
// CSRDialog dialog

class CSRDialog : public CDialog
{
// Construction
public:
	CSRDialog(CWnd* pParent = NULL);   // standard constructor
	BOOL Create( void );

// Dialog data
	//{{AFX_DATA(CSRDialog)
	enum { IDD = IDD_SET_RADIUS };
	double	m_radius_cyl;
	double	m_radius_sph;
	//}}AFX_DATA


// Overrides
	// ClassWizard generates virtual function overrides.

	//{{AFX_VIRTUAL(CSRDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSRDialog)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_SRDIALOG_H__A20A96A2_496A_11D2_AB68_00E029217E15__INCLUDED_)
