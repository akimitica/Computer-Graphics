
// OpenGL-LP5View.h : interface of the COpenGLLP5View class
//

#pragma once
#include "GLRenderer.h"


class COpenGLLP5View : public CView
{
protected: // create from serialization only
	COpenGLLP5View() noexcept;
	DECLARE_DYNCREATE(COpenGLLP5View)

// Attributes
public:
	COpenGLLP5Doc* GetDocument() const;

protected:
	CGLRenderer glDC;
	bool isClicked;
	CPoint oldPoint = { 0,0 };
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

// Implementation
public:
	virtual ~COpenGLLP5View();
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
	afx_msg void OnDestroy();
	void OnInitialUpdate();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in OpenGL-LP5View.cpp
inline COpenGLLP5Doc* COpenGLLP5View::GetDocument() const
   { return reinterpret_cast<COpenGLLP5Doc*>(m_pDocument); }
#endif

