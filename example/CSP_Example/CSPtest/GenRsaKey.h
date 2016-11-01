
////////////////////////////////////////////////////////////////////////////////////////////////////
// 工程: CSPTEST
// 版权 Copyright (c) 2006, 吴嘎
//		All rights reserved.
// 版本: 1.0
// 更新: 2006-10-31
// 说明: 
//       主要完成密钥产生导出，导入。还有本地的口令加密，会话加密在CSPTEST2工程里面，非对称的加密解密，还有
//       签名和验证 HASH函数包括在签名和验证里面。这个工程开始是DEMO主要是看调用的写的规范不好(^-^)不值得大家
//      学习(看《高质量C++编程》还行)。大家自己看吧，希望大家能和我交流。 
//
//		主要的实现类在CKeyOperation中。
//
// 联系方式 QQ 4436045(心灵的路行) MSN alfred_woo1982@hotmail.com
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_GENRSAKEY_H__44E7A422_6146_4324_B682_3EC19F2053E1__INCLUDED_)
#define AFX_GENRSAKEY_H__44E7A422_6146_4324_B682_3EC19F2053E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenRsaKey.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenRsaKey dialog

class CGenRsaKey : public CDialog
{
// Construction
public:

	CGenRsaKey(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenRsaKey)
	enum { IDD = IDD_GEN_RSA_KEY };
	int		m_SelectButton;
	CString	m_strSrcPath;
	CString	m_strDestPath;
	CString	m_strPWD;
	CString	m_strSymDestPath;
	CString	m_strSymSrcPath;
	CString	m_strSSrcPath;
	CString	m_strVDestPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenRsaKey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenRsaKey)
	afx_msg void OnGenRsaKey();
	afx_msg void OnExportKey();
	afx_msg void OnOpenButton();
	afx_msg void OnSaveButton();
	afx_msg void OnRsaEncrypt();
	afx_msg void OnRsaDecrypt();
	afx_msg void OnSymOpenButton();
	afx_msg void OnSymSaveButton2();
	afx_msg void OnSymSaveButton();
	afx_msg void OnEncrypt();
	afx_msg void OnDecrypt();
	afx_msg void OnSOpenButton();
	afx_msg void OnVerifySaveButton();
	afx_msg void OnRsaSign();
	afx_msg void OnRsaVerify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENRSAKEY_H__44E7A422_6146_4324_B682_3EC19F2053E1__INCLUDED_)
