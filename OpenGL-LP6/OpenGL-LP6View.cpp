
// OpenGL-LP6View.cpp : implementation of the COpenGLLP6View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL-LP6.h"
#endif

#include "OpenGL-LP6Doc.h"
#include "OpenGL-LP6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLLP6View

IMPLEMENT_DYNCREATE(COpenGLLP6View, CView)

BEGIN_MESSAGE_MAP(COpenGLLP6View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// COpenGLLP6View construction/destruction

COpenGLLP6View::COpenGLLP6View() noexcept
{
	mousePos = { 0, 0 };
}

COpenGLLP6View::~COpenGLLP6View()
{
}

BOOL COpenGLLP6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGLLP6View drawing

void COpenGLLP6View::OnDraw(CDC* pDC)
{
	COpenGLLP6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	glDC.DrawScene(pDC);
}


// COpenGLLP6View printing

BOOL COpenGLLP6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLLP6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLLP6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void COpenGLLP6View::OnInitialUpdate()
{
	CDC* pDC = GetDC();
	glDC.PrepareScene(pDC);
	ReleaseDC(pDC);
}

// COpenGLLP6View diagnostics

#ifdef _DEBUG
void COpenGLLP6View::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLLP6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLLP6Doc* COpenGLLP6View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLLP6Doc)));
	return (COpenGLLP6Doc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLLP6View message handlers


int COpenGLLP6View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	glDC.CreateGLContext(pDC);
	ReleaseDC(pDC);
	return 0;
}


BOOL COpenGLLP6View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void COpenGLLP6View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void COpenGLLP6View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	glDC.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void COpenGLLP6View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	glDC.DestroyScene(pDC);
	ReleaseDC(pDC);
}


void COpenGLLP6View::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	double r = this->glDC.eyeDistance + zDelta * (-0.01);

	if (r > 5 && r < 50) {
		this->glDC.eyeDistance = r;
		Invalidate();
	}

	CView::OnMouseHWheel(nFlags, zDelta, pt);
}


void COpenGLLP6View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON) {
		double moveX = point.x > mousePos.x ? 2 : -2;
		double moveY = point.y > mousePos.y ? 2 : -2;
		if (abs(point.x - mousePos.x) > abs(point.y - mousePos.y))
			moveY /= 10;
		else
			moveX /= 10;
		this->glDC.viewAngle[1] += moveX * PI / 180 * glDC.upVectorY;
		this->glDC.viewAngle[0] += moveY * PI / 180;
		mousePos = point;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}
