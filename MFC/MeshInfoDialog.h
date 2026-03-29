//
// MeshInfoDialog.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#if !defined(AFX_MESHINFODIALOG_H__DD826CC0_BDCA_11D3_98FC_0000F45A89CD__INCLUDED_)
#define AFX_MESHINFODIALOG_H__DD826CC0_BDCA_11D3_98FC_0000F45A89CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMeshInfoDialog dialog

class CMeshInfoDialog : public CDialog
{
// Construction
public:
	CMeshInfoDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog data
	//{{AFX_DATA(CMeshInfoDialog)
	enum { IDD = IDD_MESHINFO };
	CEdit	m_mesh_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generates virtual function overrides.
	//{{AFX_VIRTUAL(CMeshInfoDialog)
	public:
	virtual BOOL Create( void );
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeshInfoDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHINFODIALOG_H__DD826CC0_BDCA_11D3_98FC_0000F45A89CD__INCLUDED_)
