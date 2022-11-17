
// GDI-LP3View.h : interface of the CGDILP3View class
//

#pragma once
#include "DImage.h"


class CGDILP3View : public CView
{
protected: // create from serialization only
	CGDILP3View() noexcept;
	DECLARE_DYNCREATE(CGDILP3View)

// Attributes
public:
	CGDILP3Doc* GetDocument() const;

	DImage** pieces;
	float angle;
	float xx;

// Operations
public:

// Overrides
public:
	void DrawBackground(CDC* pDC);
	CRect* SetRegion(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply);
	float DegToRad(float angle);
	void PaintItGrey(DImage img);
	void DrawTransparent(CDC* pDC, DImage* img, int x, int y);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDILP3View();
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

#ifndef _DEBUG  // debug version in GDI-LP3View.cpp
inline CGDILP3Doc* CGDILP3View::GetDocument() const
   { return reinterpret_cast<CGDILP3Doc*>(m_pDocument); }
#endif

