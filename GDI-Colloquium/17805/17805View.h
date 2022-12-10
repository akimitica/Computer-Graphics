
// 17805View.h : interface of the CMy17805View class
//

#pragma once
#include "DImage.h"

class CMy17805View : public CView
{
protected: // create from serialization only
	CMy17805View() noexcept;
	DECLARE_DYNCREATE(CMy17805View)

// Attributes
public:
	CMy17805Doc* GetDocument() const;
	DImage* arm1, * arm2, * base, * head;
	DImage* arm1s, * arm2s, * bases, * heads;
	DImage* background;
	float angle1 = -135, angle2 = 90, hangle = 0;
	float scx = 1, scy = 1;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void Translate(CDC* pDC, float dX, float dY, bool rightMul);
	void Rotate(CDC* pDC, float angle, bool rightMul);
	void Scale(CDC* pDC, float sX, float sY, bool rightMul);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* img);
	void DrawLampBase(CDC* pDC, bool blsShadow);
	void DrawLampArm1(CDC* pDC, bool blsShadow);
	void DrawLampArm2(CDC* pDC, bool blsShadow);
	void DrawLampHead(CDC* pDC, bool blsShadow);
	void DrawLamp(CDC* pDC, bool blsShadow);
	void DrawLampShadow(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMy17805View();
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

#ifndef _DEBUG  // debug version in 17805View.cpp
inline CMy17805Doc* CMy17805View::GetDocument() const
   { return reinterpret_cast<CMy17805Doc*>(m_pDocument); }
#endif

