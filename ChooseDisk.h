# ifndef CHOOSE_DISK_HEADER
# define CHOOSE_DISK_HEADER

# include "Resources.h"

# include <Windows.h>
# include <CommCtrl.h>
# pragma comment(lib, "Comctl32.lib")

UINT ChooseDisk(LPSTR * cells, UINT nRow);
BOOL CALLBACK MainDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

# endif // CHOOSE_DISK_HEADER
