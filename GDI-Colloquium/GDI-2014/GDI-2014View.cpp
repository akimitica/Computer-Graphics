
// GDI-2014View.cpp : implementation of the CGDI2014View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-2014.h"
#endif

#include "GDI-2014Doc.h"
#include "CntrItem.h"
#include "resource.h"
#include "DImage.h"
#include "math.h"
#include "GDI-2014View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795
#define ANGMOV 10
#define DEGRAD 0.01745329251994329576923690768489

// CGDI2014View

IMPLEMENT_DYNCREATE(CGDI2014View, CView)

BEGIN_MESSAGE_MAP(CGDI2014View, CView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, &CGDI2014View::OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, &CGDI2014View::OnCancelEditCntr)
	ON_COMMAND(ID_FILE_PRINT, &CGDI2014View::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGDI2014View construction/destruction

CGDI2014View::CGDI2014View() noexcept
{
	m_pSelection = nullptr;
	// TODO: add construction code here
	angle = 0;
	wheel = new DImage();
	wheel->Load(CString("res/Wheel.png"));
	clio = GetEnhMetaFile(CString("res/clio.emf"));
	gas = 0;
}

CGDI2014View::~CGDI2014View()
{
	delete wheel;
	DeleteEnhMetaFile(clio);
}

BOOL CGDI2014View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


//=============================================================================================
//============================= D R A W I N G - F U N C T I O N S =============================
//=============================================================================================


void CGDI2014View::DrawGround(CDC* pDC, float angle)
{
	CBrush* nebo = new CBrush(RGB(200, 200, 255));
	CBrush* oldBrush = pDC->SelectObject(nebo);
	CPen* pen = new CPen(PS_NULL, 5, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(pen);
	CRect window;
	GetClientRect(&window);
	CRect* rect = new CRect(0, 0, window.Width(), window.Height());
	pDC->Rectangle(rect);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	delete rect, nebo, pen;

	CPen* pen2 = new CPen(PS_SOLID, 2, RGB(255, 0, 0));
	CBrush* patos = new CBrush(BS_HATCHED, RGB(112, 131, 76));

	pDC->SelectObject(patos);
	pDC->SelectObject(pen2);
	COLORREF oldcolor = pDC->SetBkColor(RGB(255, 242, 189));

	CPoint trougao[3];
	trougao[0] = CPoint(0, window.Height());
	trougao[1] = CPoint(window.Width(), window.Height());
	trougao[2] = CPoint(window.Width(), window.Height() - tan(angle * DEGRAD) * window.Width());

	pDC->Polygon(trougao, 3);
	pDC->SetBkColor(oldcolor);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	delete patos, pen2;


}


void CGDI2014View::DrawCar(CDC* pDC, int x, int y, int w, int h)
{

}


void CGDI2014View::DrawWheel(CDC* pDC, int x, int y, int w, int h)
{

}


void CGDI2014View::OnDraw(CDC* pDC)
{
	if (!pDC)
		return;

	CGDI2014Doc* pDoc = GetDocument();
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

	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
	DrawGround(memDC, angle);

	memDC->SetGraphicsMode(oldMode);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	delete memDC;
}

void CGDI2014View::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	// TODO: remove this code when final selection model code is written
	m_pSelection = nullptr;    // initialize selection

}


//=============================================================================================
//=============================== W O R L D - T R A N S F O R M ===============================
//=============================================================================================


XFORM CGDI2014View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = { 1 ,0,0,1,dX,dY };
	XFORM oldForm; 
	pDC->GetWorldTransform(&oldForm);
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
	return oldForm;
}

XFORM CGDI2014View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
	return oldForm;
}

XFORM CGDI2014View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
	return oldForm;
}

// CGDI2014View printing

BOOL CGDI2014View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDI2014View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDI2014View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGDI2014View::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != nullptr && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == nullptr);
   }
   CView::OnDestroy();
}



// OLE Client support and commands

BOOL CGDI2014View::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CGDI2014CntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced

	// TODO: implement this function that tests for a selected OLE client item

	return pDocItem == m_pSelection;
}

void CGDI2014View::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CGDI2014CntrItem object
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CGDI2014CntrItem* pItem = nullptr;
	TRY
	{
		// Create new item connected to this document
		CGDI2014Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CGDI2014CntrItem(pDoc);
		ASSERT_VALID(pItem);

		// Initialize the item from the dialog data
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // any exception will do
		ASSERT_VALID(pItem);

        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);
		// As an arbitrary user interface design, this sets the selection
		//  to the last item inserted

		// TODO: reimplement selection as appropriate for your application
		m_pSelection = pItem;   // set selection to last inserted item
		pDoc->UpdateAllViews(nullptr);
	}
	CATCH(CException, e)
	{
		if (pItem != nullptr)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation
void CGDI2014View::OnCancelEditCntr()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != nullptr)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == nullptr);
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place
void CGDI2014View::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != nullptr &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != nullptr)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CView::OnSetFocus(pOldWnd);
}

void CGDI2014View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != nullptr)
		pActiveItem->SetItemRects();
}

void CGDI2014View::OnFilePrint()
{
	//By default, we ask the Active document to print itself
	//using IOleCommandTarget. If you don't want this behavior
	//remove the call to COleDocObjectItem::DoDefaultPrinting.
	//If the call fails for some reason, we will try printing
	//the docobject using the IPrint interface.
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != nullptr);
	if (S_OK == COleDocObjectItem::DoDefaultPrinting(this, &printInfo))
		return;

	CView::OnFilePrint();

}



// CGDI2014View diagnostics

#ifdef _DEBUG
void CGDI2014View::AssertValid() const
{
	CView::AssertValid();
}

void CGDI2014View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDI2014Doc* CGDI2014View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDI2014Doc)));
	return (CGDI2014Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDI2014View message handlers


void CGDI2014View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar) {
	case VK_UP:
		if (angle < 80) angle += ANGMOV;
		if (angle > 80) angle = 80;
		Invalidate();
		break;
	case VK_DOWN:
		if (angle > -10) angle -= ANGMOV;
		if (angle < -10) angle = -10;
		Invalidate();
		break;
	case VK_LEFT:
		Invalidate();
		break;
	case VK_RIGHT:
		Invalidate();
		break;
	default:
		break;
	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CGDI2014View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE; //CView::OnEraseBkgnd(pDC);
}
