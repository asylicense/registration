# ifndef REGISTRATION_CLASS_HEADER
# define REGISTRATION_CLASS_HEADER

# include <Windows.h>
# include <WinCrypt.h>
# pragma comment(lib, "Crypt32.lib")

# include "..\\Utilities\\StampHash\\StampHash.h"
# include "..\\Utilities\\Base\\HexString\\HexString.h"
# include "ValidateTime.h"

# define R_SUCCESS				0
# define R_CHECK_FAIL			12

# define KEY_HASH_LEN			256

class RegistrationClass
{
private:
	HCRYPTPROV hProv;
	HCRYPTKEY hPubKey;

	FILETIME maxTime;
	BYTE pbSignature[KEY_HASH_LEN];

public:
//	RegistrationClass();
	UINT CryptInit();
	UINT CryptClose();
	UINT GetLicense();
	UINT CheckTime();
	UINT CheckHash(const LPBYTE lpData, UINT nSize);
};

# endif // REGISTRATION_CLASS_HEADER
