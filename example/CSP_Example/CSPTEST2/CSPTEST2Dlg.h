// CSPTEST2Dlg.h : header file
////////////////////////////////////////////////////////////////////////////////////////////////////
// 工程: CSPTEST2
// 版权 Copyright (c) 2006, 吴嘎
//		All rights reserved.
// 版本: 1.0
// 更新: 2006-10-31
// 说明: 
//       这个工程本来是为了帮助朋友完成她的项目需要涉及到会话密钥操作和非对称密钥操作而写，本来已经写好
//       了CSPTEST这个工程后来发现不是她想要的，才有了这个工程，主要完成的操作有 加密/签名密钥对的公/私
//		 导入/导出功能，会话密钥的导入/导出 会话密钥的加密和解密功能。编程的时候没有太注意细节，对主要有些小问题
//       当时为了省时间所以操作的数据都比较小 所以没有用new 来分配大于1024字节的空间，如果朋友有需要的可以自己去修改
//       我自己也标示出来了，总之，还有很多不太注意的地方，不过较之CSPTEST工程(当时，只是写个DEMO给她看看调用的，后来还是
//		 决心帮她到底)还是要好很多。不多说了，大家可以自己看吧，希望对大家有用，如果有什么好的建议可以和我联系。谢谢大家的阅读。
//          
//       主要的实现类在CCsp中。
// 联系方式 QQ 4436045(心灵的路行) MSN alfred_woo1982@hotmail.com
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSPTEST2DLG_H__D19F558A_F88D_4BA8_A83D_47A04206CA97__INCLUDED_)
#define AFX_CSPTEST2DLG_H__D19F558A_F88D_4BA8_A83D_47A04206CA97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCSPTEST2Dlg dialog

class CCSPTEST2Dlg : public CDialog
{
// Construction
public:
	int WriteFile(CString strFilePath, BYTE *pbFileData, INT nFileLen, CString strFileDescript);
	void OnBrownImport(CString strImportPath);
	void OnBrownExport(CString strExportPath);
	void OpenFile(CString strFilePath, BYTE *pbFileData, INT &nFileLen, CString strFileDescript);
	CCSPTEST2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCSPTEST2Dlg)
	enum { IDD = IDD_CSPTEST2_DIALOG };
	CString	m_strContainer;
	CString	m_strExportSessionPath;
	CString	m_strImportSPubPath;
	CString	m_strExEnPriPath;
	CString	m_strExEnPubPath;
	CString	m_strExSiPriPath;
	CString	m_strExSiPubPath;
	CString	m_strImEnPriPath;
	CString	m_strImEnPubPath;
	CString	m_strImSePriPath;
	CString	m_strImSiPriPath;
	CString	m_strImSiPubPath;
	CString	m_strImSePath;
	CString	m_strImSEnDataPath;
	CString	m_strExSDeDataPath;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSPTEST2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCSPTEST2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGenSessionKey();
	afx_msg void OnGenEncryptKeyPairs();
	afx_msg void OnBrown5();
	afx_msg void OnBrown6();
	afx_msg void OnBrown1();
	afx_msg void OnBrown2();
	afx_msg void OnBrown3();
	afx_msg void OnBrown4();
	afx_msg void OnBrown9();
	afx_msg void OnBrown10();
	afx_msg void OnBrown11();
	afx_msg void OnBrown12();
	afx_msg void OnBrown7();
	afx_msg void OnBrown8();
	afx_msg void OnExportSignatureKeypair();
	afx_msg void OnImportEncryptKeypair();
	afx_msg void OnImportSignatureKeypair();
	afx_msg void OnSessionEncrypt();
	afx_msg void OnSessionDecrypt();
	afx_msg void OnBrown13();
	afx_msg void OnButton14();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSPTEST2DLG_H__D19F558A_F88D_4BA8_A83D_47A04206CA97__INCLUDED_)
