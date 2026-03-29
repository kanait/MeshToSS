//
// MeshToSSDoc.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGLDOC_H__11D0ECDA_2844_11D2_AB51_00E029217E15__INCLUDED_)
#define AFX_OPENGLDOC_H__11D0ECDA_2844_11D2_AB51_00E029217E15__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CMeshToSSDoc : public CDocument
{
protected: // Create from serialization only.
	CMeshToSSDoc();
	DECLARE_DYNCREATE(CMeshToSSDoc)

// Attributes
public:
	CRectTracker m_tracker;
	BOOL m_bAllowInvert;

// Operations
public:

// Overrides
	// ClassWizard generates virtual function overrides.
	//{{AFX_VIRTUAL(CMeshToSSDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMeshToSSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMeshToSSDoc)
		// NOTE: ClassWizard adds or removes member functions here.
		//        Do not edit code generated in this section.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLDOC_H__11D0ECDA_2844_11D2_AB51_00E029217E15__INCLUDED_)
