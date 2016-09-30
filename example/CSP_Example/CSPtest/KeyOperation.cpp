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
	m_hProv = 0; //�����ṩ�̾��
	m_hKey = 0;  //��Կ�Ծ��
}

CKeyOperation::~CKeyOperation()
{

}


/**********************************************************************
+��������: CRYPTAPI_GenRsaKey(int nFlag)
+�������: nFlag ��Կ������ 1.ǩ����Կ�� 2. ������Կ��
+�������: ��
+����ֵ:
+����˵��:ͨ����������RSA��Կ��
***********************************************************************/
int CKeyOperation::CRYPTAPI_GenRsaKey(int nFlag)
{
	//ɾ��CSP�д��ڵ�����
	CryptAcquireContext(&m_hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

	if(!CryptAcquireContext(&m_hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;

	//�����ǶԳ���Կ��
	if(nFlag == 1)
	{
		if(!CryptGenKey(m_hProv, AT_SIGNATURE, CRYPT_EXPORTABLE, &m_hKey))
		{
			//ɾ������ �ͷ�������
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
			//ɾ������ �ͷ�������
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
+��������: CRYPTAPI_ExportPubKey(int nFlag, unsigned char *pbPubKeyData, int &nDataLen)
+�������: nFlag ��Կ������ 1.ǩ����Կ�� 2. ������Կ�� 
+		   pbPubKeyData ��Կ����
+		   nDataLen ��Կ���ݳ���
+�������: nDataLen ��Կ���ݳ���
+			pbPubKeyData ��Կ����
+����ֵ:
+����˵��:��������RSA��Կ�ԵĹ�Կ����
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
	
	szPubBlob = (unsigned char *)malloc(nPubBlobLen+1); //���仺����
	
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
+��������: CRYPTAPI_RSAEncrypt(unsigned char *pbSrcData, int nSrcLen, 
+				unsigned char *pbDestData, int &nDestLen)
+�������: pbSrcData ����������
+			nSrcLen  ���������ݳ���
+			pbDestData ���ܺ�����
+			nDestLen ���ܺ����ݳ���
+�������: pbDestData ���ܺ�����
+			nDestLen ���ܺ����ݳ���
+����ֵ:
+����˵��:ʹ�ü���˽Կ����������(ע��CSPĬ�ϵķǶԳƼ������ݳ��Ȳ��ܳ��� 117���ֽ�)
***********************************************************************/
int CKeyOperation::CRYPTAPI_RSAEncrypt(unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen)
{
	int r;
	
	CryptAcquireContext(&m_hProv, "ASYENCRYPT", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

	if(!CryptAcquireContext(&m_hProv, "ASYENCRYPT", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;
	
	//������Կ��	
	if(!CryptGenKey(m_hProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &m_hKey))
	{
		//ɾ������
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
+��������: CRYPTAPI_RSADecrypt(unsigned char *pbSrcData, int nSrcLen, 
+				unsigned char *pbDestData, int &nDestLen)
+�������: pbSrcData ����������
+			nSrcLen  ���������ݳ���
+			pbDestData ���ܺ�����
+			nDestLen ���ܺ����ݳ���
+�������: pbDestData ���ܺ�����
+			nDestLen ���ܺ����ݳ���
+����ֵ:
+����˵��:ʹ�ü��ܹ�Կ����������(���ڶ����ǳ�Ա����m_hProv, m_hKey���Ա������ǰҪ��ִ�м���,�������ܱ�֤˽Կ��������)
***********************************************************************/

int CKeyOperation::CRYPTAPI_RSADecrypt(unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen)
{
	if(!CryptAcquireContext(&m_hProv, "ASYENCRYPT", MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
		return -1;

	//����û�����˽Կ���
	if(!CryptGetUserKey(m_hProv, AT_KEYEXCHANGE, &m_hKey))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}

	//��ʼ��������
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
+��������: CRYPTAPI_Encrypt()
+�������:  pbPWD �ԳƼ��ܵĿ���
+			nPWDLen �����
+			pbSrcData ����������
+			nSrcLen  ���������ݳ���
+			pbDestData ���ܺ�����
+			nDestLen ���ܺ����ݳ���
+�������: pbDestData ���ܺ�����
+			nDestLen ���ܺ����ݳ���
+����ֵ:
+����˵��:ʹ�ñ��صĶԳ��㷨����������(���ÿ��ʽ,���Ҫ���ԳƼ����㷨ֻ��Ҫ�����궨��Ϳ����� Ĭ��DES�㷨)
***********************************************************************/

int CKeyOperation::CRYPTAPI_Encrypt(unsigned char *pbPWD, int nPWDLen, 
									unsigned char *pbSrcData, int nSrcLen,
									unsigned char *pbDestData, int &nDestLen)
{
	//ɾ��CSP�д��ڵ�����
	CryptAcquireContext(&m_hProv, "DES", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

	if(!CryptAcquireContext(&m_hProv, "DES", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;
	
	HCRYPTHASH hHash;
	HCRYPTKEY hEncryptKey;
	//����HASH����
	if(!CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &hHash))
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}
	
	//HASH ����
	if(!CryptHashData(hHash, pbPWD, nPWDLen, 0))
		{
			if(m_hProv) 
				CryptReleaseContext(m_hProv, 0);
			if (hHash)
				CryptDestroyKey(hHash);
			return -3;
		}
	//�����ԳƼ�����Կ
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

	//�ԳƼ���
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
+��������: CRYPTAPI_Decrypt()
+�������:  pbPWD �ԳƼ��ܵĿ���
+			nPWDLen �����
+			pbSrcData ����������
+			nSrcLen  ���������ݳ���
+			pbDestData ���ܺ�����
+			nDestLen ���ܺ����ݳ���
+�������: pbDestData ���ܺ�����
+			nDestLen ���ܺ����ݳ���
+����ֵ:
+����˵��:ʹ�ñ��صĶԳ��㷨����������(���ÿ��ʽ,���Ҫ���Գƽ����㷨ֻ��Ҫ�����궨��Ϳ����� Ĭ��DES�㷨)
***********************************************************************/
int CKeyOperation::CRYPTAPI_Decrypt(unsigned char *pbPWD, int nPWDLen, unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen)
{
	int r;
	
	if(!CryptAcquireContext(&m_hProv, "DES", MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
		return -1;
	
	HCRYPTHASH hHash;
	HCRYPTKEY hDecryptKey;
	//����HASH����
	if(!CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &hHash))
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}
	
	//HASH ����
	if(!CryptHashData(hHash, pbPWD, nPWDLen, 0))
	{
		if(m_hProv) 
			CryptReleaseContext(m_hProv, 0);
		if (hHash)
			CryptDestroyKey(hHash);
		return -3;
	}
	//�����ԳƼ�����Կ
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
	
	//��������
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
+��������: CRYPTAPI_RSASign()
+�������:  pbSrcData ��ǩ������
+			nSrcLen ��ǩ�����ݳ���
+			pbDestData ǩ��������
+			nDestLen ǩ�������ݳ���
+�������: pbDestData ǩ��������
+			nDestLen ǩ�������ݳ���
+����ֵ:
+����˵��: ǩ��HASH�������
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
	if(!CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &hHash))  //����HASH����
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -3;
	}

	if(!CryptHashData(hHash, pbSrcData, (unsigned long)nSrcLen, 0))//hash����
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(hHash)
			CryptDestroyHash(hHash);
		return -3;
	}
	
	//���ǩ�����ݳ���
	if(!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, NULL, (unsigned long *)pnDestLen))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(hHash)
			CryptDestroyHash(hHash);
		return -4;
	}

	//ǩ��HASH����
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
+��������: CRYPTAPI_RSAVerify()
+�������:  pbSrcData ����֤����
+			nSrcLen ����֤���ݳ���
+			pbDestData ǩ������
+			nDestLen ǩ�����ݳ���
+�������: pbDestData ǩ��������
+			nDestLen ǩ�����ݳ���
+����ֵ:
+����˵��: ��֤����ǩ��HASH����
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
	
	//����HASH����
	if(!CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &hHash))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		return -2;
	}

	//HASH����֤����
	if(!CryptHashData(hHash, pbSrcData, nSrcLen, 0))
	{
		if(m_hProv)
			CryptReleaseContext(m_hProv, 0);
		if(hHash)
			CryptDestroyHash(hHash);
		return -3;
	}
	
	//��֤ǩ������
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
