// CSPtest.h : main header file for the CSPTEST application
//
/*Copyright (c) 吴嘎			All Rights Reserved
+文件名: CSP操做类
+完成时间： 2006.9
+ QQ 4436045 一起学习PKI
*/

#if !defined(AFX_CSPTEST_H__F1335930_D71C_4861_8443_392DFB52C5D3__INCLUDED_)
#define AFX_CSPTEST_H__F1335930_D71C_4861_8443_392DFB52C5D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCSPtestApp:
// See CSPtest.cpp for the implementation of this class
//

class CCSPtestApp : public CWinApp
{
public:
	CCSPtestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSPtestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCSPtestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSPTEST_H__F1335930_D71C_4861_8443_392DFB52C5D3__INCLUDED_)
