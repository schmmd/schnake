#include "global.h"

CPlayer::CPlayer()
{
	iLength = 0;
	iMaxLength = 0;
	iScore = 0;
}

int CPlayer::GetX()
{
	return loc.x;
}

int CPlayer::GetY()
{
	return loc.y;
}

void CPlayer::SetX(int newX)
{
	loc.x = newX;
}

void CPlayer::SetY(int newY)
{
	loc.y = newY;
}

POINT* CPlayer::GetCurPoint()
{
	return &loc;
}

bool CPlayer::Grow()
{
	iLength++;

	loc.x += cX;
	loc.y += cY;

	if (!InList(loc))
	{
		Enqueue(loc);

		return true;
	}

	return false;
}

void CPlayer::Shrink(HDC hDC)
{
	iLength--;

	POINT point = Dequeue();
	Clear  (hDC, point);
}

void CPlayer::Draw(HDC hDC)
{
	Draw(hDC, loc);
}

void CPlayer::Draw(HDC hDC, POINT& point)
{
	int X = game.left + (point.x) * TILE_WIDTH;
	int Y = game.top  + (point.y) * TILE_HEIGHT;

	HDC destDC = CreateCompatibleDC(hDC);
	SelectObject(destDC, hTail);

	BitBlt(hDC, X, Y, TILE_WIDTH, TILE_HEIGHT, destDC, 0, 0, SRCCOPY);

	DeleteDC(destDC);
}

void CPlayer::Clear(HDC hDC, POINT& point)
{
	RECT rect = {
					rect.left   = game.left + (point.x) * TILE_WIDTH,
					rect.top    = game.top  + (point.y) * TILE_HEIGHT,
					rect.right  = rect.left + TILE_WIDTH,
					rect.bottom = rect.top  + TILE_HEIGHT
				};

	ClearRect(hDC, rect, hBrushBackground);
}

bool CPlayer::Turn(HDC hDC)
{
	bool Success = Grow();
	if (!Success || GetX() < 0 || GetY() < 0 || GetX() > WIN_TILES_X - 1 || GetY() > WIN_TILES_Y - 1)
		return false;

	Draw(hDC);

	cXold = cX;
	cYold = cY;

	if (GetX() == Food.x && GetY() == Food.y)
	{
		iMaxLength += g_iIncLength;
		iScore	   += ADDSCORE;
		Food_Place();
	}

	while (iLength > iMaxLength)
		Shrink (hDC);

	return true;
}

void CPlayer::HandleKeys()
{
	// SHORT sKey1		= KEYDOWN(VK_NUMPAD1);
	SHORT sKeyDown	= KEYDOWN(keyDown);
	// SHORT sKey3		= KEYDOWN(VK_NUMPAD3);
	SHORT sKeyLeft	= KEYDOWN(keyLeft);
	SHORT sKeyRight = KEYDOWN(keyRight);
	// SHORT sKey7		= KEYDOWN(VK_NUMPAD7);
	SHORT sKeyUp	= KEYDOWN(keyUp);
	// SHORT sKey9		= KEYDOWN(VK_NUMPAD9);

	if (sKeyUp && Player.cYold != 1)
		SetDir(0, -1);
	else if (sKeyRight && Player.cXold != -1)
		SetDir(1, 0);

	else if (sKeyDown && Player.cYold != -1)
		SetDir(0, 1);

	else if (sKeyLeft && Player.cXold != 1)
		SetDir(-1, 0);

	/*
	else if ((sKey1 || sKey3) && Player.cYold == 0)
		SetDir(0, 1);

	else if ((sKey1 || sKey7) && Player.cXold == 0)
		SetDir(-1, 0);

	else if ((sKey3 || sKey9) && Player.cXold == 0)
		SetDir(1, 0);

	else if ((sKey7 || sKey9) && Player.cYold == 0)
		SetDir(0, -1);
	*/
}

void CPlayer::SetDir(int newcx, int newcy)
{
	cX = newcx;
	cY = newcy;
}