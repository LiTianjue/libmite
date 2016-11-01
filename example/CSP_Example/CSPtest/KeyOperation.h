// KeyOperation.h: interface for the CKeyOperation class.
// Download by http://www.codefans.net
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYOPERATION_H__17F4FF0D_3366_46F4_A195_CF140DD75298__INCLUDED_)
#define AFX_KEYOPERATION_H__17F4FF0D_3366_46F4_A195_CF140DD75298__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wincrypt.h"

class CKeyOperation  
{
public:
	int CRYPTAPI_RSAVerify(unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int nDestLen);
	int CRYPTAPI_RSASign(unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int *pnDestLen);
	int CRYPTAPI_Decrypt(unsigned char * pbPWD, int nPWDLen, unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen);
	int CRYPTAPI_Encrypt(unsigned char *pbPWD, int nPWDLen, unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen);
	int CRYPTAPI_RSADecrypt(unsigned char *pbSrcData, int nSrcLen, unsigned char *pbDestData, int &nDestLen);
	int CRYPTAPI_RSAEncrypt(unsigned char *pbSrcData, int nSrcLen, 
		unsigned char * pbDestData, int &nDestLen);
	int CRYPTAPI_ExportPubKey(int nFlag, unsigned char *pbPubKeyData, int &nDataLen);

	HCRYPTKEY m_hKey;
	HCRYPTPROV m_hProv;
	int CRYPTAPI_GenRsaKey(int nFlag);
	CKeyOperation();
	virtual ~CKeyOperation();

};

#endif // !defined(AFX_KEYOPERATION_H__17F4FF0D_3366_46F4_A195_CF140DD75298__INCLUDED_)
