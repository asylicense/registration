# ifndef VALIDATE_TIME_HEADER
# define VALIDATE_TIME_HEADER

# include "Params.h"

# include <Windows.h>
# include <ShlObj.h>
# pragma comment(lib, "Shell32.lib")

# define R_LIC_VT_EXPIRED		0xB0
# define R_LIC_VT_F_VIOLATED	0xB3
# define R_LIC_VT_R_VIOLATED	0xB6

# define R_SUCCESS				0

UINT ValidateTime(FILETIME& maxTime);

# endif // VALIDATE_TIME_HEADER
