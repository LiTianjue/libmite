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
+��������:�����������Ự��Կ
+�������:	BYTE *pbContainer ������
+			BYTE *pbPubKey ��Կ����
+			DWORD dwPubLen ��Կ���ݳ���
+			BYTE *pbSessionKey �Ự��Կ����
+			DWORD &dwSessionLen �Ự��Կ���ݳ���
+����ֵ: 0 �ɹ�
+����˵��:	
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
		ReportError("GenSessionKey", "���뱣����Կʧ��!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}
	
	//�����Ự��Կ
	r = CryptGenKey(hProv, CALG_3DES, CRYPT_EXPORTABLE, &hSessionKey);
	if(!r)
	{
		ReportError("GenSessionKey", "�����Ự��Կʧ��!", -3);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		return -3;
	}
	
	//��ûỰ��Կ����
	r = CryptExportKey(hSessionKey, hKey, SIMPLEBLOB, NULL, NULL, pdwSessionLen);
	if(!r)
	{
		ReportError("GenSessionKey", "��ûỰ��Կ����ʧ��!", -4);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		if(hSessionKey)
			CryptDestroyKey(hSessionKey);
		return -4;
	}
	
	//�����Ự��Կ
	r = CryptExportKey(hSessionKey, hKey, SIMPLEBLOB, NULL, pbSessionKey, pdwSessionLen);
	if(!r)
	{
		ReportError("GenSessionKey", "��ûỰ��Կ����ʧ��!", -5);
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
+��������:����RSA��Կ�Բ�������Կ˽Կ����
+�������:	int nKeyFlag ��Կ������ 1 Ϊǩ����Կ�� 2 Ϊ������Կ��
+			BYTE *pbPubKey ��Կ����(���ܹ�Կ�ļ�.txt ǩ����Կ�ļ�.txt)
+			DWORD dwPubKeyLen ��Կ���ݳ���
+			BYTE *pbPriKey ˽Կ����(����˽Կ�ļ�.txt ǩ��˽Կ�ļ�.txt)
+			DWORD &dwSessionLen �Ự��Կ���ݳ���
+����ֵ: 0 �ɹ�
+����˵��:	
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
			//ɾ������ �ͷ�������
			ReportError("GenRsaKeyPairs", "����RSAǩ����Կ��ʧ��!", -2);
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
			//ɾ������ �ͷ�������
			ReportError("GenRsaKeyPairs", "����RSAǩ����Կ��ʧ��!", -3);
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
	
	//����˽Կ
	DWORD dwTempPriLen;
	
	int r = CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, NULL, NULL, &dwTempPriLen);
	if(!r)
	{
		ReportError("GenRsaKeyPairs", "����˽Կ����ʧ��!", -5);
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -5;
	}
	
	BYTE *pbTempPriData = (BYTE *)malloc(dwTempPriLen+1);
	
	r = CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, NULL, pbTempPriData, &dwTempPriLen);
	if(!r)
	{
		ReportError("GenRsaKeyPairs", "����˽Կ����ʧ��!", -6);
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -6;
	}
	
	//������Կ
	DWORD dwTempPubLen;
	
	r = CryptExportKey(hKey, NULL, PUBLICKEYBLOB, NULL, NULL, &dwTempPubLen);
	if(!r)
	{
		ReportError("GenRsaKeyPairs", "������Կ����ʧ��!", -7);
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -7;
	}
	
	BYTE *pbTempPubData = (BYTE *)malloc(dwTempPubLen+1);
	
	r = CryptExportKey(hKey, NULL, PUBLICKEYBLOB, NULL, pbTempPubData, &dwTempPubLen);
	if(!r)
	{
		ReportError("GenRsaKeyPairs", "������Կ����ʧ��!", -8);
		CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -8;
	}
	
	//����������
	memcpy(pbPriKey, pbTempPriData, dwTempPriLen); //����˽Կ����
	dwPubKeyLen = dwTempPubLen; //˽Կ���ݳ���
	memcpy(pbPubKey, pbTempPubData, dwTempPubLen); //������Կ����
	dwPriKeyLen = dwTempPriLen; //��Կ����
	
	//ɾ������
	CryptAcquireContext(&hProv, "RSA", MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

	//�ͷ��ڴ�
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
	m_strError.Format("������%s:��������%s:�������%d", strFunName, strErrorDescript, nErrorNo);
	return;
}

/*************************************************************************
+��������:���빫Կ����
+�������:	CString strContainer ��������
+			BYTE *pbPubKey ��Կ����(���ܹ�Կ�ļ�.txt ǩ����Կ�ļ�.txt)
+			DWORD dwPubLen ��Կ���ݳ���
+����ֵ: 0 �ɹ�
+����˵��:	
*************************************************************************/
int CCsp::ImportPubKey(CString strContainer, BYTE *pbPubKey, DWORD dwPubLen)
{
	
	HCRYPTPROV hProv;
	HCRYPTKEY hKey;

	CryptAcquireContext(&hProv, strContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	
	if(!CryptAcquireContext(&hProv, strContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;

	//���빫Կ����
	int r = CryptImportKey(hProv, pbPubKey, dwPubLen, NULL, NULL, &hKey);
	if(!r)
	{
		ReportError("ImportPubKey", "���빫Կ����ʧ��!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}

	if(hProv)
		CryptReleaseContext(hProv, 0);
	return 0;
}

/*************************************************************************
+��������:����˽Կ����
+�������:	CString strContainer ��������
+			BYTE *pbPriKey ˽Կ����(����˽Կ�ļ�.txt ǩ��˽Կ�ļ�.txt)
+			DWORD dwPriLen ˽Կ���ݳ���
+����ֵ: 0 �ɹ�
+����˵��:	
*************************************************************************/
int CCsp::ImportPriKey(CString strContainer, BYTE *pbPriKey, DWORD dwPriLen)
{
	HCRYPTPROV hProv;
	HCRYPTKEY hKey;

	CryptAcquireContext(&hProv, strContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	
	if(!CryptAcquireContext(&hProv, strContainer, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		return -1;

	//����˽Կ����
	int r = CryptImportKey(hProv, pbPriKey, dwPriLen, NULL, NULL, &hKey);
	if(!r)
	{
		ReportError("ImportPriKey", "����˽Կ����ʧ��!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}

	if(hProv)
		CryptReleaseContext(hProv, 0);

	return 0;
}

/*************************************************************************
+��������:�Ự��������
+�������:	BYTE *pbSessionKey �Ự��Կ���� (�Ự��Կ�����ļ�.txt)
+			 DWORD dwSessionLen �Ự��Կ����
+			BYTE *pbSourceData ���������� (�Ự����ԭ��.txt)
+			DWORD dwSourceLen ���������ݳ���
+			BYTE *pbPriKey ����˽Կ���� (����˽Կ�ļ�.txt)
+			DWORD dwPriLen ����˽Կ���ݳ���
+			BYTE *pbDestData ���ܺ����� (�Ự���ݼ�������.txt)
+			DWORD &dwDestLen ���ܺ����ݳ���
+			CString strContainer ��������
+����ֵ: 0 �ɹ�
+����˵��:	
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
	
	//����˽Կ��������
	int r = CryptImportKey(hProv, pbPriKey, dwPriLen, 0, NULL, &hKey);
	if(!r)
	{
		ReportError("SessionEncrypt", "����Ự����˽Կʧ��!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}
	
	r = CryptImportKey(hProv, pbSessionKey, dwSessionLen, hKey, NULL, &hSessionKey);
	if(!r)
	{
		ReportError("SessionEncrypt", "����Ự��Կʧ��!", -3);
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
		ReportError("SessionEncrypt", "��������ʧ��!", -4);
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
+��������:�Ự��������
+�������:	BYTE *pbSessionKey �Ự��Կ���� (�Ự��Կ�����ļ�.txt)
+			 DWORD dwSessionLen �Ự��Կ����
+			BYTE *pbSourceData ���������� (�Ự���ݼ�������.txt)
+			DWORD dwSourceLen ���������ݳ���
+			BYTE *pbPriKey ����˽Կ���� (����˽Կ�ļ�.txt)
+			DWORD dwPriLen ����˽Կ���ݳ���
+			BYTE *pbDestData ���ܺ����� (�Ự���ܺ�����.txt)
+			DWORD &dwDestLen ���ܺ����ݳ���
+			CString strContainer ��������
+����ֵ: 0 �ɹ�
+����˵��:	
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
	
	//����˽Կ��������
	int r = CryptImportKey(hProv, pbPriKey, dwPriLen, 0, NULL, &hKey);
	if(!r)
	{
		ReportError("SessionEncrypt", "����Ự����˽Կʧ��!", -2);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		return -2;
	}
	
	r = CryptImportKey(hProv, pbSessionKey, dwSessionLen, hKey, NULL, &hSessionKey);
	if(!r)
	{
		ReportError("SessionEncrypt", "����Ự��Կʧ��!", -3);
		if(hProv)
			CryptReleaseContext(hProv, 0);
		if(hKey)
			CryptDestroyKey(hKey);
		return -3;
	}
	
	r = CryptDecrypt(hSessionKey, NULL, TRUE, NULL, pbSourceData, &dwSourceLen);
	if(!r)
	{
		ReportError("SessionEncrypt", "�Ự��������ʧ��!", -4);
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
