
// OpenGL-LP4View.cpp : implementation of the COpenGLLP4View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL-LP4.h"
#endif

#include "OpenGL-LP4Doc.h"
#include "OpenGL-LP4View.h"
#include "DImage.h"
#include "GLRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLLP4View

IMPLEMENT_DYNCREATE(COpenGLLP4View, CView)

BEGIN_MESSAGE_MAP(COpenGLLP4View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// COpenGLLP4View construction/destruction

COpenGLLP4View::COpenGLLP4View() noexcept
{
	// TODO: add construction code here

}

COpenGLLP4View::~COpenGLLP4View()
{
}

BOOL COpenGLLP4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGLLP4View drawing

void COpenGLLP4View::OnDraw(CDC* pDC)
{
	COpenGLLP4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	glDC.DrawScene(pDC);
}


// COpenGLLP4View printing

BOOL COpenGLLP4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLLP4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLLP4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COpenGLLP4View diagnostics

#ifdef _DEBUG
void COpenGLLP4View::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLLP4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLLP4Doc* COpenGLLP4View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLLP4Doc)));
	return (COpenGLLP4Doc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLLP4View message handlers


int COpenGLLP4View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CDC* pDC = GetDC();
	glDC.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


BOOL COpenGLLP4View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void COpenGLLP4View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CDC* pDC = GetDC();
	glDC.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void COpenGLLP4View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here

	CDC* pDC = GetDC();
	glDC.DestroyScene(pDC);
	ReleaseDC(pDC);
}


void COpenGLLP4View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	glDC.PrepareScene(pDC);
	ReleaseDC(pDC);
}