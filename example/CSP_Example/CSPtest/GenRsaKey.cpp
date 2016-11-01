// GenRsaKey.cpp : implementation file
// Download by http://www.codefans.net

#include "stdafx.h"
#include "CSPtest.h"
#include "GenRsaKey.h"
//密钥操作类头文件
#include "KeyOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenRsaKey dialog


CGenRsaKey::CGenRsaKey(CWnd* pParent /*=NULL*/)
	: CDialog(CGenRsaKey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenRsaKey)
	m_SelectButton = -1;
	m_strSrcPath = _T("");
	m_strDestPath = _T("");
	m_strPWD = _T("");
	m_strSymDestPath = _T("");
	m_strSymSrcPath = _T("");
	m_strSSrcPath = _T("");
	m_strVDestPath = _T("");
	//}}AFX_DATA_INIT
}


void CGenRsaKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenRsaKey)
	DDX_Radio(pDX, IDC_RADIO1, m_SelectButton);
	DDX_Text(pDX, IDC_SRC_PATH, m_strSrcPath);
	DDX_Text(pDX, IDC_DEST_PATH, m_strDestPath);
	DDX_Text(pDX, IDC_PWD, m_strPWD);
	DDX_Text(pDX, IDC_SYM_DEST_PATH, m_strSymDestPath);
	DDX_Text(pDX, IDC_SYM_SRC_PATH, m_strSymSrcPath);
	DDX_Text(pDX, IDC_SIGN_SRC_PATH, m_strSSrcPath);
	DDX_Text(pDX, IDC_VERIFY_DEST_PATH, m_strVDestPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenRsaKey, CDialog)
	//{{AFX_MSG_MAP(CGenRsaKey)
	ON_BN_CLICKED(IDC_GEN_RSA_KEY, OnGenRsaKey)
	ON_BN_CLICKED(IDC_EXPORT_KEY, OnExportKey)
	ON_BN_CLICKED(IDC_OPEN_BUTTON, OnOpenButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, OnSaveButton)
	ON_BN_CLICKED(IDC_RSA_ENCRYPT, OnRsaEncrypt)
	ON_BN_CLICKED(IDC_RSA_DECRYPT, OnRsaDecrypt)
	ON_BN_CLICKED(IDC_SYM_OPEN_BUTTON, OnSymOpenButton)
	ON_BN_CLICKED(IDC_SYM_SAVE_BUTTON, OnSymSaveButton)
	ON_BN_CLICKED(IDC_ENCRYPT, OnEncrypt)
	ON_BN_CLICKED(IDC_DECRYPT, OnDecrypt)
	ON_BN_CLICKED(IDC_SIGN_OPEN_BUTTON, OnSOpenButton)
	ON_BN_CLICKED(IDC_VERIFY_SAVE_BUTTON, OnVerifySaveButton)
	ON_BN_CLICKED(IDC_RSA_SIGN, OnRsaSign)
	ON_BN_CLICKED(IDC_RSA_VERIFY, OnRsaVerify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenRsaKey message handlers



void CGenRsaKey::OnGenRsaKey() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int r;
	int nflag; //密钥类型
	
	CKeyOperation obj_RSA; //密钥操作类对象
	
	if(m_SelectButton == 0)
	{
		nflag = 1;
		r = obj_RSA.CRYPTAPI_GenRsaKey(nflag);
		if(r != 0)
		{
			MessageBox("产生签名密钥对失败!");
			return;
		}
	}
	else if(m_SelectButton == 1)
	{
		nflag = 2;
		r = obj_RSA.CRYPTAPI_GenRsaKey(nflag);
		if(r != 0)
		{
			MessageBox("产生加密密钥对失败!");
			return;
		}
	}
	else
	{
		MessageBox("请先选择密钥类型!");
		return;
	}
	MessageBox("生成RSA密钥对成功!");
	return;
}

void CGenRsaKey::OnExportKey() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int r;
	int nflag; //公钥类型
	
	CKeyOperation obj_PubKey;
	
	unsigned char szPubKeyBuf[1024] = {0};
	int nPubKeyLen;

	if(m_SelectButton == 0)
	{
		nflag = 1;
		r = obj_PubKey.CRYPTAPI_ExportPubKey(nflag, szPubKeyBuf, nPubKeyLen);
		if(r != 0)
		{
			MessageBox("导出签名公钥失败!");
			return;
		}
	}
	else if(m_SelectButton == 1)
	{
		nflag = 2;
		r = obj_PubKey.CRYPTAPI_ExportPubKey(nflag, szPubKeyBuf, nPubKeyLen);
		if(r != 0)
		{
			MessageBox("导出加密公钥失败!");
			return;
		}
	}
	else
	{
		MessageBox("请先选择导出公钥类型!");
		return;
	}
	MessageBox("导出公钥成功!");
	return;
}

void CGenRsaKey::OnOpenButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strSrcPath = OpenFile.GetPathName();

	UpdateData(FALSE);
	return;
}

void CGenRsaKey::OnSaveButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	static char szFilter[] = "全部文件(*.*)|*.*||";
	CFileDialog saveFileDlg(FALSE, NULL, "*.*",
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,szFilter,NULL);
	if(saveFileDlg.DoModal() != IDOK)
	{
		return;
	}
	m_strDestPath = saveFileDlg.GetPathName();
	UpdateData(FALSE);
	return;
	
}

void CGenRsaKey::OnRsaEncrypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strSrcPath == "" && m_strDestPath == "")
	{
		MessageBox("请选择要的操作文件路径!");
		return;
	}
	
	CKeyOperation obj_Encrypt;
	//读取待加密文件
	CFile fpSrcFile;
	if(fpSrcFile.Open(m_strSrcPath, CFile::modeRead) == 0)
	{
		MessageBox("读取待加密文件失败!");
		return;
	}
	
	int nSrcFileLen = fpSrcFile.GetLength();
	unsigned char *pbSrcData = new unsigned char [128+1]; //分配待加密文件缓冲区 
	//一定要分配到128以上不管明文多长
	memset(pbSrcData, 0x00, 128+1);
	fpSrcFile.Read(pbSrcData, nSrcFileLen);
	fpSrcFile.Close();
	
	int nDestFileLen = 128;
	unsigned char *pbDestData = new unsigned char [nDestFileLen+1]; //分配加密后文件缓冲区
	memset(pbDestData, 0x00, nDestFileLen+1);
	
	int r = obj_Encrypt.CRYPTAPI_RSAEncrypt(pbSrcData, nSrcFileLen, pbDestData, nDestFileLen);
	if(r != 0)
	{
		delete [] pbSrcData;
		pbSrcData = NULL;
		delete [] pbDestData;
		pbDestData = NULL;
		MessageBox("非对称加密失败!");
		return;
	}
	
	//保存加密后的数据为文件
	CFile fpDestFile;
	if(fpDestFile.Open(m_strDestPath, CFile::modeWrite | CFile::modeCreate ) == 0)
	{
		MessageBox("创建保存文件失败!");
		return;
	}
	fpDestFile.Write(pbDestData, nDestFileLen);
	fpDestFile.Close();
	
	delete [] pbSrcData;
	pbSrcData = NULL;
	delete [] pbDestData;
	pbDestData = NULL;
	
	MessageBox("非对称加密成功!");
	return;
}

void CGenRsaKey::OnRsaDecrypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_strSrcPath == "" && m_strDestPath == "")
	{
		MessageBox("请选择要的操作文件路径!");
		return;
	}
	
	CKeyOperation obj_Decrypt;
	//读取待加密文件
	CFile fpSrcFile;
	if(fpSrcFile.Open(m_strSrcPath, CFile::modeRead) == 0)
	{
		MessageBox("读取待解密文件失败!");
		return;
	}
	
	int nSrcFileLen = fpSrcFile.GetLength();
	unsigned char *pbSrcData = new unsigned char [nSrcFileLen+1]; //分配待解密文件缓冲区
	memset(pbSrcData, 0x00, nSrcFileLen+1);
	fpSrcFile.Read(pbSrcData, nSrcFileLen);
	fpSrcFile.Close();
	
	int nDestFileLen = 128;
	unsigned char *pbDestData = new unsigned char [nDestFileLen+1]; //分配加密后文件缓冲区
	memset(pbDestData, 0x00, nDestFileLen+1);
	
	int r = obj_Decrypt.CRYPTAPI_RSADecrypt(pbSrcData, nSrcFileLen, pbDestData, nDestFileLen);
	if(r != 0)
	{
		delete [] pbSrcData;
		pbSrcData = NULL;
		delete [] pbDestData;
		pbDestData = NULL;
		MessageBox("非对称解密失败!");
		return;
	}
	
	//保存加密后的数据为文件
	CFile fpDestFile;
	if(fpDestFile.Open(m_strDestPath, CFile::modeCreate | CFile::modeWrite) == 0)
	{
		MessageBox("创建保存文件失败!");
		return;
	}
	fpDestFile.Write(pbDestData, nDestFileLen);
	fpDestFile.Close();
	
	delete [] pbSrcData;
	pbSrcData = NULL;
	delete [] pbDestData;
	pbDestData = NULL;
	
	MessageBox("非对称解密成功!");
	return;
	
	return;
	
}

void CGenRsaKey::OnSymOpenButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strSymSrcPath = OpenFile.GetPathName();

	UpdateData(FALSE);
	return;
}


void CGenRsaKey::OnSymSaveButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	static char szFilter[] = "全部文件(*.*)|*.*||";
	CFileDialog saveFileDlg(FALSE, NULL, "*.*",
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,szFilter,NULL);
	if(saveFileDlg.DoModal() != IDOK)
	{
		return;
	}
	m_strSymDestPath = saveFileDlg.GetPathName();
	UpdateData(FALSE);
	return;
}

void CGenRsaKey::OnEncrypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_strPWD == "" && m_strSymDestPath == "" && m_strSymSrcPath == "")
	{
		MessageBox("请确认 口令，待加密文件，加密文件是否填写正确!");
		return;
	}
	//读取待加密文件
	CFile fpSrcFile;
	if(fpSrcFile.Open(m_strSymSrcPath, CFile::modeRead) == 0)
	{
		MessageBox("读取待对称加密文件失败!");
		return;
	}
	
	int nSrcFileLen = fpSrcFile.GetLength();
	unsigned char *pbSrcData = new unsigned char [nSrcFileLen+1];
	memset(pbSrcData, 0x00, nSrcFileLen+1);
	fpSrcFile.Read(pbSrcData, nSrcFileLen);
	fpSrcFile.Close();
	
	CKeyOperation obj_SymEn;
	
	int nDestFileLen;
	unsigned char *pbDestData = new unsigned char[nSrcFileLen+256];//对称加密会填充数据至少是16字节
	//所以开的缓冲大些防止溢出
	memset(pbDestData, 0x00, nSrcFileLen+256);					  
	//对称加密文件
	int r = obj_SymEn.CRYPTAPI_Encrypt((unsigned char *)m_strPWD.GetBuffer(0), m_strPWD.GetLength(), pbSrcData, 
		nSrcFileLen, pbDestData, nDestFileLen);
	if(r != 0)
	{
		delete [] pbSrcData;
		pbSrcData = NULL;
		delete [] pbDestData;
		pbDestData = NULL;
		MessageBox("对称加密失败!");
		return;
	}
	
	//保存对称加密文件
	CFile fpDestFile;
	if(fpDestFile.Open(m_strSymDestPath, CFile::modeCreate | CFile::modeWrite) == 0)
	{
		MessageBox("创建保存加密文件失败!");
		return;
	}

	fpDestFile.Write(pbDestData, nDestFileLen);
	fpDestFile.Close();
	
	//delete [] pbSrcData;
	//pbSrcData = NULL;
	delete [] pbDestData;
	pbDestData = NULL;
	
	MessageBox("对称加密成功!");
	
	return;
}

void CGenRsaKey::OnDecrypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_strPWD == "" && m_strSymDestPath == "" && m_strSymSrcPath == "")
	{
		MessageBox("请确认 口令，待解密文件，解密文件是否填写正确!");
		return;
	}
	
	CFile fpSrcFile;
	if(fpSrcFile.Open(m_strSymSrcPath, CFile::modeRead) == 0)
	{
		MessageBox("读取待对称解密文件失败!");
		return;
	}
	
	int nSrcFileLen = fpSrcFile.GetLength();
	unsigned char *pbSrcData = new unsigned char [nSrcFileLen+1];
	memset(pbSrcData, 0x00, nSrcFileLen+1);
	fpSrcFile.Read(pbSrcData, nSrcFileLen);
	fpSrcFile.Close();
	
	CKeyOperation obj_SymDe;
	
	int nDestFileLen;
	unsigned char *pbDestData = new unsigned char [nSrcFileLen+1]; //解密时要去填充所以缓冲可以相同
	memset(pbDestData, 0x00, nSrcFileLen+1);
	
	int r = obj_SymDe.CRYPTAPI_Decrypt((unsigned char *)m_strPWD.GetBuffer(0), m_strPWD.GetLength(), pbSrcData, 
							nSrcFileLen,pbDestData, nDestFileLen);
	if(r != 0)
	{
		delete [] pbSrcData;
		pbSrcData = NULL;
		delete [] pbDestData;
		pbDestData = NULL;
		MessageBox("对称解密失败!");
		return;
	}
	
	CFile fpDestFile;
	if(fpDestFile.Open(m_strSymDestPath, CFile::modeCreate | CFile::modeWrite) == 0)
	{
		MessageBox("创建保存加密文件失败!");
		return;
	}
	
	fpDestFile.Write(pbDestData, nDestFileLen);
	fpDestFile.Close();
	
	delete [] pbDestData;
	pbDestData = NULL;
	
	MessageBox("对称解密成功!");
	
	return;
}

void CGenRsaKey::OnSOpenButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szFilter[] = "全部文件(*.*)|*.*||";
	CFileDialog OpenFile(TRUE, NULL, "*.*", 
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR, szFilter, NULL);
	if(OpenFile.DoModal() != IDOK)
	{
		return;
	}

	m_strSSrcPath = OpenFile.GetPathName();

	UpdateData(FALSE);
	return;
}

void CGenRsaKey::OnVerifySaveButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	static char szFilter[] = "全部文件(*.*)|*.*||";
	CFileDialog saveFileDlg(FALSE, NULL, "*.*",
		OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,szFilter,NULL);
	if(saveFileDlg.DoModal() != IDOK)
	{
		return;
	}
	m_strVDestPath = saveFileDlg.GetPathName();
	UpdateData(FALSE);
	return;
}

void CGenRsaKey::OnRsaSign() 
{
	// TODO: Add your control notification handler code here
	if(m_strSSrcPath == "" && m_strVDestPath == "" )
	{
		MessageBox("请选择待签名/待验证文件路径!");
		return;
	}
	
	unsigned char *pbSourceData;
	int nSourceLen;
	unsigned char pbDestData[1024] = {0}; //这里没有用NEW 是偷懒主要是觉得不过128个字节而已
	int nDestLen;
	
	CFile fpSrcFile;
	if(fpSrcFile.Open(m_strSSrcPath, CFile::modeRead) == 0)
	{
		MessageBox("读取待签名文件失败!");
		return;
	}
	
	nSourceLen = fpSrcFile.GetLength();
	pbSourceData = new unsigned char [nSourceLen+1];
	memset(pbSourceData, 0x00, nSourceLen+1);
	fpSrcFile.Read(pbSourceData, nSourceLen);
	fpSrcFile.Close();

	
	CKeyOperation obj_Sign;
	
	int r = obj_Sign.CRYPTAPI_RSASign(pbSourceData, nSourceLen, pbDestData, &nDestLen);
	if(r != 0)
	{
		MessageBox("签名数据失败!");
		return;
	}
	
	CFile fpDestFile;
	if(fpDestFile.Open(m_strVDestPath, CFile::modeCreate | CFile::modeWrite) == 0)
	{
		MessageBox("创建保存签名数据文件失败!");
		return;
	}
	
	fpDestFile.Write(pbDestData, nDestLen);
	fpDestFile.Close();
	
	delete [] pbSourceData;
	pbSourceData = NULL;

	MessageBox("签名数据成功!");
	return;
}

void CGenRsaKey::OnRsaVerify() 
{
	// TODO: Add your control notification handler code here
	if(m_strSSrcPath == "" && m_strVDestPath == "" )
	{
		MessageBox("请选择待文件路径!");
		return;
	}
	
	unsigned char *pbSrcData; //缓冲区可能还不够啊
	int nSrcLen;
	unsigned char pbDestData[1024] = {0};
	int nDestLen;
	
	//读取原文
	CFile fpSrcFile;
	if(fpSrcFile.Open(m_strVDestPath, CFile::modeRead) == 0)
	{
		MessageBox("读取签名原文失败!");
		return;
	}
	
	nSrcLen = fpSrcFile.GetLength();
	pbSrcData = new unsigned char [nSrcLen+1];
	memset(pbSrcData, 0x00, nSrcLen+1);
	fpSrcFile.Read(pbSrcData, nSrcLen);
	fpSrcFile.Close();
	
	//读取签名数据
	CFile fpDestFile;
	if(fpDestFile.Open(m_strSSrcPath, CFile::modeRead) == 0)
	{
		MessageBox("读取签名原文失败!");
		return;
	}
	
	nDestLen = fpDestFile.GetLength();
	fpDestFile.Read(pbDestData, nDestLen);
	fpDestFile.Close();
	
	
	CKeyOperation obj_Verify;
	
	int r = obj_Verify.CRYPTAPI_RSAVerify(pbSrcData, nSrcLen, pbDestData, nDestLen);
	if(r != 0)
	{
		MessageBox("验证签名失败!");
		return;
	}
	
	delete [] pbSrcData;
	pbSrcData = NULL;

	MessageBox("验证签名成功!");
	
	return;
}
