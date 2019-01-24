#ifndef _GLOBALS
#define _GLOBALS

#include <windows.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include "resource.h"

#include "main.h"
#include "highscore.h"
#include "graphics.h"
#include "settings.h"
#include "player.h"
#include "game.h"
#include "food.h"
#include "error.h"

#define KEYDOWN(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP  (vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//Dimensions of the window in tiles
#define WIN_TILES_X 50
#define WIN_TILES_Y 30

//Dimensions of a snake tile
#define TILE_WIDTH  12
#define TILE_HEIGHT 12

//Dimensions of a wall tile
#define WALL_WIDTH  12
#define WALL_HEIGHT 12

#endif