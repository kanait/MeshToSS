// MeshToSSDoc.cpp : CMeshToSSDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "MeshToSS.h"

#include "MeshToSSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSDoc

IMPLEMENT_DYNCREATE(CMeshToSSDoc, CDocument)

BEGIN_MESSAGE_MAP(CMeshToSSDoc, CDocument)
	//{{AFX_MSG_MAP(CMeshToSSDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSDoc �N���X�̍\�z/����

CMeshToSSDoc::CMeshToSSDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CMeshToSSDoc::~CMeshToSSDoc()
{
}

BOOL CMeshToSSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

// �^�C�g��������ꏊ
// ���������@**App::InitInstance �̕����ǂ��D
//	CString strTitle = _T("Gmorph Ver2.1a1 (Win32 Version)");
//	SetTitle(strTitle);// ��CDocument�N���X�̃����o�֐�	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMeshToSSDoc �V���A���C�[�[�V����

void CMeshToSSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSDoc �N���X�̐f�f

#ifdef _DEBUG
void CMeshToSSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMeshToSSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSDoc �R�}���h
