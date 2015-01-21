#if !defined(AFX_MATERIALDIALOG_H__0F8F5B21_9FFB_11D3_98D2_0000F45A89CD__INCLUDED_)
#define AFX_MATERIALDIALOG_H__0F8F5B21_9FFB_11D3_98D2_0000F45A89CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaterialDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMaterialDialog �_�C�A���O

class CMaterialDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CMaterialDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMaterialDialog)
	enum { IDD = IDD_MATERIAL_DIALOG };
	CListBox	m_material_list;
	BOOL	m_matl_ppd;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMaterialDialog)
	public:
	virtual BOOL Create();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMaterialDialog)
	afx_msg void OnMatlPpdCheck();
	afx_msg void OnDblclkMaterialList();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MATERIALDIALOG_H__0F8F5B21_9FFB_11D3_98D2_0000F45A89CD__INCLUDED_)
