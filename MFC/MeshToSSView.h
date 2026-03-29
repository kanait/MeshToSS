//
// MeshToSSView.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGLVIEW_H__11D0ECDC_2844_11D2_AB51_00E029217E15__INCLUDED_)
#define AFX_OPENGLVIEW_H__11D0ECDC_2844_11D2_AB51_00E029217E15__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMeshToSSView : public CView
{
protected: // Create from serialization only.
	CMeshToSSView();
	DECLARE_DYNCREATE(CMeshToSSView)

// Attributes
public:
	CMeshToSSDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generates virtual function overrides.
	//{{AFX_VIRTUAL(CMeshToSSView)
	public:
	virtual void OnDraw(CDC* pDC);  // Override to draw this view.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMeshToSSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMeshToSSView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL SetupPixelFormat( void );

private:
	void SetError( int e );
	BOOL InitializeOpenGL();

	HGLRC m_hRC;
	CDC *m_pDC;

	static const char* const _ErrorStrings[];
	const char* m_ErrorString;
	
	int scrn_x, scrn_y; 
	BOOL moving, scaling, rotating;
};

#ifndef _DEBUG  // OpenGLView.cpp (debug build).
inline CMeshToSSDoc* CMeshToSSView::GetDocument()
   { return (CMeshToSSDoc*)m_pDocument; }
#endif

// CMyView

//class CMyView : public CMeshToSSView
//{
//private:
//
//	DECLARE_DYNCREATE(CMyView)
//
//protected:
//
//	virtual void RenderStockScene( void );
//	virtual BOOL RenderScene( void );
//	
//};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio inserts additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLVIEW_H__11D0ECDC_2844_11D2_AB51_00E029217E15__INCLUDED_)
