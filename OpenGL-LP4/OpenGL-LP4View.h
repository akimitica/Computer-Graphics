
// OpenGL-LP4View.h : interface of the COpenGLLP4View class
//

#pragma once
#include "GLRenderer.h"

class COpenGLLP4View : public CView
{
protected: // create from serialization only
	COpenGLLP4View() noexcept;
	DECLARE_DYNCREATE(COpenGLLP4View)

// Attributes
public:
	COpenGLLP4Doc* GetDocument() const;

protected:
	CGLRenderer glDC;
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
	virtual ~COpenGLLP4View();
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in OpenGL-LP4View.cpp
inline COpenGLLP4Doc* COpenGLLP4View::GetDocument() const
   { return reinterpret_cast<COpenGLLP4Doc*>(m_pDocument); }
#endif

