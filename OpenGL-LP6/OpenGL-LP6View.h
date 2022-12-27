
// OpenGL-LP6View.h : interface of the COpenGLLP6View class
//

#pragma once
#include "CGLRenderer.h"


class COpenGLLP6View : public CView
{
protected: // create from serialization only
	COpenGLLP6View() noexcept;
	DECLARE_DYNCREATE(COpenGLLP6View)

// Attributes
public:
	COpenGLLP6Doc* GetDocument() const;

protected:
	CGLRenderer glDC;
	POINT mousePos;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	void OnInitialUpdate();

// Implementation
public:
	virtual ~COpenGLLP6View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in OpenGL-LP6View.cpp
inline COpenGLLP6Doc* COpenGLLP6View::GetDocument() const
   { return reinterpret_cast<COpenGLLP6Doc*>(m_pDocument); }
#endif

