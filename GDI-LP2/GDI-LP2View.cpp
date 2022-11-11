
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
	emf_cactus_light = GetEnhMetaFile(CString("cactus_part_light"));
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
	
	DrawBackground(memDC);
	DrawCactus(memDC);


	DrawPot(memDC);
	PrintName(memDC);
	if (drawGrid) DrawGrid(memDC);

	memDC->SelectClipRgn(&region); 	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	
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
	CRect* rect = new CRect(0, 0, 501, 501);
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

	SetTransformation0(pDC);
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus_light, CRect(250 - (30 / 1), 350, 250 + (30 / 1), 425));
	DrawMiddle(pDC);

	pDC->Ellipse(250 - 10, 350 - 10, 250 + 10, 350 + 10);
	pDC->Ellipse(250 - 10, 17 * 25 - 10, 250 + 10, 17 * 25 + 10);

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

	SetRotation(pDC, Pi / 2.0, 475, 25);

	CString* name = new CString("17805 Andrija Mitic");
	CFont* font = new CFont();
	font->CreateFontW(35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	CFont* oldFont = pDC->SelectObject(font);
	CPoint textPosition(475, 25);
	pDC->SetBkMode(TRANSPARENT);

	COLORREF oldColor = pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(textPosition.x + 1, textPosition.y + 1, *name);
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOutW(textPosition.x, textPosition.y, *name);

	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(OPAQUE);
	delete font;
	delete name;
	pDC->SetWorldTransform(&oldForm);
	pDC->SetGraphicsMode(oldMode);

}

void CGDILP2View::DrawMiddle(CDC* pDC)
{
	XFORM saveForm, form, oldForm;
	pDC->GetWorldTransform(&oldForm);
	//Postavljamo manju promenjivu rotaciju 
	SetTransformation1(pDC);

	PlayEnhMetaFile(pDC->m_hDC, emf_cactus_light, CRect(250 - (30 / 3), 275, 250 + 30 / 3, 350));
	pDC->GetWorldTransform(&saveForm);


	pDC->SetWorldTransform(&saveForm);
	pDC->SetWorldTransform(&oldForm);






	/*
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus_light, CRect(250 - (30 / 3), 275, 250 + (30 / 3), 350));
	pDC->GetWorldTransform(&saveForm);
	//Ovde smo pod uticajem obe promenjive transformacije

	//Prvo rotiramo za Pi/4
	const double angleStep4 = Pi / 4;

	SetRotation(pDC, angleStep4, 250, 275);

	//Gornji desni deo
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 200, 250 + (30 / 3), 275));
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 1), 125, 250 + (30 / 1), 200));
	pDC->Ellipse(250 - 10, 200 - 10, 250 + 10, 200 + 10);

	//Vracamo uticaj na samo promenjive transformacije
	pDC->SetWorldTransform(&saveForm);
	//Crtamo gornji deo
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 200, 250 + (30 / 3), 275));
	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 1), 125, 250 + (30 / 1), 200));
	pDC->Ellipse(250 - 10, 200 - 10, 250 + 10, 200 + 10);

	//Rotiramo za -Pi/4, a pod uticajem smo obe promenjive transformacije

	SetRotation(pDC, -angleStep4, 250, 275);

	PlayEnhMetaFile(pDC->m_hDC, emf_cactus, CRect(250 - (30 / 3), 200, 250 + (30 / 3), 275));

	//Vracamo uticaj na samo promenjive transformacije
	pDC->SetWorldTransform(&saveForm);
	//Crtamo krugove koji su ispred kaktusa i koji su pod uticajem obe promenjive transformacije
	pDC->Ellipse(250 - 10, 275 - 10, 250 + 10, 275 + 10);
	pDC->Ellipse(250 - 10, 200 - 10, 250 + 10, 200 + 10);

	pDC->SetWorldTransform(&oldForm);*/
}



void CGDILP2View::DrawLeft(CDC* pDC)
{

}



void CGDILP2View::DrawRight(CDC* pDC)
{

}
//void CGDILP2View::





void CGDILP2View::SetTransformation0(CDC* pDC)
{
	XFORM  form1;

	form1 = {
		cosf(angle0),
		sinf(angle0),
		-sinf(angle0),
		cosf(angle0),
		250,
		425
	};
	pDC->SetWorldTransform(&form1);

	form1 = {
		1,
		0,
		0,
		1,
		-250,
		-425
	};
	pDC->ModifyWorldTransform(&form1, MWT_LEFTMULTIPLY);
}
void CGDILP2View::SetTransformation1(CDC* pDC)
{
	XFORM form2;
	form2 = {
		cosf(angle1),
		sinf(angle1),
		-sinf(angle1),
		cosf(angle1),
		250,
		350
	};
	pDC->ModifyWorldTransform(&form2, MWT_LEFTMULTIPLY);
	form2 = {
		1,
		0,
		0,
		1,
		-250,
		-350
	};
	pDC->ModifyWorldTransform(&form2, MWT_LEFTMULTIPLY);
}
void CGDILP2View::SetRotation(CDC* pDC, float angle, float x, float y)
{
	XFORM form;
	form = {
		cosf(angle),
		sinf(angle),
		-sinf(angle),
		cosf(angle),
		x,
		y
	};
	pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
	form = {
		1,
		0,
		0,
		1,
		-x,
		-y
	};
	pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);
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
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


void CGDILP2View::DrawFigure(CDC* pDC)
{

}

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
