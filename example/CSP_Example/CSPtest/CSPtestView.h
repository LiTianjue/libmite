// CSPtestView.h : interface of the CCSPtestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSPTESTVIEW_H__329A0052_976E_450D_8886_73B186D2BA38__INCLUDED_)
#define AFX_CSPTESTVIEW_H__329A0052_976E_450D_8886_73B186D2BA38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCSPtestView : public CView
{
protected: // create from serialization only
	CCSPtestView();
	DECLARE_DYNCREATE(CCSPtestView)

// Attributes
public:
	CCSPtestDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSPtestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCSPtestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCSPtestView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CSPtestView.cpp
inline CCSPtestDoc* CCSPtestView::GetDocument()
   { return (CCSPtestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSPTESTVIEW_H__329A0052_976E_450D_8886_73B186D2BA38__INCLUDED_)
