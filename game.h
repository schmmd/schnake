#ifndef _H_GAME
#define _H_GAME

extern CPlayer Player;

//Gamestate variables
extern bool bEnterName;
extern bool bHighScore;
extern bool bPlaying;
extern bool bPaused;

void Game_Init ();
void Game_Main ();
void Game_Pause();
void Game_End  ();

#endif