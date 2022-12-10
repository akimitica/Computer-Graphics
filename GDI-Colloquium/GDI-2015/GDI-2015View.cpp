
// GDI-2015View.cpp : implementation of the CGDI2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-2015.h"
#endif

#include "GDI-2015Doc.h"
#include "GDI-2015View.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795

// CGDI2015View

IMPLEMENT_DYNCREATE(CGDI2015View, CView)

BEGIN_MESSAGE_MAP(CGDI2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGDI2015View construction/destruction

CGDI2015View::CGDI2015View() noexcept
{
	// TODO: add construction code here
	background = new DImage();
	background->Load(CString("res/blue.png"));
}

CGDI2015View::~CGDI2015View()
{
	delete background;
}

BOOL CGDI2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDI2015View drawing


//===========================================================================================
//======================== W O R L D - T R A N S F O R M A T I O N S ========================
//===========================================================================================

void CGDI2015View::Translate(CDC* pDC, float dX, float dY, bool rightMul)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2015View::Rotate(CDC* pDC, float angle, bool rightMul)
{
	angle *= PI / 180;
	XFORM form = { cos(angle), sin(angle),-sin(angle),cos(angle),0,0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

//===========================================================================================
//============================ D R A W I N G - F U N C T I O N S ============================
//===========================================================================================

void CGDI2015View::DrawBackground(CDC* pDC)
{
	CRect window;
	GetClientRect(&window);
	background->Draw(pDC, window, window);
}

void CGDI2015View::DrawPackman(CDC* pDC, CRect rect, float angle)
{

}

void CGDI2015View::DrawGhost(CDC* pDC, CRect rect)
{

}

void CGDI2015View::OnDraw(CDC* pDC)
{
	CGDI2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect window;
	GetClientRect(&window);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap img;
	img.CreateCompatibleBitmap(pDC, window.Width(), window.Height());

	memDC->SelectObject(img);

	XFORM oldForm;
	memDC->GetWorldTransform(&oldForm);
	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);

	DrawBackground(memDC);
	DrawPackman(memDC, window, angle);

	memDC->SetGraphicsMode(oldMode);
	memDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, window.Width(), window.Height(), memDC, 0, 0, SRCCOPY);
}


// CGDI2015View printing

BOOL CGDI2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDI2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDI2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDI2015View diagnostics

#ifdef _DEBUG
void CGDI2015View::AssertValid() const
{
	CView::AssertValid();
}

void CGDI2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDI2015Doc* CGDI2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDI2015Doc)));
	return (CGDI2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDI2015View message handlers


BOOL CGDI2015View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CView::OnEraseBkgnd(pDC);
}


void CGDI2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
