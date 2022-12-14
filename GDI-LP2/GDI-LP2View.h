
// GDI-LP2View.h : interface of the CGDILP2View class
//

#pragma once


class CGDILP2View : public CView
{
protected: // create from serialization only
	CGDILP2View() noexcept;
	DECLARE_DYNCREATE(CGDILP2View);

// Attributes
public:
	CGDILP2Doc* GetDocument() const;
	bool drawGrid;
	float angle0, angle1;
	const float angleStep = 5;
	HENHMETAFILE emf_cactus_light, emf_cactus;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	CRect* SetRegion(CDC* pDC);
	void DrawBackground(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	float DegToRad(float angle);
	float RadToDeg(float angle);
	void DrawFigure(CDC* pDC, HENHMETAFILE type, float sX, float sY, int tX, int tY, float rotAngle, bool rightMul);
	void DrawPot(CDC* pDC);
	void DrawCactus(CDC* pDC);
	void PrintName(CDC* pDC);
	void DrawGrid(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDILP2View();
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

#ifndef _DEBUG  // debug version in GDI-LP2View.cpp
inline CGDILP2Doc* CGDILP2View::GetDocument() const
   { return reinterpret_cast<CGDILP2Doc*>(m_pDocument); }
#endif

