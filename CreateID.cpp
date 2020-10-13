# include "CreateId.h"

# define R_ERROR_CRYPT_ACQ		0xA1
# define R_ERROR_HASH_CREATE	0xA7
# define R_ERROR_HASH_GET		0xAA

# define R_ERROR_HASH_DATA		0xAB

# define S_MESSAGE_COPY_ID		"Copy Reg ID to Clipboard"
# define S_ERROR_COPY			L"Error during copy"
# define S_MESSAGE_TITLE		L" " L"Reg-ID"

UINT CreateID(const LPBYTE lpData, UINT nSize, LPSTR szMD5)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;

	BYTE hashVal[16];
	DWORD dwHashLen = 16;

	if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return R_ERROR_CRYPT_ACQ;

	if(!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		return R_ERROR_HASH_CREATE;

	if(!CryptHashData(hHash, lpData, nSize, 0))
		return R_ERROR_HASH_DATA;

	if(!CryptGetHashParam(hHash, HP_HASHVAL, hashVal, &dwHashLen, 0))
		return R_ERROR_HASH_GET;

	HexStringA(szMD5, hashVal, dwHashLen);

	if(hHash) 
		CryptDestroyHash(hHash);

	return R_SUCCESS;
}
