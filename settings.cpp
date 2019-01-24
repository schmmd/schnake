#include "global.h"
#include <assert.h>

void ChangeKey(HWND hWnd, int object, short int keycode);
bool VirtualToText(short iVirtual, char* szText);

int keyUp    = VK_NUMPAD8;
int keyRight = VK_NUMPAD6;
int keyDown  = VK_NUMPAD5;
int keyLeft  = VK_NUMPAD4;

int tempkeyUp    = VK_NUMPAD8;
int tempkeyRight = VK_NUMPAD6;
int tempkeyDown  = VK_NUMPAD5;
int tempkeyLeft  = VK_NUMPAD4;

#define MAX_GAME_DELAY  100
#define MAX_INIT_LENGTH 100
#define MAX_INC_LENGTH  500

unsigned int g_iGameDelay  = 50;
unsigned int g_iInitLength = 5;
unsigned int g_iIncLength  = 5;

HWND hGameSpeed;
HWND hInitLength;
HWND hGrowth;

BOOL CALLBACK SettingsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int RunModalWindow(HWND hwndDialog, HWND hwndParent);

bool bDone = false;

void LoadSettingsDialog()
{
	tempkeyUp    = keyUp;
	tempkeyDown  = keyDown;
	tempkeyRight = keyRight;
	tempkeyLeft  = keyLeft;

	HWND hParent = g_hMainWnd;
	HWND hDialog = CreateDialog(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SETTINGS), hParent, &SettingsProc);

	bDone = false;
	RunModalWindow(hDialog, hParent);
}

int RunModalWindow(HWND hwndDialog, HWND hwndParent)
{
	if(hwndParent != NULL)
		EnableWindow(hwndParent,FALSE);

	ShowWindow(hwndDialog, TRUE);

	MSG msg;

	while(!bDone)
	{
		WaitMessage();

		while(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				bDone = true;
				PostMessage(NULL, WM_QUIT, 0, 0);
				break;
			}
			else if (msg.message == WM_DESTROY)
			{
				bDone = true;
				break;
			}

			if(!IsDialogMessage(hwndDialog, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_KEYDOWN)
			{
				SettingsProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
			}
		}
	}

	if(hwndParent != NULL)
		EnableWindow(hwndParent,TRUE);

	DestroyWindow(hwndDialog);

	return 1;
}

BOOL CALLBACK SettingsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char szBuffer[48];
	static char szPrevEntry[48];
	static HWND hEdit;
	static int  iEdit;
	static bool bResetEditOnUpdate = 0;

	int vk;
	int pos;
	int fst_pos;
	RECT rect;
	RECT rect2;

	switch (message)
	{
		case WM_INITDIALOG:
			// Center
			GetWindowRect(g_hMainWnd, &rect );
			GetWindowRect(hWnd , &rect2);
			SetWindowPos (hWnd, HWND_TOP, ((rect.left + rect.right) / 2) - ((rect2.right - rect2.left) / 2), ((rect.top + rect.bottom) / 2) - ((rect2.bottom - rect2.top) / 2), 0, 0, SWP_NOSIZE);

			SendDlgItemMessage(hWnd, IDE_UP   , EM_LIMITTEXT, 5, 0);
			SendDlgItemMessage(hWnd, IDE_RIGHT, EM_LIMITTEXT, 5, 0);
			SendDlgItemMessage(hWnd, IDE_DOWN , EM_LIMITTEXT, 5, 0);
			SendDlgItemMessage(hWnd, IDE_LEFT , EM_LIMITTEXT, 5, 0);

			VirtualToText(keyLeft, szBuffer);
			SendDlgItemMessage(hWnd, IDE_LEFT , WM_SETTEXT, 0, (LPARAM)&szBuffer);
			VirtualToText(keyRight, szBuffer);
			SendDlgItemMessage(hWnd, IDE_RIGHT , WM_SETTEXT, 0, (LPARAM)&szBuffer);
			VirtualToText(keyUp, szBuffer);
			SendDlgItemMessage(hWnd, IDE_UP , WM_SETTEXT, 0, (LPARAM)&szBuffer);
			VirtualToText(keyDown, szBuffer);
			SendDlgItemMessage(hWnd, IDE_DOWN , WM_SETTEXT, 0, (LPARAM)&szBuffer);

			hGameSpeed  = GetDlgItem(hWnd, IDS_GAMESPEED );
			hInitLength = GetDlgItem(hWnd, IDS_INITLENGTH);
			hGrowth     = GetDlgItem(hWnd, IDS_GROWTH    );

			SendMessage(hGameSpeed,  SBM_SETRANGE, 0, MAX_INIT_LENGTH);
			SendMessage(hInitLength, SBM_SETRANGE, 1, MAX_INC_LENGTH );
			SendMessage(hGrowth,     SBM_SETRANGE, 0, MAX_GAME_DELAY );

			SendMessage(hGameSpeed,  SBM_SETPOS, g_iGameDelay , 1);
			SendMessage(hInitLength, SBM_SETPOS, g_iInitLength, 1);
			SendMessage(hGrowth,     SBM_SETPOS, g_iIncLength , 1);

			sprintf(szBuffer, "Game Speed - %d", g_iGameDelay);
			SendMessage(hGameSpeed,  WM_SETTEXT, 4, (LPARAM)&szBuffer);
			sprintf(szBuffer, "Initial Length - %d", g_iInitLength);
			SendMessage(hInitLength, WM_SETTEXT, 4, (LPARAM)&szBuffer);
			sprintf(szBuffer, "Growth Interval - %d", g_iIncLength);
			SendMessage(hGrowth,     WM_SETTEXT, 4, (LPARAM)&szBuffer);

			sprintf(szBuffer, "Game Delay - %d", g_iGameDelay);
			SendDlgItemMessage(hWnd, IDF_SPEED, WM_SETTEXT, 4, (LPARAM)&szBuffer);
			sprintf(szBuffer, "Initial Length - %d", g_iInitLength);
			SendDlgItemMessage(hWnd, IDF_INITLENGTH, WM_SETTEXT, 4, (LPARAM)&szBuffer);
			sprintf(szBuffer, "Growth Interval - %d", g_iIncLength);
			SendDlgItemMessage(hWnd, IDF_GROWTH, WM_SETTEXT, 4, (LPARAM)&szBuffer);

			return 1;

		case WM_KEYDOWN:
			vk = (int)wParam;

			GetWindowText(hEdit, szPrevEntry, 48);
			if (VirtualToText(vk, szBuffer))
			{
				switch(iEdit)
				{
				case IDE_LEFT:
					tempkeyLeft = vk;
					break;
				case IDE_RIGHT:
					tempkeyRight = vk;
					break;
				case IDE_UP:
					tempkeyUp = vk;
					break;
				case IDE_DOWN:
					tempkeyDown = vk;
					break;

				default:
					break;
				}
			}
			else
			{
				strcpy(szBuffer, szPrevEntry);
			}

			SetWindowText(hEdit, szBuffer);
			bResetEditOnUpdate = true;
			break;


		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDE_LEFT:  case IDE_RIGHT:  case IDE_UP:  case IDE_DOWN:
					hEdit = (HWND)lParam;
					iEdit = (int )LOWORD(wParam);
					if (HIWORD(wParam) == EN_UPDATE)
					{
						if (bResetEditOnUpdate)
						{
							bResetEditOnUpdate = false;
							SetWindowText(hEdit, szBuffer);
							break;
						}
					}
					else if (HIWORD(wParam) == EN_SETFOCUS || HIWORD(wParam) == EN_CHANGE)
					{
						SendMessage((HWND)lParam, EM_SETSEL, 0, 16);
					}
					return 1;

				case IDC_OK:
					keyLeft = tempkeyLeft;
					keyRight = tempkeyRight;
					keyUp = tempkeyUp;
					keyDown = tempkeyDown;

					g_iGameDelay  = SendDlgItemMessage(hWnd, IDS_GAME_DELAY , SBM_GETPOS, 0, 0);
					g_iInitLength = SendDlgItemMessage(hWnd, IDS_INIT_LENGTH, SBM_GETPOS, 0, 0);
					g_iIncLength  = SendDlgItemMessage(hWnd, IDS_INC_LENGTH , SBM_GETPOS, 0, 0);

					SaveHighscores();

					SendMessage(hWnd, WM_DESTROY, 0, 0);
					bDone = true;
					return 1;

				case IDC_CANCEL:
					SendMessage(hWnd, WM_DESTROY, 0, 0);
					bDone = true;
					return 1;
			}
		break;

		case WM_HSCROLL:
			fst_pos = pos = SendMessage((HWND)lParam, SBM_GETPOS, 0, 0);

			switch (LOWORD(wParam))
			{
				case SB_LINELEFT:
					pos -= 1;
					break;

				case SB_LINERIGHT:
					pos += 1;
					break;

				case SB_PAGELEFT:
					pos -= 10;
					break;

				case SB_PAGERIGHT:
					pos += 10;
					break;

				case SB_THUMBPOSITION:
					pos = HIWORD(wParam);
					break;
			}

			if (pos != fst_pos)
			{
				SendMessage((HWND)lParam, SBM_SETPOS, pos, 1);

				if (lParam == (LPARAM)hGameSpeed)
				{
					sprintf(szBuffer, "Game Speed - %d", pos);
					SendDlgItemMessage(hWnd, IDF_SPEED, WM_SETTEXT, 4, (LPARAM)&szBuffer);
				}
				else if (lParam == (LPARAM)hInitLength)
				{
					sprintf(szBuffer, "Initial Length - %d", pos);
					SendDlgItemMessage(hWnd, IDF_INITLENGTH, WM_SETTEXT, 4, (LPARAM)&szBuffer);
				}
				else if (lParam == (LPARAM)hGrowth)
				{
					sprintf(szBuffer, "Growth Interval - %d", pos);
					SendDlgItemMessage(hWnd, IDF_GROWTH, WM_SETTEXT, 4, (LPARAM)&szBuffer);
				}

			}
			return 1;

		case WM_DESTROY:
			EndDialog(hWnd, 0);
			return 1;
	}

	return 0;
}

void ChangeKey(HWND hWnd, int object, short int keycode)
{
	char text[16] = "";

	text[0] = (char)keycode;
	SetDlgItemText(hWnd, IDE_UP, text);
}

bool VirtualToText(short iVirtual, char* szText)
{
	assert(iVirtual != 0);

	// Extract shift
	bool bShift = (iVirtual & 256) == 256; // or >> 8
	// Clear shift
	iVirtual &= 0xFEFF;
	switch(iVirtual)
	{
		case VK_LBUTTON:
			sprintf(szText, "Left Button");
			return true;
		case VK_RBUTTON:
			sprintf(szText, "Right Button");
			return true;
		case VK_MBUTTON:
			sprintf(szText, "Mid Button");
			return true;
		case VK_BACK:
			sprintf(szText, "Backspace");
			return true;
		case VK_TAB:
			sprintf(szText, "Tab");
			return true;
		case VK_RETURN:
			sprintf(szText, "Return");
			return true;
		case VK_SHIFT:
			sprintf(szText, "Shift");
			return true;
		case VK_CONTROL:
			sprintf(szText, "Control");
			return true;
		case VK_MENU:
			sprintf(szText, "Alt");
			return true;
		case VK_SPACE:
			sprintf(szText, "Space");
			return true;
		case VK_LEFT:
			sprintf(szText, "Left");
			return true;
		case VK_RIGHT:
			sprintf(szText, "Right");
			return true;
		case VK_UP:
			sprintf(szText, "Up");
			return true;
		case VK_DOWN:
			sprintf(szText, "Down");
			return true;
		case VK_ADD:
			sprintf(szText, "Add");
			return true;
		case VK_SUBTRACT:
			sprintf(szText, "Subtract");
			return true;
		case VK_MULTIPLY:
			sprintf(szText, "Multiply");
			return true;
		case VK_DIVIDE:
			sprintf(szText, "Divide");
			return true;
	}

	if (iVirtual >= 48 && iVirtual < 58)
	{
		// Number
		sprintf(szText, "Num %d", iVirtual - 48);
		return true;
	}
	if (iVirtual >= 96 && iVirtual <= 105)
	{
		// Number
		sprintf(szText, "Num %d", iVirtual - 96);
		return true;
	}
	if (iVirtual > 64 && iVirtual <= 90)
	{
		// Letter
		sprintf(szText, "Key %c", (char)iVirtual);
		return true;
	}
	if (iVirtual > 38 && iVirtual < 48)
	{
		// Letter
		sprintf(szText, "Key %c", (char)iVirtual);
		return true;
	}

	return false;
}