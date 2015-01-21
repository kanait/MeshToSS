#if !defined(AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_)
#define AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConvSubdiv.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CConvSubdiv �_�C�A���O

class CConvSubdiv : public CDialog
{
// �R���X�g���N�V����
public:
	CConvSubdiv(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CConvSubdiv)
	enum { IDD = IDD_CONVERT_SUBDIV };
	CEdit	m_simp_l2norm;
	CEdit	m_cur_l2norm;
	CEdit	m_simp_nvertex;
	CEdit	m_cur_nvertex;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CConvSubdiv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

 public:
	virtual BOOL Create( void );
	
// �C���v�������e�[�V����
 protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_)
