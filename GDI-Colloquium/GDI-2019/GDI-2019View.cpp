
// GDI-2019View.cpp : implementation of the CGDI2019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-2019.h"
#endif

#include "GDI-2019Doc.h"
#include "GDI-2019View.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795

// CGDI2019View

IMPLEMENT_DYNCREATE(CGDI2019View, CView)

BEGIN_MESSAGE_MAP(CGDI2019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGDI2019View construction/destruction

CGDI2019View::CGDI2019View() noexcept
{
	// TODO: add construction code here
	arm1 = new DImage();
	arm2 = new DImage();
	leg1 = new DImage();
	leg2 = new DImage();
	body = new DImage();
	bkg = new DImage();

	arm1->Load(CString("res/arm1.png"));
	arm2->Load(CString("res/arm2.png"));
	leg1->Load(CString("res/leg1.png"));
	leg2->Load(CString("res/leg2.png"));
	body->Load(CString("res/body1.png"));
	bkg->Load(CString("res/background.jpg"));



}

CGDI2019View::~CGDI2019View()
{
	delete arm1, arm2, leg1, leg2, body, bkg;
}

BOOL CGDI2019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDI2019View drawing



//===================================================================================================
//============================ W O R L D - T R A N S F O R M A T I O N S ============================
//===================================================================================================


void CGDI2019View::Translate(CDC* pDC, float dX, float dY, bool rightMul)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2019View::Scale(CDC* pDC, float sX, float sY, bool rightMul)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2019View::Rotate(CDC* pDC, float angle, bool rightMul)
{
	angle *= PI / 180;
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


//===================================================================================================
//================================ D R A W I N G - F U N C T I O N S ================================
//===================================================================================================

void CGDI2019View::DrawBackground(CDC* pDC, CRect rc)
{
	bkg->Draw(pDC, rc, rc);
}

void CGDI2019View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	unsigned char* bits = pImage->GetDIBBits();
	COLORREF background = COLORREF(RGB(bits[2], bits[1], bits[0]));
	CRect rect = CRect(0, 0, pImage->Width(), pImage->Height());
	pImage->DrawTransparent(pDC, rect, rect, background);
}

void CGDI2019View::DrawArm1(CDC* pDC)
{
	Translate(pDC, 34, 31, false);
	Rotate(pDC, -A1, false);
	Translate(pDC, -34, -31, false);

	DrawImgTransparent(pDC, arm1);
}

void CGDI2019View::DrawArm2(CDC* pDC)
{
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	Translate(pDC, 178, 54, false);
	Translate(pDC, 34, 31, false);
	Rotate(pDC, -A1, false);
	Translate(pDC, -34, -31, false);
	Translate(pDC, 187, 41, false);
	Translate(pDC, 23, 61, false);
	Rotate(pDC, 180.0 - A2, false);
	Translate(pDC, -23, -61, false);
	DrawImgTransparent(pDC, arm2);
	pDC->SetWorldTransform(&xform);
}

void CGDI2019View::DrawLeg1(CDC* pDC)
{
	Translate(pDC, 30, 125, false);
	Rotate(pDC, -L1, false);
	Translate(pDC, -30, -125, false);
	DrawImgTransparent(pDC, leg1);
}

void CGDI2019View::DrawLeg2(CDC* pDC)
{
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	Translate(pDC, 35, 60, false);
	Rotate(pDC, L2, false);
	Translate(pDC, -35, -60, false);
	DrawImgTransparent(pDC, leg2);
	pDC->SetWorldTransform(&xform);
}

void CGDI2019View::DrawBody(CDC* pDC)
{
	Translate(pDC, 26, 133, false);
	Rotate(pDC, -B, false);
	Translate(pDC, -26, -133, false);
	DrawImgTransparent(pDC, body);
}

void CGDI2019View::DrawTransformer(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	Translate(pDC, 200, 500, false);
	DrawLeg2(pDC);
	Translate(pDC, 5, -65, false);
	DrawLeg1(pDC);
	Translate(pDC, 212, -7, false);
	DrawBody(pDC);
	DrawArm2(pDC);
	Translate(pDC, 178, 54, false);
	DrawArm1(pDC);
}


//===================================================================================================
//========================================== O N - D R A W ==========================================
//===================================================================================================


void CGDI2019View::OnDraw(CDC* pDC)
{
	CGDI2019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect window;
	GetClientRect(&window);
	CDC* memDC = new CDC;
	memDC->CreateCompatibleDC(pDC);
	CBitmap img;
	img.CreateCompatibleBitmap(pDC, window.Width(), window.Height());
	memDC->SelectObject(img);
	
	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
	
	XFORM form;
	memDC->GetWorldTransform(&form);

	DrawBackground(memDC, window);
	DrawTransformer(memDC);

	memDC->SetWorldTransform(&form);
	memDC->SetGraphicsMode(oldMode);

	pDC->BitBlt(0, 0, window.Width(), window.Height(), memDC, 0, 0, SRCCOPY);
}


// CGDI2019View printing

BOOL CGDI2019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDI2019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDI2019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDI2019View diagnostics

#ifdef _DEBUG
void CGDI2019View::AssertValid() const
{
	CView::AssertValid();
}

void CGDI2019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDI2019Doc* CGDI2019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDI2019Doc)));
	return (CGDI2019Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDI2019View message handlers


void CGDI2019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar) {
	case VK_LEFT:
		A1 += 15;
		if (A1 > 270) A1 = 270;
		A2 += 15;
		if (A2 > 270) A2 = 270;
		B -= 1;
		if (B < -20) B = -20;
		L1 += 5;
		if (L1 > 110) L1 = 110;
		L2 += 5;
		if (L2 > 110) L2 = 110;
		break;
	case VK_RIGHT:
		A1 -= 15;
		if (A1 < 0) A1 = 0;
		A2 -= 15;
		if (A2 < 0) A2 = 0;
		B += 1;
		if (B > 0) B = 0;
		L1 -= 5;
		if (L1 < 0) L1 = 0;
		L2 -= 5;
		if (L2 < 0) L2 = 0;
		break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CGDI2019View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}
