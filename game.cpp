#include "global.h"

CPlayer Player;

// Gamestate variables
bool bEnterName = 0; // Entering name for high scores
bool bHighScore = 0; // High scores are visible
bool bPlaying   = 0; // A game is in progress (may still be paused)
bool bPaused    = 0; // Gameplay is paused

void Game_Main()
{
	if (!Player.Turn(memdc))
	{
		// Ran into wall
		Game_End();
	}
	else
	{
		DisplayScoreboard(memdc);

		InvalidateRect(g_hMainWnd, NULL, 0);
	}
}

void Game_Init()
{
	// Initialize player
	Player.Create();

	Player.SetX(0);
	Player.SetY(0);

	Player.cY = 0;
	Player.cX = 1;
	Player.cYold = 0;
	Player.cXold = 1;

	Player.iLength    = 0;
	Player.iMaxLength = g_iInitLength;
	Player.iScore     = (g_iInitLength-5);

	// Initialize DCs and paint
	ClearDC  (topdc, hBrushBackground);
	ClearRect(memdc, game, hBrushBackground);
	Player.Grow(     );
	Player.Draw(memdc);

	// Initialize gamestate variables
	bHighScore = 0;
	bEnterName = 0;
	bPaused    = 0;
	bPlaying   = 1;
	
	Food_Place();

	DisplayScoreboard(memdc);

	InvalidateRect(g_hMainWnd, NULL, 0);
}

void Game_End()
{
	if (bPaused) //If pause is on, clean up and unpause
		Game_Pause();

	bPlaying = 0;

	//Free list memory
	Player.Delete();

	int y = DisplayGameover(topdc);

	if (Player.iScore > HighScore[9].Score)
		GetHighscoreName(y);
}

void Game_Pause()
{
	if (bPlaying)
	{
		bPaused = !bPaused;

		if (bPaused)
			DisplayPaused(topdc);
		else
			ClearDC(topdc, hBrushBackground);
	}
}