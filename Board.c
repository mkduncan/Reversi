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

#include "Board.h"
#include <SDL.h>
#include <string.h>

static int insideBoard(const char, const char);
static char nextTurn(const char);
static int searchMinMax(const Board*, const char, const char, int, int);

void resetBoard(Board *board)
{
	memset(board->State, 2, sizeof(char) << 6);
	board->State[3][3] = 1;
	board->State[3][4] = 0;
	board->State[4][3] = 0;
	board->State[4][4] = 1;
	board->Turn = 0;
	board->moveCount = 0;
}

void copyBoard(Board *board, const Board *source)
{
	memcpy(board->Moves, source->Moves, sizeof(Move) << 5);
	memcpy(board->State, source->State, sizeof(char) << 6);
	board->Turn = source->Turn;
	board->moveCount = source->moveCount;
}

int playBoard(Board *board, const char positionX, const char positionY)
{
	int result = 0;
	char offsetX, offsetY;

	if(board->moveCount == 0)
		return 1;

	for(int moveIndex = 0; moveIndex < board->moveCount; ++moveIndex)
		if(board->Moves[moveIndex].positionX == positionX && board->Moves[moveIndex].positionY == positionY)
		{
			result = 1;
			offsetX = positionX;
			offsetY = positionY;

			for(int flipIndex = 0; flipIndex < board->Moves[moveIndex].flipCount; ++flipIndex)
			{
				board->State[offsetX][offsetY] = board->Turn;
				offsetX += board->Moves[moveIndex].strideX;
				offsetY += board->Moves[moveIndex].strideY;
			}
		}

	return result;
}

int searchBoard(Board *board, const char depth)
{
	Board child;
	int search, bestSearch = -99999999, bestIndex = -1;

	if(board->moveCount == 0)
		return 1;

	for(int movesIndex = 0; movesIndex < board->moveCount; ++movesIndex)
	{
		copyBoard(&child, board);
		playBoard(&child, board->Moves[movesIndex].positionX, board->Moves[movesIndex].positionY);
		changeTurnBoard(&child);
		search = searchMinMax(&child, board->Turn, depth, -999999, 999999);

		if(bestSearch < search)
		{
			bestSearch = search;
			bestIndex = movesIndex;
		}
	}

	if(bestIndex == -1)
		return 0;

	playBoard(board, board->Moves[bestIndex].positionX, board->Moves[bestIndex].positionY);
	return 1;
}

int getScoreBoard(const Board *board)
{
	int totalScore = 0;
	const char evaluation[8][8] =
	{
		{ 99, -8, 8, 6, 6, 8, -8, 99 },
		{ -8, -24, -4, -3, -3, -4, -24, -8 },
		{ 8, -4, 7, 4, 4, 7, -4, 8 },
		{ 6, -3, 4, 0, 0, 4, -3, 6 },
		{ 6, -3, 4, 0, 0, 4, -3, 6 },
		{ 8, -4, 7, 4, 4, 7, -4, 8 },
		{ -8, -24, -4, -3, -3, -4, -24, -8 },
		{ 99, -8, 8, 6, 6, 8, -8, 99 }
	};

	for(int indexX = 0; indexX < 8; ++indexX)
		for(int indexY = 0; indexY < 8; ++indexY)
		{
			if(board->State[indexX][indexY] == board->Turn)
				totalScore += evaluation[indexX][indexY];

			else if(board->State[indexX][indexY] == nextTurn(board->Turn))
				totalScore -= evaluation[indexX][indexY];
		}

	return totalScore;
}

int getCountBoard(const Board *board, const char turn)
{
	int totalCount = 0;

	for(int indexX = 0; indexX < 8; ++indexX)
		for(int indexY = 0; indexY < 8; ++indexY)
			if(board->State[indexX][indexY] == turn)
				++totalCount;

	return totalCount;
}

void changeTurnBoard(Board *board)
{
	char offsetX, offsetY;

	board->moveCount = 0;
	board->Turn = nextTurn(board->Turn);

	for(int indexX = 0; indexX < 8; ++indexX)
		for(int indexY = 0; indexY < 8; ++indexY)
			if(board->State[indexX][indexY] == 2)
				for(int strideX = -1; strideX <= 1; ++strideX)
					for(int strideY = -1; strideY <= 1; ++strideY)
						if(strideX != 0 || strideY != 0)
						{
							offsetX = indexX + strideX;
							offsetY = indexY + strideY;

							if(insideBoard(offsetX, offsetY) && board->State[offsetX][offsetY] == nextTurn(board->Turn))
								for(int flipCount = 0; flipCount < 6; ++flipCount)
								{
									offsetX += strideX;
									offsetY += strideY;

									if(board->State[offsetX][offsetY] == 2)
										break;

									else if(insideBoard(offsetX, offsetY) == 1 && board->State[offsetX][offsetY] == board->Turn)
									{
										board->Moves[board->moveCount].positionX = indexX;
										board->Moves[board->moveCount].positionY = indexY;
										board->Moves[board->moveCount].strideX = strideX;
										board->Moves[board->moveCount].strideY = strideY;
										board->Moves[board->moveCount].flipCount = flipCount + 2;

										board->moveCount += 1;

										break;
									}
								}
						}
}

static int insideBoard(const char positionX, const char positionY)
{
	if(positionX >= 0 && positionX < 8 && positionY >= 0 && positionY < 8)
		return 1;

	return 0;
}

static char nextTurn(const char currentTurn)
{
	return (currentTurn + 1) & 1;
}

static int searchMinMax(const Board *parent, const char rootTurn, const char depth, int alpha, int beta)
{
	Board child;
	int search, value;

	if(depth == 0)
		return getScoreBoard(parent);

	if(parent->Turn == rootTurn)
	{
		value = -999999;
		
		for(int movesIndex = 0; movesIndex < parent->moveCount; ++movesIndex)
		{
			copyBoard(&child, parent);
			playBoard(&child, parent->Moves[movesIndex].positionX, parent->Moves[movesIndex].positionY);
			changeTurnBoard(&child);
			search = searchMinMax(&child, rootTurn, depth - 1, alpha, beta);
			value = SDL_max(value, search);
			alpha = SDL_max(alpha, value);

			if(beta <= alpha)
				break;
		}

		return value;
	}

	value = 999999;
		
	for(int movesIndex = 0; movesIndex < parent->moveCount; ++movesIndex)
	{
		copyBoard(&child, parent);
		playBoard(&child, parent->Moves[movesIndex].positionX, parent->Moves[movesIndex].positionY);
		changeTurnBoard(&child);
		search = searchMinMax(&child, rootTurn, depth - 1, alpha, beta);
		value = SDL_min(value, search);
		alpha = SDL_min(alpha, value);

		if(beta <= alpha)
			break;
	}

	return value;
}