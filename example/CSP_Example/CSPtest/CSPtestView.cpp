// CSPtestView.cpp : implementation of the CCSPtestView class
// Download by http://www.codefans.net

#include "stdafx.h"
#include "CSPtest.h"

#include "CSPtestDoc.h"
#include "CSPtestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSPtestView

IMPLEMENT_DYNCREATE(CCSPtestView, CView)

BEGIN_MESSAGE_MAP(CCSPtestView, CView)
	//{{AFX_MSG_MAP(CCSPtestView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSPtestView construction/destruction

CCSPtestView::CCSPtestView()
{
	// TODO: add construction code here

}

CCSPtestView::~CCSPtestView()
{
}

BOOL CCSPtestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCSPtestView drawing

void CCSPtestView::OnDraw(CDC* pDC)
{
	CCSPtestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCSPtestView printing

BOOL CCSPtestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCSPtestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCSPtestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCSPtestView diagnostics

#ifdef _DEBUG
void CCSPtestView::AssertValid() const
{
	CView::AssertValid();
}

void CCSPtestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCSPtestDoc* CCSPtestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCSPtestDoc)));
	return (CCSPtestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCSPtestView message handlers
