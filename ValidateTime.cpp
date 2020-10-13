# include "ValidateTime.h"

# define TIME_GREATER_FLAG		1

# define OBSCURE_REG_KEY		L"System" L"\\" OBSCURE_GUID
# define CHECK_FILE				L"\\" OBSCURE_GUID

# define OBSCURE_REG_VALUE_NAME	L"X"

# define MAX_PATH_LEN			300

# define R_ERROR_LIC_VT_F_WRITE		0xB1
# define R_ERROR_LIC_VT_F_READ		0xB2
# define R_ERROR_LIC_VT_F			0xB4
# define R_ERROR_LIC_VT_R			0xB5

UINT ValidateTime(FILETIME& maxTime)
{
	SYSTEMTIME stCur;
	FILETIME ftCur;
	FILETIME ftMin;

	LPWSTR szAppdataPath;
	szAppdataPath = new WCHAR[MAX_PATH_LEN];

	HKEY hRegKey;
	DWORD nBytes;

	GetSystemTime(&stCur);
	SystemTimeToFileTime(&stCur, &ftCur);
	if(CompareFileTime(&ftCur, &maxTime) == TIME_GREATER_FLAG)
		return R_LIC_VT_EXPIRED;

	SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szAppdataPath);
	wcscat_s(szAppdataPath, MAX_PATH_LEN, CHECK_FILE);

	HANDLE hFile;
	DWORD nWritten;
	DWORD dwNumRead;
	hFile = CreateFile(szAppdataPath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	switch(GetLastError())
	{
	case 0:
		if(!WriteFile(hFile, (BYTE*)&ftCur, sizeof(FILETIME), &nWritten, NULL))
			return(R_ERROR_LIC_VT_F_WRITE);
		break;
	case ERROR_ALREADY_EXISTS:
		if(!ReadFile(hFile, (LPBYTE)&ftMin, sizeof(FILETIME), &dwNumRead, NULL))
			return(R_ERROR_LIC_VT_F_READ);
		//ftMin ^= ftMin;
		if(CompareFileTime(&ftMin, &ftCur) == TIME_GREATER_FLAG)
			return R_LIC_VT_F_VIOLATED;
		break;
	default:
		return R_ERROR_LIC_VT_F;
	}
	CloseHandle(hFile);

	RegOpenKeyEx(HKEY_CURRENT_USER, OBSCURE_REG_KEY, NULL, KEY_QUERY_VALUE, &hRegKey);
	nBytes = sizeof(FILETIME);
	if(RegQueryValueEx(hRegKey, OBSCURE_REG_VALUE_NAME, NULL, NULL, (LPBYTE)&ftMin, &nBytes) != ERROR_SUCCESS)
	{
		DWORD dispos;
		RegCloseKey(hRegKey);
		RegCreateKeyEx(HKEY_CURRENT_USER, OBSCURE_REG_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hRegKey, &dispos);
		if(RegSetValueEx(hRegKey, NULL, 0, REG_QWORD, (LPBYTE)&ftCur, nBytes) != ERROR_SUCCESS)
			return R_ERROR_LIC_VT_R;
		RegCloseKey(hRegKey);
		return R_SUCCESS;
	}
	//ftMin ^= ftMin;
	if(CompareFileTime(&ftMin, &ftCur) == TIME_GREATER_FLAG)
		return R_LIC_VT_R_VIOLATED;
	return R_SUCCESS;
}
