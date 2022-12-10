
// GDI-2015View.h : interface of the CGDI2015View class
//

#pragma once
#include "DImage.h"


class CGDI2015View : public CView
{
protected: // create from serialization only
	CGDI2015View() noexcept;
	DECLARE_DYNCREATE(CGDI2015View)

// Attributes
public:
	CGDI2015Doc* GetDocument() const;
	DImage* background;
	float angle;
	int x = 0, y = 0;
	bool endGame = false;

// Operations
public:

// Overrides
public:
	void DrawBackground(CDC* pDC);
	void DrawPackman(CDC* pDC, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void Translate(CDC* pDC, float dX, float dY, bool rightMul);
	void Rotate(CDC* pDC, float angle, bool rightMul);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDI2015View();
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

#ifndef _DEBUG  // debug version in GDI-2015View.cpp
inline CGDI2015Doc* CGDI2015View::GetDocument() const
   { return reinterpret_cast<CGDI2015Doc*>(m_pDocument); }
#endif

