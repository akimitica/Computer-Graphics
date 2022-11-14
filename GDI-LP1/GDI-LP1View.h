
// GDI-LP1View.h : interface of the CGDILP1View class
//

#pragma once


class CGDILP1View : public CView
{
protected: // create from serialization only
	CGDILP1View() noexcept;
	bool keyPressed = false;
	DECLARE_DYNCREATE(CGDILP1View)

// Attributes
public:
	CGDILP1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	CRect* SetRegion(CDC* pDC);
	void DrawTriangle(CDC* pDC, CPoint& p1, CPoint& p2, CPoint& p3, CBrush* brush);
	void DrawTetragon(CDC* pDC, CPoint& p1, CPoint& p2, CPoint& p3, CPoint& p4, CBrush* brush);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawGrid(CDC* pDC);
	double getX(int radius, float angle);
	double GetY(int radius, float angle);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDILP1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in GDI-LP1View.cpp
inline CGDILP1Doc* CGDILP1View::GetDocument() const
   { return reinterpret_cast<CGDILP1Doc*>(m_pDocument); }
#endif

