
// GDI-LP2View.cpp : implementation of the CGDILP2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-LP2.h"
#endif

#include "GDI-LP2Doc.h"
#include "GDI-LP2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ANGLESTEP 0.08726646259971647884618453842443
#define PI 3.1415926535897932384626433832795
#define SQUARE 25
// CGDILP2View

IMPLEMENT_DYNCREATE(CGDILP2View, CView)

BEGIN_MESSAGE_MAP(CGDILP2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGDILP2View construction/destruction

CGDILP2View::CGDILP2View() noexcept
{
	// TODO: add construction code here
	emf_cactus = GetEnhMetaFile(CString("cactus_part.emf"));
	emf_cactus_light= GetEnhMetaFile(CString("cactus_part_light.emf"));
	angle0 = 0;
	angle1 = 0;
	drawGrid = false;
}

CGDILP2View::~CGDILP2View()
{
	DeleteEnhMetaFile(emf_cactus);
	DeleteEnhMetaFile(emf_cactus_light);
}

BOOL CGDILP2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDILP2View drawing

void CGDILP2View::OnDraw(CDC* pDC)
{
	CGDILP2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	
	CRect rect;
	GetClientRect(&rect);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap img;
	img.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC->SelectObject(img);

	CRgn region;
	region.CreateRectRgnIndirect(SetRegion(memDC));

	XFORM oldForm;

	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&oldForm);

	DrawBackground(memDC);
	

	Translate(memDC, 10 * SQUARE, 17 * SQUARE, false);
	Rotate(memDC, DegToRad(angle0), false);
	Translate(memDC, -10 * SQUARE, -17 * SQUARE, false);
	
	DrawCactus(memDC);
	
	memDC->SetWorldTransform(&oldForm);

	DrawPot(memDC);
	PrintName(memDC);
	if (drawGrid) DrawGrid(memDC);

	memDC->SelectClipRgn(&region); 	
	pDC->BitBlt(0, 0, 500, 500, memDC, 0, 0, SRCCOPY);
	
	delete memDC;
}



CRect* CGDILP2View::SetRegion(CDC* pDC)
{
	CRect res;
	pDC->GetClipBox(res);
	CRgn region;
	region.CreateRectRgn(0, 0, 500, 500);
	pDC->SelectClipRgn(&region);
	return &res;
}


void CGDILP2View::DrawBackground(CDC* pDC)
{
	CBrush* brush = new CBrush(RGB(135, 206, 235));
	CBrush* oldBrush = pDC->SelectObject(brush);
	CPen* pen = new CPen(PS_NULL, 0, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(pen);
	CRect* rect = new CRect(0, 0, 500, 500);
	pDC->Rectangle(rect);
	delete rect;
	pDC->SelectObject(oldBrush);
	delete brush;
	pDC->SelectObject(oldPen);
	delete pen;
}


void CGDILP2View::DrawPot(CDC* pDC)
{
	CBrush* pottyMcPottyface = new CBrush(RGB(222, 148, 0));
	CBrush* oldBrush = pDC->SelectObject(pottyMcPottyface);

	CPoint* baza = new CPoint[4]();
	baza[0] = CPoint(200, 450);
	baza[1] = CPoint(300, 450);
	baza[2] = CPoint(290, 500);
	baza[3] = CPoint(210, 500);
	pDC->Polygon(baza, 4);
	delete[] baza;

	pDC->Rectangle(190, 430, 310, 450);

	pDC->SelectObject(oldBrush);


}


void CGDILP2View::DrawCactus(CDC* pDC)
{
	CBrush* brush = new CBrush(RGB(0, 204, 0));
	CBrush* oldBrush = pDC->SelectObject(brush);
	XFORM oldForm;

	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->GetWorldTransform(&oldForm);


	//=========================================================================
	//======================== R I G H T - B R A N C H ========================
	//=========================================================================
	
	DrawFigure(pDC, emf_cactus, 1, 3, 11.4 * SQUARE, 11.8 * SQUARE, 45, false);
	DrawFigure(pDC, emf_cactus, 2, 3, 12.3 * SQUARE, 12 * SQUARE, -90, false);
	DrawFigure(pDC, emf_cactus, 2, 3, 16.4 * SQUARE, 9.3 * SQUARE, 45, false);
	DrawFigure(pDC, emf_cactus, 2, 3, 17.1 * SQUARE, 12.9 * SQUARE, 135, false);
	Translate(pDC, 12.2 * SQUARE, 11.9 * SQUARE, false);
	Rotate(pDC, DegToRad(angle1), false);
	Translate(pDC, -12.2 * SQUARE, -11.9 * SQUARE, false);
	DrawFigure(pDC, emf_cactus_light, 2, 3, 11.2 * SQUARE, 8.8 * SQUARE, 0, false);
	pDC->SetWorldTransform(&oldForm);
	pDC->Ellipse(11.7 * SQUARE, 11.4 * SQUARE, 12.7 * SQUARE, 12.4 * SQUARE);
	pDC->Ellipse(14.8 * SQUARE, 11.4 * SQUARE, 15.8 * SQUARE, 12.4 * SQUARE);


	//=========================================================================
	//========================= L E F T - B R A N C H =========================
	//=========================================================================

	DrawFigure(pDC, emf_cactus, 1, 3, 7.6 * SQUARE, 11.8 * SQUARE, -45, false);
	DrawFigure(pDC, emf_cactus, 2, 3, 4.7 * SQUARE, 12 * SQUARE, -90, false);
	DrawFigure(pDC, emf_cactus, 2, 3, 6.8 * SQUARE, 8.8 * SQUARE, 0, false);
	DrawFigure(pDC, emf_cactus, 2.5, 3, 6.5 * SQUARE, 5.7 * SQUARE, 0, false);
	pDC->Ellipse(7.3 * SQUARE, 11.4 * SQUARE, 8.3 * SQUARE, 12.4 * SQUARE);
	pDC->Ellipse(7.2 * SQUARE, 8.3 * SQUARE, 8.2 * SQUARE, 9.3 * SQUARE);


	//=========================================================================
	//======================= M I D D L E - B R A N C H =======================
	//=========================================================================

	DrawFigure(pDC, emf_cactus_light, 2.5, 3, 8.75 * SQUARE, 14 * SQUARE, 0, false);
	DrawFigure(pDC, emf_cactus, 1, 3, 9.5 * SQUARE, 11 * SQUARE, 0, false);
	pDC->Ellipse(9.5 * SQUARE, 16.5 * SQUARE, 10.5 * SQUARE, 17.5 * SQUARE);
	pDC->Ellipse(9.5 * SQUARE, 13.5 * SQUARE, 10.5 * SQUARE, 14.5 * SQUARE);


	//=========================================================================
	//========================= W O R L D - R E S E T =========================
	//=========================================================================

	pDC->SetWorldTransform(&oldForm);
	pDC->SelectObject(oldBrush);
	delete brush;
	pDC->SetGraphicsMode(oldMode);
}


void CGDILP2View::PrintName(CDC* pDC)
{
	XFORM oldForm;
	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->GetWorldTransform(&oldForm);

	CString* name = new CString("17805 Andrija Mitic");
	CFont* font = new CFont();
	font->CreateFontW(35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	CFont* oldFont = pDC->SelectObject(font);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	
	Rotate(pDC, DegToRad(90.0), true);
	Translate(pDC, 475, 25, true);

	COLORREF oldColor = pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOut(1, 1, *name);
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOut(0, 0, *name);

	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(oldBkMode);
	delete font;
	delete name;
	pDC->SetWorldTransform(&oldForm);
	pDC->SetGraphicsMode(oldMode);

}


void CGDILP2View::DrawFigure(CDC* pDC, HENHMETAFILE cactus, float sX, float sY, int tX, int tY, float rotAngle, bool rightMul)
{
	XFORM oldXForm;
	pDC->GetWorldTransform(&oldXForm);

	Translate(pDC, tX + SQUARE / (float)2, tY + SQUARE / (float)2, rightMul);
	Rotate(pDC, DegToRad(rotAngle), rightMul);
	Translate(pDC, -SQUARE / (float)2, -SQUARE / (float)2, rightMul);
	Scale(pDC, sX, sY, rightMul);
	PlayEnhMetaFile(*pDC, cactus, CRect(0, 0, SQUARE, SQUARE));

	pDC->SetWorldTransform(&oldXForm);
}



void CGDILP2View::DrawGrid(CDC* pDC)
{
	int oldMode = pDC->SetROP2(R2_MERGEPEN);
	CPen* whPen = new CPen(PS_GEOMETRIC, 2, RGB(255, 255, 255));
	CPen* oldPen = pDC->SelectObject(whPen);
	for (int i = 1; i < 20; i++)
	{
		pDC->MoveTo(i * 25, 0);
		pDC->LineTo(i * 25, 500);
		pDC->MoveTo(0, i * 25);
		pDC->LineTo(500, i * 25);
	}
	pDC->SelectObject(oldPen);
	pDC->SetROP2(oldMode);
}



//=============================================================================
//============= W O R L D - T R A N S F O R M - F U N C T I O N S =============
//=============================================================================


void CGDILP2View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


void CGDILP2View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


void CGDILP2View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


float CGDILP2View::DegToRad(float angle) { return angle * (2 * PI) / 360; }
float CGDILP2View::RadToDeg(float angle) { return angle / (2 * PI) * 360; }

// CGDILP2View printing

BOOL CGDILP2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDILP2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDILP2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDILP2View diagnostics

#ifdef _DEBUG
void CGDILP2View::AssertValid() const
{
	CView::AssertValid();
}

void CGDILP2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDILP2Doc* CGDILP2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDILP2Doc)));
	return (CGDILP2Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDILP2View message handlers


void CGDILP2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar)
	{
	case VK_LEFT:
		angle0 -= angleStep;
		Invalidate();
		break;
	case VK_RIGHT:
		angle0 += angleStep;
		Invalidate();
		break;
	case VK_UP:
		angle1 -= angleStep;
		Invalidate();
		break;
	case VK_DOWN:
		angle1 += angleStep;
		Invalidate();
		break;
	default: 
		drawGrid = !drawGrid;
		Invalidate();
		break;

	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CGDILP2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}