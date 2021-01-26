/*********************************************************************************
** MIT License
**
** Copyright(c) 2018 duncanmk
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files(the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions :
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*********************************************************************************/

#include "Game.h"

int startGame(Game *game)
{
	if(openWindow(&game->Frame) == 0)
		return 0;

	else if(loadSprite(&game->Tiles[0], &game->Frame, "tile_black.bmp", 64, 64) == 0)
	{
		closeWindow(&game->Frame);
		return 0;
	}

	else if(loadSprite(&game->Tiles[1], &game->Frame, "tile_white.bmp", 64, 64) == 0)
	{
		unloadSprite(&game->Tiles[0]);
		closeWindow(&game->Frame);

		return 0;
	}

	else if(loadSprite(&game->Tiles[2], &game->Frame, "tile_empty.bmp", 64, 64) == 0)
	{
		unloadSprite(&game->Tiles[1]);
		unloadSprite(&game->Tiles[0]);
		closeWindow(&game->Frame);

		return 0;
	}

	else if(loadSprite(&game->Tiles[3], &game->Frame, "tile_valid.bmp", 64, 64) == 0)
	{
		unloadSprite(&game->Tiles[2]);
		unloadSprite(&game->Tiles[1]);
		unloadSprite(&game->Tiles[0]);
		closeWindow(&game->Frame);

		return 0;
	}

	else if(loadMenu(&game->Screens, &game->Frame) == 0)
	{
		unloadSprite(&game->Tiles[3]);
		unloadSprite(&game->Tiles[2]);
		unloadSprite(&game->Tiles[1]);
		unloadSprite(&game->Tiles[0]);
		closeWindow(&game->Frame);

		return 0;
	}

	game->Screens.menuState = 0;
	resetBoard(&game->Grid);
	changeTurnBoard(&game->Grid);
	changeTurnBoard(&game->Grid);

	return 1;
}

int updateGame(Game *game)
{
	int mouseX, mouseY, tileIndex;

	if(game->Screens.menuState == 1 && game->Frame.Event.type == SDL_MOUSEBUTTONDOWN && game->Frame.Event.button.button == SDL_BUTTON_LEFT)
	{
		mouseX = game->Frame.Event.button.x >> 6;
		mouseY = game->Frame.Event.button.y >> 6;

		if(playBoard(&game->Grid, mouseX, mouseY) == 1)
			changeTurnBoard(&game->Grid);

		SDL_Delay(100);
	}

	else if(game->Screens.menuState == 2 || game->Screens.menuState == 3)
	{
		if(game->playerTurn == 1)
		{
			if(game->Frame.Event.type == SDL_MOUSEBUTTONDOWN && game->Frame.Event.button.button == SDL_BUTTON_LEFT)
			{
				mouseX = game->Frame.Event.button.x >> 6;
				mouseY = game->Frame.Event.button.y >> 6;

				if(playBoard(&game->Grid, mouseX, mouseY) == 1)
				{
					changeTurnBoard(&game->Grid);
					game->playerTurn = 0;
				}

				SDL_Delay(100);
			}
		}

		else if(game->playerTurn == 0)
		{
			SDL_Delay(250);

			if(searchBoard(&game->Grid, 3) == 1)
				game->playerTurn = 1;

			changeTurnBoard(&game->Grid);
		}
	}

	else if(game->Screens.menuState == 4)
	{
		searchBoard(&game->Grid, 3);
		changeTurnBoard(&game->Grid);
		SDL_Delay(250);
	}

	else if(game->Screens.menuState == 0 && updateMenu(&game->Screens, &game->Frame, &game->Grid, &game->playerTurn) == 0)
		return 0;

	if(game->Screens.menuState > 0 && game->Screens.menuState < 5)
	{
		if(game->Grid.moveCount == 0)
		{
			changeTurnBoard(&game->Grid);

			if(game->playerTurn == 0)
				game->playerTurn = 1;

			else
				game->playerTurn = 0;

			if(game->Grid.moveCount == 0)
				reportMenu(&game->Screens, &game->Grid);
		}
	}

	for(int indexX = 0; indexX < 8; ++indexX)
		for(int indexY = 0; indexY < 8; ++indexY)
		{
			tileIndex = game->Grid.State[indexX][indexY];

			for(int moveIndex = 0; moveIndex < game->Grid.moveCount; ++moveIndex)
				if(game->Grid.Moves[moveIndex].positionX == indexX && game->Grid.Moves[moveIndex].positionY == indexY)
				{
					tileIndex = 3;
					break;
				}

			drawSprite(&game->Tiles[tileIndex], &game->Frame, indexX << 6, indexY << 6);
		}

	if(updateMenu(&game->Screens, &game->Frame, &game->Grid, &game->playerTurn) == 1 && updateWindow(&game->Frame) == 1)
		return 1;

	return 0;
}

void finishGame(Game *game)
{
	unloadMenu(&game->Screens);
	unloadSprite(&game->Tiles[3]);
	unloadSprite(&game->Tiles[2]);
	unloadSprite(&game->Tiles[1]);
	unloadSprite(&game->Tiles[0]);
	closeWindow(&game->Frame);
}