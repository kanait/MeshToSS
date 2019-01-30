// MeshToSSView.cpp : CMeshToSSView �N���X�̓���̒�`���s���܂��B

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Include the OpenGL headers
#include "gl\gl.h"
#include "gl\glu.h"

// from C 
#include "../optmesh/smd.h"

#include "screen.h"
#include "draw.h"
#include "pick.h"
//  #include "dialog.h"
#include "bitmap.h"
#include "Material.h"
#include "MainFrm.h"
#include "MeshToSS.h"
#include "MeshToSSDoc.h"
#include "MeshToSSView.h"

const char* const CMeshToSSView::_ErrorStrings[]= {
  {"No Error"}, // 0
  {"Unable to get a DC"}, // 1
  {"ChoosePixelFormat failed"}, // 2
  {"SelectPixelFormat failed"}, // 3
  {"wglCreateContext failed"}, // 4
  {"wglMakeCurrent failed"}, // 5
  {"wglDeleteContext failed"}, // 6
  {"SwapBuffers failed"},  // 7
};

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSView

IMPLEMENT_DYNCREATE(CMeshToSSView, CView)

  BEGIN_MESSAGE_MAP(CMeshToSSView, CView)
  //{{AFX_MSG_MAP(CMeshToSSView)
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_CHAR()
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // CMeshToSSView �N���X�̍\�z/����

  CMeshToSSView::CMeshToSSView() :
    m_hRC(0), m_pDC(0), m_ErrorString(_ErrorStrings[0])
{
  // TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B

  //	swin->screenatr[SCREEN_SRC].wnd = GetTopLevelParent();
}

CMeshToSSView::~CMeshToSSView()
{
}

BOOL CMeshToSSView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
  //  �C�����Ă��������B

  cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

  //	cs.cx = swin->screenatr[SCREEN_SRC].width;
  //	cs.cy = swin->screenatr[SCREEN_SRC].height;

  moving	 = FALSE;
  rotating = FALSE;
  scaling	 = FALSE;

  return CView::PreCreateWindow(cs);

}

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSView �N���X�̕`��

void CMeshToSSView::OnDraw(CDC* pDC)
{
  CMeshToSSDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  if ( ::wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC ) == FALSE ) {
    SetError(5);
    return;
  }

  // TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B

  redraw();

  if ( ::SwapBuffers( m_pDC->GetSafeHdc() ) == FALSE ) {
    SetError(7);
  }

}

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSView �N���X�̈��

BOOL CMeshToSSView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // �f�t�H���g�̈������
  return DoPreparePrinting(pInfo);
}

void CMeshToSSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CMeshToSSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: �����̌㏈����ǉ����Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSView �N���X�̐f�f

#ifdef _DEBUG
void CMeshToSSView::AssertValid() const
{
  CView::AssertValid();
}

void CMeshToSSView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CMeshToSSDoc* CMeshToSSView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeshToSSDoc)));
  return (CMeshToSSDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMeshToSSView �N���X�̃��b�Z�[�W �n���h��

int CMeshToSSView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;
	
  // TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	
  InitializeOpenGL();

  return 0;
}

//////////////////////////////////////////////////////////////////////
// GL Helper functions

void CMeshToSSView::SetError( int e )
{
  // if there was no previous error, 
  // then save this one
  if ( _ErrorStrings[0] == m_ErrorString ) {
    m_ErrorString = _ErrorStrings[e];
  }
}

BOOL CMeshToSSView::InitializeOpenGL()
{
  m_pDC = new CClientDC( this );

  if ( m_pDC == NULL ) { // failure to get DC
    SetError(1);
    return FALSE;
  }

  if ( !SetupPixelFormat() ) {
    return FALSE;
  }

  if ( (m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc()) )
       == 0 ) {
    SetError(4);
    return FALSE;
  }

  if ( ::wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC ) == FALSE ) {
    SetError(5);
    return FALSE;
  }

  // fonts initialize
  ScreenAtr *screen;
  screen = &(swin->screenatr);
  screen->fontOffset = ::glGenLists( 256 );

  screen->wnd = GetTopLevelParent();
  
  CFont *fontInfo;
  fontInfo = (CFont*) m_pDC->SelectStockObject( SYSTEM_FONT );
  ::wglUseFontBitmaps( m_pDC->m_hDC, 0, 256, screen->fontOffset );
  m_pDC->SelectObject( fontInfo );

  // window size
//    RECT rect;
//    rect.left = 0;
//    rect.right = 700;
//    rect.top = 0;
//    rect.bottom = 700;
  //MoveWindow( 0, 0, 700, 700, FALSE );
  
  init_gl3d();
	
  return TRUE;
}

BOOL CMeshToSSView::SetupPixelFormat()
{
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),	// size of this pfd
    1,					// version number
    PFD_DRAW_TO_WINDOW |		// support window
    PFD_SUPPORT_OPENGL |		// support OpenGL
    PFD_DOUBLEBUFFER,			// double buffered
    PFD_TYPE_RGBA,			// RGBA type
    24,					// 24-bit color depth
    0, 0, 0, 0, 0, 0,			// color bits ignored
    0,					// no alpha buffer
    0,					// shift bit ignored
    0,					// no accumulation buffer
    0, 0, 0, 0,				// accum bits ignored
    16,					// 16-bit z-buffer
    0,					// no stencil buffer
    0,					// no auxiliary buffer
    PFD_MAIN_PLANE,			// main layer
    0,					// reserved
    0, 0, 0				// layer masks ignored
  };
  int pixelformat;

  if ( (pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd))
       == 0 ) {
    SetError(2);
    return FALSE;
  }

  if ( ::SetPixelFormat( m_pDC->GetSafeHdc(), pixelformat, &pfd )
       == FALSE ) {
    SetError(3);
    return FALSE;
  }

  return TRUE;
}

void CMeshToSSView::OnDestroy() 
{
  CView::OnDestroy();
	
  // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
  if ( ::wglMakeCurrent( 0, 0 ) == FALSE ) {
    SetError(2);
  }
  if ( ::wglDeleteContext( m_hRC ) == FALSE ) {
    SetError(6);
  }
  if ( m_pDC ) {
    delete m_pDC;
  }
}

BOOL CMeshToSSView::OnEraseBkgnd(CDC* pDC) 
{
  // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
  // return CView::OnEraseBkgnd(pDC);
  return TRUE; // tell Windows not to erase the Background
}

void CMeshToSSView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
	
  // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

  //	GLdouble aspect_ratio; // width/height ratio
	
  if ( (cx <= 0) || (cy <= 0) )  {
    return;
  }

  ScreenAtr *screen;
  screen = &(swin->screenatr);
  screen->width = cx; screen->height = cy;

  ::glViewport((GLint) 0, (GLint) 0,
	       (GLsizei) screen->width, (GLsizei) screen->height);
  InvalidateRect( NULL, FALSE );
}

//static int scrn_x, scrn_y; 
//static BOOL moving = FALSE, scaling = FALSE;

void CMeshToSSView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
  CView::OnLButtonDown(nFlags, point);

  scrn_x = point.x;
  scrn_y = point.y;

	// move
  if ( (GetKeyState( VK_SHIFT ) < 0) && 
       (GetKeyState( VK_CONTROL ) < 0) ) { 
    moving = TRUE;
    return;
  }

  // rotate
  if ( GetKeyState( VK_SHIFT ) < 0 ) { 
    rotating = TRUE;
    return;
  }

  if ( GetKeyState( VK_CONTROL ) < 0 ) {
    scaling = TRUE;
    return;
  }

  moving   = FALSE;
  rotating = FALSE;
  scaling  = FALSE;

  ScreenAtr *screen = &(swin->screenatr);

  CMeshToSSDoc* pDoc = GetDocument();
  CRect rectSave;
  pDoc->m_tracker.GetTrueRect(rectSave);
  if (pDoc->m_tracker.HitTest(point) < 0) {

    // just to demonstrate CRectTracker::TrackRubberBand
    CRectTracker tracker;
    // (8,8) �ȏ�̃T�C�Y�ɂȂ�Ȃ��ƃ��o�[�o���h�ɂȂ�Ȃ�
#if 0    
    if (tracker.TrackRubberBand(this, point, pDoc->m_bAllowInvert)) {

      // pick area
      if ( edit_pick_area( 
			  scrn_x, 
			  scrn_y, 
			  scrn_x + tracker.m_rect.Width(),
			  scrn_y + tracker.m_rect.Height(),
			  swin->edit_type, 
			  screen
			  ) == TRUE ) {

				// input vertex ID to src edit box
	SelectToDialog( swin->edit_type, screen );

	InvalidateRect( NULL, FALSE );
      }
      // see if rubber band intersects with the doc's tracker

    } else {
#endif

      // pick only
      if ( edit_pick( point.x, point.y, swin->edit_type, screen )
	   == TRUE ) {

	// input vertex ID to src edit box
	//SelectToDialog( swin->edit_type, screen );

	InvalidateRect( NULL, FALSE );
      }
		  
#if 0    
    }
#endif

  } else if ( pDoc->m_tracker.Track(this, point, pDoc->m_bAllowInvert) ) {

    // normal tracking action, when tracker is "hit"
    pDoc->SetModifiedFlag();
    pDoc->UpdateAllViews(NULL, (LPARAM)(LPCRECT)rectSave);
    pDoc->UpdateAllViews(NULL);

  } else {

  }
}

void CMeshToSSView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
  CView::OnLButtonUp(nFlags, point);

  moving   = FALSE;
  rotating = FALSE;
  scaling  = FALSE;
}

void CMeshToSSView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
  CView::OnMouseMove(nFlags, point);

  // rotate
  if ( rotating && (GetKeyState(VK_SHIFT) < 0 ) ) { 

    int dx = point.x - scrn_x;
    int dy = point.y - scrn_y;
    update_screen3d_rotate( &(swin->screenatr), dx, dy );
    scrn_x = point.x;
    scrn_y = point.y;
    InvalidateRect( NULL, FALSE );

  } else if ( moving && (GetKeyState(VK_SHIFT) < 0 ) 
	      && (GetKeyState(VK_CONTROL) < 0 ) ) {  // trans

    int dx = point.x - scrn_x;
    int dy = point.y - scrn_y;
    update_screen3d_translate( &(swin->screenatr), dx, dy );
    scrn_x = point.x;
    scrn_y = point.y;
    InvalidateRect( NULL, FALSE );

  } else if ( scaling && (GetKeyState(VK_CONTROL) < 0 ) ) { 

    int dx = point.x - scrn_x;
    int dy = point.y - scrn_y;
    update_screen3d_zoom( &(swin->screenatr), dx, dy );
    scrn_x = point.x;
    scrn_y = point.y;
    InvalidateRect( NULL, FALSE );

  }

}

#include "../optmesh/subdiv.h"
#include "mtl.h"

//
// �L�[�{�[�h����
//
void CMeshToSSView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
  CView::OnChar(nChar, nRepCnt, nFlags);

  switch (nChar) {
  case 'z': // shading ON/OFF
    { 
      if ( swin->dis3d.shading == TRUE )
	swin->dis3d.shading = FALSE;
      else
	swin->dis3d.shading = TRUE;
    }
    break;
  case 'x': // wireframe ON/OFF
    { 
      if ( swin->dis3d.wire == TRUE )
	swin->dis3d.wire = FALSE;
      else
	swin->dis3d.wire = TRUE;
    }
    break;
  case 'c': // vertex ON/OFF
    { 
      if ( swin->dis3d.vertex == TRUE )
	swin->dis3d.vertex = FALSE;
      else
	swin->dis3d.vertex = TRUE;
    }
    break;
  case 'n': // subdivision level down
    {
      if ( swin->screenatr.view_ppd == NULL ) return;

      Sppd* ppd = swin->screenatr.view_ppd;

#if 0
      Sppd* tppd = subdiv_top( ppd );
      if ( tppd->file_type != FILE_EVRML_SUBDIV )
        {
          AfxMessageBox(_T("Subdivision can be executed only for a Extended VRML Subdiv. File"));
          return;
        }
#endif

      // level 3 �ȏ�͂����Ȃ��悤�ɐݒ�
      if ( ppd->sub_level >= 3 ) return;

      Sppd* child;
      if ( (child = ppd->child) == NULL ) {
        child = ppdsubdiv( ppd, SUBDIV_LOOP );
        copy_material( &(child->matl), 0, (float *) mtlall );
      }

      swin->screenatr.view_ppd = child;

    }
    break;
  case 'm': // subdivision level up
    {
      if ( swin->screenatr.view_ppd == NULL ) return;

      Sppd* ppd = swin->screenatr.view_ppd;

#if 0
      Sppd* tppd = subdiv_top( ppd );
      if ( tppd->file_type != FILE_EVRML_SUBDIV )
        {
          AfxMessageBox(_T("Subdivision can be executed only for a Extended VRML Subdiv. File"));
          return;
        }
#endif

      if ( ppd->parent == NULL ) return;
      swin->screenatr.view_ppd = ppd->parent;
    }
    break;
  default:
    break;
  }

  InvalidateRect( NULL, FALSE );
}

