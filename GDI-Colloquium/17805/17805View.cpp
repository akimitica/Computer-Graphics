
// 17805View.cpp : implementation of the CMy17805View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "17805.h"
#endif

#include "17805Doc.h"
#include "17805View.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795
// CMy17805View

IMPLEMENT_DYNCREATE(CMy17805View, CView)

BEGIN_MESSAGE_MAP(CMy17805View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMy17805View construction/destruction

CMy17805View::CMy17805View() noexcept
{
	// TODO: add construction code here
	arm1 = new DImage();
	arm1s = new DImage();
	arm2 = new DImage();
	arm2s = new DImage();
	base = new DImage();
	bases = new DImage();
	head = new DImage();
	heads = new DImage();
	background = new DImage();

	arm1->Load(CString("res/arm1.png"));
	arm1s->Load(CString("res/arm1_shadow.png"));
	arm2->Load(CString("res/arm2.png"));
	arm2s->Load(CString("res/arm2_shadow.png"));
	base->Load(CString("res/base.png"));
	bases->Load(CString("res/base_shadow.png"));
	head->Load(CString("res/head.png"));
	heads->Load(CString("res/head_shadow.png"));
	background->Load(CString("res/pozadina.jpg"));

}

CMy17805View::~CMy17805View()
{
	delete arm1, arm2, base, head;
	delete arm1s, arm2s, bases, heads;
	delete background;
}

BOOL CMy17805View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy17805View drawing

//===================================================================================================
//============================ W O R L D - T R A N S F O R M A T I O N S ============================
//===================================================================================================


void CMy17805View::Translate(CDC* pDC, float dX, float dY, bool rightMul)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMy17805View::Rotate(CDC* pDC, float angle, bool rightMul)
{
	angle *= PI / 180;
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };;
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMy17805View::Scale(CDC* pDC, float sX, float sY, bool rightMul)
{

	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

//===================================================================================================
//========================================== D R A W I N G ==========================================
//===================================================================================================

void CMy17805View::DrawBackground(CDC* pDC)
{
	CRect window;
	GetClientRect(&window);
	CRect bkgr = CRect(0, 0, background->Width(), background->Height());
	background->Draw(pDC, bkgr, window);
}
void CMy17805View::DrawImgTransparent(CDC* pDC, DImage* img)
{
	unsigned char* bits = img->GetDIBBits();
	COLORREF bkg = COLORREF(RGB(bits[2], bits[1], bits[0]));
	CRect imgwin = CRect(0, 0, img->Width(), img->Height());
	img->DrawTransparent(pDC, imgwin, imgwin, bkg);
}


void CMy17805View::DrawLampBase(CDC* pDC, bool blsShadow)
{
	if (blsShadow)
	{
		DrawImgTransparent(pDC, base);
		Translate(pDC, base->Width() / 4.-35, -5, false);
	}
	else
	{
		Scale(pDC, 1, 0.25, false);
		Rotate(pDC, 90, false);
		DrawImgTransparent(pDC, bases);
		Translate(pDC, base->Width() / 4. -35, 15, false);
	}
}

void CMy17805View::DrawLampArm1(CDC* pDC, bool blsShadow)
{

	if (blsShadow)
	{
		Translate(pDC, 58, 61, false);
		Rotate(pDC, angle1, false);
		Translate(pDC, -58, -61, false);
		DrawImgTransparent(pDC, arm1);
		Translate(pDC, 309-37, 61-37, false);
	}
	else
	{
		Translate(pDC, 58, 61, false);
		Rotate(pDC, angle1, false);
		Translate(pDC, -58, -61, false);
		DrawImgTransparent(pDC, arm1s);
		Translate(pDC, 309 - 37, 61 - 37, false);

	}
}

void CMy17805View::DrawLampArm2(CDC* pDC, bool blsShadow)
{

	if (blsShadow)
	{
		Translate(pDC, 36, 40, false);
		Rotate(pDC, angle2, false);
		Translate(pDC, -36, -40, false);
		Translate(pDC, 272-30, 40-100, false);
	}
	else
	{

		Translate(pDC, 36, 40, false);
		Rotate(pDC, angle2, false);
		Translate(pDC, -36, -40, false);
		Translate(pDC, 272 - 30, 40 - 100, false);
	}
}

void CMy17805View::DrawLampHead(CDC* pDC, bool blsShadow)
{

	if (blsShadow)
	{
		Translate(pDC, 30, 100, false);
		Rotate(pDC, hangle, false);
		Translate(pDC, -30, -100, false);
		DrawImgTransparent(pDC, head);
		Translate(pDC, 30, 100, false);
		Rotate(pDC, -hangle, false);
		Translate(pDC, -30, -100, false);
		Translate(pDC, -(272 - 30), -(40 - 100), false);
		DrawImgTransparent(pDC, arm2);
	}
	else
	{

		Translate(pDC, 30, 100, false);
		Rotate(pDC, hangle, false);
		Translate(pDC, -30, -100, false);
		DrawImgTransparent(pDC, heads);
		Translate(pDC, 30, 100, false);
		Rotate(pDC, -hangle, false);
		Translate(pDC, -30, -100, false);
		Translate(pDC, -(272 - 30), -(40 - 100), false);
		DrawImgTransparent(pDC, arm2s);
	}
}

void CMy17805View::DrawLamp(CDC* pDC, bool blsShadow)
{

	if (blsShadow)
	{
		DrawLampBase(pDC, true);
		DrawLampArm1(pDC, true);
		DrawLampArm2(pDC, true);
		DrawLampHead(pDC, true);
	}
	else
	{
		DrawLampBase(pDC, false);
		DrawLampArm1(pDC, false);
		DrawLampArm2(pDC, false);
		DrawLampHead(pDC, false);
	}
}

void CMy17805View::DrawLampShadow(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	DrawLamp(pDC, false);
	pDC->SetWorldTransform(&old);
	
}

void CMy17805View::OnDraw(CDC* pDC)
{
	CMy17805Doc* pDoc = GetDocument();
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
	memDC->SetGraphicsMode(GM_ADVANCED);

	DrawBackground(memDC);
	XFORM old;
	memDC->GetWorldTransform(&old);
	Translate(memDC, 500, 450, false);
	Translate(memDC, 150, 50, false);
	DrawLampShadow(memDC);
	memDC->SetWorldTransform(&old);
	Translate(memDC, 500, 450, false);
	DrawLamp(memDC, true);
	Translate(memDC, -500, -450, false);
	memDC->SetWorldTransform(&old);

	pDC->BitBlt(0, 0, window.Width(), window.Height(), memDC, 0, 0, SRCCOPY);
}


// CMy17805View printing

BOOL CMy17805View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy17805View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy17805View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMy17805View diagnostics

#ifdef _DEBUG
void CMy17805View::AssertValid() const
{
	CView::AssertValid();
}

void CMy17805View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy17805Doc* CMy17805View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy17805Doc)));
	return (CMy17805Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy17805View message handlers


BOOL CMy17805View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


void CMy17805View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch(nChar)
	{
	case '1':
		angle1 -= 10;
		break;
	case '2':
		angle1 += 10;
		break;
	case '3':
		angle2 -= 10;
		break;
	case '4':
		angle2 += 10;
		break;
	case '5':
		hangle -= 10;
		break;
	case '6':
		hangle += 10;
		break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
