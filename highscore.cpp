#include "global.h"

CHighScore HighScore[HIGHSCORES];

int FindHighscorePos(char* Name, int Score);

char  Name[NAMESIZE];
POINT pCursor;
int   hCursor;
bool  bCursor = 0;

void LoadHighscores()
{
	ifstream fin;
	ofstream fout;

	char buffer[16];
	int i = 0;

	int hFile = open("settings.dat", O_RDONLY);
	if (hFile == -1)
	{
		fout.open("settings.dat");

		fout << VK_NUMPAD4 << endl << VK_NUMPAD6 << endl << VK_NUMPAD8 << endl << VK_NUMPAD5 << endl;
		fout << 50 << endl << 5 << endl << 5 << endl;
		for (i = HIGHSCORES; i > 0; --i)
			fout << "John Doe" << endl << i * 500 << endl;
		fout.close();

		hFile = open("settings.dat", O_RDONLY);
	}

	fin.attach(hFile);

	// Load 
	fin >> keyLeft >> keyRight >> keyUp >> keyDown;
	fin >> g_iGameDelay >> g_iInitLength >> g_iIncLength;
	fin.ignore();

	while(!fin.eof() && i < HIGHSCORES)
	{
		fin.getline(HighScore[i].Name,  NAMESIZE+1);
		fin.getline(buffer,             16      );

		HighScore[i].Score = strtol(buffer, 0, 10);

		++i;
	}

	fin.close();
}

void SaveHighscores()
{
	ofstream fout("settings.dat");

	// Save settings
	fout << keyLeft << endl << keyRight << endl << keyUp << endl << keyDown << endl;
	fout << g_iGameDelay << endl << g_iInitLength << endl << g_iIncLength << endl;

	// Save highscores
	for (int i = 0; i < HIGHSCORES; ++i)
		fout << HighScore[i].Name << endl << HighScore[i].Score << endl;
}

void DisplayHighscores(HDC destDC)
{
	SIZE size;
	SIZE size2;
	char buffer[39] = "Highscore List";
	int y = game.bottom / 2;

	GetTextExtentPoint32(destDC, buffer, strlen(buffer), &size);
	TextOut(destDC, (game.right - size.cx) / 2, y, buffer, strlen(buffer));

	strcpy(buffer, "0123456789012345678901234567890");
	GetTextExtentPoint32(destDC, buffer, strlen(buffer), &size);

	for (int i = 0; i < HIGHSCORES; ++i)
	{
		y += size.cy;

		sprintf(buffer, "%d", HighScore[i].Score);

		GetTextExtentPoint32(destDC, buffer, strlen(buffer), &size2);

		TextOut(destDC, (game.right - size.cx) / 2,            y, HighScore[i].Name, strlen(HighScore[i].Name));
		TextOut(destDC, (game.right + size.cx) / 2 - size2.cx, y, buffer, strlen(buffer));
	}
}

void GetHighscoreName(int firstY)
{
	SIZE size;
	char str[48] = "You have made it to the High Score list!";
	int  y = firstY + 15;

	// Output text
	GetTextExtentPoint32(buffer, str, strlen(str), &size);
	MaskedTextOut(topdc, buffer, (game.right - size.cx) / 2, y, size.cx, size.cy, str);

	strcpy(str, "Please enter your name:");
	y += size.cy;

	GetTextExtentPoint32(buffer, str, strlen(str), &size);
	MaskedTextOut(topdc, buffer, (game.right - size.cx) / 2, y, size.cx, size.cy, str);

	y += size.cy;

	bEnterName = true;

	// Align cursor
	pCursor.x = game.right / 2;
	pCursor.y = y;
	hCursor = size.cy;

	// Show old name
	UpdateHighscoreName(0);

	// Show cursor
	bCursor = 1;
	DisplayCursor(topdc, pCursor, hCursor, bCursor);

	SetTimer(g_hMainWnd, NULL, 500, NULL);

	InvalidateRect(g_hMainWnd, 0, firstY, game.right, game.bottom, 0);
}

int FindHighscorePos(char* Name, int Score)
{
	int Pos = 9;

	while (Pos > -1)
	{
		if (HighScore[Pos].Score < Score)
			--Pos;
		else
			return Pos + 1;
	}

	return 0;
}

void AddHighscore(char* Name, int Score)
{
	int i;
	int Pos = FindHighscorePos(Name, Score);

	if (Pos < HIGHSCORES)
	{
		for (i = 9; i > Pos; --i)
		{
			// Make room for new highscore
			// Copy name
			sprintf(HighScore[i].Name, HighScore[i - 1].Name);

			// Copy score
			HighScore[i].Score = HighScore[i - 1].Score;
		}

		// Add new highscore
		strcpy(HighScore[i].Name, Name);
		HighScore[i].Score = Score;
	}

	SaveHighscores();

	// Clear top DC and background
	ClearRect(memdc, game, hBrushBackground);
	ClearDC(topdc, hBrushBackground);

	KillTimer(g_hMainWnd, NULL);

	bEnterName = 0;
	bHighScore = 1;

	DisplayTitle     (topdc);
	DisplayHighscores(topdc);

	InvalidateRect(g_hMainWnd, NULL, 0);
}

void UpdateHighscoreName(char Letter)
{
	HDC hDC = NULL;

	SIZE size;
	RECT rect;

	GetTextExtentPoint32(topdc, Name, strlen(Name), &size);
	rect.left   = game.left;
	rect.top    = pCursor.y;
	rect.right  = game.right;
	rect.bottom = rect.top + size.cy;

	if      (Letter == 8) //Delete
	{
		Name[strlen(Name)-1] = NULL;
		ClearRect(topdc,  rect, hBrushBackground);
		InvalidateRect(g_hMainWnd, &rect, 0);
	}
	else if (Letter == 13)
	{
		AddHighscore(Name, Player.iScore);
		return;
	}
	else if (strlen(Name) < NAMESIZE - 1)
	{
		Name[strlen(Name)+1] = NULL;
		Name[strlen(Name)  ] = Letter;
	}

	// Hide cursor
	if (bCursor)
		DisplayCursor(topdc, pCursor, hCursor, 0);

	// Display name again
	GetTextExtentPoint32(topdc, Name, strlen(Name), &size);
	rect.left   = game.left;
	rect.top    = pCursor.y;
	rect.right  = (game.right + size.cx) / 2;
	rect.bottom = rect.top + size.cy;

	TextOut(topdc, (game.right - size.cx) / 2, pCursor.y, Name, strlen(Name));
	InvalidateRect (g_hMainWnd, &rect, 0);

	// Realign cursor x
	pCursor.x = rect.right;
	
	// Show cursor
	bCursor = 1;
	DisplayCursor(topdc, pCursor, hCursor, bCursor);
}
