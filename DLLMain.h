# ifndef REG_DLL_HEADER
# define REG_DLL_HEADER

# define WIN32_LEAN_AND_MEAN
# include <Windows.h>

# include "..\Utilities\HDSerial\HDSerialClass.h"
# include "RegistrationClass.h"
# include "CreateID.h"
# include "ChooseDisk.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
UINT GetLic();
UINT GetHDInfo();
UINT TestPeriod();
UINT TestHash();
UINT TestHash_();
UINT CreateID(LPSTR szMD5);

# endif // REG_DLL_HEADER
