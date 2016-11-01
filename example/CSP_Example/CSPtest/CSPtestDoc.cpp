// CSPtestDoc.cpp : implementation of the CCSPtestDoc class
// Download by http://www.codefans.net

#include "stdafx.h"
#include "CSPtest.h"

#include "CSPtestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSPtestDoc

IMPLEMENT_DYNCREATE(CCSPtestDoc, CDocument)

BEGIN_MESSAGE_MAP(CCSPtestDoc, CDocument)
	//{{AFX_MSG_MAP(CCSPtestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSPtestDoc construction/destruction

CCSPtestDoc::CCSPtestDoc()
{
	// TODO: add one-time construction code here

}

CCSPtestDoc::~CCSPtestDoc()
{
}

BOOL CCSPtestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCSPtestDoc serialization

void CCSPtestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCSPtestDoc diagnostics

#ifdef _DEBUG
void CCSPtestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCSPtestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCSPtestDoc commands
