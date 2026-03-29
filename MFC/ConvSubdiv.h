//
// ConvSubdiv.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#if !defined(AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_)
#define AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CConvSubdiv dialog

class CConvSubdiv : public CDialog
{
// Construction
public:
	CConvSubdiv(CWnd* pParent = NULL);   // standard constructor

// Dialog data
	//{{AFX_DATA(CConvSubdiv)
	enum { IDD = IDD_CONVERT_SUBDIV };
	CEdit	m_simp_l2norm;
	CEdit	m_cur_l2norm;
	CEdit	m_simp_nvertex;
	CEdit	m_cur_nvertex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generates virtual function overrides.
	//{{AFX_VIRTUAL(CConvSubdiv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

 public:
	virtual BOOL Create( void );
	
// Implementation
 protected:
	// Generated message map functions
	//{{AFX_MSG(CConvSubdiv)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnL2NormOk();
	afx_msg void OnL2NormCancel();
	afx_msg void OnConvReset();
	afx_msg void OnRadioDis();
	afx_msg void OnRadioNumvert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_)
