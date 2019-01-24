#include "global.h"

inline void ReportError(char* sMessage)
{
	MessageBox(g_hMainWnd, sMessage, sWindowTitle, MB_ICONERROR | MB_OK);
	exit(-1);
}