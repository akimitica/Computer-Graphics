
// CntrItem.h : interface of the CGDI2014CntrItem class
//

#pragma once

class CGDI2014Doc;
class CGDI2014View;

class CGDI2014CntrItem : public COleClientItem
{
	DECLARE_SERIAL(CGDI2014CntrItem)

// Constructors
public:
	CGDI2014CntrItem(CGDI2014Doc* pContainer = nullptr);
		// Note: pContainer is allowed to be null to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-null document pointer

// Attributes
public:
	CGDI2014Doc* GetDocument()
		{ return reinterpret_cast<CGDI2014Doc*>(COleClientItem::GetDocument()); }
	CGDI2014View* GetActiveView()
		{ return reinterpret_cast<CGDI2014View*>(COleClientItem::GetActiveView()); }

public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();

protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow);

// Implementation
public:
	~CGDI2014CntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

