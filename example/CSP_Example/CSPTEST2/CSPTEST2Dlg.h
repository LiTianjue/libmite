// CSPTEST2Dlg.h : header file
////////////////////////////////////////////////////////////////////////////////////////////////////
// ����: CSPTEST2
// ��Ȩ Copyright (c) 2006, ���
//		All rights reserved.
// �汾: 1.0
// ����: 2006-10-31
// ˵��: 
//       ������̱�����Ϊ�˰����������������Ŀ��Ҫ�漰���Ự��Կ�����ͷǶԳ���Կ������д�������Ѿ�д��
//       ��CSPTEST������̺������ֲ�������Ҫ�ģ�������������̣���Ҫ��ɵĲ����� ����/ǩ����Կ�ԵĹ�/˽
//		 ����/�������ܣ��Ự��Կ�ĵ���/���� �Ự��Կ�ļ��ܺͽ��ܹ��ܡ���̵�ʱ��û��̫ע��ϸ�ڣ�����Ҫ��ЩС����
//       ��ʱΪ��ʡʱ�����Բ��������ݶ��Ƚ�С ����û����new ���������1024�ֽڵĿռ䣬�����������Ҫ�Ŀ����Լ�ȥ�޸�
//       ���Լ�Ҳ��ʾ�����ˣ���֮�����кܶ಻̫ע��ĵط���������֮CSPTEST����(��ʱ��ֻ��д��DEMO�����������õģ���������
//		 ���İ�������)����Ҫ�úܶࡣ����˵�ˣ���ҿ����Լ����ɣ�ϣ���Դ�����ã������ʲô�õĽ�����Ժ�����ϵ��лл��ҵ��Ķ���
//          
//       ��Ҫ��ʵ������CCsp�С�
// ��ϵ��ʽ QQ 4436045(�����·��) MSN alfred_woo1982@hotmail.com
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
