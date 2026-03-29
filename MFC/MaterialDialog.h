//
// MaterialDialog.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#if !defined(AFX_MATERIALDIALOG_H__0F8F5B21_9FFB_11D3_98D2_0000F45A89CD__INCLUDED_)
#define AFX_MATERIALDIALOG_H__0F8F5B21_9FFB_11D3_98D2_0000F45A89CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMaterialDialog dialog

class CMaterialDialog : public CDialog
{
// Construction
public:
	CMaterialDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog data
	//{{AFX_DATA(CMaterialDialog)
	enum { IDD = IDD_MATERIAL_DIALOG };
	CListBox	m_material_list;
	BOOL	m_matl_ppd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generates virtual function overrides.
	//{{AFX_VIRTUAL(CMaterialDialog)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMaterialDialog)
	afx_msg void OnMatlPpdCheck();
	afx_msg void OnDblclkMaterialList();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_MATERIALDIALOG_H__0F8F5B21_9FFB_11D3_98D2_0000F45A89CD__INCLUDED_)
