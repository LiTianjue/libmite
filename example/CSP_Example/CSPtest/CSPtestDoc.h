// CSPtestDoc.h : interface of the CCSPtestDoc class
// Download by http://www.codefans.net
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSPTESTDOC_H__D07D4D7D_6F6D_46D4_B64E_B2AF5964E778__INCLUDED_)
#define AFX_CSPTESTDOC_H__D07D4D7D_6F6D_46D4_B64E_B2AF5964E778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCSPtestDoc : public CDocument
{
protected: // create from serialization only
	CCSPtestDoc();
	DECLARE_DYNCREATE(CCSPtestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSPtestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCSPtestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCSPtestDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSPTESTDOC_H__D07D4D7D_6F6D_46D4_B64E_B2AF5964E778__INCLUDED_)
