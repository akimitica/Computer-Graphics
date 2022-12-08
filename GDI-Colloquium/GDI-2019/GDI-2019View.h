
// GDI-2019View.h : interface of the CGDI2019View class
//

#pragma once
#include "DImage.h"


class CGDI2019View : public CView
{
protected: // create from serialization only
	CGDI2019View() noexcept;
	DECLARE_DYNCREATE(CGDI2019View)

// Attributes
public:
	CGDI2019Doc* GetDocument() const;
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnEraseBkgnd(CDC* pDC);
	DImage* arm1, * arm2, * leg1, * leg2, * body, * bkg;
	float A1 = 0, A2 = 0, L1 = 0, L2 = 0, B = 0;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void Translate(CDC* pDC, float dX, float dY, bool rightMul);
	void Scale(CDC* pDC, float sX, float sY, bool rightMul);
	void Rotate(CDC* pDC, float angle, bool rightMul);
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody(CDC* pDC);
	void DrawTransformer(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDI2019View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GDI-2019View.cpp
inline CGDI2019Doc* CGDI2019View::GetDocument() const
   { return reinterpret_cast<CGDI2019Doc*>(m_pDocument); }
#endif

