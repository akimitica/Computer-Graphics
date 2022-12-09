
// GDI-2017View.h : interface of the CGDI2017View class
//

#pragma once
#include "DImage.h"


class CGDI2017View : public CView
{
protected: // create from serialization only
	CGDI2017View() noexcept;
	DECLARE_DYNCREATE(CGDI2017View)

// Attributes
public:
	CGDI2017Doc* GetDocument() const;

	DImage* body, * leg1, * leg2, * leg3, * back;
	float b1, b2, l1, l2, l3;
	int bkgPomX = 0, bkgPomY = 0, phase = 0, movement = 0;
	float size = 1;
	CRect windoe;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void LoadIdentity(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMul);
	void Scale(CDC* pDC, float sX, float sY, bool rightMul);
	void Rotate(CDC* pDC, float angle, bool rightMul);
	void DrawImgTransparent(CDC* pDC, DImage* img);
	void DrawScene(CDC* pDC, CRect rect);
	void DrawLeg(CDC* pDC, double angle, double dX, double dY);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDI2017View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in GDI-2017View.cpp
inline CGDI2017Doc* CGDI2017View::GetDocument() const
   { return reinterpret_cast<CGDI2017Doc*>(m_pDocument); }
#endif

