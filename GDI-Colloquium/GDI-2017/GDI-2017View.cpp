
// GDI-2017View.cpp : implementation of the CGDI2017View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-2017.h"
#endif

#include "DImage.h"
#include "GDI-2017Doc.h"
#include "GDI-2017View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795
// CGDI2017View

IMPLEMENT_DYNCREATE(CGDI2017View, CView)

BEGIN_MESSAGE_MAP(CGDI2017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGDI2017View construction/destruction

CGDI2017View::CGDI2017View() noexcept
{
	// TODO: add construction code here
	body = new DImage();
	leg1 = new DImage();
	leg2 = new DImage();
	leg3 = new DImage();
	back = new DImage();

	body->Load(CString("res/Body.png"));
	leg1->Load(CString("res/Leg1.png"));
	leg2->Load(CString("res/Leg2.png"));
	leg3->Load(CString("res/Leg3.png"));
	back->Load(CString("res/Back2.jpg"));
}

CGDI2017View::~CGDI2017View()
{
	delete body, leg1, leg2, leg3, back;
}

BOOL CGDI2017View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDI2017View drawing

//===========================================================================================
//======================== W O R L D - T R A N S F O R M A T I O N S ========================
//===========================================================================================


void CGDI2017View::LoadIdentity(CDC* pDC)
{
	XFORM form = { 1,0,0,1,0,0 };
	pDC->SetWorldTransform(&form);
}

void CGDI2017View::Translate(CDC* pDC, float dX, float dY, bool rightMul)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2017View::Scale(CDC* pDC, float sX, float sY, bool rightMul)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2017View::Rotate(CDC* pDC, float angle, bool rightMul)
{
	angle *= PI / 180;
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


//===========================================================================================
//============================ D R A W I N G - F U N C T I O N S ============================
//===========================================================================================


void CGDI2017View::DrawImgTransparent(CDC* pDC, DImage* img)
{
	unsigned char* bits = img->GetDIBBits();
	COLORREF bkg = COLORREF(RGB(bits[2], bits[1], bits[0]));
	CRect image = CRect(0, 0, img->Width(), img->Height());
	img->DrawTransparent(pDC, image, image, bkg);
}

void CGDI2017View::DrawScene(CDC* pDC, CRect rect)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	back->Draw(pDC, CRect(bkgPomX, bkgPomY, bkgPomX + rect.Width(), bkgPomY + rect.Height()), rect);

	Scale(pDC, size, size, false);
	Translate(pDC, 100 + movement, windoe.Height() / 5, false);
	DrawImgTransparent(pDC, body);

	int phase1 = (phase + 2) % 8;
	int angle[8] = { -20, -10, 0, 10, 20, 10, 0, -10 };
	DrawLeg(pDC, 0 + angle[phase], 52, 168);
	DrawLeg(pDC, 0 - angle[phase], 52, 168);
	DrawLeg(pDC, 0 + angle[phase1], 270, 168);
	DrawLeg(pDC, 0 - angle[phase1], 270, 168);
	pDC->SetWorldTransform(&oldForm);

}

void CGDI2017View::DrawLeg(CDC* pDC, double angle, double dX, double dY)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, dX, dY,false);
	Translate(pDC, 28, 28, false);
	Rotate(pDC, angle, false);
	Translate(pDC, -28, -28, false);

	DrawImgTransparent(pDC, leg1);

	Translate(pDC, 0, leg1->Height() - 28 - 14, false);
	Translate(pDC, 29, 29, false);
	Rotate(pDC, -angle, false);
	Translate(pDC, -29, -29, false);

	DrawImgTransparent(pDC, leg2);

	Translate(pDC, -30, leg2->Height() - 29, false);

	DrawImgTransparent(pDC, leg3);

	pDC->SetWorldTransform(&old);
}


//===========================================================================================
//====================================== O N - D R A W ======================================
//===========================================================================================


void CGDI2017View::OnDraw(CDC* pDC)
{
	CGDI2017Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CRect window;
	GetClientRect(&window);
	windoe = window;

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap img;
	img.CreateCompatibleBitmap(pDC, window.Width(), window.Height());
	memDC->SelectObject(img);
	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);

	DrawScene(memDC, window);

	pDC->BitBlt(0, 0, window.Width(), window.Height(), memDC, 0, 0, SRCCOPY);
}


// CGDI2017View printing

BOOL CGDI2017View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDI2017View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDI2017View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDI2017View diagnostics

#ifdef _DEBUG
void CGDI2017View::AssertValid() const
{
	CView::AssertValid();
}

void CGDI2017View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDI2017Doc* CGDI2017View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDI2017Doc)));
	return (CGDI2017Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDI2017View message handlers


BOOL CGDI2017View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}


void CGDI2017View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar) {
	case 'S':
		bkgPomY += 200;
		if (bkgPomY > back->Height() - windoe.Height()) bkgPomY = back->Height() - windoe.Height();
		break;
	case 'D':
		bkgPomX += 200;
		if (bkgPomX > back->Width() - windoe.Width()) bkgPomX = back->Width() - windoe.Width();
		break;
	case 'W':
		bkgPomY -= 200;
		if (bkgPomY < 0) bkgPomY = 0;
		break;
	case 'A':
		bkgPomX -= 200;
		if (bkgPomX < 0) bkgPomX = 0;
		break;
	case VK_LEFT:
		if (movement > 0)
		{
			movement -= 20;
			phase--;
			if (phase == 0) phase = 7;
		}
		phase = phase % 8;
		break;
	case VK_RIGHT:
		phase++;
		phase=phase % 8;
		movement += 20;
		break;

	case VK_UP:
		size += 0.1;
		break;
	case VK_DOWN:
		size -= 0.1;
		break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
