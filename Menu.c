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
#include "Menu.h"

static int Between(const int, const int, const int, const int);

int loadMenu(Menu *menu, Window *window)
{
	if(loadSprite(&menu->Menus[0], window, "menu_main.bmp", 384, 384) == 0)
		return 0;

	else if(loadSprite(&menu->Menus[1], window, "menu_win_black.bmp", 192, 96) == 0)
	{
		unloadSprite(&menu->Menus[0]);
		return 0;
	}

	else if(loadSprite(&menu->Menus[2], window, "menu_win_white.bmp", 192, 96) == 0)
	{
		unloadSprite(&menu->Menus[1]);
		unloadSprite(&menu->Menus[0]);

		return 0;
	}

	else if(loadSprite(&menu->Menus[3], window, "menu_draw.bmp", 192, 96) == 0)
	{
		unloadSprite(&menu->Menus[2]);
		unloadSprite(&menu->Menus[1]);
		unloadSprite(&menu->Menus[0]);

		return 0;
	}

	menu->menuState = 0;
	return 1;
}

int updateMenu(Menu *menu, Window *window, Board *board, char *playerTurn)
{
	int mouseX, mouseY;

	if(Between(menu->menuState, 1, 4, 0))
		return 1;

	if(menu->menuState == 0)
	{
		drawSprite(&menu->Menus[0], window, 64, 64);

		if(window->Event.type == SDL_MOUSEBUTTONDOWN && window->Event.button.button == SDL_BUTTON_LEFT)
		{
			mouseX = window->Event.button.x;
			mouseY = window->Event.button.y;

			if(Between(mouseX, 241, 333, 64) && Between(mouseY, 247, 289, 64))
				return 0;

			else if(Between(mouseX, 51, 143, 64))
			{
				if(Between(mouseY, 125, 167, 64))
				{
					menu->menuState = 1;
					SDL_Delay(250);

					return 1;
				}

				else if(Between(mouseY, 186, 228, 64))
				{
					menu->menuState = 2;
					(*playerTurn) = 1;
					SDL_Delay(250);

					return 1;
				}

				else if(Between(mouseY, 247, 289, 64))
				{
					menu->menuState = 3;
					SDL_Delay(250);

					return 1;
				}

				else if(Between(mouseY, 308, 350, 64))
				{
					menu->menuState = 4;
					SDL_Delay(250);

					return 1;
				}
			}
		}
	}

	else if(Between(menu->menuState, 5, 7, 0))
	{
		drawSprite(&menu->Menus[menu->menuState - 4], window, 160, 208);

		if(window->Event.type == SDL_MOUSEBUTTONDOWN && window->Event.button.button == SDL_BUTTON_LEFT)
		{
			mouseX = window->Event.button.x;
			mouseY = window->Event.button.y;

			if(Between(mouseX, 14, 178, 160) && Between(mouseY, 57, 85, 208))
			{
				menu->menuState = 0;
				resetBoard(board);
				changeTurnBoard(board);
				changeTurnBoard(board);
			}
		}
	}

	return 1;
}

void reportMenu(Menu *menu, const Board *board)
{
	const int blackCount = getCountBoard(board, 0), whiteCount = getCountBoard(board, 1);

	if(blackCount > whiteCount)
		menu->menuState = 5;

	else if(blackCount < whiteCount)
		menu->menuState = 6;

	else
		menu->menuState = 7;
}

void unloadMenu(Menu *menu)
{
	unloadSprite(&menu->Menus[3]);
	unloadSprite(&menu->Menus[2]);
	unloadSprite(&menu->Menus[1]);
	unloadSprite(&menu->Menus[0]);
}

static int Between(const int position, const int minimum, const int maximum, const int offset)
{
	if(position >= minimum + offset && position <= maximum + offset)
		return 1;

	return 0;
}