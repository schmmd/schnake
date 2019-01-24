#include "global.h"

// Variable Definitions
HWND      g_hMainWnd;
HINSTANCE g_hInstance;

// Window sizes
RECT client;
RECT score;
RECT game;

const char* sWindowTitle = "Schnake";
const char* sWindowClass = "CWindow";

BOOL CALLBACK SimpleDlgProc
   (HWND hwnd, UINT message, UINT wParam, LPARAM lParam);

void App_Init()
{
	//Seed number generator
	srand(GetTickCount());

	//Get dimensions
	GetClientRect(g_hMainWnd, &client);
	GetClientRect(g_hMainWnd, &game);

	//Adjust game dimentions
	game.left   += WALL_WIDTH;
	game.top    += WALL_HEIGHT;
	game.right  -= WALL_WIDTH;
	game.bottom -= WALL_HEIGHT + SCOREBOARDH;

	//Set scoreboard dimentions
	score.left   = client.left;
	score.top    = game.bottom + WALL_HEIGHT;
	score.right  = client.right;
	score.bottom = client.bottom;

	// Call graphical initializaiton routines
	Bitmaps_Init();
	Brushes_Init();
	Pens_Init   ();
	Fonts_Init  ();
	DC_Init     ();

	// Initialize variables
	bHighScore = true;
	LoadHighscores();

	// Initialize view
	DrawWall         (memdc);
	DisplayTitle     (topdc);
	DisplayHighscores(topdc);
}

void App_Kill()
{
	Player.Delete();

	//Delete brushes
	DeleteObject(hBrushBackground);
	DeleteObject(hHighBrush      );

	//Delete fonts
	DeleteObject(hScoreFont);
	DeleteObject(hHighFont );

	//Delete bitmaps
	DeleteObject(hWall);
	DeleteObject(hFood);
	DeleteObject(hTail);

	//Delete DCs
	DeleteDC(memdc );
	DeleteDC(topdc );
	DeleteDC(buffer);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int x = 0;
	HDC hdc;
	PAINTSTRUCT paint;

	switch(message)
	{
		case WM_PAINT:
			// Get device contex
			hdc = BeginPaint(hWnd, &paint);

			// Copy memory image onto screen
			BitBlt(hdc, 0, 0, client.right, client.bottom, memdc, 0, 0, SRCCOPY);

			// OR top image
			BitBlt(hdc, 0, 0, game.right,   game.bottom,   topdc, 0, 0, SRCPAINT);

			EndPaint(hWnd, &paint);

			x++;
			break;

		case WM_TIMER:
			if (bEnterName)
			{
				// Invert cursor
				bCursor = !bCursor;
				DisplayCursor(topdc, pCursor, hCursor, bCursor);
			}
			break;

		case WM_CHAR:
			if ((!bPlaying && !bEnterName && !bHighScore) && (wParam == 13 || wParam == 32))
			{
				// Show highscore
				bHighScore = 1;

				ClearRect(memdc, game, hBrushBackground);
				ClearDC(topdc, hBrushBackground);
				DisplayTitle	 (topdc);
				DisplayHighscores(topdc);

				InvalidateRect(hWnd, NULL, 0);
			}
			else if (bEnterName)
			{
				UpdateHighscoreName(wParam);
			}

			break;

		case WM_LBUTTONDOWN:
			if (!bPlaying && !bEnterName && !bHighScore)
			{
				// Show highscore
				bHighScore = 1;

				ClearRect(memdc, game, hBrushBackground);
				ClearDC(topdc, hBrushBackground);
				DisplayTitle	 (topdc);
				DisplayHighscores(topdc);

				InvalidateRect(hWnd, NULL, 0);
			}

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case MNU_FILE_NEWGAME:
					Game_Init();
					break;

				case MNU_FILE_PAUSEGAME:
					Game_Pause();
					break;

				case MNU_FILE_ENDGAME:
					if (bPlaying)
						Game_End();
					break;

				case MNU_FILE_EXIT:
					SendMessage(hWnd, WM_DESTROY, 0, 0);
					break;

				case MNU_HELP_SETTINGS:
					LoadSettingsDialog();
					break;

				case MNU_HELP_SCHMITZTECH:
					ShellExecute(hWnd, "Open", "http://www.schmitztech.com/", "", "", SW_SHOWNORMAL);
					break;

				case MNU_HELP_ABOUT:
					DialogBox(g_hInstance, (char*)IDD_ABOUT, hWnd, SimpleDlgProc);
			}

			break;

		case WM_DESTROY: 
			PostQuitMessage(0);
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND GetRunningWindow (const char* szClass, const char* szTitle)
{
    HWND hwnd = ::FindWindow (szClass, szTitle);
    if (::IsWindow (hwnd))
    {
        HWND hwndPopup = ::GetLastActivePopup (hwnd);
        if (::IsWindow (hwndPopup))
            hwnd = hwndPopup;
    }
    else 
        hwnd = 0;

    return hwnd;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwndOther = GetRunningWindow(sWindowClass, sWindowTitle);
    if (hwndOther != 0)
    {
        ::SetForegroundWindow (hwndOther);
        if (::IsIconic (hwndOther))
            ::ShowWindow (hwndOther, SW_RESTORE);
        exit(1);
    }

	MSG msg;
	DWORD dwStyle;
	DWORD dwStartTime;
	WNDCLASS wcl;
	HACCEL hAccel;

	// Save a global copy of the instance
	g_hInstance = hInstance;

	// Save styles for window
	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// Setup the window class structure
	wcl.hInstance     = hInstance;
	wcl.lpszClassName = sWindowClass;
	wcl.lpszMenuName  = MAKEINTRESOURCE(IDM_MAIN); 
	wcl.lpfnWndProc   = WinProc;

	wcl.hIcon   = LoadIcon  (hInstance, (char*)IDI_SNAKE); 
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW); 

	wcl.style = 0;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

	// Create window client rect
	RECT rect = { 
				  rect.left   = 0, 
		          rect.top    = 0, 
				  rect.right  = rect.left + WALL_WIDTH * 2 + WIN_TILES_X * TILE_WIDTH, 
				  rect.bottom = rect.top  + WALL_WIDTH * 2 + WIN_TILES_Y * TILE_HEIGHT + SCOREBOARDH
				};

	// Adjust rect from client to window
	AdjustWindowRect (&rect, dwStyle, !(wcl.lpszMenuName == NULL));

	// Register class with windows
	if(!RegisterClass (&wcl)) 
		return 0;
	
	// Create window and store a global handle
	if(!(g_hMainWnd = CreateWindow(sWindowClass,			// Class
								   sWindowTitle,			// Caption
								   dwStyle,					// Style
								   25,						// Left
								   25,						// Right
								   rect.right-rect.left,	// Width
								   rect.bottom-rect.top,	// Height
								   NULL,					// Parent
								   NULL,					// Menu
								   hInstance,				// Instance
								   NULL)))					// Extra
		return 0;

	// Load Accelerators (menu shortcuts)
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDA_MAIN));

	// Show Window
	ShowWindow  (g_hMainWnd, nCmdShow);

	// Paint Window
	UpdateWindow(g_hMainWnd);



	////////////////////////////////////////////////////////////////////////////////



	// Initialize variables and buffers
	App_Init();

	// Window message loop
	dwStartTime = GetTickCount();
	while(true)
	{
		// Process next message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(g_hMainWnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// Main game loop
		if (bPlaying && !bPaused)
		{
			if ((GetTickCount() - dwStartTime) > g_iGameDelay || dwStartTime == 0)
			{
				dwStartTime = GetTickCount();
				Game_Main();
			}
			Player.HandleKeys();
		}
	}

	App_Kill();

	return msg.wParam;
}

BOOL CALLBACK SimpleDlgProc
   (HWND hwnd, UINT message, UINT wParam, LPARAM lParam)
{
	RECT rect, rect2;

    switch (message)
    {
    case WM_INITDIALOG:
		// Center dialog
		GetWindowRect(GetParent(hwnd), &rect);
		GetWindowRect(hwnd , &rect2);
		SetWindowPos (hwnd, HWND_TOP, ((rect.left + rect.right) / 2) - ((rect2.right - rect2.left) / 2), ((rect.top + rect.bottom) / 2) - ((rect2.bottom - rect2.top) / 2), 0, 0, SWP_NOSIZE);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
