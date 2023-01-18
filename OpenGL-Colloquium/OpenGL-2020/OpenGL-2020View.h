
// OpenGL-2020View.h : interface of the COpenGL2020View class
//

#pragma once
#include "GLRenderer.h"


class COpenGL2020View : public CView
{
protected: // create from serialization only
	COpenGL2020View() noexcept;
	DECLARE_DYNCREATE(COpenGL2020View)

// Attributes
public:
	COpenGL2020Doc* GetDocument() const;

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

// Implementation
public:
	virtual ~COpenGL2020View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in OpenGL-2020View.cpp
inline COpenGL2020Doc* COpenGL2020View::GetDocument() const
   { return reinterpret_cast<COpenGL2020Doc*>(m_pDocument); }
#endif

