#ifndef _H_PLAYER
#define _H_PLAYER

#include "global.h"
#include "list.h"

#define ADDSCORE    95 + g_iIncLength - int(int((g_iGameDelay - 50)) * 1.5)

class CPlayer : public CList<POINT>
{
	private:
		POINT loc; 

	public:
		CPlayer();

		void HandleKeys();

		bool Turn(HDC hDC);

		void Draw  (HDC hDC);

		void Draw  (HDC hDC, POINT&);
		void Clear (HDC hDC, POINT&);

		bool Grow  (	   );
		void Shrink(HDC hDC);

		int  GetX();
		int  GetY();
		void SetX(int newX);
		void SetY(int newY);

		void SetDir(int newcx, int newcy);

		POINT* GetCurPoint();

		int iLength;
		int iMaxLength;
		int iScore;

		int cX, cY, cXold, cYold;
};

#endif