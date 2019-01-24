#include "global.h"

POINT Food;

void Food_Draw()
{
	int X = game.left + (Food.x) * TILE_WIDTH;
	int Y = game.top  + (Food.y) * TILE_HEIGHT;

	HDC hDC = CreateCompatibleDC(memdc);
	SelectObject(hDC, hFood);

	BitBlt(memdc, X, Y, TILE_WIDTH, TILE_HEIGHT, hDC, 0, 0, SRCCOPY);

	DeleteDC(hDC);
}

void Food_Place()
{
	do //INEFFICIENT (infinite loop)
	{
		Food.x = rand() % (WIN_TILES_X-2)+1;
		Food.y = rand() % (WIN_TILES_Y-2)+1;
	} while (Player.InList(Food));

	Food_Draw();
}