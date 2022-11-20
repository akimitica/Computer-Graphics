
// GDI-2014View.h : interface of the CGDI2014View class
//

#pragma once
#include "DImage.h"
class CGDI2014CntrItem;

class CGDI2014View : public CView
{
protected: // create from serialization only
	CGDI2014View() noexcept;
	DECLARE_DYNCREATE(CGDI2014View)

// Attributes
public:
	CGDI2014Doc* GetDocument() const;
	// m_pSelection holds the selection to the current CGDI2014CntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CGDI2014CntrItem objects.  This selection
	//  mechanism is provided just to help you get started

	// TODO: replace this selection mechanism with one appropriate to your app
	CGDI2014CntrItem* m_pSelection;

	float angle;
	DImage* wheel;
	HENHMETAFILE clio;
	float gas;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawGround(CDC* pDC, float angle);
	void DrawCar(CDC* pDC, int x, int y, int w, int h);
	void DrawWheel(CDC* pDC, int x, int y, int w, float h);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	XFORM Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	XFORM Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	XFORM Rotate(CDC* pDC, float angle, bool rightMultiply);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support

// Implementation
public:
	virtual ~CGDI2014View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnFilePrint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in GDI-2014View.cpp
inline CGDI2014Doc* CGDI2014View::GetDocument() const
   { return reinterpret_cast<CGDI2014Doc*>(m_pDocument); }
#endif

