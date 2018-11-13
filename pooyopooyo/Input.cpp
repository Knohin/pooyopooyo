#include "Input.h"
#include <SDL_events.h> 
#include "GameManager.h"

SDL_Event e;

InputHandler::~InputHandler()
{
	delete buttonW;
	delete buttonA;
	delete buttonS;
	delete buttonD;
	delete buttonQ;
	delete buttonSpace;
	delete buttonEnter;
}

void InputHandler::SetButtonW(Command* command) { delete buttonW; buttonW = command; }
void InputHandler::SetButtonA(Command* command) { delete buttonA; buttonA = command; }
void InputHandler::SetButtonS(Command* command) { delete buttonS; buttonS = command; }
void InputHandler::SetButtonD(Command* command) { delete buttonD; buttonD = command; }
void InputHandler::SetButtonQ(Command* command) { delete buttonQ; buttonQ = command; }
void InputHandler::SetButtonSpace(Command* command) { delete buttonSpace; buttonSpace = command; }
void InputHandler::SetButtonEnter(Command* command) { delete buttonEnter; buttonEnter = command; }

Command* InputHandler::handleInput()
{
	SDL_FlushEvent(SDL_KEYDOWN);
	if (SDL_PollEvent(&e))
	{
		if (e.key.type == SDL_KEYDOWN)
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
		else if (e.type == SDL_QUIT)
			GameManager::GetInstance().StopRunning();
	}
	return nullptr;
}

void MoveLeftCommand::execute(Scene* scene)
{ 
	Board* board = dynamic_cast<SoloGameScene*>(scene)->board;
	if (!board->IsCollideAt(board->curPooyo[0]->getX() - 1, board->curPooyo[0]->getY()) &&
		!board->IsCollideAt(board->curPooyo[1]->getX() - 1, board->curPooyo[1]->getY()))
	{
		board->curPooyo[0]->moveLeft();
		board->curPooyo[1]->moveLeft();
	}
}
void MoveRightCommand::execute(Scene* scene)
{
	Board* board = dynamic_cast<SoloGameScene*>(scene)->board;
	if (!board->IsCollideAt(board->curPooyo[0]->getX() + 1, board->curPooyo[0]->getY()) &&
		!board->IsCollideAt(board->curPooyo[1]->getX() + 1, board->curPooyo[1]->getY()))
	{
		board->curPooyo[0]->moveRight();
		board->curPooyo[1]->moveRight();
	}
}
void MoveDownCommand::execute(Scene* scene)
{
	Board* board = dynamic_cast<SoloGameScene*>(scene)->board;
	if (board->IsCollideAt(board->curPooyo[0]->getX(), board->curPooyo[0]->getY()+1) ||
		board->IsCollideAt(board->curPooyo[1]->getX(), board->curPooyo[1]->getY()+1) )
	{
		// Set curPooyo on the board
		board->AddPooyo(board->curPooyo[0]);
		board->AddPooyo(board->curPooyo[1]);
		board->curPooyo[0] = nullptr;
		board->curPooyo[1] = nullptr;

		board->ChangeState(Stacking);
	}
	else
	{
		board->curPooyo[0]->moveDown();
		board->curPooyo[1]->moveDown();
	}
	board->SetElapsedTime(0.0f);
}
void DropDownCommand::execute(Scene* scene)
{
	Board* board = dynamic_cast<SoloGameScene*>(scene)->board;
	if (board->GetDirection() == 0) // vertical
	{
		int x = board->curPooyo[1]->getX();
		int bottom = -1;
		for (int y = board->GetHeight()-1; 0 < y; y--)
		{
			if ((*board)[y][x] == nullptr)
			{
				bottom = y;
				break;
			}
		}

		board->curPooyo[0]->setY(bottom - 1);
		board->curPooyo[1]->setY(bottom);

	}
	else // horizontal
	{
		int x0 = board->curPooyo[0]->getX();
		int y0 = -1;
		for (int y = board->GetHeight() - 1; 0 < y; y--)
		{
			if ((*board)[y][x0] == 0)
			{
				y0 = y;
				break;
			}
		}
		int x1 = board->curPooyo[1]->getX();
		int y1 = -1;
		for (int y = board->GetHeight() - 1; 0 < y; y--)
		{
			if ((*board)[y][x1] == 0)
			{
				y1 = y;
				break;
			}
		}

		if (y0 < y1)
		{
			board->curPooyo[0]->setY(y0);
			board->curPooyo[1]->setY(y0);
		}
		else
		{
			board->curPooyo[0]->setY(y1);
			board->curPooyo[1]->setY(y1);
		}
	}

	// Set curPooyo on the board
	board->AddPooyo(board->curPooyo[0]);
	board->AddPooyo(board->curPooyo[1]);
	board->curPooyo[0] = nullptr;
	board->curPooyo[1] = nullptr;

	board->ChangeState(Stacking);
}
void RotateCommand::execute(Scene* scene)
{
	dynamic_cast<SoloGameScene*>(scene)->board->RotateCurPooyo();
}
void QuitCommand::execute(Scene* scene)
{
	dynamic_cast<SoloGameScene*>(scene)->board->SetIsFinished(true);
}