#include "global.h"

//Device Context variables
HDC memdc;
HDC topdc;
HDC buffer;

// Font variables
HFONT hScoreFont;
HFONT hHighFont;

// Brush variables
HBRUSH hBrushBackground;
HBRUSH hHighBrush;
HPEN   hPenBackground;

// Bitmap variables
HBITMAP hWall;
HBITMAP hTail;
HBITMAP hFood;

void Bitmaps_Init()
{
	// Setup bitmaps
	hWall  = (HBITMAP)LoadImage(g_hInstance, MAKEINTRESOURCE(IDB_WALL), IMAGE_BITMAP, 0, 0, 0);
	hTail  = (HBITMAP)LoadImage(g_hInstance, MAKEINTRESOURCE(IDB_TAIL), IMAGE_BITMAP, 0, 0, 0);
	hFood  = (HBITMAP)LoadImage(g_hInstance, MAKEINTRESOURCE(IDB_FOOD), IMAGE_BITMAP, 0, 0, 0);
}

void Brushes_Init()
{
	// Setup brushes
	hBrushBackground = (HBRUSH) CreateSolidBrush(BACKGROUND_COLOR);
	hHighBrush       = (HBRUSH) CreateSolidBrush(SCOREBOARD_COLOR);
}

void Pens_Init()
{
	// Setup fonts
}

void Fonts_Init()
{
	// Setup fonts
	hScoreFont = CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, 0);
	hHighFont  = CreateFont(14, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, 0);
}

void DC_Init()
{
	// Setup device contexts
	HDC hdc;
	SIZE size;
	HBITMAP hbit;

	// Get device context
	hdc = GetDC(g_hMainWnd);

	// Allocate memory
	memdc = CreateCompatibleDC(hdc);
	hbit  = CreateCompatibleBitmap(hdc, client.right, client.bottom);
	SelectObject(memdc, hbit);

	topdc = CreateCompatibleDC(hdc);
	hbit  = CreateCompatibleBitmap(hdc, game.right, game.bottom);
	SelectObject(topdc, hbit);

	buffer = CreateCompatibleDC(hdc);
	GetTextExtentPoint32(buffer, "A", 1, &size);
	hbit  = CreateCompatibleBitmap(hdc, game.right - game.left, size.cy);
	SelectObject(buffer, hbit);

	// Release device context
	ReleaseDC(g_hMainWnd, hdc);

	// Clear new DCs
	ClearDC(memdc,  hBrushBackground);
	ClearDC(topdc,  hBrushBackground);
	ClearDC(buffer, hBrushBackground);

	// Select fonts
	SelectObject(memdc,  hScoreFont);
	SelectObject(topdc,  hHighFont );
	SelectObject(buffer, hHighFont );

	// Set text color
	SetTextColor(memdc,  SCOREBOARD_COLOR);
	SetBkColor  (memdc,  BACKGROUND_COLOR);
	SetTextColor(topdc,  SCOREBOARD_COLOR);
	SetBkColor  (topdc,  BACKGROUND_COLOR);
	SetTextColor(buffer, SCOREBOARD_COLOR);
	SetBkColor  (buffer, BACKGROUND_COLOR);
}

void DisplayTitle(HDC destDC)
{
	HBITMAP hbmp1;
	BITMAP  bmp1;

	int y = game.bottom / 2;

	HDC sourceDC = CreateCompatibleDC(destDC);

	// Get title pictures dimensions
	hbmp1 = (HBITMAP)LoadImage(g_hInstance, MAKEINTRESOURCE(IDB_TITLE), IMAGE_BITMAP, 0, 0, 0);
	GetObject(hbmp1, sizeof(BITMAP), &bmp1);

	// Display title
	SelectObject(sourceDC, hbmp1);
	BitBlt(destDC, (game.right - bmp1.bmWidth) / 2, y - bmp1.bmHeight - 15, bmp1.bmWidth, bmp1.bmHeight, sourceDC, 0, 0, SRCCOPY);

	DeleteDC(sourceDC);
}

void DisplayScoreboard(HDC hDestDC)
{
	SIZE size;
	char buffer[48];

	ClearRect(hDestDC, score, hBrushBackground);

	sprintf(buffer, "(%.2d,%.2d)", Player.GetX(), Player.GetY());
	TextOut(hDestDC, 20, score.top, buffer, strlen(buffer));

	sprintf(buffer, "Score: %d", Player.iScore);
	GetTextExtentPoint32(memdc, buffer, strlen(buffer), &size);
	TextOut(hDestDC, (score.right-size.cx)/2, score.top, buffer, strlen(buffer));

	sprintf(buffer, "Length: %d", Player.iLength);
	GetTextExtentPoint32(memdc, buffer, strlen(buffer), &size);
	TextOut(hDestDC, score.right - size.cx - 20, score.top, buffer, strlen(buffer));
}

int DisplayGameover(HDC destDC)
{
	//Get game over pictures dimensions
	HBITMAP hGameOver = (HBITMAP)LoadImage(g_hInstance, MAKEINTRESOURCE(IDB_GAMEOVER), IMAGE_BITMAP, 0, 0, 0);
	BITMAP  bmp;
	GetObject(hGameOver, sizeof(BITMAP), &bmp);

	//Display game over
	HDC sourceDC = CreateCompatibleDC(destDC);
	SelectObject(sourceDC, hGameOver);

	BitBlt(destDC, game.right/2 - bmp.bmWidth/2, game.bottom/2 - bmp.bmHeight/2, bmp.bmWidth, bmp.bmHeight, sourceDC, 0, 0, SRCCOPY);

	//Free objects
	DeleteDC    (sourceDC);
	DeleteObject(hGameOver);

	//Force redraw
	InvalidateRect(g_hMainWnd, game.right/2 - bmp.bmWidth/2, game.bottom/2 - bmp.bmHeight/2, bmp.bmWidth + game.right/2 - bmp.bmWidth/2, bmp.bmHeight + game.bottom/2 - bmp.bmHeight/2, 0);

	return (game.bottom + bmp.bmHeight) / 2;
}

void DisplayPaused(HDC destDC)
{
	//Get game over pictures dimensions
	HBITMAP hPaused = (HBITMAP)LoadImage(g_hInstance, MAKEINTRESOURCE(IDB_PAUSED), IMAGE_BITMAP, 0, 0, 0);
	BITMAP  bmp;
	GetObject(hPaused, sizeof(BITMAP), &bmp);

	//Display pause
	HDC sourceDC = CreateCompatibleDC(destDC);
	SelectObject(sourceDC, hPaused);

	BitBlt(destDC, game.right/2 - bmp.bmWidth/2, game.bottom/2 - bmp.bmHeight/2, bmp.bmWidth, bmp.bmHeight, sourceDC, 0, 0, SRCCOPY);

	//Free objects
	DeleteDC(sourceDC);
	DeleteObject(hPaused);

	//Force redraw
	InvalidateRect(g_hMainWnd, game.right/2 - bmp.bmWidth/2, game.bottom/2 - bmp.bmHeight/2, bmp.bmWidth + game.right/2 - bmp.bmWidth/2, bmp.bmHeight + game.bottom/2 - bmp.bmHeight/2, 0);
}

void DrawWall(HDC hDestDC)
{
	int X, Y;

	HDC hDC = CreateCompatibleDC(hDestDC);
	SelectObject(hDC, hWall);

	for (X = 0; X < game.right / WALL_WIDTH + 1; ++X)
	{
		DrawWallBlock(hDestDC, hDC, X * WALL_WIDTH, 0);
	} 

	for (Y = 1; Y < game.bottom / WALL_HEIGHT + 0; ++Y)
	{
		DrawWallBlock(hDestDC, hDC, 0, Y * WALL_HEIGHT);
	}

	for (Y = 1, --X; Y <= game.bottom / WALL_HEIGHT + 0; ++Y)
	{
		DrawWallBlock(hDestDC, hDC, X * WALL_WIDTH, Y * WALL_HEIGHT);
	}

	for (X = 0, --Y; X < game.right  / WALL_WIDTH + 1; ++X)
	{
		DrawWallBlock(hDestDC, hDC, X * WALL_WIDTH, Y * WALL_HEIGHT);
	}

	DeleteDC(hDC);
	InvalidateRect(g_hMainWnd, NULL, 0);
}

void DrawWallBlock(HDC hDestDC, HDC hSourceDC, int X, int Y)
{
	BitBlt(hDestDC, X, Y, WALL_WIDTH, WALL_HEIGHT, hSourceDC, 0, 0, SRCCOPY);
}

/*
void RedrawAll()
{
	ClearDC(memdc, hBrushBackground);
	DrawWall();
	DrawFood();
	for (int i = 1; i < Tail.GetLength(); ++i)
	{
		Tail.Draw(memdc, *Tail.GetTail(i));
	}
	UpdateScoreboard();
}
*/

void DisplayCursor(HDC destDC, POINT point, int Height, bool Display)
{
	RECT rect;
	rect.left   = point.x;
	rect.top    = point.y;
	rect.right  = rect.left + 2;
	rect.bottom = rect.top  + Height;

	if (Display)
		ClearRect(destDC, rect, hHighBrush);
	else
		ClearRect(destDC, rect, hBrushBackground);

	InvalidateRect(g_hMainWnd, &rect, 0);
}

void MaskedTextOut(HDC destDC, HDC buffer, int x, int y, int width, int height, char* str)
{
	COLORREF old = SetTextColor(buffer, RGB(255, 255, 255));
	TextOut(buffer, 0, 0, str, strlen(str));
	BitBlt (destDC, x, y, width, height, buffer, 0, 0, SRCPAINT);

	ClearDC(buffer, hBrushBackground);

	SetTextColor(buffer, SCOREBOARD_COLOR);
	TextOut(buffer, 0, 0, str, strlen(str));
	BitBlt (destDC, x, y, width, height, buffer, 0, 0, SRCAND);

	SetTextColor(buffer, old);

	InvalidateRect(g_hMainWnd, x, y, width, height, 0);
}

void ClearDC(HDC destDC, HBRUSH& hBrush)
{
	//Select white brush
	HBRUSH oldbrush = (HBRUSH)SelectObject(destDC, hBrush);

	//Patch over screen
	PatBlt(destDC, 0, 0, client.right, client.bottom, PATCOPY);

	//Restore old brush
	SelectObject(destDC, oldbrush);
}

void ClearRect(HDC destDC, RECT& rect, HBRUSH& hBrush)
{
	//Select white brush
	HBRUSH oldbrush = (HBRUSH)SelectObject(destDC, hBrush);

	//Patch over screen
	PatBlt(destDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY);

	//Restore old brush
	SelectObject(destDC, oldbrush);
}

int InvalidateRect(HWND hWnd, int X1, int Y1, int X2, int Y2, BOOL bErase)
{
	RECT rect;
	rect.left = X1;
	rect.right = X2;
	rect.top = Y1;
	rect.bottom = Y2;
	return InvalidateRect(hWnd, &rect, bErase);
}
