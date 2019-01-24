#ifndef _H_HIGHSCORE
#define _H_HIGHSCORE

#define HIGHSCORES 10
#define NAMESIZE   24

void GetHighscoreName   (int y		);
void UpdateHighscoreName(char Letter);

void LoadHighscores();
void SaveHighscores();

extern char  Name[NAMESIZE];

extern POINT pCursor;
extern int   hCursor;
extern bool  bCursor;

//Highscore variables
extern struct CHighScore
{
	char Name[NAMESIZE];
	long Score;
} HighScore[HIGHSCORES];

#endif