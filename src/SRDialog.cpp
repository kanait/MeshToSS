// SRDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../optmesh/smd.h"

#include "screen.h"

#include "MeshToSS.h"
#include "SRDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CSRDialog �_�C�A���O


CSRDialog::CSRDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSRDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSRDialog)
  //}}AFX_DATA_INIT
}

BOOL CSRDialog::Create( void )
{
  return CDialog::Create( CSRDialog::IDD );
}

BOOL CSRDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  // TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
  ScreenAtr *screen = &(swin->screenatr);
  m_radius_cyl = screen->rad_cyl;
  m_radius_sph = screen->rad_sph;
  
  return TRUE;
  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
  // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CSRDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CSRDialog)
  DDX_Text(pDX, IDC_RADIUS_CYL, m_radius_cyl);
  DDX_Text(pDX, IDC_RADIUS_SPH, m_radius_sph);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSRDialog, CDialog)
  //{{AFX_MSG_MAP(CSRDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSRDialog ���b�Z�[�W �n���h��

void CSRDialog::OnOK() 
{
  // TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	
  //	CDialog::OnOK();
  UpdateData(TRUE);

  ScreenAtr *screen;
  screen = &(swin->screenatr);
  screen->wnd = GetTopLevelParent();
  screen->rad_sph = m_radius_sph;
  screen->rad_cyl = m_radius_cyl;
  screen->wnd->RedrawWindow();

}

void CSRDialog::OnCancel() 
{
  // TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
  //	CDialog::OnCancel();
  DestroyWindow();
}

void CSRDialog::PostNcDestroy() 
{
  // TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
  //	CDialog::PostNcDestroy();
  delete this;
}

