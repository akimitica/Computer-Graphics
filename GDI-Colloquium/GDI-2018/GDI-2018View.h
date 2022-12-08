
// GDI-2018View.h : interface of the CGDI2018View class
//

#pragma once
#include "DImage.h"

class CGDI2018View : public CView
{
protected: // create from serialization only
	CGDI2018View() noexcept;
	DECLARE_DYNCREATE(CGDI2018View)

// Attributes
public:
	CGDI2018Doc* GetDocument() const;
	DImage* bagger, * arm1, * arm2, *bkg;
	HENHMETAFILE fork;
	float ugaoArm1 = 0, ugaoArm2 = 0, ugaoFork = 0, gas = 0;
	ENHMETAHEADER header_fork;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void Translate(CDC* pDC, float dx, float dy, bool rightMul);
	void Scale(CDC* pDC, float sx, float sy, bool rightMul);
	void Rotate(CDC* pDC, double angle, bool rightMul);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawExcavator(CDC* pDC);
	void DrawBody(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDI2018View();
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

#ifndef _DEBUG  // debug version in GDI-2018View.cpp
inline CGDI2018Doc* CGDI2018View::GetDocument() const
   { return reinterpret_cast<CGDI2018Doc*>(m_pDocument); }
#endif

