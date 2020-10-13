# include "ChooseDisk.h"

# define ListView_SetItemTextA(hwndLV, i, iSubItem_, pszText_)							\
{																						\
	LV_ITEMA _macro_lvi;																\
	_macro_lvi.iSubItem = (iSubItem_);													\
	_macro_lvi.pszText = (pszText_);													\
	SNDMSG((hwndLV), LVM_SETITEMTEXTA, (WPARAM)(i), (LPARAM)(LV_ITEMA *)&_macro_lvi);	\
}

extern HMODULE hModule;
LPSTR* cells;
UINT nRow;

UINT ChooseDisk(LPSTR* cells, UINT nRow)
{
	::cells = cells;
	::nRow = nRow;

	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	INT_PTR rVal = DialogBox(::hModule, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, (DLGPROC)MainDlgProc);
	// GetModuleHandle(NULL) - would return exe's hinstance - which would not contain the Res -> Error 0x0...715
	switch(rVal)
	{
	// case 0: NA - Since hWParent = NULL - so no parent error
	case -1:
		DWORD X;
		X = GetLastError();
		//return 0;
		exit(-1);
	default:

	return (UINT)rVal;
	}
}

BOOL CALLBACK MainDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
        {
			HICON hIcon = LoadIcon(::hModule, MAKEINTRESOURCE(IDI_REG_ICON));
			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			//SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            HWND listView = GetDlgItem(hwnd, IDC_LISTVIEW);
            ListView_SetExtendedListViewStyle(listView, LVS_EX_FULLROWSELECT);
            
            //Columns
            LVCOLUMN lc;
            lc.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
            
            lc.iSubItem = 0;
            lc.cx = 148;
            lc.pszText = L"Serial";
            ListView_InsertColumn(listView, 0, &lc);
            
            lc.iSubItem = 1;
            lc.cx = 146;
            lc.pszText = L"Details";
            ListView_InsertColumn(listView, 1, &lc);
            
            lc.iSubItem = 2;
            lc.cx = 146;
            lc.pszText = L"Size";
            ListView_InsertColumn(listView, 2, &lc);
            
            //items
            LVITEM li = {0};
            li.mask = LVIF_TEXT| LVIF_GROUPID | LVIF_COLUMNS;
            
			for(UINT i = 0, id = 0; i < ::nRow; i++)
			{
				li.pszText = NULL;
				li.iItem = id;
				li.iSubItem = 0;
				li.iGroupId = 101;
				ListView_InsertItem(listView, &li);
				for(UINT j = 0; j < 3; j++)
					ListView_SetItemTextA(listView, i, j, ::cells[id++]);
			}
        }
        break;

		case WM_COMMAND:
			switch(wParam)
			{
			case IDC_BUTTON_SELECT:
				{
				HWND listView = GetDlgItem(hwnd, IDC_LISTVIEW);
				UINT n;
				n = ListView_GetSelectedCount(listView);
				if(n != 1)
					break;
				UINT x;
				x = ListView_GetNextItem(listView, -1, LVNI_SELECTED);
				EndDialog(hwnd, x);
				}
				break;
			case IDC_BUTTON_CANCEL:
	            EndDialog(hwnd, -1);	// OverLap - With return
			}
			break;
        case WM_CLOSE:
            EndDialog(hwnd, 0);
            break;
    }

    return FALSE;
}
