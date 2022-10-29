
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


// CGDILP1View

IMPLEMENT_DYNCREATE(CGDILP1View, CView)

BEGIN_MESSAGE_MAP(CGDILP1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
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

void CGDILP1View::OnDraw(CDC* /*pDC*/)
{
	CGDILP1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
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
