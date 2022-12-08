
// GDI-2018View.cpp : implementation of the CGDI2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-2018.h"
#endif

#include "GDI-2018Doc.h"
#include "GDI-2018View.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795

// CGDI2018View

IMPLEMENT_DYNCREATE(CGDI2018View, CView)

BEGIN_MESSAGE_MAP(CGDI2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGDI2018View construction/destruction

CGDI2018View::CGDI2018View() noexcept
{
	// TODO: add construction code here
	bagger = new DImage();
	arm1 = new DImage();
	arm2 = new DImage();
	bkg = new DImage();

	fork = GetEnhMetaFile(CString("res/viljuska.emf"));
	GetEnhMetaFileHeader(fork, sizeof(ENHMETAHEADER), &header_fork);

	bagger->Load(CString("res/bager.png"));
	arm1->Load(CString("res/arm1.png"));
	arm2->Load(CString("res/arm2.png"));
	bkg->Load(CString("res/pozadina.png"));

}

CGDI2018View::~CGDI2018View()
{
	delete[] bagger, arm1, arm2, bkg;
	DeleteEnhMetaFile(fork);
}

BOOL CGDI2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDI2018View drawing

//===================================================================================================
//============================ W O R L D - T R A N S F O R M A T I O N S ============================
//===================================================================================================

void CGDI2018View::Translate(CDC* pDC, float dx, float dy, bool rightMul)
{
	XFORM form = { 1, 0, 0, 1, dx, dy };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2018View::Scale(CDC* pDC, float sx, float sy, bool rightMul)
{
	XFORM form = { sx, 0, 0, sy, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDI2018View::Rotate(CDC* pDC, double angle, bool rightMul)
{
	angle *= PI / 180;
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}



//===================================================================================================
//================================ D R A W I N G - F U N C T I O N S ================================
//===================================================================================================



void CGDI2018View::DrawBackground(CDC* pDC)
{
	CRect window;
	GetClientRect(&window);
	bkg->Draw(pDC, CRect(0, 0, bkg->Width(), bkg->Height()), window);
}

void CGDI2018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	unsigned char* bits = pImage->GetDIBBits();
	COLORREF bg = COLORREF(RGB(bits[2], bits[1], bits[0]));
	CRect digga = CRect(0, 0, pImage->Width(), pImage->Height());
	pImage->DrawTransparent(pDC, digga, digga, bg);
}

void CGDI2018View::DrawArm1(CDC* pDC)
{
	Translate(pDC, 58, 61, false);
	Rotate(pDC, ugaoArm1, false);
	Translate(pDC, -58, -61, false);

	DrawImgTransparent(pDC, arm1);

	Translate(pDC, 309 - 39, 61 + 37, false);
	Rotate(pDC, -90, false);
}

void CGDI2018View::DrawArm2(CDC* pDC)
{
	Translate(pDC, 36, 40, false);
	Rotate(pDC, ugaoArm2, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, arm2);

	Translate(pDC, 272, 40, false);
	Rotate(pDC, -45, false);
}

void CGDI2018View::DrawFork(CDC* pDC)
{
	Translate(pDC, 0, 0, false);
	Rotate(pDC, ugaoFork, false);
	Translate(pDC, -0, -20, false);

	CRect forccRect = CRect(header_fork.rclBounds.left,
		header_fork.rclBounds.top,
		header_fork.rclBounds.right,
		header_fork.rclBounds.bottom
	);
	Scale(pDC, 2.5, 2.5, false);
	pDC->PlayMetaFile(fork, forccRect);
	Scale(pDC, -2.5, -2.5, false);
}

void CGDI2018View::DrawExcavator(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);

	Translate(pDC, client.Width() - 400 + gas, client.Height() - 300, false);
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}

void CGDI2018View::DrawBody(CDC* pDC)
{
	DrawImgTransparent(pDC, bagger);
	Translate(pDC, 0, 220, false);
	Rotate(pDC, -90, false);
}



//===================================================================================================
//========================================== O N - D R A W ==========================================
//===================================================================================================



void CGDI2018View::OnDraw(CDC* pDC)
{
	CGDI2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect window;
	GetClientRect(&window);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap img;
	img.CreateCompatibleBitmap(pDC, window.Width(), window.Height());
	memDC->SelectObject(img);

	memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM form;
	memDC->GetWorldTransform(&form);

	DrawBackground(memDC);
	DrawExcavator(memDC);

	memDC->SetWorldTransform(&form);
	pDC->BitBlt(0, 0, window.Width(), window.Height(), memDC, 0, 0, SRCCOPY);
}


// CGDI2018View printing

BOOL CGDI2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDI2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDI2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDI2018View diagnostics

#ifdef _DEBUG
void CGDI2018View::AssertValid() const
{
	CView::AssertValid();
}

void CGDI2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDI2018Doc* CGDI2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDI2018Doc)));
	return (CGDI2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDI2018View message handlers


BOOL CGDI2018View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}


void CGDI2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar) {
	case '1':
		ugaoArm1 -= 10;
		break;
	case '2':
		ugaoArm1 += 10;
		break;
	case '3':
		ugaoArm2 -= 10;
		break;
	case '4':
		ugaoArm2 += 10;
		break;
	case '5':
		ugaoFork -= 10;
		break;
	case '6':
		ugaoFork += 10;
		break;
	case VK_LEFT:
		gas -= 10;
		break;
	case VK_RIGHT:
		gas += 10;
		break;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
