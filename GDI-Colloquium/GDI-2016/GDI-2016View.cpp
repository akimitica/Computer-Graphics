
// GDI-2016View.cpp : implementation of the CGDI2016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-2016.h"
#endif

#include "GDI-2016Doc.h"
#include "GDI-2016View.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MOV 10
#define PI 3.1415926535897932384626433832795
#define DEGRAD 0.01745329251994329576923690768489
// CGDI2016View

IMPLEMENT_DYNCREATE(CGDI2016View, CView)

BEGIN_MESSAGE_MAP(CGDI2016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGDI2016View construction/destruction

CGDI2016View::CGDI2016View() noexcept
{
	// TODO: add construction code here

	table = new DImage * [2];
	for (int i = 0; i < 2; i++)
		table[i] = new DImage();

	table[0]->Load(CString("res/wood.jpg"));
	table[1]->Load(CString("res/felt2.jpg"));

	ballDistX = 0;
	ballDistY = 0;
	x = 1; y = 1;
	queueDist = 100;
	angle = 0;
}

CGDI2016View::~CGDI2016View()
{
	for (int i = 0; i < 2; i++)
		delete[] table[i];
	delete[] table;
}

BOOL CGDI2016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


//=================================================================================
//=================== W O R L D - T R A N S F O R M A T I O N S ===================
//=================================================================================


XFORM CGDI2016View::Translate(CDC* pDC, int dX, int dY, bool rightMul)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

	return oldForm;
}

XFORM CGDI2016View::Rotate(CDC* pDC, float angle, bool rightMul)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	XFORM form = { cos(DEGRAD * angle), sin(DEGRAD * angle), -sin(DEGRAD * angle), cos(DEGRAD * angle), 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
	return oldForm;
}


//=================================================================================
//======================= D R A W I N G - F U N C T I O N S =======================
//=================================================================================

void CGDI2016View::DrawStick(CDC* pDC, int w)
{
	CPen* ochre = new CPen(PS_SOLID, w / 100, RGB(245, 222, 173));
	CPen* brown = new CPen(PS_SOLID, w / 100, RGB(139, 69, 19));
	CPen* oldPen = pDC->SelectObject(ochre);

	pDC->MoveTo(-cos(DEGRAD * angle) * queueDist, sin(DEGRAD * angle) * queueDist);
	pDC->LineTo(-cos(DEGRAD * angle) * (queueDist + 2 * w / 3), sin(DEGRAD * angle) * (queueDist + 2 * w / 3));
	pDC->SelectObject(brown);
	pDC->LineTo(-cos(DEGRAD * angle) * (queueDist + w), sin(DEGRAD * angle) * (queueDist + w));
	pDC->SelectObject(oldPen);
	delete ochre, brown;
}

void CGDI2016View::DrawBall(CDC* pDC, int w)
{
	CBrush* ball = new CBrush(RGB(128, 0, 32));;
	CBrush* oldBrush = pDC->SelectObject(ball);

	pDC->Ellipse(cos(angle*DEGRAD)*ballDistX - w / 2, -sin(angle * DEGRAD) * ballDistY - w / 2, cos(angle * DEGRAD) * ballDistX + w / 2, -sin(angle * DEGRAD) * ballDistY + w / 2);

	CBrush* bela = new CBrush(RGB(255, 255, 255));
	pDC->SelectObject(bela);
	pDC->Ellipse(cos(angle * DEGRAD) * ballDistX - w / 5, -sin(angle * DEGRAD) * ballDistY - w / 2 + 3, cos(angle * DEGRAD) * ballDistX + w / 2 - 3, -sin(angle * DEGRAD) * ballDistY + w / 5);

	pDC->SelectObject(oldBrush);
	delete ball, bela;
}

void CGDI2016View::DrawTable(CDC* pDC, CRect rect)
{

}

void CGDI2016View::DrawBorder(CDC* pDC, CRect, int w)
{

}

void CGDI2016View::DrawHoles(CDC* pDC, CRect rect, int size)
{

	pDC->Ellipse(-size / 2, -size / 2, size / 2, size / 2);

}

void CGDI2016View::OnDraw(CDC* pDC)
{
	CGDI2016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect window;
	GetClientRect(&window);
	width = window.Width();
	height = window.Height();
	start = (500, 500);
	CDC* memDC = new CDC();

	memDC->CreateCompatibleDC(pDC);
	CBitmap img;
	img.CreateCompatibleBitmap(pDC, window.Width(), window.Height());
	memDC->SelectObject(img);

	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm = Translate(memDC, start.x, 500, false);
	DrawStick(memDC, 500);
	DrawBall(memDC, 25);
	Translate(memDC, -start.x, -500, false);
	memDC->SetWorldTransform(&oldForm);

	memDC->SetGraphicsMode(oldMode);
	pDC->BitBlt(0, 0, window.Width(), window.Height(), memDC, 0, 0, SRCCOPY);
	delete memDC;
}


// CGDI2016View printing

BOOL CGDI2016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDI2016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDI2016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDI2016View diagnostics

#ifdef _DEBUG
void CGDI2016View::AssertValid() const
{
	CView::AssertValid();
}

void CGDI2016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDI2016Doc* CGDI2016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDI2016Doc)));
	return (CGDI2016Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDI2016View message handlers


BOOL CGDI2016View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE /*CView::OnEraseBkgnd(pDC)*/;
}


void CGDI2016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar)
	{
	case VK_LEFT:
		if (queueDist > 0)
			angle += 1;
		Invalidate();
		break;
	case VK_RIGHT:
		if (queueDist > 0)
			angle -= 1;
		Invalidate();
		break;
	case VK_UP:
		if (queueDist > 0)
		{
			queueDist -= 5;
			if (queueDist < 0) queueDist = 0;
			Invalidate();
		}
		else
		{
			ballDistX += x * 10;
			ballDistY += y * 10;
			if ((ballDistX * cos(angle * DEGRAD) > width - start.x) || (ballDistX * cos(angle * DEGRAD) < -start.x)) x = -x;
			if ((ballDistY * sin(angle * DEGRAD) > 500 && sin(angle * DEGRAD) * y > 0) || (ballDistY * sin(angle * DEGRAD) < 500 - height && sin(angle * DEGRAD) * y < 0)) y = -y;
			Invalidate();
			break;
		}

		break;
	case VK_DOWN:
		ballDistX = ballDistY = 0;
		x = 1;
		y = 1;
		queueDist = 100;
		Invalidate();
		break;


	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
