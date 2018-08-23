#include "Input.h"
#include <SDL_events.h> 

SDL_Event e;

Command* InputHandler::handleInput()
{
	//SDL_FlushEvents(NULL, NULL);
	if (SDL_PollEvent(&e))
	{
		//if (e.key.type == SDL_KEYDOWN)
		if (e.key.state == SDL_PRESSED)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				return buttonW;
			case SDLK_a:
				return buttonA;
			case SDLK_s:
				return buttonS;
			case SDLK_d:
				return buttonD;
			case SDLK_q:
				return buttonQ;
			case SDLK_SPACE:
				return buttonSpace;

			default:
				return nullptr;
			}
		}
	}
	return nullptr;
}

void MoveLeftCommand::execute(GameManager& gameManager)
{ 
	//if ( ! gameManager.isBlockCollided(gameManager.curBlock->getX()-1, gameManager.curBlock->getY()) )
	//	gameManager.curBlock->MoveLeft();

	if (!gameManager.board.IsCollideAt(gameManager.curPooyo[0]->getGridX() - 1, gameManager.curPooyo[0]->getGridY()) &&
		!gameManager.board.IsCollideAt(gameManager.curPooyo[1]->getGridX() - 1, gameManager.curPooyo[1]->getGridY()))
	{
		gameManager.curPooyo[0]->moveLeft();
		gameManager.curPooyo[1]->moveLeft();
	}
}
void MoveRightCommand::execute(GameManager& gameManager)
{
	//if (!gameManager.isBlockCollided(gameManager.curBlock->getX() + 1, gameManager.curBlock->getY()))
	//	gameManager.curBlock->MoveRight();
	if (!gameManager.board.IsCollideAt(gameManager.curPooyo[0]->getGridX() + 1, gameManager.curPooyo[0]->getGridY()) &&
		!gameManager.board.IsCollideAt(gameManager.curPooyo[1]->getGridX() + 1, gameManager.curPooyo[1]->getGridY()))
	{
		gameManager.curPooyo[0]->moveRight();
		gameManager.curPooyo[1]->moveRight();
	}
}
void MoveDownCommand::execute(GameManager& gameManager)
{
	if (gameManager.board.IsCollideAt(gameManager.curPooyo[0]->getX(), gameManager.curPooyo[0]->getY() + 1.1f) ||
		gameManager.board.IsCollideAt(gameManager.curPooyo[1]->getX(), gameManager.curPooyo[1]->getY() + 1.1f))
	{
		gameManager.setCurPooyoOntoBoard();
		gameManager.state = Stacking;
	}
	else
	{
		gameManager.curPooyo[0]->moveDown();
		gameManager.curPooyo[1]->moveDown();
	}
	gameManager.elapsedTime = 0;
}
void DropDownCommand::execute(GameManager& gameManager)
{
	//while (!gameManager.isBlockCollided(gameManager.curBlock->getX(), gameManager.curBlock->getY() + 1))
	//{
	//	gameManager.curBlock->MoveDown();
	//}
	//gameManager.setCurBlockOntoBoard();
	//gameManager.state = Stacking;
	//gameManager.elapsedTime = 0;
	if (gameManager.direction == 0) // vertical
	{
		int x = gameManager.curPooyo[1]->getGridX();
		int bottom = -1;
		for (int y = gameManager.board.GetHeight()-1; 0 < y; y--)
		{
			if (gameManager.board[y][x] == 0)
			{
				bottom = y;
				break;
			}
		}

		gameManager.curPooyo[0]->setY(bottom - 1);
		gameManager.curPooyo[1]->setY(bottom);

	}
	else // horizontal
	{
		int x0 = gameManager.curPooyo[0]->getGridX();
		int y0 = -1;
		for (int y = gameManager.board.GetHeight() - 1; 0 < y; y--)
		{
			if (gameManager.board[y][x0] == 0)
			{
				y0 = y;
				break;
			}
		}
		int x1 = gameManager.curPooyo[1]->getGridX();
		int y1 = -1;
		for (int y = gameManager.board.GetHeight() - 1; 0 < y; y--)
		{
			if (gameManager.board[y][x1] == 0)
			{
				y1 = y;
				break;
			}
		}

		if (y0 < y1)
		{
			gameManager.curPooyo[0]->setY(y0);
			gameManager.curPooyo[1]->setY(y0);
		}
		else
		{
			gameManager.curPooyo[0]->setY(y1);
			gameManager.curPooyo[1]->setY(y1);
		}
	}

	gameManager.setCurPooyoOntoBoard();
	gameManager.state = Stacking;
}
void RotateCommand::execute(GameManager& gameManager)
{
	gameManager.rotateCurPooyo();
}
void QuitCommand::execute(GameManager& gameManager)
{
	gameManager.isRunning = false;
}