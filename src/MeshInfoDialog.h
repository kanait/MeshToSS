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
// CMeshInfoDialog �_�C�A���O

class CMeshInfoDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CMeshInfoDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMeshInfoDialog)
	enum { IDD = IDD_MESHINFO };
	CEdit	m_mesh_info;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMeshInfoDialog)
	public:
	virtual BOOL Create( void );
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMeshInfoDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MESHINFODIALOG_H__DD826CC0_BDCA_11D3_98FC_0000F45A89CD__INCLUDED_)
