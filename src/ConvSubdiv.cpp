//
// ConvSubdiv.cpp
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#include "StdAfx.h"
#include "MeshToSS.h"
#include "ConvSubdiv.h"

#include "../optmesh/smd.h"
#include "../optmesh/ppd.h"
#include "../optmesh/file.h"
#include "screen.h"
#include "mtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CConvSubdiv *csdlg;

/////////////////////////////////////////////////////////////////////////////
// CConvSubdiv �_�C�A���O


CConvSubdiv::CConvSubdiv(CWnd* pParent /*=NULL*/)
  : CDialog(CConvSubdiv::IDD, pParent)
{
  //{{AFX_DATA_INIT(CConvSubdiv)
	//}}AFX_DATA_INIT
}

void CConvSubdiv::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CConvSubdiv)
  DDX_Control(pDX, IDC_CONV_SIMP_DIS, m_simp_l2norm);
  DDX_Control(pDX, IDC_CONV_CUR_DIS, m_cur_l2norm);
  DDX_Control(pDX, IDC_CONV_SIMP_VNUM, m_simp_nvertex);
  DDX_Control(pDX, IDC_CONV_CUR_VNUM, m_cur_nvertex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConvSubdiv, CDialog)
  //{{AFX_MSG_MAP(CConvSubdiv)
  ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDOK2, OnL2NormOk)
	ON_BN_CLICKED(IDCANCEL2, OnL2NormCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnConvReset)
	ON_BN_CLICKED(IDC_RADIO_DIS, OnRadioDis)
	ON_BN_CLICKED(IDC_RADIO_NUMVERT, OnRadioNumvert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConvSubdiv ���b�Z�[�W �n���h��

BOOL CConvSubdiv::Create( void )
{
  // TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
  return CDialog::Create( CConvSubdiv::IDD );
}

BOOL CConvSubdiv::OnInitDialog() 
{
  CDialog::OnInitDialog();
	
  // TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

  // conv type 
  swin->conv_type = CONV_NONE;
  
  CString string;
  // n_vert
  if ( swin->screenatr.org_ppd != NULL )
    string.Format( _T("%d"), swin->screenatr.org_ppd->vn );
  else
    string.Format( _T("%d"), swin->screenatr.current_ppd->vn );
  m_cur_nvertex.SetWindowText( string );
  m_simp_nvertex.SetWindowText( string );

  // dis
  if ( swin->screenatr.org_ppd != NULL )
    string.Format( _T("%d"), swin->screenatr.org_ppd->dis );
  else
    string.Format( _T("%d"), swin->screenatr.current_ppd->dis );
  m_cur_l2norm.SetWindowText( string );
  m_simp_l2norm.SetWindowText( string );
  
  return TRUE;
  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
  // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CConvSubdiv::PostNcDestroy() 
{
  // TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
  //CDialog::PostNcDestroy();

  csdlg = NULL;
  delete this;
  
}

#include "../optmesh/params.h"
#include "../optmesh/simpmesh.h"
#include "../optmesh/qem.h"

void CConvSubdiv::OnOK() 
{
  if ( swin->conv_type != CONV_NUMV ) return;
  
  // TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
  //CDialog::OnOK();
  CString string;
  m_simp_nvertex.GetWindowText( string );
  LPTSTR lpszbuffer= string.GetBuffer( BUFSIZ );
  int nvertex = _tstoi( lpszbuffer );

#if 0
  char str[BUFSIZ];
  sprintf(str, "%d", nvertex );
  AfxMessageBox( (LPTSTR) str );
#endif  

  // Simplification
  params_init( &params );
  
  //params.num_specified = TRUE;
  params.vnum = nvertex;
  
//    params.qemmode = MODE_EQEM;
  params.subdivision = EVAL_2SUB;
//    params.subdivision = EVAL_VERTEX;
  
  Sppd* ppd = swin->screenatr.current_ppd;

  // �R�s�[
  swin->screenatr.org_ppd = copy_ppd( ppd );
  copy_material( &(swin->screenatr.org_ppd->matl), 0, (float *) mtlall );
  
  simpmesh( ppd, nvertex );

  AfxMessageBox( _T("Conversion successfully finished."));

  string.Format( _T("%g"), ppd->err );
  m_simp_l2norm.SetWindowText( string );
  
  ppd->file_type = FILE_EVRML_SUBDIV;
  GetTopLevelParent()->RedrawWindow();
}

void CConvSubdiv::OnCancel() 
{
  if ( swin->conv_type != CONV_NUMV ) return;
  
  // TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
  //CDialog::OnCancel();
  CString string;
  string.Format((LPCTSTR)"");
  m_simp_nvertex.SetWindowText( string );
}

void CConvSubdiv::OnClose() 
{
  // TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
  DestroyWindow();
}

void CConvSubdiv::OnL2NormOk() 
{
  if ( swin->conv_type != CONV_DIS ) return;
  // TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
  CString string;
  m_simp_l2norm.GetWindowText( string );
  LPTSTR lpszbuffer= string.GetBuffer( BUFSIZ );
  double l2norm = _tstof( lpszbuffer );

  // Simplification
  params_init( &params );
  
  //params.num_specified = TRUE;
//    params.vnum = l2norm;
//    params.qemmode = MODE_EQEM;
  
  params.qemmode = MODE_VQEM;
  params.subdivision = EVAL_2SUB;
//    params.subdivision = EVAL_VERTEX;
  
  Sppd* ppd = swin->screenatr.current_ppd;

  // �R�s�[
  swin->screenatr.org_ppd = copy_ppd( ppd );
  copy_material( &(swin->screenatr.org_ppd->matl), 0, (float *) mtlall );
  
//    simpmesh( ppd, nvertex );
  simpmesh_l2norm( ppd, l2norm );

  AfxMessageBox( (LPTSTR)"Conversion successfully finished.");

  string.Format( (LPCTSTR)"%d", ppd->vn );
  m_simp_nvertex.SetWindowText( string );
  
  ppd->file_type = FILE_EVRML_SUBDIV;
  
  GetTopLevelParent()->RedrawWindow();
}

void CConvSubdiv::OnL2NormCancel() 
{
  if ( swin->conv_type != CONV_DIS ) return;
  // TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
  CString string;
  string.Format((LPCTSTR)"");
  m_simp_l2norm.SetWindowText( string );
}

void CConvSubdiv::OnConvReset()
{
  // TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
  if ( swin->screenatr.org_ppd == NULL ) return;

  // org_ppd �� current_ppd �ɂ���
  free_ppd( swin->screenatr.current_ppd );
  swin->screenatr.current_ppd = swin->screenatr.org_ppd;
  swin->screenatr.view_ppd = swin->screenatr.org_ppd;
  swin->screenatr.org_ppd = NULL;

  // �e�L�X�g�t�H�[���̃��Z�b�g
  CString string;
  string.Format( _T("%d"), swin->screenatr.current_ppd->vn );
  m_cur_nvertex.SetWindowText( string );
  m_simp_nvertex.SetWindowText( string );

  string.Format(_T("0"));
  m_cur_l2norm.SetWindowText( string );
  m_simp_l2norm.SetWindowText( string );
  
  GetTopLevelParent()->RedrawWindow();
}

void CConvSubdiv::OnRadioDis()
{
  // TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
  swin->conv_type = CONV_DIS;
}

void CConvSubdiv::OnRadioNumvert()
{
  // TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
  swin->conv_type = CONV_NUMV;
}
