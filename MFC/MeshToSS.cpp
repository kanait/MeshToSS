//
// MeshToSS.cpp
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#include "stdafx.h"

#include "../optmesh/smd.h"

#include "screen.h"

#include "MainFrm.h"
#include "MeshToSSDoc.h"
#include "MeshToSSView.h"

#include "MeshToSS.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSApp

BEGIN_MESSAGE_MAP(CMeshToSSApp, CWinApp)
	//{{AFX_MSG_MAP(CMeshToSSApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	//}}AFX_MSG_MAP
	// Standard file-based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSApp construction

CMeshToSSApp::CMeshToSSApp()
{
	// TODO: Add construction code here.
	// Place important InitInstance initialization here.
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMeshToSSApp object

CMeshToSSApp theApp;

Swin *swin;

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSApp initialization

BOOL CMeshToSSApp::InitInstance()
{
	// Standard MFC initialization
	// To reduce executable size, remove any of the following init routines you do not need.

	// Change the registry key below where settings are stored (company or organization string).
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI options (including MRU)

	// Register the document template (binds document, frame window, and view).

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMeshToSSDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMeshToSSView)
		);
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands (DDE, File Open, etc.).
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.
	if (!ProcessShellCommand(cmdInfo)) return FALSE;

	// Show and update the main window.
	CString strTitle = _T("MeshToSS Version 1.1");
	m_pMainWnd ->SetWindowText( strTitle );
	m_pMainWnd->ShowWindow( SW_SHOW );
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for application version information

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generates virtual function overrides.
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMeshToSSApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSApp commands

#include "QuitDialog.h"

void CMeshToSSApp::OnAppExit() 
{
  // TODO: Add command handler code here
  CQuitDialog ced;

  if ( ced.DoModal() == IDOK ) {
    CWinApp::OnAppExit();
  }
}

