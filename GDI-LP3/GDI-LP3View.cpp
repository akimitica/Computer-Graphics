
// GDI-LP3View.cpp : implementation of the CGDILP3View class
//

#include "pch.h"
#include "framework.h"
#include "DImage.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI-LP3.h"
#endif

#include "GDI-LP3Doc.h"
#include "GDI-LP3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SQUARE 25
#define PI 3.1415926535897932384626433832795

// CGDILP3View

IMPLEMENT_DYNCREATE(CGDILP3View, CView)

BEGIN_MESSAGE_MAP(CGDILP3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGDILP3View construction/destruction

CGDILP3View::CGDILP3View() noexcept
{
	// TODO: add construction code here
	pieces = new DImage * [3];
	for (int i = 0; i < 3; i++)
		pieces[i] = new DImage[3];

	pieces[0][0].Load(CString("Bitmaps/download.dib"));
	pieces[0][1].Load(CString("Bitmaps/download8.dib"));
	pieces[0][2].Load(CString("Bitmaps/download3.dib"));
	pieces[1][0].Load(CString("Bitmaps/download5.dib"));
	pieces[1][1].Load(CString("Bitmaps/download1.dib"));
	pieces[1][2].Load(CString("Bitmaps/download6.dib"));
	pieces[2][0].Load(CString("Bitmaps/download7.dib"));
	pieces[2][1].Load(CString("Bitmaps/download2.dib"));
	pieces[2][2].Load(CString("Bitmaps/download4.dib"));

	/*for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			PaintItGrey(pieces[i][j]);
	}*/


}

CGDILP3View::~CGDILP3View()
{
	for (int i = 0; i < 3; i++)
		delete[] pieces[i];
	delete[] pieces;
}

BOOL CGDILP3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDILP3View drawing

void CGDILP3View::DrawBackground(CDC* pDC)
{
	CBrush* brush = new CBrush(RGB(255, 255, 255));
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
	
	CPen* grid = new CPen(PS_SOLID, 2, RGB(175, 175, 175));
	pDC->SelectObject(grid);

	for (int i = 1; i < 20; i++)
	{
		pDC->MoveTo(0, i * SQUARE);
		pDC->LineTo(500, i * SQUARE);
		pDC->MoveTo(i * SQUARE, 0);
		pDC->LineTo(i * SQUARE, 500);
	}
	pDC->SelectObject(oldPen);
	delete grid;
	
}

CRect* CGDILP3View::SetRegion(CDC* pDC)
{
	CRect res;
	pDC->GetClipBox(res);
	CRgn region;
	region.CreateRectRgn(0, 0, 500, 500);
	pDC->SelectClipRgn(&region);
	return &res;
}



//=============================================================================
//============= W O R L D - T R A N S F O R M - F U N C T I O N S =============
//=============================================================================


void CGDILP3View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


void CGDILP3View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


void CGDILP3View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDILP3View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM form = { mx ? -1 : 1, 0, 0, my ? -1 : 1, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

float CGDILP3View::DegToRad(float angle) { return angle * (2 * PI) / 360; }


//=================================================================================================
//=============================== D R A W I N G - F U N C T I O N S ===============================
//=================================================================================================


void CGDILP3View::PaintItGrey(DImage img)
{
	DWORD sizeOfPicture = img.Width() * img.Height() * img.BPP();
	unsigned char* bits =img.GetDIBBits();
	int firstPixel[3];
	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img.BPP())
	{
		int pixel = 0;
		bool bg = true;

		for (int k = 0; k < 3; k++)
		{
			if (ptr == 0)
				firstPixel[k] = bits[k];
			else
				bg = (firstPixel[k] == bits[ptr + k]) && bg;
			pixel += bits[ptr + k];
		}
		pixel /= 3;

		if (pixel > 255)
			pixel = 255;

		for (int i = 0; i < 3 && !bg; i++)
			bits[ptr + i] = pixel;
	}
	img.Update();
		
}

void CGDILP3View::DrawTransparent(CDC* pDC, DImage* img, int x, int y)
{
	//Kreiramo masku 1 bit po pikselu monohromatska
	CBitmap mask;
	mask.CreateBitmap(img->Width(), img->Height(), 1, 1, NULL);
	//Kopiramo bajtove iz DImage u Bitmapu sa kojom radimo
	CBitmap image;
	image.CreateBitmap(img->Width(), img->Height(), 4, 8, NULL);
	image.SetBitmapBits(img->Width() * img->Height() * img->BPP(), img->GetDIBBits());

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* srcOldBitmap = srcDC->SelectObject(&image);
	CBitmap* dstOldBitmap = dstDC->SelectObject(&mask);

	//Pravimo masku
	COLORREF removeColor = srcDC->GetPixel(0, 0);
	COLORREF srcOldBckColor = srcDC->SetBkColor(removeColor);
	dstDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCCOPY);

	//Menjamo boje i za bk i text i odstranjujemo pozadinsku boju
	COLORREF srcOldTextColor = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, img->Width(), img->Height(), dstDC, 0, 0, SRCAND);

	dstDC->SelectObject(dstOldBitmap);
	delete dstDC;


	//Selektujemo masku i ANDUJEMO sa pDC, tj u nasem slucaju tu se prosledjuje memDC
	srcDC->SelectObject(&mask);
	pDC->BitBlt(x, y, img->Width(), img->Height(), srcDC, 0, 0, SRCAND);
	//I sad crtamo sliku tj OR
	srcDC->SelectObject(&image);
	pDC->BitBlt(x, y, img->Width(), img->Height(), srcDC, 0, 0, SRCPAINT);
	srcDC->SelectObject(srcOldBitmap);
	delete srcDC;
}

void CGDILP3View::OnDraw(CDC* pDC)
{
	CGDILP3Doc* pDoc = GetDocument();
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

	int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	memDC->GetWorldTransform(&oldForm);

	//DrawTransparent(memDC, &pieces[0][0], -22, 22);


	Translate(memDC, 128, 128, false);
	//Mirror(memDC, false, true, false);
	Rotate(memDC, DegToRad(115), false);
	Translate(memDC, -(128) - SQUARE, -(128) - SQUARE, false);
	DrawTransparent(memDC, &pieces[0][0], 0, 0);
	memDC->SetWorldTransform(&oldForm);


	memDC->SetGraphicsMode(oldMode);
	memDC->SelectClipRgn(&region);
	pDC->BitBlt(0, 0, 500, 500, memDC, 0, 0, SRCCOPY);

	delete memDC;
}


// CGDILP3View printing

BOOL CGDILP3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDILP3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDILP3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDILP3View diagnostics

#ifdef _DEBUG
void CGDILP3View::AssertValid() const
{
	CView::AssertValid();
}

void CGDILP3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDILP3Doc* CGDILP3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDILP3Doc)));
	return (CGDILP3Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDILP3View message handlers


BOOL CGDILP3View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}
