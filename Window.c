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

#include "Window.h"

int openWindow(Window *window)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return 0;

	window->Frame = SDL_CreateWindow("Reversi", SDL_WINDOWPOS_CENTERED_MASK, SDL_WINDOWPOS_CENTERED_MASK, 512, 512, SDL_WINDOW_SHOWN);

	if(window->Frame == NULL)
	{
		SDL_Quit();
		return 0;
	}

	window->Renderer = SDL_CreateRenderer(window->Frame, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(window->Renderer == NULL)
	{
		SDL_DestroyWindow(window->Frame);
		SDL_Quit();

		return 0;
	}

	SDL_RenderClear(window->Renderer);
	return 1;
}

int updateWindow(Window *window)
{
	SDL_RenderPresent(window->Renderer);
	SDL_RenderClear(window->Renderer);

	if(SDL_PollEvent(&window->Event) == 1 && window->Event.type == SDL_QUIT)
		return 0;

	return 1;
}

void closeWindow(Window *window)
{
	SDL_DestroyRenderer(window->Renderer);
	SDL_DestroyWindow(window->Frame);
	SDL_Quit();
}