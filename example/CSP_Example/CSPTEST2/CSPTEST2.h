// CSPTEST2.h : main header file for the CSPTEST2 application
//

#if !defined(AFX_CSPTEST2_H__26D5A96F_FCC6_4766_BC18_4D3FB5C15D6A__INCLUDED_)
#define AFX_CSPTEST2_H__26D5A96F_FCC6_4766_BC18_4D3FB5C15D6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCSPTEST2App:
// See CSPTEST2.cpp for the implementation of this class
//

class CCSPTEST2App : public CWinApp
{
public:
	CCSPTEST2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSPTEST2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCSPTEST2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSPTEST2_H__26D5A96F_FCC6_4766_BC18_4D3FB5C15D6A__INCLUDED_)
