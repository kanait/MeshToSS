// MeshInfoDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"

#include "../optmesh/smd.h"

#include "screen.h"
#include "MeshToSS.h"

#include "MeshInfoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMeshInfoDialog *midlg;

/////////////////////////////////////////////////////////////////////////////
// CMeshInfoDialog �_�C�A���O


CMeshInfoDialog::CMeshInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMeshInfoDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(CMeshInfoDialog)
  //}}AFX_DATA_INIT

}


void CMeshInfoDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CMeshInfoDialog)
  DDX_Control(pDX, IDC_MESH_INFO_EDIT, m_mesh_info);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMeshInfoDialog, CDialog)
	//{{AFX_MSG_MAP(CMeshInfoDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshInfoDialog ���b�Z�[�W �n���h��

BOOL CMeshInfoDialog::Create( void )
{
  // TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
  return CDialog::Create( CMeshInfoDialog::IDD );
}

BOOL CMeshInfoDialog::OnInitDialog() 
{
  CDialog::OnInitDialog();
	
  // TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

  Sppd *ppd = swin->screenatr.current_ppd;
  CString string;
  string.Format("ppd\n\nvertices:\t%d\nfaces:\t%d\nedges:\t%d\n",
		ppd->vn, ppd->fn, ppd->en); 
  m_mesh_info.FmtLines( TRUE );
  m_mesh_info.SetWindowText( string );
  

  return TRUE;
  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
  // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CMeshInfoDialog::PostNcDestroy() 
{
  // TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
  //CDialog::PostNcDestroy();

  midlg = NULL;
  delete this;
}

void CMeshInfoDialog::OnOK() 
{
  // TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	
  //CDialog::OnOK();
  DestroyWindow();
}

