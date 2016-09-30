// KeyOperation.cpp: implementation of the CKeyOperation class.
// Download by http://www.codefans.net
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSPtest.h"
#include "KeyOperation.h"
#include "wincrypt.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyOperation::CKeyOperation()
{
	m_hProv = 0; //加密提供商句柄
	m_hKey = 0;  //密钥对句柄
}

CKeyOperation::~CKeyOperation()
{

}


/**********************************************************************
+函数名称: CRYPTAPI_GenRsaKey(int nFlag)
+输入参数: nFlag 密钥对类型 1.签名密钥对 2. 加密密钥对
+输出参数: 无
+返回值:
+其他说明:通过函数产生RSA密钥对
***********************************************************************/
int CKeyOperation::CRYPTAPI_GenRsaKey(int nFlag)
{
	//删除CSP中存在的容器
	CryptAcquireContext(&m_hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

	if(!CryptAcquireContext(&m_hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;

	//产生非对称密钥对
	if(nFlag == 1)
	{
		if(!CryptGenKey(m_hProv, AT_SIGNATURE, CRYPT_EXPORTABLE, &m_hKey))
		{
			//删除容器 释放上下文
			CryptAcquireContext(&m_hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
			if(m_hProv)
				CryptReleaseContext(m_hProv, 0);
			return -3;
		}
	}
	else if(nFlag == 2)
	{
		if(!CryptGenKey(m_hProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &m_hKey))
		{
			//删除容器 释放上下文
			CryptAcquireContext(&m_hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
			if(m_hProv)
				CryptReleaseContext(m_hProv, 0);
			return -4;
		}
	}
	else
	{
		CryptAcquireContext(&m_hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
			if(m_hProv)
				CryptReleaseContext(m_hProv, 0);
			return -5;
	}

	return 0;
}

/**********************************************************************
+函数名称: CRYPTAPI_ExportPubKey(int nFlag, unsigned char *pbPubKeyData, int &nDataLen)
+输入参数: nFlag 密钥对类型 1.签名密钥对 2. 加密密钥对 
+		   pbPubKeyData 公钥数据
+		   nDataLen 公钥数据长度
+输出参数: nDataLen 公钥数据长度
+			pbPubKeyData 公钥数据
+返回值:
+其他说明:将产生的RSA密钥对的公钥导出
***********************************************************************/

int CKeyOperation::CRYPTAPI_ExportPubKey(int nFlag, unsigned char *pbPubKeyData, int &nDataLen)
{
	int r;

	unsigned char *szPubBlob;
	int nPubBlobLen;

	r = CRYPTAPI_GenRsaKey(nFlag);
	if(r != 0)
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -1;
	}
	
	r = CryptExportKey(m_hKey, NULL, PUBLICKEYBLOB, NULL, NULL, (unsigned long *)&nPubBlobLen);
	if(!r)
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}
	
	szPubBlob = (unsigned char *)malloc(nPubBlobLen+1); //分配缓冲区
	
	r = CryptExportKey(m_hKey, NULL, PUBLICKEYBLOB, NULL, szPubBlob, (unsigned long *)&nPubBlobLen);
	int b = GetLastError();
	if(!r)
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		free(szPubBlob);
			return -3;
	}
	
	nDataLen = nPubBlobLen;
	memcpy(pbPubKeyData, szPubBlob, nPubBlobLen);
	
	return 0;
}

/**********************************************************************
+函数名称: CRYPTAPI_RSAEncrypt(unsigned char *pbSrcData, int nSrcLen, 
+				unsigned char *pbDestData, int &nDestLen)
+输入参数: pbSrcData 待加密数据
+			nSrcLen  待加密数据长度
+			pbDestData 加密后数据
+			nDestLen 加密后数据长度
+输出参数: pbDestData 加密后数据
+			nDestLen 加密后数据长度
+返回值:
+其他说明:使用加密私钥来加密数据(注意CSP默认的非对称加密数据长度不能超过 117个字节)
***********************************************************************/
int CKeyOperation::CRYPTAPI_RSAEncrypt(unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen)
{
	int r;
	
	CryptAcquireContext(&m_hProv, "ASYENCRYPT", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

	if(!CryptAcquireContext(&m_hProv, "ASYENCRYPT", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;
	
	//产生密钥对	
	if(!CryptGenKey(m_hProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &m_hKey))
	{
		//删除容器
		CryptAcquireContext(&m_hProv, "ASYENCRYPT", MS_ENHANCED_PROV, 
								PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		CryptReleaseContext(m_hProv, 0);
		return -2;
	}

	r = CryptEncrypt(m_hKey, 0, TRUE, 0, pbSrcData, (unsigned long *)&nSrcLen, 128);
	if(!r)
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(m_hKey)
			CryptDestroyKey(m_hKey);
		return -2;
	}

	nDestLen = nSrcLen;
	memcpy(pbDestData, pbSrcData, nDestLen);

	if(m_hKey)
			CryptDestroyKey(m_hKey);
	if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		
	return 0;
}

/**********************************************************************
+函数名称: CRYPTAPI_RSADecrypt(unsigned char *pbSrcData, int nSrcLen, 
+				unsigned char *pbDestData, int &nDestLen)
+输入参数: pbSrcData 待解密数据
+			nSrcLen  待解密数据长度
+			pbDestData 解密后数据
+			nDestLen 解密后数据长度
+输出参数: pbDestData 解密后数据
+			nDestLen 解密后数据长度
+返回值:
+其他说明:使用加密公钥来解密数据(由于定义是成员变量m_hProv, m_hKey所以必须解密前要先执行加密,这样才能保证私钥不被覆盖)
***********************************************************************/

int CKeyOperation::CRYPTAPI_RSADecrypt(unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen)
{
	if(!CryptAcquireContext(&m_hProv, "ASYENCRYPT", MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
		return -1;

	//获得用户加密私钥句柄
	if(!CryptGetUserKey(m_hProv, AT_KEYEXCHANGE, &m_hKey))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}

	//开始解密数据
	int r = CryptDecrypt(m_hKey, 0, TRUE, 0, pbSrcData, (unsigned long *)&nSrcLen);
	if(!r)
	{
		return -3;
	}

	nDestLen = nSrcLen;
	memcpy(pbDestData, pbSrcData, nDestLen);

	return 0;
}
/**********************************************************************
+函数名称: CRYPTAPI_Encrypt()
+输入参数:  pbPWD 对称加密的口令
+			nPWDLen 口令长度
+			pbSrcData 待加密数据
+			nSrcLen  待加密数据长度
+			pbDestData 加密后数据
+			nDestLen 加密后数据长度
+输出参数: pbDestData 加密后数据
+			nDestLen 加密后数据长度
+返回值:
+其他说明:使用本地的对称算法来加密数据(采用口令方式,如果要换对称加密算法只需要换个宏定义就可以拉 默认DES算法)
***********************************************************************/

int CKeyOperation::CRYPTAPI_Encrypt(unsigned char *pbPWD, int nPWDLen, 
									unsigned char *pbSrcData, int nSrcLen,
									unsigned char *pbDestData, int &nDestLen)
{
	//删除CSP中存在的容器
	CryptAcquireContext(&m_hProv, "DES", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

	if(!CryptAcquireContext(&m_hProv, "DES", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;
	
	HCRYPTHASH hHash;
	HCRYPTKEY hEncryptKey;
	//创建HASH对象
	if(!CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &hHash))
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}
	
	//HASH 口令
	if(!CryptHashData(hHash, pbPWD, nPWDLen, 0))
		{
			if(m_hProv) 
				CryptReleaseContext(m_hProv, 0);
			if (hHash)
				CryptDestroyKey(hHash);
			return -3;
		}
	//导出对称加密密钥
	if(!CryptDeriveKey(m_hProv, CALG_DES, hHash, CRYPT_CREATE_SALT, &hEncryptKey))
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		if (hHash)
			CryptDestroyKey(hHash);
		return -4;
	}

	CryptDestroyKey(hHash);
	hHash = 0;

	//对称加密
	int r = CryptEncrypt(hEncryptKey, NULL, TRUE, NULL, pbSrcData, (unsigned long *)&nSrcLen, (unsigned long)nSrcLen+256);
	if(!r)
	{
        if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		if (hEncryptKey)
			CryptDestroyKey(hEncryptKey); 
		return -5;	
	}

	nDestLen = nSrcLen;
	memcpy(pbDestData, pbSrcData, nDestLen);

	if(hEncryptKey)
		CryptDestroyKey(hEncryptKey);
	if(m_hProv)
		CryptReleaseContext(m_hProv, 0);
	
	return 0 ;
}

/**********************************************************************
+函数名称: CRYPTAPI_Decrypt()
+输入参数:  pbPWD 对称加密的口令
+			nPWDLen 口令长度
+			pbSrcData 待解密数据
+			nSrcLen  待解密数据长度
+			pbDestData 解密后数据
+			nDestLen 解密后数据长度
+输出参数: pbDestData 解密后数据
+			nDestLen 解密后数据长度
+返回值:
+其他说明:使用本地的对称算法来解密数据(采用口令方式,如果要换对称解密算法只需要换个宏定义就可以拉 默认DES算法)
***********************************************************************/
int CKeyOperation::CRYPTAPI_Decrypt(unsigned char *pbPWD, int nPWDLen, unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen)
{
	int r;
	
	if(!CryptAcquireContext(&m_hProv, "DES", MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
		return -1;
	
	HCRYPTHASH hHash;
	HCRYPTKEY hDecryptKey;
	//创建HASH对象
	if(!CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &hHash))
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}
	
	//HASH 口令
	if(!CryptHashData(hHash, pbPWD, nPWDLen, 0))
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		if (hHash)
			CryptDestroyKey(hHash);
		return -3;
	}
	//导出对称加密密钥
	if(!CryptDeriveKey(m_hProv, CALG_DES, hHash, CRYPT_CREATE_SALT, &hDecryptKey))
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		if (hHash)
			CryptDestroyKey(hHash);
		return -4;
	}
	
	CryptDestroyKey(hHash);
	hHash = 0;
	
	//解密数据
	r = CryptDecrypt(hDecryptKey, NULL, TRUE, NULL, pbSrcData, (unsigned long *)&nSrcLen);
	if(!r)
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		if(hDecryptKey)
			CryptDestroyKey(hDecryptKey);
		return -5;
	}
	
	nDestLen = nSrcLen;
	memcpy(pbDestData, pbSrcData, nDestLen);
	
	if(hDecryptKey)
		CryptDestroyKey(hDecryptKey);
	if(m_hProv) 
		CryptReleaseContext(m_hProv, 0);
	
	return 0;
}

/**********************************************************************
+函数名称: CRYPTAPI_RSASign()
+输入参数:  pbSrcData 待签名数据
+			nSrcLen 待签名数据长度
+			pbDestData 签名后数据
+			nDestLen 签名后数据长度
+输出参数: pbDestData 签名后数据
+			nDestLen 签名后数据长度
+返回值:
+其他说明: 签名HASH后的数据
***********************************************************************/
int CKeyOperation::CRYPTAPI_RSASign(unsigned char *pbSrcData, int nSrcLen, 
									unsigned char *pbDestData, int *pnDestLen)
{
	
	CryptAcquireContext(&m_hProv, "ASYSIGN", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	if(!CryptAcquireContext(&m_hProv, "ASYSIGN", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;

	int r = CryptGenKey(m_hProv, AT_SIGNATURE, CRYPT_EXPORTABLE, &m_hKey);
	if(!r)
	{
		CryptAcquireContext(&m_hProv, "ASYSIGN", MS_ENHANCED_PROV, 
								PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}

	HCRYPTHASH hHash;
	if(!CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &hHash))  //创建HASH对象
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -3;
	}

	if(!CryptHashData(hHash, pbSrcData, (unsigned long)nSrcLen, 0))//hash数据
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(hHash)
			CryptDestroyHash(hHash);
		return -3;
	}
	
	//获得签名数据长度
	if(!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, NULL, (unsigned long *)pnDestLen))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(hHash)
			CryptDestroyHash(hHash);
		return -4;
	}

	//签名HASH数据
	if(!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, pbDestData, (unsigned long *)pnDestLen))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(hHash)
			CryptDestroyHash(hHash);
		return -5;
	}

	if(hHash)
		CryptDestroyHash(hHash);
	if(m_hProv)
			CryptReleaseContext(m_hProv, 0);

	return 0;
}

/**********************************************************************
+函数名称: CRYPTAPI_RSAVerify()
+输入参数:  pbSrcData 待验证数据
+			nSrcLen 待验证数据长度
+			pbDestData 签名数据
+			nDestLen 签名数据长度
+输出参数: pbDestData 签名后数据
+			nDestLen 签名数据长度
+返回值:
+其他说明: 验证经过签名HASH数据
***********************************************************************/

int CKeyOperation::CRYPTAPI_RSAVerify(unsigned char *pbSrcData, int nSrcLen, 
											unsigned char *pbDestData, int nDestLen)
{
	if(!CryptAcquireContext(&m_hProv, "ASYSIGN", MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
		return -1;
	
	if(!CryptGetUserKey(m_hProv, AT_SIGNATURE, &m_hKey))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}

	HCRYPTHASH hHash;
	
	//创建HASH对象
	if(!CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &hHash))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}

	//HASH待验证数据
	if(!CryptHashData(hHash, pbSrcData, nSrcLen, 0))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(hHash)
			CryptDestroyHash(hHash);
		return -3;
	}
	
	//验证签名数据
	if(!CryptVerifySignature(hHash, pbDestData, nDestLen, m_hKey, NULL, 0))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(hHash)
			CryptDestroyHash(hHash);
		return -4;
	}

	if(hHash)
		CryptDestroyHash(hHash);
	if(m_hProv)
		CryptReleaseContext(m_hProv, 0);

	return 0;
}
