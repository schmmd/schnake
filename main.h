#ifndef _H_MAIN
#define _H_MAIN

#define SCOREBOARDH      20
#define SCOREBOARD_COLOR RGB(18, 176, 65)

extern HINSTANCE g_hInstance;
extern HWND      g_hMainWnd;

extern RECT client;
extern RECT game;
extern RECT score;

void App_Init();
void App_Kill();

extern const char* sWindowTitle;
extern const char* sWindowClass;

#endif