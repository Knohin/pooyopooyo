#include "Board.h"
#include <cmath>


const int Board::shape[4][2][2] = {
	{ { 1,2 },{ 0,0 } },
{ { 1,0 },{ 2,0 } },
{ { 2,1 },{ 0,0 } },
{ { 2,0 },{ 1,0 } },
}; // 4가지 방향, 2x2 모양

Board::Board(int _width=0, int _height=0)
	:width(_width), height(_height)
{
	Init();
}
void Board::Init()
{
	ChangeState(Moving);
	isFinished = false;

	grid = new Pooyo**[height];
	for (int i = 0; i<height; ++i)
		grid[i] = new Pooyo*[width]();
	linkedBlock = new bool*[height];
	for (int i = 0; i<height; ++i)
		linkedBlock[i] = new bool[width]();

	isChecked = new bool*[height];
	for (int i = 0; i<height; ++i)
		isChecked[i] = new bool[width]();

	linkedBlockCount = 0;

	moveDownInterval = 1.0f;
	elapsedTime = 0.0f;

	totalErasedBlockCount = 0;
	comboCount = 0;
}

Board::~Board()
{
	Release();
}
void Board::Release()
{
	if (grid)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[i][j] != nullptr)
					delete grid[i][j];
			}
			delete[] grid[i];
		}
		delete[] grid;
	}
	if (linkedBlock)
	{
		for (int i = 0; i < height; i++)
			delete[] linkedBlock[i];
		delete[] linkedBlock;
	}
	if (isChecked)
	{
		for (int i = 0; i < height; i++)
			delete[] isChecked[i];
		delete[] isChecked;
	}

	delete curPooyo[0];
	delete curPooyo[1];
}

void Board::Update(float deltaTime)
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

			if (IsCollideAt(curPooyo[0]->getX(), curPooyo[0]->getY() + 0.5f) ||
				IsCollideAt(curPooyo[1]->getX(), curPooyo[1]->getY() + 0.5f))
			{
				// Set curPooyo on the board
				AddPooyo(curPooyo[0]);
				AddPooyo(curPooyo[1]);
				curPooyo[0] = nullptr; // ??
				curPooyo[1] = nullptr;
				ChangeState(Stacking);
			}
			else
			{
				curPooyo[0]->moveDown();
				curPooyo[1]->moveDown();
			}
		}

		// Check Game Over
		if (grid[0][(width - 1) / 2])
		{
			isFinished = true;
		}
	}
	else if (state == Stacking)
	{
		static const float gravity = 100.0f;
		static float vel = 0.0f;
		static float deltaDistance = 0.0f;

		vel += gravity * deltaTime;
		vel = (vel > 10) ? 10 : vel;
		deltaDistance = vel * deltaTime;


		// Move Down by dist_i
		if (pooyoToStack.size() > 0)
		{
			for (int i = pooyoToStack.size() - 1; 0 <= i; --i)
			{
				float movedY = pooyoToStack[i]->getY() + deltaDistance;
				float x = pooyoToStack[i]->getX();
				if (IsCollideAt(x, movedY))
				{
					float _y = floorf(movedY);
					pooyoToStack[i]->setY(_y);
					grid[(int)_y][(int)x] = pooyoToStack[i];
					pooyoToStack.erase(pooyoToStack.begin() + i);
				}
				else
				{
					pooyoToStack[i]->setY(movedY);
				}
			}
		}
		else
		{
			ChangeState(Popping);
			vel = 0.0f;
		}
	}
	else if (state == Popping)
	{
		/// Check linking and change sprite

		// Pop 4 or above
		if (EraseLinkedBlocks())
		{
			ChangeState(Stacking);
		}
		else
		{
			ChangeState(Moving);
		}
	}

	return;
}

Pooyo*** Board::GetGrid() { return grid; }
int Board::GetHeight() { return height; }
int Board::GetWidth() { return width; }
int Board::GetDirection() { return direction; }
bool Board::IsFinished() { return isFinished; }
void Board::SetElapsedTime(float t) { elapsedTime = t; }
void Board::SetIsFinished(bool b) { isFinished = b; }

bool Board::IsCollideAt(float x, float y)
{
	int gridX = (int)ceilf(x);
	int gridY = (int)ceilf(y);
	if(width <= gridX || height <= gridY
		|| grid[gridY][gridX] != nullptr)
		return true;

	gridX = (int)floorf(x);
	gridY = (int)floorf(y);
	if (gridX < 0 || gridY < 0
		|| grid[gridY][gridX] != nullptr)
		return true;

	return false;
}
bool Board::IsCollide(Pooyo* pooyo)
{
	return IsCollideAt(pooyo->getX(), pooyo->getY());
}
bool Board::IsCollide(Pooyo* pooyo, float x, float y)
{
	return IsCollideAt(pooyo->getX() + x, pooyo->getY() + y);
}

void Board::AddPooyo(Pooyo* newPooyo)
{
	grid[newPooyo->getGridY()][newPooyo->getGridX()] = newPooyo;
}

void Board::RotateCurPooyo()
{
	if (direction == 0) // 세로
	{
		if (!IsCollideAt(curPooyo[0]->getX() + 1, curPooyo[0]->getY()))
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
		if (!IsCollideAt(curPooyo[0]->getX(), curPooyo[0]->getY() + 1))
		{
			curPooyo[1]->moveDown(1.0f);
			curPooyo[1]->moveLeft();

			direction = 0;
		}
	}
}

void Board::ChangeState(eBoardState toState)
{
	switch (toState)
	{
	case Moving:
		state = toState;
		curPooyo[0] = new Pooyo(2, 0, rand() % 4);
		curPooyo[1] = new Pooyo(3, 0, rand() % 4);
		direction = 1;
		elapsedTime = 0.0f;
		break;
	case Stacking:
		state = toState;
		CheckPooyoToStack();
		elapsedTime = 0.0f;
		break;
	case Popping:
		state = toState;
		break;
	}
}


void Board::CheckPooyoToStack()
{
	for (int x = 0; x < width; x++)
	{
		bool isBlank = false;

		for (int y = height- 1; 0 <= y; y--)
		{
			// IF there is no blank, just loop 
			if (isBlank == false)
			{
				if (grid[y][x] == nullptr)
					isBlank = true;
			}
			// ELSE there's a blank so the blocks should be dropped
			else
			{
				if (grid[y][x] != nullptr)
				{
					pooyoToStack.push_back(grid[y][x]);
					grid[y][x] = nullptr;
				}
			}
		}
	}
}

bool Board::StackDownBlocks(float distanceToMove)
{
	bool moved = false;

	for (int x = 0; x < width; x++)
	{
		int bottom = -1;

		for (int y = height - 1; 0 < y; y--)
		{
			// IF there is no blank, just loop 
			if (bottom == -1)
			{
				if (grid[y][x] == nullptr)
					bottom = y;
			}
			// ELSE there's a blank so the blocks should be dropped
			else
			{
				if (grid[y][x] != nullptr)
				{
					float movedY = grid[y][x]->getY() + distanceToMove;
					if (IsCollideAt(x, movedY))
					{
						grid[(int)floorf(movedY)][x] = grid[y][x];
						grid[y][x] = nullptr;
					}
					else
					{
						grid[y][x]->setY(movedY);
						moved = true;
					}
				}
			}
		}
	}

	return moved;
}

bool Board::EraseLinkedBlocks()
{
	bool isErased = false;

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			isChecked[y][x] = false;

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			if (grid[y][x] == nullptr)
				continue;
			linkedBlockCount = 0;
			checkLinked(x, y, grid[y][x]->getColor());
			for (int y = 0; y < height; ++y)
				for (int x = 0; x < width; ++x)
				{
					if (linkedBlock[y][x] == true)
					{
						if (linkedBlockCount > 3)
						{
							// Erase
							delete grid[y][x];
							grid[y][x] = nullptr;
						}
						linkedBlock[y][x] = false;
					}
				}
			if (linkedBlockCount > 3)
				isErased = true;
		}

	return isErased;
}

bool Board::checkLinked(int x, int y, int color)
{
	if (x < 0 || width <= x || y < 0 || height <= y
		|| isChecked[y][x] == true
		|| grid[y][x] == nullptr
		|| grid[y][x]->getColor() != color)
		return false;

	linkedBlockCount++;
	linkedBlock[y][x] = true;
	isChecked[y][x] = true;

	checkLinked(x + 1, y, color);
	checkLinked(x - 1, y, color);
	checkLinked(x, y + 1, color);
	checkLinked(x, y - 1, color);
}