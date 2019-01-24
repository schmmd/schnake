#ifndef _H_GRAPHICS
#define _H_GRAPHICS

//Device context variables
extern HDC memdc;
extern HDC topdc;
extern HDC buffer;

//Font variables
extern HFONT hScoreFont;
extern HFONT hHighFont;

#define BACKGROUND_COLOR RGB(0,0,0)

//Brush variables
extern HBRUSH hBrushBackground;
extern HBRUSH hHighBrush;

//Bitmap variables
extern HBITMAP hWall;
extern HBITMAP hTail;
extern HBITMAP hFood;

void Bitmaps_Init();
void Brushes_Init();
void Pens_Init   ();
void Fonts_Init  ();
void DC_Init     ();

void DisplayTitle     (HDC);
void DisplayScoreboard(HDC);
void DisplayHighscores(HDC);
void DisplayPaused    (HDC);
int  DisplayGameover  (HDC);
void DisplayCursor    (HDC destDC, POINT point, int Height, bool Display);

void DrawWall       (HDC);
void DrawWallBlock  (HDC, HDC, int, int);

void ClearDC        (HDC destDC, HBRUSH& hBrush);
void ClearRect      (HDC destDC, RECT& rect, HBRUSH& hBrush);
void MaskedTextOut  (HDC destDC, HDC buffer, int x, int y, int width, int height, char* str);
int  InvalidateRect (HWND hWnd, int X1, int Y1, int X2, int Y2, BOOL bErase);

#endif