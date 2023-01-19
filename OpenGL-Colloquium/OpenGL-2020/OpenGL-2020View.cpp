
// OpenGL-2020View.cpp : implementation of the COpenGL2020View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL-2020.h"
#endif

#include "OpenGL-2020Doc.h"
#include "OpenGL-2020View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795

// COpenGL2020View

IMPLEMENT_DYNCREATE(COpenGL2020View, CView)

BEGIN_MESSAGE_MAP(COpenGL2020View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// COpenGL2020View construction/destruction

COpenGL2020View::COpenGL2020View() noexcept
{
	// TODO: add construction code here
	mousePos = { 0, 0 };

}

COpenGL2020View::~COpenGL2020View()
{
}

BOOL COpenGL2020View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGL2020View drawing

void COpenGL2020View::OnDraw(CDC* pDC)
{
	COpenGL2020Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	glDC.DrawScene(pDC);
}


// COpenGL2020View printing

BOOL COpenGL2020View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGL2020View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGL2020View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COpenGL2020View diagnostics

#ifdef _DEBUG
void COpenGL2020View::AssertValid() const
{
	CView::AssertValid();
}

void COpenGL2020View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGL2020Doc* COpenGL2020View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGL2020Doc)));
	return (COpenGL2020Doc*)m_pDocument;
}
#endif //_DEBUG


// COpenGL2020View message handlers


void COpenGL2020View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CDC* pDC = GetDC();
	glDC.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void COpenGL2020View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	CDC* pDC = GetDC();
	glDC.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL COpenGL2020View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


int COpenGL2020View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	CDC* pDC = GetDC();
	glDC.CreateGLContext(pDC);
	ReleaseDC(pDC);
	return 0;
}


void COpenGL2020View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
}


void COpenGL2020View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON) 
	{
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

void COpenGL2020View::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	CDC* pDC = GetDC();
	glDC.PrepareScene(pDC);
	ReleaseDC(pDC);
}

void COpenGL2020View::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	double r = this->glDC.eyeDistance + zDelta * (-0.01);

	if (r > 5 && r < 50) {
		this->glDC.eyeDistance = r;
		Invalidate();
	}

	CView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL COpenGL2020View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	glDC.Zoom(zDelta < 0);
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
