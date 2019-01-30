//
// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MeshToSS.h"

#include "MainFrm.h"
#include "MeshToSSDoc.h"
#include "MeshToSSView.h"

#include "../optmesh/smd.h"
#include "../optmesh/ppd.h"
#include "../optmesh/ppdlist.h"
#include "../optmesh/ppdface.h"
#include "../optmesh/ppdnormal.h"
#include "../optmesh/veclib.h"
#include "../optmesh/file.h"
#include "../optmesh/subdiv.h"

#include "screen.h"
#include "swin.h"
#include "vw.h"
#include "smf.h"
#include "mtl.h"
#include "vrml.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_SHADING, OnUpdateMenuScreenShading)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_WIREFRAME, OnUpdateMenuScreenWireframe)
ON_COMMAND(ID_MENU_SCREEN_SHADING, OnMenuScreenShading)
ON_COMMAND(ID_MENU_SCREEN_WIREFRAME, OnMenuScreenWireframe)
ON_COMMAND(ID_FILE_SAVE_SRC, OnFileSaveSrc)
ON_COMMAND(ID_MENU_SCREEN_SMOOTH, OnMenuScreenSmooth)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_SMOOTH, OnUpdateMenuScreenSmooth)
ON_COMMAND(ID_VIEW_BACKGROUND, OnViewBackgroundColor)
ON_COMMAND(ID_FILE_OPEN_VW, OnFileOpenVw)
ON_COMMAND(ID_FILE_SAVE_VW, OnFileSaveVw)
ON_COMMAND(ID_VIEW_MATERIAL, OnViewMaterial)
ON_COMMAND(ID_VIEW_GRADIENT, OnViewGradient)
ON_UPDATE_COMMAND_UI(ID_VIEW_GRADIENT, OnUpdateViewGradient)
ON_COMMAND(ID_MENU_SCREEN_LIGHT0, OnMenuScreenLight0)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_LIGHT0, OnUpdateMenuScreenLight0)
ON_COMMAND(ID_MENU_SCREEN_LIGHT1, OnMenuScreenLight1)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_LIGHT1, OnUpdateMenuScreenLight1)
ON_COMMAND(ID_MENU_SCREEN_LIGHT2, OnMenuScreenLight2)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_LIGHT2, OnUpdateMenuScreenLight2)
ON_COMMAND(ID_MENU_SCREEN_LIGHT3, OnMenuScreenLight3)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_LIGHT3, OnUpdateMenuScreenLight3)
ON_COMMAND(ID_FILE_OPEN_SMF, OnFileOpenSmf)
ON_COMMAND(ID_FILE_SAVE_SMF, OnFileSaveSmf)
ON_COMMAND(ID_MENU_SCREEN_HIDDEN, OnMenuScreenHidden)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_HIDDEN, OnUpdateMenuScreenHidden)
//ON_COMMAND(ID_FILE_OPEN_VRML, OnFileOpenVrml)
ON_COMMAND(ID_MENU_EDIT_SUBDIV_UP, OnMenuEditSubdivUp)
ON_COMMAND(ID_MENU_EDIT_SUBDIV_DOWN, OnMenuEditSubdivDown)
ON_COMMAND(ID_MENU_EDIT_CONV, OnMenuEditConv)
ON_COMMAND(ID_MENU_SCREEN_SUBBOWN, OnMenuScreenSubbown)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_SUBBOWN, OnUpdateMenuScreenSubbown)
//ON_COMMAND(ID_FILE_SAVE_VRML_SUB, OnFileSaveVrmlSub)
//ON_COMMAND(ID_FILE_SAVE_VRML_POLY, OnFileSaveVrmlPoly)
ON_COMMAND(ID_MENU_SCREEN_ORGPPD, OnMenuScreenOrgppd)
ON_UPDATE_COMMAND_UI(ID_MENU_SCREEN_ORGPPD, OnUpdateMenuScreenOrgppd)
//ON_COMMAND(ID_FILE_OPEN_EVRML_POLY, OnFileOpenEvrmlPoly)
//ON_COMMAND(ID_FILE_OPEN_EVRML_SS, OnFileOpenEvrmlSs)
ON_COMMAND(ID_FILE_SAVE_PPD_POLY, OnFileSavePpdPoly)
ON_COMMAND(ID_FILE_SAVE_SMF_POLY, OnFileSaveSmfPoly)
//}}AFX_MSG_MAP

END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // ステータス ライン インジケータ
  ID_INDICATOR_KANA,
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
  // TODO: この位置にメンバの初期化処理コードを追加してください。
  swin = create_swin();
}

CMainFrame::~CMainFrame() 
{
  void free_swin( Swin * );

  free_swin( swin );
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  // StatusBar
  if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
                                    sizeof(indicators)/sizeof(UINT)) )
    {
      TRACE0("Failed to create status bar\n");
      return -1;      // 作成に失敗
    }

  // TODO: もしツール チップスが必要ない場合、ここを削除してください。
  //	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
  //		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  // TODO: ツール バーをドッキング可能にしない場合は以下の３行を削除
  //       してください。
  //	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  //	EnableDocking( CBRS_ALIGN_ANY );
  //	DockControlBar( &m_wndToolBar );

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: この位置で CREATESTRUCT cs を修正して、Window クラスやスタイルを
  //       修正してください。

  // window size
  cs.cx = swin->width;
  cs.cy = swin->height;

  // リサイズ不可
  //	cs.style &= ~WS_MAXIMIZEBOX;
  //	cs.style &= ~WS_THICKFRAME;

  return CFrameWnd::PreCreateWindow( cs );
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

static TCHAR BASED_CODE szppdFilter[] = _T("PPD Files (*.ppd)|*.ppd|All Files (*.*)|*.*||");

//
// open PPD file
//
void CMainFrame::OnFileOpen() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください

  // 第一引数は OPEN のとき TRUE
  // 第二、三引数はファイルの種類
  CFileDialog filedlg( TRUE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szppdFilter, NULL );

  if( filedlg.DoModal() == IDOK ) {
    //		CWaitCursor wait;
    CString filename;
    filename = filedlg.GetPathName();
    //filenameに選択したファイルのフルパスが入る

    TCHAR *lp;
    lp = filename.GetBuffer( BUFSIZ );

    char f[BUFSIZ];
    WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

    Sppd *ppd;
    //if ( (ppd = open_ppd( (char*)lp, TRUE )) == NULL )
    if ( (ppd = open_ppd( f )) == NULL )
      {
        AfxMessageBox( _T("Filename was bad!"));
        return;
      }

    // set material (default)
    copy_material( &(ppd->matl), 0, (float *) mtlall );
		
    if ( swin->screenatr.current_ppd != NULL )
      {
	free_ppd( swin->screenatr.current_ppd );
	swin->screenatr.view_ppd = NULL;
      }

    swin->screenatr.current_ppd = ppd;
    swin->screenatr.view_ppd = ppd;
    //      swin->screenatr.org_ppd = ppd;
			
    // normalization
    // enhanced
    swin->screenatr.rad_sph *= ppd->scale;
    swin->screenatr.rad_cyl *= ppd->scale;

    // for setting subdivision boundary flag
    set_subdiv_boundary( ppd );
    // gamma の決定
    //      swin->screenatr.sp_gamma = GAMMA * ppd->scale;
    
    InvalidateRect( NULL, FALSE );
  }
	
}

//
// save PPD file
//
void CMainFrame::OnFileSaveSrc()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.current_ppd == (Sppd *) NULL ) return;

  CFileDialog filedlg( FALSE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szppdFilter, NULL );

  if( filedlg.DoModal() != IDOK) return;
  
  //		CWaitCursor wait;
  CString filename;
  filename = filedlg.GetPathName();
  //filenameに選択したファイルのフルパスが入る

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

  write_ppd_file( f, swin->screenatr.current_ppd );
  strcpy_s( swin->screenatr.current_ppd->filename,
            _countof(swin->screenatr.current_ppd->filename), f );
}

void CMainFrame::OnFileSavePpdPoly()
{
  if ( swin->screenatr.view_ppd == NULL ) return;
  
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  CFileDialog filedlg( FALSE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szppdFilter, NULL );

  if( filedlg.DoModal() != IDOK) return;
  
  //		CWaitCursor wait;
  CString filename;
  filename = filedlg.GetPathName();
  //filenameに選択したファイルのフルパスが入る

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

  write_ppd_file( f, swin->screenatr.view_ppd );
  strcpy_s( swin->screenatr.view_ppd->filename,
            _countof(swin->screenatr.view_ppd->filename), f );
}

#if 0
static TCHAR BASED_CODE szwrlFilter[] = _T("VRML Files (*.wrl)|*.wrl|All Files (*.*)|*.*||");

void CMainFrame::OnFileOpenVrml()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください

  if ( swin->screenatr.current_ppd != NULL ) {
    AfxMessageBox( _T("A file has already opened."));
    return;
  }
  
  CFileDialog filedlg( TRUE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szwrlFilter, NULL );

  if( filedlg.DoModal() != IDOK ) return;

  // filename に選択したファイルのフルパスが入る
  CString filename = filedlg.GetPathName();

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

  // check VRML
  int file_type;
  if ( (file_type = check_vrml( f )) != FILE_VRML )
    {
      AfxMessageBox( _T("This file is not a VRML file."));
      return;
    }

  Sppd *ppd;
  if ( (ppd = open_vrml( f )) == NULL ) {
    char string[BUFSIZ];
    sprintf( string, "Can't open %s.", f );
    AfxMessageBox( (LPTSTR) (string) );
    return;
  }
  ppd->file_type = file_type;
  
  // set material (default)
  copy_material( &(ppd->matl), 0, (float *) mtlall );

  swin->screenatr.current_ppd = ppd;
  swin->screenatr.view_ppd = ppd;
  //    swin->screenatr.org_ppd = ppd;

  // normalization
  // enhanced
  swin->screenatr.rad_sph *= ppd->scale;
  swin->screenatr.rad_cyl *= ppd->scale;

  // for setting subdivision boundary flag
  set_subdiv_boundary( ppd );

  InvalidateRect( NULL, FALSE );  
}

void CMainFrame::OnFileOpenEvrmlPoly() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください

  if ( swin->screenatr.current_ppd != NULL ) {
    AfxMessageBox( _T("A file has already opened."));
    return;
  }

  CFileDialog filedlg( TRUE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szwrlFilter, NULL );

  if( filedlg.DoModal() != IDOK ) return;

  // filename に選択したファイルのフルパスが入る
  CString filename = filedlg.GetPathName();

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

  // check VRML
  int file_type;
  if ( (file_type = check_vrml( f )) != FILE_EVRML_POLY )
    {
      AfxMessageBox( _T("This file is not a Extended VRML Polygon file."));
      return;
    }

  Sppd *ppd;
  if ( (ppd = open_vrml( f )) == NULL ) {
    char string[BUFSIZ];
    sprintf( string, "Can't open %s.", lp );
    AfxMessageBox( (LPTSTR) string );
    return;
  }
  ppd->file_type = file_type;

  // set material (default)
  copy_material( &(ppd->matl), 0, (float *) mtlall );

  //    if ( swin->screenatr.current_ppd != NULL ) {
  //      swin->screenatr.view_ppd = NULL;
  //      free_ppd( swin->screenatr.current_ppd );
  //    }

  swin->screenatr.current_ppd = ppd;
  swin->screenatr.view_ppd = ppd;
  //    swin->screenatr.org_ppd = ppd;

  // normalization
  // enhanced
  swin->screenatr.rad_sph *= ppd->scale;
  swin->screenatr.rad_cyl *= ppd->scale;

  // for setting subdivision boundary flag
  set_subdiv_boundary( ppd );
  
  InvalidateRect( NULL, FALSE );  

}

void CMainFrame::OnFileOpenEvrmlSs() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください

  if ( swin->screenatr.current_ppd != NULL ) {
    AfxMessageBox( _T("A file has already opened."));
    return;
  }

  CFileDialog filedlg( TRUE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szwrlFilter, NULL );

  if( filedlg.DoModal() != IDOK ) return;

  // filename に選択したファイルのフルパスが入る
  CString filename = filedlg.GetPathName();

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

  // check VRML
  int file_type;
  if ( (file_type = check_vrml( f )) != FILE_EVRML_SUBDIV )
    {
      AfxMessageBox( _T("This file is not a Extended VRML Subdiv. file."));
      return;
    }

  Sppd *ppd;
  if ( (ppd = open_vrml( f )) == NULL ) {
    char string[BUFSIZ];
    sprintf( string, "Can't open %s.", lp );
    AfxMessageBox( (LPTSTR) string );
    return;
  }
  ppd->file_type = file_type;

  // set material (default)
  copy_material( &(ppd->matl), 0, (float *) mtlall );
		
  //    if ( swin->screenatr.current_ppd != NULL ) {
  //      swin->screenatr.view_ppd = NULL;
  //      free_ppd( swin->screenatr.current_ppd );
  //    }

  swin->screenatr.current_ppd = ppd;
  swin->screenatr.view_ppd = ppd;
  //    swin->screenatr.org_ppd = ppd;
			
  // normalization
  // enhanced
  swin->screenatr.rad_sph *= ppd->scale;
  swin->screenatr.rad_cyl *= ppd->scale;

  // for setting subdivision boundary flag
  set_subdiv_boundary( ppd );
  
  InvalidateRect( NULL, FALSE );  
}

void CMainFrame::OnFileSaveVrmlSub()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.current_ppd == NULL ) return;
  
  CFileDialog filedlg( FALSE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szwrlFilter, NULL );

  if( filedlg.DoModal() != IDOK ) return;

  //	CWaitCursor wait;
  //filenameに選択したファイルのフルパスが入る
  CString filename;
  filename = filedlg.GetPathName();

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

  write_vrml( f, swin->screenatr.current_ppd, SAVE_VRML_SUBDIV );
  strcpy_s( swin->screenatr.current_ppd->filename,
            _countof(swin->screenatr.current_ppd->filename), f );
}

void CMainFrame::OnFileSaveVrmlPoly()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.view_ppd == NULL ) return;
  
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  CFileDialog filedlg( FALSE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szwrlFilter, NULL );

  if( filedlg.DoModal() != IDOK ) return;

  //	CWaitCursor wait;
  CString filename;
  filename = filedlg.GetPathName();
  //filenameに選択したファイルのフルパスが入る

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);
  
  write_vrml( f, swin->screenatr.view_ppd, SAVE_VRML_POLY );
  strcpy_s( swin->screenatr.view_ppd->filename,
            _countof(swin->screenatr.view_ppd->filename), f );
}

#include <string>

void ConvertCS2CHAR(char pchStr[], CString csStr)
{
	// CString → TCHAR*
	TCHAR	*ptcStr = new TCHAR[csStr.GetLength()+1]; 
	_tcscpy(ptcStr, csStr);

	// TCHAR → char*
	size_t wLen = 0;
	errno_t err = 0;

	// ロケール指定
	setlocale(LC_ALL, "japanese");
	
	err = wcstombs_s(&wLen, pchStr, BUFSIZ, ptcStr, _TRUNCATE);

        delete ptcStr;
}
#endif

static TCHAR BASED_CODE szsmfFilter[] = _T("SMF Files (*.smf)|*.smf|All Files (*.*)|*.*||");

//
// open SMF file
//
void CMainFrame::OnFileOpenSmf() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください

  // 第一引数は OPEN のとき TRUE
  // 第二、三引数はファイルの種類
  CFileDialog filedlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szsmfFilter, NULL );

  if( filedlg.DoModal() == IDOK) {
    //		CWaitCursor wait;
    CString filename;
    filename = filedlg.GetPathName();
    //filenameに選択したファイルのフルパスが入る

    TCHAR *lp;
    lp = filename.GetBuffer( BUFSIZ );

    char f[BUFSIZ];
    WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);
    // AfxMessageBox( filename );
    // AfxMessageBox( (LPTSTR) lp );

    Sppd *ppd;
    if ( (ppd = open_smf( f )) == NULL ) {
      AfxMessageBox( _T("Filename was bad!") );
      return;
    }

    // set material (default)
    copy_material( &(ppd->matl), 0, (float *) mtlall );

    if ( swin->screenatr.current_ppd != NULL ) {
      free_ppd( swin->screenatr.current_ppd );
      swin->screenatr.view_ppd = NULL;
    }

    swin->screenatr.current_ppd = ppd;
    swin->screenatr.view_ppd = ppd;
    //      swin->screenatr.org_ppd = ppd;

    // normalization
    // enhanced
    swin->screenatr.rad_sph *= ppd->scale;
    swin->screenatr.rad_cyl *= ppd->scale;

    // for setting subdivision boundary flag
    set_subdiv_boundary( ppd );
    
    InvalidateRect( NULL, FALSE );
  }
}

//
// save SMF file
//
void CMainFrame::OnFileSaveSmf() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.current_ppd == (Sppd *) NULL ) return;
  
  CFileDialog filedlg( FALSE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szsmfFilter, NULL );

  if( filedlg.DoModal() != IDOK ) return;

  //	CWaitCursor wait;
  CString filename;
  filename = filedlg.GetPathName();
  //filenameに選択したファイルのフルパスが入る

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

  write_smf( f, swin->screenatr.current_ppd );
  strcpy_s( swin->screenatr.current_ppd->filename,
            _countof(swin->screenatr.current_ppd->filename), f );
}

void CMainFrame::OnFileSaveSmfPoly()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.view_ppd == (Sppd *) NULL ) return;
  
  CFileDialog filedlg( FALSE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szsmfFilter, NULL );

  if( filedlg.DoModal() != IDOK ) return;

  //	CWaitCursor wait;
  CString filename;
  filename = filedlg.GetPathName();
  //filenameに選択したファイルのフルパスが入る

  TCHAR *lp = filename.GetBuffer(BUFSIZ);

  char f[BUFSIZ];
  WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

  write_smf( f, swin->screenatr.view_ppd );
  strcpy_s( swin->screenatr.view_ppd->filename,
            _countof(swin->screenatr.view_ppd->filename), f );
}

static TCHAR BASED_CODE szvwFilter[] = _T("VW Files (*.vw)|*.vw||");

//
// open VW file
//
void CMainFrame::OnFileOpenVw() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  CFileDialog filedlg( TRUE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szvwFilter, NULL );

  if( filedlg.DoModal() == IDOK) {
    // CWaitCursor wait;
    CString filename;
    filename = filedlg.GetPathName();
    //filenameに選択したファイルのフルパスが入る

    TCHAR *lp;
    lp = filename.GetBuffer( BUFSIZ );

    char f[BUFSIZ];
    WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

    double mmat[16], pmat[16];
    open_vw( f, (double *) mmat, (double *) pmat );

    InvalidateRect( NULL, FALSE );
  }

}

//
// save VW file
//
void CMainFrame::OnFileSaveVw()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  CFileDialog filedlg( TRUE, NULL, NULL,
                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                       szvwFilter, NULL );

  if( filedlg.DoModal() == IDOK) {
    CString filename;
    filename = filedlg.GetPathName();
    //filenameに選択したファイルのフルパスが入る

    TCHAR *lp;
    lp = filename.GetBuffer( BUFSIZ );

    char f[BUFSIZ];
    WideCharToMultiByte(CP_ACP,0,lp,-1,f,sizeof(f),NULL,NULL);

    InvalidateRect( NULL, FALSE );
    write_vw_file( f );

    //		write_gmh_file( lp, swin->hppd );
  }

}

////////////////////////////////////////////////////////////////////////
// Menu Screen Functions
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Update Menu Screen Functions
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateMenuScreenShading(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.shading == FALSE ) {
    //swin->dis3d.shading = FALSE;
    pCmdUI->SetCheck( 0 );
  } else {
    //		swin->dis3d.shading = TRUE;
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnUpdateMenuScreenWireframe( CCmdUI* pCmdUI ) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.wire == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnUpdateMenuScreenHidden(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.hidden == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

#if 0
void CMainFrame::OnUpdateMenuScreenEdgeid(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.edgeid == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}
#endif

#if 0
void CMainFrame::OnUpdateMenuScreenCoaxis(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.coaxis == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}
#endif

#if 0
void CMainFrame::OnUpdateMenuScreenVert(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.vertex == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnUpdateMenuScreenVertid(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.vertexid == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}
#endif

void CMainFrame::OnUpdateMenuScreenSubbown(CCmdUI* pCmdUI)
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.subdiv_boundary == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnMenuScreenShading() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.shading == TRUE ) 
    swin->dis3d.shading = FALSE;
  else 
    swin->dis3d.shading = TRUE;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnMenuScreenWireframe()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.wire == TRUE ) 
    swin->dis3d.wire = FALSE;
  else 
    swin->dis3d.wire = TRUE;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnMenuScreenHidden() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.hidden == TRUE ) 
    swin->dis3d.hidden = FALSE;
  else 
    swin->dis3d.hidden = TRUE;

  InvalidateRect( NULL, FALSE );
}

#if 0
void CMainFrame::OnMenuScreenEdgeid() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.edgeid == TRUE ) 
    swin->dis3d.edgeid = FALSE;
  else 
    swin->dis3d.edgeid = TRUE;

  InvalidateRect( NULL, FALSE );
}
#endif

#if 0
void CMainFrame::OnMenuScreenCoaxis() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.coaxis == TRUE ) 
    swin->dis3d.coaxis = FALSE;
  else 
    swin->dis3d.coaxis = TRUE;

  InvalidateRect( NULL, FALSE );
}
#endif

#if 0
void CMainFrame::OnMenuScreenVert() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.vertex == TRUE ) 
    swin->dis3d.vertex = FALSE;
  else 
    swin->dis3d.vertex = TRUE;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnMenuScreenVertid() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.vertexid == TRUE ) 
    swin->dis3d.vertexid = FALSE;
  else 
    swin->dis3d.vertexid = TRUE;

  InvalidateRect( NULL, FALSE );
}
#endif

void CMainFrame::OnMenuScreenSubbown() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.subdiv_boundary == TRUE ) 
    swin->dis3d.subdiv_boundary = FALSE;
  else 
    swin->dis3d.subdiv_boundary = TRUE;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnMenuScreenOrgppd() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.org_ppd == TRUE )
    {
      swin->dis3d.org_ppd = FALSE;
      swin->screenatr.view_ppd = swin->screenatr.current_ppd;
    }
  else
    {
      swin->dis3d.org_ppd = TRUE;
      swin->screenatr.view_ppd = swin->screenatr.org_ppd;
    }

  InvalidateRect( NULL, FALSE );
}

////////////////////////////////////////////////////////////////////////
// smooth Display.
////////////////////////////////////////////////////////////////////////
#if 0
void CMainFrame::OnUpdateMenuScreenEnhanced(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.smooth == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnMenuScreenEnhanced() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.enhanced == TRUE ) 
    swin->dis3d.enhanced = FALSE;
  else 
    swin->dis3d.enhanced = TRUE;

  InvalidateRect( NULL, FALSE );
}
#endif

////////////////////////////////////////////////////////////////////////
// Smooth Shading
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnMenuScreenSmooth() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.smooth == TRUE ) {
    swin->dis3d.smooth = FALSE;
    
    freeppdnorm( swin->screenatr.view_ppd );
  } else {
    swin->dis3d.smooth = TRUE;
    ppdnorm( swin->screenatr.view_ppd, 40.0 );
  }

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnUpdateMenuScreenSmooth(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.smooth == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}


void CMainFrame::drawwindow()
{
  RedrawWindow();
}

#if 0
#include "SRDialog.h"

void CMainFrame::OnMenuScreenRadius() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください

  CSRDialog* srd = new CSRDialog;
  ASSERT_VALID( srd );

  BOOL bResult = srd->Create();
  ASSERT( bResult );
}
#endif

void CMainFrame::OnViewBackgroundColor()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください

  ScreenAtr *screen = &(swin->screenatr);
  CColorDialog dlg;
	
  unsigned short r = (unsigned short) (screen->bgrgb[0] * 255);
  unsigned short g = (unsigned short) (screen->bgrgb[1] * 255);
  unsigned short b = (unsigned short) (screen->bgrgb[2] * 255);

  //CString string;
  //string.Format("%d %d %d", r, g, b );
  //AfxMessageBox( (LPTSTR) string );

  //dlg.SetCurrentColor( RGB( r, g, b ) );
  //dlg.m_cc.rgbResult = RGB( r, g, b );

  if ( dlg.DoModal() == IDOK ) {
    COLORREF color = dlg.GetColor();
    screen->bgrgb[0] = (float) (GetRValue( color ) / 255.0);
    screen->bgrgb[1] = (float) (GetGValue( color ) / 255.0);
    screen->bgrgb[2] = (float) (GetBValue( color ) / 255.0);
    InvalidateRect( NULL, FALSE );
  }
}

#include "MaterialDialog.h"
CMaterialDialog *md = NULL;

void CMainFrame::OnViewMaterial()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.current_ppd == NULL ) {
    AfxMessageBox( _T("ppd is not found."));
    return;
  }

  if ( md != NULL ) return;

  md = new CMaterialDialog;
  ASSERT_VALID( md );

  BOOL bResult = md->Create();
  ASSERT( bResult );

}

#if 0
#include "MeshInfoDialog.h"
CMeshInfoDialog *midlg;

//
// Help Menu Command
//
void CMainFrame::OnMeshInfo()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください

  if ( swin->screenatr.current_ppd == NULL ) {
    AfxMessageBox( _T("ppd is not found."));
    return;
  }

  midlg = new CMeshInfoDialog;
  ASSERT_VALID( midlg );

  BOOL bResult = midlg->Create();
  ASSERT( bResult );
}
#endif

////////////////////////////////////////////////////////////////////////
// Gradient Background
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewGradient()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.gradient == TRUE ) 
    swin->dis3d.gradient = FALSE;
  else 
    swin->dis3d.gradient = TRUE;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnUpdateViewGradient(CCmdUI* pCmdUI)
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.gradient == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnUpdateMenuScreenOrgppd(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.org_ppd == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
  
}

//
// Light
// 

void CMainFrame::OnMenuScreenLight0() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.light0 == TRUE ) 
    swin->dis3d.light0 = FALSE;
  else 
    swin->dis3d.light0 = TRUE;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnUpdateMenuScreenLight0(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.light0 == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnMenuScreenLight1() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.light1 == TRUE ) 
    swin->dis3d.light1 = FALSE;
  else 
    swin->dis3d.light1 = TRUE;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnUpdateMenuScreenLight1(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.light1 == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnMenuScreenLight2() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.light2 == TRUE ) 
    swin->dis3d.light2 = FALSE;
  else 
    swin->dis3d.light2 = TRUE;

  InvalidateRect( NULL, FALSE );
	
}

void CMainFrame::OnUpdateMenuScreenLight2(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.light2 == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

void CMainFrame::OnMenuScreenLight3() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->dis3d.light3 == TRUE ) 
    swin->dis3d.light3 = FALSE;
  else 
    swin->dis3d.light3 = TRUE;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnUpdateMenuScreenLight3(CCmdUI* pCmdUI) 
{
  // TODO: この位置に command update UI ハンドラ用のコードを追加してください
  if ( swin->dis3d.light3 == FALSE ) {
    pCmdUI->SetCheck( 0 );
  } else {
    pCmdUI->SetCheck( 1 );
  }
}

//
// "Edit" Menu
//
void CMainFrame::OnMenuEditSubdivUp() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.view_ppd == NULL ) return;

  Sppd* ppd = swin->screenatr.view_ppd;

  //
#if 0
  Sppd* tppd = subdiv_top( ppd );
  if ( tppd->file_type != FILE_EVRML_SUBDIV )
    {
      AfxMessageBox( _T("Subdivision can be executed only for a Extended VRML Subdiv. File"));
      return;
    }
#endif

  if ( ppd->parent == NULL ) return;
  swin->screenatr.view_ppd = ppd->parent;

  InvalidateRect( NULL, FALSE );
}

void CMainFrame::OnMenuEditSubdivDown() 
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.view_ppd == NULL ) return;

  Sppd* ppd = swin->screenatr.view_ppd;

  //
#if 0
  Sppd* tppd = subdiv_top( ppd );
  if ( tppd->file_type != FILE_EVRML_SUBDIV )
    {
      AfxMessageBox( _T("Subdivision can be executed only for a Extended VRML Subdiv. File"));
      return;
    }
#endif
  // level 3 以上はいかないように設定
  if ( ppd->sub_level >= 3 ) return;
  
  Sppd* child;
  if ( (child = ppd->child) == NULL ) {
    child = ppdsubdiv( ppd, SUBDIV_LOOP );
    child->file_type = FILE_EVRML_POLY;
    copy_material( &(child->matl), 0, (float *) mtlall );
  }
  
  swin->screenatr.view_ppd = child;
  
  InvalidateRect( NULL, FALSE );
}

#include "ConvSubdiv.h"

CConvSubdiv *csdlg;

void CMainFrame::OnMenuEditConv()
{
  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
  if ( swin->screenatr.current_ppd == NULL ) {
    AfxMessageBox( _T("vrml is not open."));
    return;
  }

  csdlg = new CConvSubdiv;
  ASSERT_VALID( csdlg );

  BOOL bResult = csdlg->Create();
  ASSERT( bResult );
  
}

