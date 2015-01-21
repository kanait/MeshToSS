// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__11D0ECD8_2844_11D2_AB51_00E029217E15__INCLUDED_)
#define AFX_MAINFRM_H__11D0ECD8_2844_11D2_AB51_00E029217E15__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Instead of using a class here we could have just put these controls
// directly into CMainFrame.  As it is they are sending messages to the
// main frame just like they were part of it instead of part of a control
// bar.
class CStyleBar : public CToolBar
{
public:
	CComboBox   m_comboBox;
	CFont       m_font;
};

//class CMySplitterWnd : public CSplitterWnd {
//
//private:
//
//	DECLARE_DYNCREATE(CSplitterWnd)
//
//protected:
//
//	virtual void OnLButtonDown(UINT nFlags, CPoint point);
//
//};

	
class CMainFrame : public CFrameWnd
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// �A�g���r���[�g
public:

//	CMySplitterWnd m_wndSplitter;
//	CSplitterWnd m_wndSplitter;

// �I�y���[�V����
public:
	void drawwindow( void );

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL CreateStyleBar();

protected:  // �R���g���[�� �o�[�p�����o
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
// temp
//	CToolBar    m_wndEditBar;
	CStyleBar   m_wndStyleBar;

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateMenuScreenCoaxis(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuScreenCv(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuScreenGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuScreenHmap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuScreenPcm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuScreenShading(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuScreenWireframe(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenShading();
	afx_msg void OnMenuScreenWireframe();
	afx_msg void OnMenuScreenCoaxis();
	afx_msg void OnMenuScreenVert();
	afx_msg void OnUpdateMenuScreenVert(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveSrc();
	afx_msg void OnMorphReset();
	afx_msg void OnMorphSetDiv();
	afx_msg void OnMenuScreenCv();
	afx_msg void OnMenuScreenGroup();
	afx_msg void OnMenuScreenHmap();
	afx_msg void OnMenuScreenPcm();
	afx_msg void OnUpdateMenuScreenEnhanced(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenEnhanced();
	afx_msg void OnMenuScreenSmooth();
	afx_msg void OnUpdateMenuScreenSmooth(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenInterp();
	afx_msg void OnUpdateMenuScreenInterp(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenRadius();
	afx_msg void OnViewBackgroundColor();
	afx_msg void OnMenuScreenVertid();
	afx_msg void OnUpdateMenuScreenVertid(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenEdgeid();
	afx_msg void OnUpdateMenuScreenEdgeid(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenVw();
	afx_msg void OnFileSaveVw();
	afx_msg void OnViewMaterial();
	afx_msg void OnMeshInfo();
	afx_msg void OnViewGradient();
	afx_msg void OnUpdateViewGradient(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenLight0();
	afx_msg void OnUpdateMenuScreenLight0(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenLight1();
	afx_msg void OnUpdateMenuScreenLight1(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenLight2();
	afx_msg void OnUpdateMenuScreenLight2(CCmdUI* pCmdUI);
	afx_msg void OnMenuScreenLight3();
	afx_msg void OnUpdateMenuScreenLight3(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenSmf();
	afx_msg void OnFileSaveSmf();
	afx_msg void OnMenuScreenHidden();
	afx_msg void OnUpdateMenuScreenHidden(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenVrml();
	afx_msg void OnMenuEditSubdivUp();
	afx_msg void OnMenuEditSubdivDown();
	afx_msg void OnMenuEditConv();
	afx_msg void OnMenuScreenSubbown();
	afx_msg void OnUpdateMenuScreenSubbown(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveVrmlSub();
	afx_msg void OnFileSaveVrmlPoly();
	afx_msg void OnMenuScreenOrgppd();
	afx_msg void OnUpdateMenuScreenOrgppd(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenEvrmlPoly();
	afx_msg void OnFileOpenEvrmlSs();
	afx_msg void OnFileSavePpdPoly();
	afx_msg void OnFileSaveSmfPoly();
	//}}AFX_MSG

	afx_msg void OnUpdateStyle(CCmdUI* pCmdUI);
	afx_msg void OnChangeStyle(UINT nID);
//	afx_msg void OnChangeStyleCombo(UINT nID);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MAINFRM_H__11D0ECD8_2844_11D2_AB51_00E029217E15__INCLUDED_)
