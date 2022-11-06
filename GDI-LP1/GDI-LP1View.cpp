
// GDI-LP1View.cpp : implementation of the CGDILP1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-LP1.h"
#endif

#include "GDI-LP1Doc.h"
#include "GDI-LP1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <cmath>



// CGDILP1View

IMPLEMENT_DYNCREATE(CGDILP1View, CView)

BEGIN_MESSAGE_MAP(CGDILP1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGDILP1View construction/destruction

CGDILP1View::CGDILP1View() noexcept
{
	// TODO: add construction code here
	keyPressed = false;
}

CGDILP1View::~CGDILP1View()
{
}

BOOL CGDILP1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDILP1View drawing

void CGDILP1View::OnDraw(CDC* pDC)
{
	CGDILP1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	
	/*	C O L O U R - C O D E S - U S E D
	* RED			RGB(255,0,0)
	* ORANGE		RGB(255,153,51)
	* GREEN			RGB(50,205,50)	
	* GREY			RGB(221,221,221)
	* YELLOW		RGB(255,255,0)
	* PURPLE		RGB(153,0,204)
	* BLUE			RGB(102,153,255)
	* PINK			RGB(255,153,204)
	* GRID GREY		RGB()
	*/

	//===================================================================================
	//=============================== B A C K G R O U N D =============================== 
	//===================================================================================
	
	
	CRect rect;
	GetClientRect(&rect);
	//pDC->SetROP2(R2_MASKPEN);

	//pDC->SetMapMode(MM_ISOTROPIC);
	/*pDC->SetWindowExt(500, -500);
	pDC->SetViewportExt(500, 500);	//(rect.right, rect.bottom);	Nemam pojma zasto ovo nije radilo
	pDC->SetWindowOrg(-500, 500);*/
	CBrush* brush = new CBrush(RGB(211, 211, 211));
	CBrush* oldBrush = pDC->SelectObject(brush);
	//brush.UnrealizeObject();
	//pDC->SetBrushOrg(-250, 250);

	//CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(0, 500, 500, 0);
	pDC->SelectObject(oldBrush);
	delete brush;
	//pDC->SetBkMode(TRANSPARENT);

	//===================================================================================
	//=============================== Y E L L O W - P E N ===============================
	//===================================================================================
	 

	LOGBRUSH logBrush;
	logBrush.lbStyle = PS_SOLID;
	logBrush.lbColor = RGB(255, 255, 0);
	CPen* yellowPen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 5, &logBrush);
	CPen* oldYellowPen = pDC->SelectObject(yellowPen);
	
	
	//===================================================================================
	//========================== O R A N G E - T R I A N G L E ==========================
	//===================================================================================


	CBrush* orBrush = new CBrush(RGB(255, 153, 51));
	DrawTriangle(pDC,
		CPoint(1.25 * 25, 10 * 25),
		CPoint(5.0 * 25, 6.25 * 25),
		CPoint(5.0 * 25, 13.75 * 25),
		orBrush);
	delete orBrush;
	
	
	//===================================================================================
	//=============================== R E D - S Q U A R E ===============================
	//===================================================================================

	
	CBrush* rBrush = new CBrush(RGB(255, 0, 0));
	DrawTetragon(pDC,
		CPoint(5.0 * 25, 11.25 * 25),
		CPoint(7.50 * 25, 11.25 * 25),
		CPoint(7.50 * 25, 8.75 * 25),
		CPoint(5.0 * 25, 8.75 * 25),
		rBrush);
	delete rBrush;
	
	
	//===================================================================================
	//=========================== G R E E N - T R I A N G L E ===========================
	//===================================================================================


	CBrush* gBrush = new CBrush(RGB(50, 205, 50));
	DrawTriangle(pDC,
		CPoint(7.50 * 25, 11.25 * 25),
		CPoint(7.50 * 25, 8.75 * 25),
		CPoint(10 * 25, 11.25 * 25),
		gBrush);
	delete gBrush;


	//===================================================================================
	//============================ P I N K - T R I A N G L E ============================
	//===================================================================================


	CBrush* pBrush = new CBrush(RGB(255, 153, 204));
	DrawTriangle(pDC,
		CPoint(7.50 * 25, 8.75 * 25),
		CPoint(10 * 25, 11.25 * 25),
		CPoint(12.50 * 25, 8.75 * 25),
		pBrush);
	delete pBrush;


	//===================================================================================
	//========================== Y E L L O W - T E T R A G O N ==========================
	//===================================================================================

	
	CBrush* yBrush = new CBrush(RGB(255, 255, 0));
	DrawTetragon(pDC,
		CPoint(10 * 25, 11.25 * 25),
		CPoint(12.50 * 25, 8.75 * 25),
		CPoint(15 * 25, 8.75 * 25),
		CPoint(12.50 * 25, 11.25 * 25),
		yBrush);
	delete yBrush;


	//===================================================================================
	//========================= S T R I P E D - T R I A N G L E =========================
	//===================================================================================


	CBrush* strBrush = new CBrush(HS_HORIZONTAL, RGB(102, 153, 255));
	DrawTriangle(pDC,
		CPoint(12.50 * 25, 11.25 * 25),
		CPoint(15 * 25, 8.75 * 25),
		CPoint(15 * 25, 11.25 * 25),
		strBrush);
	delete strBrush;


	//===================================================================================
	//========================== P U R P L E - T R I A N G L E ==========================
	//===================================================================================


	CBrush* prBrush = new CBrush(RGB(153, 0, 204));
	DrawTriangle(pDC,
		CPoint(15 * 25, 6.25 * 25),
		CPoint(15 * 25, 13.75 * 25),
		CPoint(18.75 * 25, 10 * 25),
		prBrush);
	delete prBrush;


	//===================================================================================
	//================================= P O L Y G O N S =================================
	//===================================================================================


	pDC->SelectObject(oldYellowPen);
	yellowPen->DeleteObject();
	yellowPen = new CPen(PS_SOLID, 3, RGB(255, 255, 0));
	pDC->SelectObject(yellowPen);

	DrawRegularPolygon(pDC, 3.25*25, 10*25, 0.75*25, 6, 0);	//(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
	DrawRegularPolygon(pDC, 10 * 25, 9.75 * 25, 0.6 * 25, 4, 0);
	DrawRegularPolygon(pDC, 14.25 * 25, 10.5 * 25, 0.5 * 25, 7, 0);
	DrawRegularPolygon(pDC, 8.25 * 25, 10.5 * 25, 0.5 * 25, 8, 0);
	DrawRegularPolygon(pDC, 16.75 * 25, 10 * 25, 0.9 * 25, 5, 0);


	//===================================================================================
	//===================================== G R I D =====================================
	//===================================================================================


	if (keyPressed)
	{
		CPen* gridPen = new CPen(PS_DASH,2, RGB(250, 250, 250));
		CPen* oldPen = pDC->SelectObject(gridPen);
		int oldMod = pDC->SetROP2(R2_MERGEPEN);
	
		for (int i = 0; i < 21; i++)
		{
			pDC->MoveTo(i * 25, 0);
			pDC->LineTo(i * 25, 500);


			pDC->MoveTo(0, i * 25);
			pDC->LineTo(500, i * 25);
		}

		pDC->SelectObject(oldPen);
		pDC->SetROP2(oldMod);
	}
}

//Used to draw triangles

void CGDILP1View::DrawTriangle(CDC* pDC, CPoint& p1, CPoint& p2, CPoint& p3, CBrush* brush)
{
	CBrush* oldBrush = pDC->SelectObject(brush);

	CPoint* triangle = new CPoint[3];
	triangle[0] = p1;
	triangle[1] = p2;
	triangle[2] = p3;
	pDC->Polygon(triangle, 3);
	delete[] triangle;

	pDC->SelectObject(oldBrush);
}


//Used to draw tetragons

void CGDILP1View::DrawTetragon(CDC* pDC, CPoint& p1, CPoint& p2, CPoint& p3, CPoint& p4, CBrush* brush)
{
	CBrush* oldBrush = pDC->SelectObject(brush);

	CPoint* tetragon = new CPoint[4];
	tetragon[0] = p1;
	tetragon[1] = p2;
	tetragon[2] = p3;
	tetragon[3] = p4;
	pDC->Polygon(tetragon, 4);
	delete[] tetragon;

	pDC->SelectObject(oldBrush);

}


void CGDILP1View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	pDC->SelectStockObject(NULL_BRUSH);
	double dAngle = 2 * 3.1415926535897932384626433832795 / (double)n;
	double angle = rotAngle;
	CPoint* polygon = new CPoint[n];
	for (int i = 0; i < n; i++)
	{
		polygon[i].x = cx + r * cos(angle);
		polygon[i].y = cy + r * sin(angle);
		angle += dAngle;
	}

	pDC->Polygon(polygon, n);
	delete[] polygon;
}


double CGDILP1View::getX(int radius, float angle)
{
	return double(radius * std::cosf(angle));
}



double CGDILP1View::GetY(int radius, float angle)
{
	return double(radius * std::sinf(angle));
}


// CGDILP1View printing

BOOL CGDILP1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDILP1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDILP1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDILP1View diagnostics

#ifdef _DEBUG
void CGDILP1View::AssertValid() const
{
	CView::AssertValid();
}

void CGDILP1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDILP1Doc* CGDILP1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDILP1Doc)));
	return (CGDILP1Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDILP1View message handlers


void CGDILP1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if (nChar)
	{
		this->keyPressed = !this->keyPressed;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
