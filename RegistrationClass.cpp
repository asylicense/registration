# include "..\\..\\Registration\\Params.h"
# include "RegistrationClass.h"

# define CERT_STORE_NAME		L"TrustedPublisher"
# define COMPANY_ID				L"CN = " COMPANY

# define SOFTWARE_REG_KEY		L"SOFTWARE" L"\\" COMPANY L"\\" KEY_PATH L"\\" L"Key"
# define REG_KEY_VALUE_NAME		L"Hash"
# define REG_TIME_VALUE_NAME	L"Valid Time"

# define PUB_KEY_BLOB_LEN		276

# define R_ERROR_CRYPT_ACQ		0xA1
# define R_ERROR_PBK_IMP		0xA2
# define R_ERROR_CRYPT			0xA6
# define R_ERROR_HASH_CREATE	0xA7
# define R_ERROR_HASH_CREATE_	0xA8
# define R_ERROR_HASH_SET		0xA9
# define R_ERROR_HASH_GET		0xAA
# define R_ERROR_HASH_DATA		0xAB

# define R_ERROR_PBK_DTO		0xC1
# define R_ERROR_CRYPT_RLS		0xC2

# define R_ERROR_KEY_OPEN		3
# define R_ERROR_KEYVAL_KEY		4
# define R_ERROR_KEYVAL_TIME	5

# define MD5_HASH_LEN			16

UINT RegistrationClass::CryptInit()
{
	if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return R_ERROR_CRYPT_ACQ;

	const BYTE xgPKeyB[PUB_KEY_BLOB_LEN] = PUBKEY_BLOB;
	
	PCCERT_CONTEXT certContext = CertCreateCertificateContext(X509_ASN_ENCODING, xgPKeyB, PUB_KEY_BLOB_LEN);

	if(!CryptImportKey(hProv, xgPKeyB, PUB_KEY_BLOB_LEN, 0, 0, &hPubKey))
		return R_ERROR_PBK_IMP;

	return R_SUCCESS;
}

UINT RegistrationClass::CryptClose()
{
	if(!CryptDestroyKey(hPubKey))
		return R_ERROR_PBK_DTO;

	if(!CryptReleaseContext(hProv, 0))
		return R_ERROR_CRYPT_RLS;

	return R_SUCCESS;
}

UINT RegistrationClass::GetLicense()
{
	HKEY hRegKey;
	DWORD nBytes;
	
	nBytes = KEY_HASH_LEN;
	if(RegOpenKeyEx(HKEY_CURRENT_USER, SOFTWARE_REG_KEY, NULL, KEY_QUERY_VALUE, &hRegKey) != ERROR_SUCCESS)
		return R_ERROR_KEY_OPEN;
	if(RegQueryValueEx(hRegKey, REG_KEY_VALUE_NAME, NULL, NULL, pbSignature, &nBytes) != ERROR_SUCCESS)
	{
		RegCloseKey(hRegKey);
		return R_ERROR_KEYVAL_KEY;
	}

	nBytes = sizeof(FILETIME);
	if(RegQueryValueEx(hRegKey, REG_TIME_VALUE_NAME, NULL, NULL, (LPBYTE)&maxTime, &nBytes) != ERROR_SUCCESS)
	{
		RegCloseKey(hRegKey);
		return R_ERROR_KEYVAL_TIME;
	}

	RegCloseKey(hRegKey);
	return R_SUCCESS;
}

UINT RegistrationClass::CheckTime()
{
	return ValidateTime(maxTime);
}

UINT RegistrationClass::CheckHash(const LPBYTE lpData, UINT nSize)
{
	HCRYPTHASH hHash;
	if(!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) 
		return R_ERROR_HASH_CREATE;

	if(!CryptHashData(hHash, lpData, nSize, 0)) 
	{
		CryptDestroyHash(hHash);
		return R_ERROR_HASH_DATA;
	}

	DWORD dwHashLen = MD5_HASH_LEN;
	BYTE hashVal[MD5_HASH_LEN];

	if(!CryptGetHashParam(hHash, HP_HASHVAL, hashVal, &dwHashLen, 0))
		return R_ERROR_HASH_GET;

	StampHash(hashVal, dwHashLen, (LPBYTE) &maxTime, sizeof FILETIME);

	//Needs to be recreated - before reuse
	CryptDestroyHash(hHash);
	if(!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) 
		return R_ERROR_HASH_CREATE_;

	if(!CryptSetHashParam(hHash, HP_HASHVAL, hashVal, 0))
	//UINT x = GetLastError();
	{
		CryptDestroyHash(hHash);
		return R_ERROR_HASH_SET;
	}

	if(CryptVerifySignature(hHash, pbSignature, KEY_HASH_LEN, hPubKey, NULL, 0)) 
	{
		CryptDestroyHash(hHash);
		return R_SUCCESS;
	}

	CryptDestroyHash(hHash);
	return R_CHECK_FAIL;
}
