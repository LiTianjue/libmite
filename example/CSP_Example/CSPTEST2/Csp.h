// Csp.h: interface for the CCsp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSP_H__202B3511_B4CA_4B95_8BCB_E04C51F73172__INCLUDED_)
#define AFX_CSP_H__202B3511_B4CA_4B95_8BCB_E04C51F73172__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef unsigned char   BYTE;
typedef unsigned long   DOWRD;
typedef int             BOOL;
typedef int				INT;
typedef char			CHAR;
typedef unsigned char   UCHAR;

#define KEYLENGTH  0x04000001

class CCsp  
{
public:
	int SessionDecrypt(BYTE *pbSessionKey, DWORD dwSessionLen, BYTE *pbSourceData, DWORD dwSourceLen, BYTE *pbPriKey, DWORD dwPriLen, BYTE *pbDestData, DWORD *pdwDestLen, BYTE *pbContainer);
	int SessionEncrypt(BYTE *pbSessionKey, DWORD dwSessionLen, 
						BYTE *pbSourceData, DWORD dwSourceLen, 
						BYTE *pbPriKey, DWORD dwPriLen,
						BYTE *pbDestData,DWORD *pdwDestLen, 
						BYTE *pbContainer);

	int ImportPriKey(CString strContainer, BYTE *pbPriKey, DWORD dwPriLen);
	int ImportPubKey(CString strContainer, BYTE *pbPubKey, DWORD dwPubLen);
	int GenRsaKeyPairs(int nKeyFlag, BYTE *pbPubKey, DWORD &dwPubKeyLen, 
						BYTE *pbPriKey, DWORD &dwPriKeyLen);
	void ReportError(CString strFunName, CString strErrorDescript, 
					int nErrorNo);
	CString m_strError;
	int GenSessionKey(BYTE *pbContainer, BYTE *pbPubKey, DWORD dwPubLen, 
					BYTE *pbSessionKey, DWORD *pdwSessionLen);
	CCsp();
	virtual ~CCsp();

};

#endif // !defined(AFX_CSP_H__202B3511_B4CA_4B95_8BCB_E04C51F73172__INCLUDED_)
