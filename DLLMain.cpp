#include "DLLMain.h"

# define R_BAD_DISK_INFO		536

# define MESSAGEBOX_TITLE		COMPANY L" " KEY_PATH

# define DECIMAL				10
# define MAX_DISK_SIZE_LEN		50

# define R_SUCCESS				0

HMODULE hModule;
DiskInfo_** DiskInfo = NULL;
UINT* arI = NULL;
UINT n = 0;
RegistrationClass Registration;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		::hModule = hModule;
		DiskInfo = new DiskInfo_*[MAX_IDE_DRIVES];
		arI = new UINT[MAX_IDE_DRIVES];
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		delete arI;
		delete DiskInfo;
		break;
	default: break;
	}
	return TRUE;
}

UINT GetLic()
{
	return Registration.GetLicense();
}

UINT GetHDInfo()
{
	HDSerialClass HDSerial(DiskInfo);
	HDSerial.AcquireDiskInfo(arI, n);
	if(!n)
		return R_BAD_DISK_INFO;
	return R_SUCCESS;
}

UINT TestPeriod()
{
	return Registration.CheckTime();
}

UINT TestHash()
{
	UINT rVal;
		
	rVal = Registration.CryptInit();
	if(rVal != R_SUCCESS)
		return(rVal);

	rVal = TestHash_();// Registration.CryptInit();
	if(rVal != R_SUCCESS)
		return(rVal);

	rVal = Registration.CryptClose();
	if(rVal != R_SUCCESS)
		return(rVal);

	return R_SUCCESS;
}

UINT TestHash_()
{
	for(UINT i = 0; i < n; i++)
	{
		UINT rVal = Registration.CheckHash((const LPBYTE) DiskInfo[i], sizeof DiskInfo_);
		switch(rVal)
		{
		case R_SUCCESS:
			return R_SUCCESS;
		case R_CHECK_FAIL:
			break;	// Continue checking with remaining
		default:
			return rVal;
			;// log: (rVal);
		}
	}
	return R_CHECK_FAIL;
}

UINT CreateID(LPSTR szMD5)
{
	UINT iDisk;
	// n == 0 - not Expected Over GetHD_
	if(n == 1)
		iDisk = 0;
	else
	{
		LPSTR * szCells = new LPSTR[n*4];
		for(UINT i = 0, j = 0; i < n; i++)
		{
			szCells[j++] = DiskInfo[arI[i]]->szSerialNumber;
			szCells[j++] = DiskInfo[arI[i]]->szModelNumber;
			szCells[j] = new CHAR[MAX_DISK_SIZE_LEN];
			_i64toa_s(DiskInfo[arI[i]]->size, szCells[j++], MAX_DISK_SIZE_LEN, DECIMAL);
		}
		iDisk = ChooseDisk(szCells, n);
	}
	return CreateID((const LPBYTE) DiskInfo[arI[iDisk]], sizeof DiskInfo_, szMD5);
}
