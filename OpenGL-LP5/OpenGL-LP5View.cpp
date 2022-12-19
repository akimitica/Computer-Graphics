
// OpenGL-LP5View.cpp : implementation of the COpenGLLP5View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL-LP5.h"
#endif

#include "OpenGL-LP5Doc.h"
#include "OpenGL-LP5View.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795

// COpenGLLP5View

IMPLEMENT_DYNCREATE(COpenGLLP5View, CView)

BEGIN_MESSAGE_MAP(COpenGLLP5View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// COpenGLLP5View construction/destruction

COpenGLLP5View::COpenGLLP5View() noexcept
{
	// TODO: add construction code here
	isClicked = false;

}

COpenGLLP5View::~COpenGLLP5View()
{
}

BOOL COpenGLLP5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGLLP5View drawing

void COpenGLLP5View::OnDraw(CDC* pDC)
{
	COpenGLLP5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	glDC.DrawScene(pDC);
}


// COpenGLLP5View printing

BOOL COpenGLLP5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLLP5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLLP5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COpenGLLP5View diagnostics

#ifdef _DEBUG
void COpenGLLP5View::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLLP5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLLP5Doc* COpenGLLP5View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLLP5Doc)));
	return (COpenGLLP5Doc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLLP5View message handlers


int COpenGLLP5View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CDC* pDC = GetDC();
	glDC.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


BOOL COpenGLLP5View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void COpenGLLP5View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	glDC.DestroyScene(pDC);
	ReleaseDC(pDC);
}

void COpenGLLP5View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CDC* pDC = GetDC();
	glDC.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}

void COpenGLLP5View::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CDC* pDC = GetDC();
	glDC.PrepareScene(pDC);
	ReleaseDC(pDC);
}

void COpenGLLP5View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case 'Z':
		glDC.light0 = !glDC.light0;
		break;
	case 'X':
		glDC.light1 = !glDC.light1;
		break;
	case 'C':
		glDC.light2 = !glDC.light2;
		break;
	default:
		glDC.showNorm = !glDC.showNorm;
		break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void COpenGLLP5View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	isClicked = true;
	oldPoint = point;
	CView::OnLButtonDown(nFlags, point);
}


void COpenGLLP5View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isClicked = false;
	CView::OnLButtonUp(nFlags, point);
}


void COpenGLLP5View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (isClicked)
	{
		double moveX = point.x > oldPoint.x ? 1 : -1;
		double moveY = point.y > oldPoint.y ? 1 : -1;
		if (abs(point.x - oldPoint.x) > abs(point.y - oldPoint.y))
			moveY /= 10;
		else
			moveX /= 10;
		glDC.viewAngle[1] += 2 * moveX * PI / 180;
		glDC.viewAngle[0] += 2 * moveY * PI / 180;
		oldPoint = point;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL COpenGLLP5View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	glDC.Zoom(zDelta < 0);
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}



