
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
	* RED		RGB(255,0,0)
	* ORANGE	RGB(255,153,51)
	* GREEN		RGB(50,205,50)	
	* GREY		RGB(221,221,221)
	* YELLOW	RGB(255,255,0)
	* PURPLE	RGB(153,0,204)
	* BLUE		RGB(102,153,255)
	*/

	//===================================================================================
	//=============================== B A C K G R O U N D =============================== 
	//===================================================================================
	
	
	CRect rect;
	GetClientRect(&rect);

	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetWindowExt(500, -500);
	pDC->SetViewportExt(500, 500);	//(rect.right, rect.bottom);	Nemam pojma zasto ovo nije radilo
	pDC->SetWindowOrg(-500, 500);
	CBrush brush(RGB(211, 211, 211));
	brush.UnrealizeObject();
	pDC->SetBrushOrg(-250, 250);

	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(0, 0, -500, 500);
	pDC->SelectObject(oldBrush);
	brush.DeleteObject();
	pDC->SetBkMode(TRANSPARENT);

	//===================================================================================
	//=============================== Y E L L O W - P E N ===============================
	//===================================================================================
	 
	
	CPen* yellowPen = new CPen(PS_SOLID, 5, RGB(255, 255, 0));
	CPen* oldYellowPen = pDC->SelectObject(yellowPen);
	
	
	//===================================================================================
	//===================================== G R I D =====================================
	//===================================================================================
	
	
	
	
	
	//===================================================================================
	//===================================== G R I D =====================================
	//===================================================================================

	
	
	
	
	//===================================================================================
	//===================================== G R I D =====================================
	//===================================================================================


	if (keyPressed)
	{
		pDC->SetBkMode(OPAQUE);
		CBrush brush;
		brush.CreateHatchBrush(HS_CROSS, RGB(255, 255, 255));
		brush.UnrealizeObject();
		pDC->SetBrushOrg(0, 0);

		CBrush* pOldBrush = pDC->SelectObject(&brush);
		pDC->SetBkMode(TRANSPARENT);
		pDC->Rectangle(CRect(0, 0, -500, 500));
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
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
