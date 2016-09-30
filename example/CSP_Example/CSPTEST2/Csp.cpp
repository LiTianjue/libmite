// Csp.cpp: implementation of the CCsp class.
// Download by http://www.codefans.net
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSPTEST2.h"
#include "Csp.h"
#include "wincrypt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCsp::CCsp()
{

}

CCsp::~CCsp()
{

}

/*************************************************************************
+方法名称:产生并导出会话密钥
+输入参数:	BYTE *pbContainer 容器名
+			BYTE *pbPubKey 公钥数据
+			DWORD dwPubLen 公钥数据长度
+			BYTE *pbSessionKey 会话密钥数据
+			DWORD &dwSessionLen 会话密钥数据长度
+返回值: 0 成功
+其他说明:	
*************************************************************************/
int CCsp::GenSessionKey(BYTE *pbContainer, BYTE *pbPubKey, DWORD dwPubLen, 
						BYTE *pbSessionKey, DWORD *pdwSessionLen)
{
	HCRYPTPROV hProv;
	HCRYPTKEY hKey;
	HCRYPTKEY hSessionKey;
	
	CryptAcquireContext(&hProv, (char *)pbContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	
	if(!CryptAcquireContext(&hProv, (char *)pbContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;
	
	int r = CryptImportKey(hProv, pbPubKey, dwPubLen, 0, 0, &hKey);
	if(!r)
	{
		ReportError("GenSessionKey", "导入保护公钥失败!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}
	
	//产生会话密钥
	r = CryptGenKey(hProv, CALG_3DES, CRYPT_EXPORTABLE, &hSessionKey);
	if(!r)
	{
		ReportError("GenSessionKey", "产生会话密钥失败!", -3);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		return -3;
	}
	
	//获得会话密钥长度
	r = CryptExportKey(hSessionKey, hKey, SIMPLEBLOB, NULL, NULL, pdwSessionLen);
	if(!r)
	{
		ReportError("GenSessionKey", "获得会话密钥长度失败!", -4);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		if(hSessionKey)
			CryptDestroyKey(hSessionKey);
		return -4;
	}
	
	//导出会话密钥
	r = CryptExportKey(hSessionKey, hKey, SIMPLEBLOB, NULL, pbSessionKey, pdwSessionLen);
	if(!r)
	{
		ReportError("GenSessionKey", "获得会话密钥数据失败!", -5);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		if(hSessionKey)
			CryptDestroyKey(hSessionKey);
		return -5;
	}
	
	if(hProv)
		CryptReleaseContext(hProv, 0);
	if(hKey)
		CryptDestroyKey(hKey);
	if(hSessionKey)
		CryptDestroyKey(hSessionKey);
	
	return 0;
}

/*************************************************************************
+方法名称:产生RSA密钥对并导出公钥私钥数据
+输入参数:	int nKeyFlag 密钥对类型 1 为签名密钥对 2 为加密密钥对
+			BYTE *pbPubKey 公钥数据(加密公钥文件.txt 签名公钥文件.txt)
+			DWORD dwPubKeyLen 公钥数据长度
+			BYTE *pbPriKey 私钥数据(加密私钥文件.txt 签名私钥文件.txt)
+			DWORD &dwSessionLen 会话密钥数据长度
+返回值: 0 成功
+其他说明:	
*************************************************************************/
int CCsp::GenRsaKeyPairs(int nKeyFlag, BYTE *pbPubKey, DWORD &dwPubKeyLen, 
										BYTE *pbPriKey, DWORD &dwPriKeyLen)
{
	HCRYPTPROV hProv;
	HCRYPTKEY hKey;
	
	CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	
	if(!CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;
	
	if(nKeyFlag == 1)
	{
		if(!CryptGenKey(hProv, AT_SIGNATURE, CRYPT_EXPORTABLE, &hKey))
		{
			//删除容器 释放上下文
			ReportError("GenRsaKeyPairs", "产生RSA签名密钥对失败!", -2);
			CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
			if(hProv)
				CryptReleaseContext(hProv, 0);
			return -2;
		}
	}
	else if(nKeyFlag == 2)
	{
		if(!CryptGenKey(hProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hKey))
		{
			//删除容器 释放上下文
			ReportError("GenRsaKeyPairs", "产生RSA签名密钥对失败!", -3);
			CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
			if(hProv)
				CryptReleaseContext(hProv, 0);
			return -3;
		}
	}
	else
	{
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -4;
	}
	
	//导出私钥
	DWORD dwTempPriLen;
	
	int r = CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, NULL, NULL, &dwTempPriLen);
	if(!r)
	{
		ReportError("GenRsaKeyPairs", "导出私钥长度失败!", -5);
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -5;
	}
	
	BYTE *pbTempPriData = (BYTE *)malloc(dwTempPriLen+1);
	
	r = CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, NULL, pbTempPriData, &dwTempPriLen);
	if(!r)
	{
		ReportError("GenRsaKeyPairs", "导出私钥数据失败!", -6);
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -6;
	}
	
	//导出公钥
	DWORD dwTempPubLen;
	
	r = CryptExportKey(hKey, NULL, PUBLICKEYBLOB, NULL, NULL, &dwTempPubLen);
	if(!r)
	{
		ReportError("GenRsaKeyPairs", "导出公钥长度失败!", -7);
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -7;
	}
	
	BYTE *pbTempPubData = (BYTE *)malloc(dwTempPubLen+1);
	
	r = CryptExportKey(hKey, NULL, PUBLICKEYBLOB, NULL, pbTempPubData, &dwTempPubLen);
	if(!r)
	{
		ReportError("GenRsaKeyPairs", "导出公钥数据失败!", -8);
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -8;
	}
	
	//缓冲区拷贝
	memcpy(pbPriKey, pbTempPriData, dwTempPriLen); //拷贝私钥数据
	dwPubKeyLen = dwTempPubLen; //私钥数据长度
	memcpy(pbPubKey, pbTempPubData, dwTempPubLen); //拷贝公钥数据
	dwPriKeyLen = dwTempPriLen; //公钥长度
	
	//删除容器
	CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

	//释放内存
	if(pbTempPriData)
		free(pbTempPriData);
	if(pbTempPubData)
		free(pbTempPubData);
	if(hProv)
		CryptReleaseContext(hProv, 0);
	
	return 0;
}

void CCsp::ReportError(CString strFunName, CString strErrorDescript, int nErrorNo)
{
	m_strError.Format("出错函数%s:出错描述%s:出错代码%d", strFunName, strErrorDescript, nErrorNo);
	return;
}

/*************************************************************************
+方法名称:导入公钥数据
+输入参数:	CString strContainer 容器名称
+			BYTE *pbPubKey 公钥数据(加密公钥文件.txt 签名公钥文件.txt)
+			DWORD dwPubLen 公钥数据长度
+返回值: 0 成功
+其他说明:	
*************************************************************************/
int CCsp::ImportPubKey(CString strContainer, BYTE *pbPubKey, DWORD dwPubLen)
{
	
	HCRYPTPROV hProv;
	HCRYPTKEY hKey;

	CryptAcquireContext(&hProv, strContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	
	if(!CryptAcquireContext(&hProv, strContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;

	//导入公钥数据
	int r = CryptImportKey(hProv, pbPubKey, dwPubLen, NULL, NULL, &hKey);
	if(!r)
	{
		ReportError("ImportPubKey", "导入公钥数据失败!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}

	if(hProv)
		CryptReleaseContext(hProv, 0);
	return 0;
}

/*************************************************************************
+方法名称:导入私钥数据
+输入参数:	CString strContainer 容器名称
+			BYTE *pbPriKey 私钥数据(加密私钥文件.txt 签名私钥文件.txt)
+			DWORD dwPriLen 私钥数据长度
+返回值: 0 成功
+其他说明:	
*************************************************************************/
int CCsp::ImportPriKey(CString strContainer, BYTE *pbPriKey, DWORD dwPriLen)
{
	HCRYPTPROV hProv;
	HCRYPTKEY hKey;

	CryptAcquireContext(&hProv, strContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	
	if(!CryptAcquireContext(&hProv, strContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;

	//导入私钥数据
	int r = CryptImportKey(hProv, pbPriKey, dwPriLen, NULL, NULL, &hKey);
	if(!r)
	{
		ReportError("ImportPriKey", "导入私钥数据失败!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}

	if(hProv)
		CryptReleaseContext(hProv, 0);

	return 0;
}

/*************************************************************************
+方法名称:会话加密数据
+输入参数:	BYTE *pbSessionKey 会话密钥数据 (会话密钥数据文件.txt)
+			 DWORD dwSessionLen 会话密钥长度
+			BYTE *pbSourceData 待加密数据 (会话加密原文.txt)
+			DWORD dwSourceLen 待加密数据长度
+			BYTE *pbPriKey 解密私钥数据 (加密私钥文件.txt)
+			DWORD dwPriLen 解密私钥数据长度
+			BYTE *pbDestData 加密后数据 (会话数据加密密文.txt)
+			DWORD &dwDestLen 加密后数据长度
+			CString strContainer 容器名称
+返回值: 0 成功
+其他说明:	
*************************************************************************/
int CCsp::SessionEncrypt(BYTE *pbSessionKey, DWORD dwSessionLen, 
						BYTE *pbSourceData, DWORD dwSourceLen, 
						BYTE *pbPriKey, DWORD dwPriLen,
						BYTE *pbDestData,DWORD *pdwDestLen, 
						BYTE *pbContainer)
{
	HCRYPTPROV hProv;
	HCRYPTKEY hSessionKey;
	HCRYPTKEY hKey;
	
	CryptAcquireContext(&hProv, (char *)pbContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	
	if(!CryptAcquireContext(&hProv, (char *)pbContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;
	
	//导入私钥到容器中
	int r = CryptImportKey(hProv, pbPriKey, dwPriLen, 0, NULL, &hKey);
	if(!r)
	{
		ReportError("SessionEncrypt", "导入会话解密私钥失败!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}
	
	r = CryptImportKey(hProv, pbSessionKey, dwSessionLen, hKey, NULL, &hSessionKey);
	if(!r)
	{
		ReportError("SessionEncrypt", "导入会话密钥失败!", -3);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		return -3;
	}
	
	r = CryptEncrypt(hSessionKey, NULL, TRUE, NULL, pbSourceData, &dwSourceLen, 
		dwSourceLen+256);
	if(!r)
	{
		ReportError("SessionEncrypt", "加密数据失败!", -4);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		if(hSessionKey)
			CryptDestroyKey(hSessionKey);
		return -4;
	}
	
	*pdwDestLen = dwSourceLen;
	memcpy(pbDestData, pbSourceData, dwSourceLen);
	
	if(hProv)
		CryptReleaseContext(hProv, 0);
	if(hSessionKey)
		CryptDestroyKey(hSessionKey);
	if(hKey)
		CryptDestroyKey(hKey);
	return 0;
}


/*************************************************************************
+方法名称:会话解密数据
+输入参数:	BYTE *pbSessionKey 会话密钥数据 (会话密钥数据文件.txt)
+			 DWORD dwSessionLen 会话密钥长度
+			BYTE *pbSourceData 待解密数据 (会话数据加密密文.txt)
+			DWORD dwSourceLen 待解密数据长度
+			BYTE *pbPriKey 解密私钥数据 (加密私钥文件.txt)
+			DWORD dwPriLen 解密私钥数据长度
+			BYTE *pbDestData 解密后数据 (会话解密后数据.txt)
+			DWORD &dwDestLen 解密后数据长度
+			CString strContainer 容器名称
+返回值: 0 成功
+其他说明:	
*************************************************************************/
int CCsp::SessionDecrypt(BYTE *pbSessionKey, DWORD dwSessionLen, 
						 BYTE *pbSourceData, DWORD dwSourceLen, 
						 BYTE *pbPriKey, DWORD dwPriLen, 
						 BYTE *pbDestData, DWORD *pdwDestLen, 
						 BYTE *pbContainer)
{
	HCRYPTPROV hProv;
	HCRYPTKEY hKey;
	HCRYPTKEY hSessionKey;
	
	CryptAcquireContext(&hProv, (char *)pbContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	
	if(!CryptAcquireContext(&hProv, (char *)pbContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;
	
	//导入私钥到容器中
	int r = CryptImportKey(hProv, pbPriKey, dwPriLen, 0, NULL, &hKey);
	if(!r)
	{
		ReportError("SessionEncrypt", "导入会话解密私钥失败!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}
	
	r = CryptImportKey(hProv, pbSessionKey, dwSessionLen, hKey, NULL, &hSessionKey);
	if(!r)
	{
		ReportError("SessionEncrypt", "导入会话密钥失败!", -3);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		return -3;
	}
	
	r = CryptDecrypt(hSessionKey, NULL, TRUE, NULL, pbSourceData, &dwSourceLen);
	if(!r)
	{
		ReportError("SessionEncrypt", "会话解密数据失败!", -4);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		if(hSessionKey)
			CryptDestroyKey(hSessionKey);
		return -4;
	}
	
	*pdwDestLen = dwSourceLen;
	memcpy(pbDestData, pbSourceData, *pdwDestLen);
	
	if(hProv)
		CryptReleaseContext(hProv, 0);
	if(hSessionKey)
		CryptDestroyKey(hSessionKey);
	if(hKey)
		CryptDestroyKey(hKey);
	return 0;
	
}
