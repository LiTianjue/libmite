// CSPTEST2Dlg.cpp : implementation file
// Download by http://www.codefans.net

#include "stdafx.h"
#include "CSPTEST2.h"
#include "CSPTEST2Dlg.h"
#include "Csp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog data
	//{{AFX_datA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_datA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSPTEST2Dlg dialog

CCSPTEST2Dlg::CCSPTEST2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSPTEST2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSPTEST2Dlg)
	m_strContainer = _T("");
	m_strExportSessionPath = _T("");
	m_strImportSPubPath = _T("");
	m_strImSEnDataPath = _T("");
	m_strExSDeDataPath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCSPTEST2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSPTEST2Dlg)
	DDX_Text(pDX, IDC_EDIT_CONTAINER, m_strContainer);
	DDX_Text(pDX, IDC_EDIT_EXPORT_SESSIONKEY, m_strExportSessionPath);
	DDX_Text(pDX, IDC_EDIT_IMPORT_SESSION_PUBKEY, m_strImportSPubPath);
	DDX_Text(pDX, IDC_EDIT_EXPORT_ENCRYPTPRI, m_strExEnPriPath);
	DDX_Text(pDX, IDC_EDIT_EXPORT_ENCRYTPUB, m_strExEnPubPath);
	DDX_Text(pDX, IDC_EDIT_EXPORT_SIGNPRI, m_strExSiPriPath);
	DDX_Text(pDX, IDC_EDIT_EXPORT_SIGNPUB, m_strExSiPubPath);
	DDX_Text(pDX, IDC_EDIT_IMPORT_ENCRYPTPRI, m_strImEnPriPath);
	DDX_Text(pDX, IDC_EDIT_IMPORT_ENCRYTPUB, m_strImEnPubPath);
	DDX_Text(pDX, IDC_EDIT_IMPORT_SESSION_PRIVATEKEY, m_strImSePriPath);
	DDX_Text(pDX, IDC_EDIT_IMPORT_SIGNPRI, m_strImSiPriPath);
	DDX_Text(pDX, IDC_EDIT_IMPORT_SIGNPUB, m_strImSiPubPath);
	DDX_Text(pDX, IDC_EDIT_IMPORT_SESSIONKEY, m_strImSePath);
	DDX_Text(pDX, IDC_EDIT_ENCRYPTDATA_PATH, m_strImSEnDataPath);
	DDX_Text(pDX, IDC_EDIT_DECRYPTDATA_PATH, m_strExSDeDataPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCSPTEST2Dlg, CDialog)
	//{{AFX_MSG_MAP(CCSPTEST2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GEN_SESSION_KEY, OnGenSessionKey)
	ON_BN_CLICKED(IDC_EXPORT_ENCRYPT_KEYPAIR, OnGenEncryptKeyPairs)
	ON_BN_CLICKED(IDC_BROWN5, OnBrown5)
	ON_BN_CLICKED(IDC_BROWN6, OnBrown6)
	ON_BN_CLICKED(IDC_BROWN1, OnBrown1)
	ON_BN_CLICKED(IDC_BROWN2, OnBrown2)
	ON_BN_CLICKED(IDC_BROWN3, OnBrown3)
	ON_BN_CLICKED(IDC_BROWN4, OnBrown4)
	ON_BN_CLICKED(IDC_BROWN9, OnBrown9)
	ON_BN_CLICKED(IDC_BROWN10, OnBrown10)
	ON_BN_CLICKED(IDC_BROWN11, OnBrown11)
	ON_BN_CLICKED(IDC_BROWN12, OnBrown12)
	ON_BN_CLICKED(IDC_BROWN7, OnBrown7)
	ON_BN_CLICKED(IDC_BROWN8, OnBrown8)
	ON_BN_CLICKED(IDC_EXPORT_SIGNATURE_KEYPAIR, OnExportSignatureKeypair)
	ON_BN_CLICKED(IDC_IMPORT_ENCRYPT_KEYPAIR, OnImportEncryptKeypair)
	ON_BN_CLICKED(IDC_IMPORT_SIGNATURE_KEYPAIR, OnImportSignatureKeypair)
	ON_BN_CLICKED(IDC_SESSION_ENCRYPT, OnSessionEncrypt)
	ON_BN_CLICKED(IDC_SESSION_DECRYPT, OnSessionDecrypt)
	ON_BN_CLICKED(IDC_BROWN13, OnBrown13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSPTEST2Dlg message handlers

BOOL CCSPTEST2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCSPTEST2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCSPTEST2Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCSPTEST2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCSPTEST2Dlg::OnGenSessionKey() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strExportSessionPath == "" )
	{
		MessageBox("请输入会话密钥保存文件路径!");
		return;
	}
	
	if(m_strImportSPubPath == "")
	{
		MessageBox("请输入保护公钥文件文件路径!");
		return;
	}

	if(m_strContainer == "")
	{
		MessageBox("请输入容器名称!");
		return;
	}
	//读取公钥数据
	BYTE pbPubData[1024] = {0};
	int nPubLen;
	
	OpenFile(m_strImportSPubPath, pbPubData, nPubLen, "保护公钥文件");
	
	CCsp objCsp;
	//导出会话密钥
	BYTE pbSessionData[1024] = {0};
	DWORD nSessionLen;
	
	int r = objCsp.GenSessionKey((BYTE*)m_strContainer.GetBuffer(0), pbPubData, nPubLen, 
		pbSessionData, &nSessionLen);
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}
	
	//保存会话密钥文件
	WriteFile(m_strExportSessionPath, pbSessionData, nSessionLen, "会话密钥文件");
	MessageBox("导出会话密钥成功!");
	return;
}

void CCSPTEST2Dlg::OnGenEncryptKeyPairs()
{
	UpdateData(TRUE);
	
	if(m_strExEnPriPath == "" && m_strExEnPubPath == "")
	{
		MessageBox("请输入导出公钥文件路径!");
		return;
	}

	if(m_strExEnPubPath == "")
	{
		MessageBox("请输入导出私钥文件路径!");
		return;
	}

	BYTE pbPubData[1024] = {0};
	DWORD dwPubLen;
	BYTE pbPriData[1024] = {0};
	DWORD dwPriLen;
	
	CCsp objCsp;
	
	int r = objCsp.GenRsaKeyPairs(2, pbPubData, dwPubLen, pbPriData, dwPriLen);
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}
	
	//保存公/私钥文件
	WriteFile(m_strExEnPubPath, pbPubData, dwPubLen, "加密公钥文件");
	WriteFile(m_strExEnPriPath, pbPriData, dwPriLen, "加密私钥文件");
	
	MessageBox("产生并导出加密公/私钥成功!");
	
	return;
}

void CCSPTEST2Dlg::OnExportSignatureKeypair() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_strExSiPriPath == "")
	{
		MessageBox("请输入导出公钥文件路径!");
		return;
	}
	
	if(m_strExSiPubPath == "")
	{
		MessageBox("请输入导出私钥文件路径!");
		return;
	}

	BYTE pbPubData[1024] = {0};
	DWORD dwPubLen;
	BYTE pbPriData[1024] = {0};
	DWORD dwPriLen;
	
	CCsp objCsp;
	
	int r = objCsp.GenRsaKeyPairs(1, pbPubData, dwPubLen, pbPriData, dwPriLen);
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}
	
	//保存公/私钥文件
	WriteFile(m_strExSiPubPath, pbPubData, dwPubLen, "签名公钥文件");
	WriteFile(m_strExSiPriPath, pbPriData, dwPriLen, "签名私钥文件");
	
	MessageBox("产生并导出签名公/私钥成功!");
	
	return;
}

void CCSPTEST2Dlg::OnImportEncryptKeypair() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strImEnPubPath == "" )
	{
		MessageBox("请输入导入公钥文件路径");
		return;
	}
	
	if(m_strImEnPriPath == "")
	{
		MessageBox("请输入导入私钥文件路径!");
		return;
	}

	if(m_strContainer == "")
	{
		MessageBox("请输入容器名称!");
		return;
	}

	BYTE pbPubData[1024] = {0};
	int nPubLen;
	BYTE pbPriData[1024] = {0};
	int  nPriLen;

	OpenFile(m_strImEnPubPath, pbPubData, nPubLen, "加密公钥文件!");
	OpenFile(m_strImEnPriPath, pbPriData, nPriLen, "加密私钥文件!");
	
	CCsp objCsp;
	int r = objCsp.ImportPubKey(m_strContainer, pbPubData, nPubLen);
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}
	
	r = objCsp.ImportPriKey(m_strContainer, pbPriData, nPriLen);
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}
	
	MessageBox("导入加密公钥/私钥成功!");
	return;
}

void CCSPTEST2Dlg::OnImportSignatureKeypair() 
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
	if(m_strImSiPubPath == "")
	{
		MessageBox("请输入导入公钥文件路径!");
		return;
	}
	if(m_strImSiPriPath == "")
	{
		MessageBox("请输入私钥文件路径!");
		return;
	}
	if(m_strContainer == "")
	{
		MessageBox("请输入容器名称!");
		return;
	}

	BYTE pbPubData[1024] = {0};
	int nPubLen;
	BYTE pbPriData[1024] = {0};
	int  nPriLen;

	OpenFile(m_strImSiPubPath, pbPubData, nPubLen, "签名公钥导入!");
	OpenFile(m_strImSiPriPath, pbPriData, nPriLen, "签名私钥导入!");
	
	CCsp objCsp;
	int r = objCsp.ImportPubKey(m_strContainer, pbPubData, nPubLen);
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}
	
	r = objCsp.ImportPriKey(m_strContainer, pbPriData, nPriLen);
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}
	
	MessageBox("导入签名公钥/私钥成功!");
	return;
	
}

void CCSPTEST2Dlg::OnSessionEncrypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strImSEnDataPath == "")
	{
		MessageBox("请输入待加密数据文件路径!");
		return;
	}
	
	if(m_strImSePath == "")
	{
		MessageBox("请输入会话密钥文件路径!");
		return;
	}
	
	if(m_strContainer == "")
	{
		MessageBox("请输入容器名称!");
		return;
	}
	
	if(m_strImSePriPath == "")
	{
		MessageBox("请输入保护私钥文件路径!");
		return;
	}

	if(m_strExSDeDataPath == "")
	{
		MessageBox("请输入加/解密后文件!");
		return;
	}

	BYTE pbSessionKey[1024] = {0};
	int nSessionLen;
	BYTE pbSourceData[1024] = {0};//由于赶时间测试的数据非常小 偷懒没有用new的空间 
								  //大家还是用new分配空间养成好习惯 记得delete []
	int nSourceLen;
	BYTE pbDestData[1024] = {0};
	int nDestLen;
	BYTE pbPriKey[1024] = {0};
	int nPriLen;
	
	OpenFile(m_strImSePriPath, pbPriKey, nPriLen, "会话解密私钥文件");
	OpenFile(m_strImSePath, pbSessionKey, nSessionLen, "会话密钥数据文件!");
	OpenFile(m_strImSEnDataPath, pbSourceData, nSourceLen, "待加/解密数据文件!");

	CCsp objCsp;
	int r = objCsp.SessionEncrypt(pbSessionKey, nSessionLen, pbSourceData, nSourceLen, pbPriKey,
		nPriLen, pbDestData, (DWORD *)&nDestLen, (BYTE*)m_strContainer.GetBuffer(0));
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}

	WriteFile(m_strExSDeDataPath, pbDestData, nDestLen, "加密后数据文件!");
	
	MessageBox("会话加密数据成功!");
	
	return;
}

void CCSPTEST2Dlg::OnSessionDecrypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_strImSePath == "")
	{
		MessageBox("请输入会话密钥文件路径!");
		return;
	}
	
	if(m_strContainer == "")
	{
		MessageBox("请输入容器名称!");
		return;
	}
	
	if(m_strImSePriPath == "")
	{
		MessageBox("请输入保护私钥文件路径!");
		return;
	}
	
	if(m_strImSEnDataPath == "")
	{
		MessageBox("请输入待解密数据文件路径!");
		return;
	}
	
	if(m_strExSDeDataPath == "")
	{
		MessageBox("请输入解密后文件!");
		return;
	}
	
	BYTE pbSessionKey[1024] = {0};
	int nSessionLen;
	BYTE pbSourceData[1024] = {0};
	int nSourceLen;
	BYTE pbDestData[1024] = {0};
	int nDestLen;
	BYTE pbPriKey[1024] = {0};
	int nPriLen;
	
	OpenFile(m_strImSePriPath, pbPriKey, nPriLen, "会话解密私钥文件");
	OpenFile(m_strImSePath, pbSessionKey, nSessionLen, "会话密钥数据文件!");
	OpenFile(m_strImSEnDataPath, pbSourceData, nSourceLen, "待解密数据文件!");

	CCsp objCsp;
	int r = objCsp.SessionDecrypt(pbSessionKey, nSessionLen, pbSourceData, nSourceLen, pbPriKey,
		nPriLen, pbDestData, (DWORD *)&nDestLen, (BYTE*)m_strContainer.GetBuffer(0));
	if(r != 0)
	{
		MessageBox(objCsp.m_strError);
		return;
	}

	WriteFile(m_strExSDeDataPath, pbDestData, nDestLen, "解密后数据文件!");
	
	MessageBox("会话解密数据成功!");
	
	return;


}

void CCSPTEST2Dlg::OpenFile(CString strFilePath, BYTE *pbFiledata, 
									INT &nFileLen, CString strFileDescript)
{
	UpdateData(TRUE);

	CFile fp;
	if(fp.Open(strFilePath, CFile::modeRead) == 0)
	{
		MessageBox("无法打开!"+strFileDescript);
		return;
	}
	
	int nLenTemp = fp.GetLength();
	fp.Read(pbFiledata, nLenTemp);
	nFileLen = nLenTemp;
	fp.Close();

	return;
}

void CCSPTEST2Dlg::OnBrownExport(CString strExportPath)
{
	UpdateData(TRUE);
	CString strTempExportPath;
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(FALSE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	strTempExportPath = OpenFile.GetPathName();
	m_strExEnPubPath = strTempExportPath;
	UpdateData(FALSE);
	return;

}

void CCSPTEST2Dlg::OnBrownImport(CString strImportPath)
{
	UpdateData(TRUE);
	CString strTempImportPath;
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	strTempImportPath = OpenFile.GetPathName();
	strImportPath = strTempImportPath;
	UpdateData(FALSE);
	return;

}

int CCSPTEST2Dlg::WriteFile(CString strFilePath, BYTE *pbFiledata, INT nFileLen, CString strFileDescript)
{
	CFile fp;
	if(fp.Open(strFilePath, CFile::modeWrite | CFile::modeCreate) == 0)
	{
		MessageBox("无法写入文件!"+strFileDescript);
		return -1;
	}
	fp.Write(pbFiledata, nFileLen);
	fp.Close();
	return 0;
}


void CCSPTEST2Dlg::OnBrown5() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(FALSE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strExportSessionPath = OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown6() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strImportSPubPath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(FALSE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strExEnPubPath = OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(FALSE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strExEnPriPath = OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(FALSE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strExSiPubPath = OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(FALSE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strExSiPriPath = OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown9() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strImEnPubPath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown10() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}
	m_strImEnPriPath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown11() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}
	m_strImSiPubPath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown12() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}
	m_strImSiPriPath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}
	m_strImSePath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnBrown8() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}
	m_strImSePriPath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}



void CCSPTEST2Dlg::OnBrown13() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}
	m_strImSEnDataPath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}

void CCSPTEST2Dlg::OnButton14() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "*.txt文件(*.txt)|*.txt|全部文件(*.*)|*.*||";
	CFileDialog OpenFile(FALSE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}
	m_strExSDeDataPath= OpenFile.GetPathName();
	UpdateData(FALSE);
	return;
}
