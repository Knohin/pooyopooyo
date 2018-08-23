#include "GameManager.h"

#include <cstdlib>
#include <SDL.h>
#include "sdl_renderer.h"


const int GameManager::shape[4][2][2] = {
	{ { 1,2 },{ 0,0 } },
{ { 1,0 },{ 2,0 } },
{ { 2,1 },{ 0,0 } },
{ { 2,0 },{ 1,0 } },
}; // 4가지 방향, 2x2 모양

GameManager::GameManager(int width, int height)
{
	isRunning = true;
	isBlockHit = false;

	level = 0;
	moveDownInterval = 1000;
	elapsedTime = 0;

	totalErasedBlockCount = 0;
	comboCount = 1;
	score = 0;

	state = Moving;

	board = Board(width, height);

	curPooyo[0] = new Pooyo(2, 0, rand() % 4);
	curPooyo[1] = new Pooyo(3, 0, rand() % 4);
	direction = 1;
}

GameManager::~GameManager()
{
	if (curPooyo[0])
	{
		delete curPooyo[0];
		delete curPooyo[1];
	}
}


void GameManager::update(int deltaTime)
{
	elapsedTime += deltaTime;

	if (state == Moving)
	{
		curPooyo[0]->updateMoving(deltaTime);
		curPooyo[1]->updateMoving(deltaTime);

		// clock tick
		if (moveDownInterval < elapsedTime)
		{
			elapsedTime -= moveDownInterval;

			if (board.IsCollideAt(curPooyo[0]->getX(), curPooyo[0]->getY() + 1.1f) ||
				board.IsCollideAt(curPooyo[1]->getX(), curPooyo[1]->getY() + 1.1f))
			{
				setCurPooyoOntoBoard();
				state = Stacking;
			}
			else
			{
				curPooyo[0]->moveDown();
				curPooyo[1]->moveDown();
			}
		}
	}
	else if (state == Stacking)
	{
		static const float gravity = 20.0f;
		static float vel = 0.7f;
		static float distance = 0.0f;

		vel += gravity * deltaTime / 1000.0f;
		distance += vel * deltaTime / 1000.0f;

		// Draging
		if (1.0f <= distance)
		{
			int dist_i = (int)distance;

			// Move Down by dist_i
			if (stackDownBlocksOnBoard(dist_i))
				distance -= dist_i;
			else
			{
				if (!board.EraseLinkedBlocks())
				{
					state = Moving;
					curPooyo[0] = new Pooyo(2, 0, rand() % 4);
					curPooyo[1] = new Pooyo(3, 0, rand() % 4);
					direction = 1;
					vel = 0.0f;
					distance = 0.0f;
				}
			}
		}

		/// Check linking
	}

	// Level
	if (level != score / 10000)
	{
		level = score / 10000;
		moveDownInterval = 1000 - 70 * level;
	}

	// Game Over
	if (board.GetGrid()[0][(board.GetWidth() - 1) / 2] || level > 9)
	{
		isRunning = false;
	}

	return;
}

void GameManager::rotateCurPooyo()
{
	if (direction == 0) // 세로
	{
		if (!board.IsCollideAt(curPooyo[0]->getX() + 1, curPooyo[0]->getY()))
		{
			curPooyo[0]->moveRight();
			curPooyo[1]->moveUp();

			Pooyo* temp = curPooyo[0];
			curPooyo[0] = curPooyo[1];
			curPooyo[1] = temp;

			direction = 1;
		}
	}
	else // dirention == 1 // 가로
	{
		if (!board.IsCollideAt(curPooyo[0]->getX(), curPooyo[0]->getY()+1))
		{
			curPooyo[1]->moveDown(1.0f);
			curPooyo[1]->moveLeft();

			direction = 0;
		}
	}
}

void GameManager::setCurPooyoOntoBoard()
{
	board.AddPooyo(curPooyo[0]);
	board.AddPooyo(curPooyo[1]);
	delete curPooyo[0];
	delete curPooyo[1];
	curPooyo[0] = nullptr;
	curPooyo[1] = nullptr;
}

bool GameManager::stackDownBlocksOnBoard(int distanceToMove)
{
	bool stacked = false;


	for (int x = 0; x < board.GetWidth(); x++)
	{
		int bottom = -1;

		for (int y = board.GetHeight() - 1; 0 < y; y--)
		{
			if (bottom == -1)
			{
				if (board[y][x] == 0)
					bottom = y;
			}
			else
			{
				if (board[y][x] != 0)
				{
					board[bottom--][x] = board[y][x];
					board[y][x] = 0;
					stacked = true;
				}
			}
		}
	}

	return stacked;
}