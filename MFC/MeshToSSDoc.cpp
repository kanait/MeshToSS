//
// MeshToSSDoc.cpp
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
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
		// NOTE: ClassWizard adds or removes mapping macros here.
		//        Do not edit code generated in this section.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSDoc construction/destruction

CMeshToSSDoc::CMeshToSSDoc()
{
	// TODO: Add one-time construction code here.

}

CMeshToSSDoc::~CMeshToSSDoc()
{
}

BOOL CMeshToSSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

// Where to set the document title
// **App::InitInstance is usually a better place.
//	CString strTitle = _T("Gmorph Ver2.1a1 (Win32 Version)");
//	SetTitle(strTitle);  // CDocument member  // TODO: Add reinitialization code here.
	// (SDI reuses this document.)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMeshToSSDoc serialization

void CMeshToSSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: Add saving code here.
	}
	else
	{
		// TODO: Add loading code here.
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSDoc diagnostics

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
// CMeshToSSDoc commands
