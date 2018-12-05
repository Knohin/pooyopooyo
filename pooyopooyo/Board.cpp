#include "Board.h"
#include <cmath>
#include <queue>


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
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			grid[y][x] = nullptr;
	linkedBlock = new bool*[height];
	for (int i = 0; i<height; ++i)
		linkedBlock[i] = new bool[width]();

	linkedBlockCount = 0;

	moveDownInterval = 0.5f;
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

	delete curPooyo[0];
	delete curPooyo[1];
}

void Board::Update(float deltaTime)
{
	elapsedTime += deltaTime;

	if (state == Moving)
	{
		//curPooyo[0]->updateMoving(deltaTime);
		//curPooyo[1]->updateMoving(deltaTime);

		// clock tick
		if (moveDownInterval < elapsedTime)
		{
			elapsedTime -= moveDownInterval;

			MoveDownCurPooyo();
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
				float x = (float)pooyoToStack[i]->getX();
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
		CheckConnection();

		// Pop 4 or above
		if (pooyoToDelete.empty())
		{
			ChangeState(Moving);
		}
		else
		{
			EraseOver4();
			ChangeState(Stacking);
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

bool Board::IsCollideAt(int x, int y)
{
	if (x < 0 || y < 0
		|| width <= x || height <= y 
		|| grid[y][x] != nullptr)
		return true;
	else
		return false;
}
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


void Board::AddPooyo(Pooyo* newPooyo)
{
	grid[newPooyo->getY()][newPooyo->getX()] = newPooyo;
}

void Board::MoveDownCurPooyo()
{
	// 이동 후 충돌처리
	curPooyo[0]->moveDown();
	curPooyo[1]->moveDown();

	if (IsCollideAt(curPooyo[0]->getX(), curPooyo[0]->getY()) ||
		IsCollideAt(curPooyo[1]->getX(), curPooyo[1]->getY()))
	{
		curPooyo[0]->moveUp();
		curPooyo[1]->moveUp();
		AddPooyo(curPooyo[0]);
		AddPooyo(curPooyo[1]);
		curPooyo[0]->isHalfwayUp = false;
		curPooyo[1]->isHalfwayUp = false;
		curPooyo[0] = nullptr;
		curPooyo[1] = nullptr;

		ChangeState(Stacking);
	}

	SetElapsedTime(0.0f);
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
			curPooyo[1]->moveDown();
			curPooyo[1]->moveDown();
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
	pooyoToStack.clear();
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

	// Set Pooyo connection
	for (auto pooyo : pooyoToStack)
	{
		int x = pooyo->getX();
		int y = pooyo->getY();

		if (y > 0 && grid[y - 1][x] != nullptr)
		{
			grid[y - 1][x]->connection &= ~POOYO_CONN_UP;
		}
		if (y + 1 < height && grid[y + 1][x] != nullptr)
		{
			grid[y + 1][x]->connection &= ~POOYO_CONN_DN;
		}
		if (x > 0 && grid[y][x - 1] != nullptr)
		{
			grid[y][x - 1]->connection &= ~POOYO_CONN_LT;
		}
		if (x + 1 < width && grid[y][x + 1] != nullptr)
		{
			grid[y][x + 1]->connection &= ~ POOYO_CONN_RT;
		}

		pooyo->connection = POOYO_CONN_NO;
	}
}


bool Board::EraseLinkedBlocks()
{
	bool isErased = false;

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
		//|| isChecked[y][x] == true
		|| grid[y][x] == nullptr
		|| grid[y][x]->getColor() != color)
		return false;

	linkedBlockCount++;
	linkedBlock[y][x] = true;

	checkLinked(x + 1, y, color);
	checkLinked(x - 1, y, color);
	checkLinked(x, y + 1, color);
	checkLinked(x, y - 1, color);
}

void Board::CheckConnection()
{
	std::queue<Pooyo*> q;
	bool** isChecked = new bool*[height];
	for (int i = 0; i < height; ++i)
		isChecked[i] = new bool[width]();

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (grid[y][x] == nullptr)
				continue;

			q.push(grid[y][x]);
			int color = grid[y][x]->getColor();

			isChecked[y][x] = true;
			int count = 1;
			std::vector<Pooyo*> connected;

			while (!q.empty())
			{
				Pooyo* py = q.front();
				q.pop();
				connected.push_back(py);

				int x = py->getX();
				int y = py->getY();

				if (y > 0 && !isChecked[y-1][x] && grid[y - 1][x] != nullptr && grid[y - 1][x]->getColor() == color)
				{
					isChecked[y-1][x] = true;
					count++;
					py->connection |= POOYO_CONN_UP;
					q.push(grid[y - 1][x]);
				}
				if (y + 1 < height && !isChecked[y + 1][x] && grid[y + 1][x] != nullptr && grid[y + 1][x]->getColor() == color)
				{
					isChecked[y+1][x] = true;
					count++;
					py->connection |= POOYO_CONN_DN;
					q.push(grid[y + 1][x]);
				}
				if (x > 0 && !isChecked[y][x-1] && grid[y][x - 1] != nullptr && grid[y][x - 1]->getColor() == color)
				{
					isChecked[y][x-1] = true;
					count++;
					py->connection |= POOYO_CONN_LT;
					q.push(grid[y][x - 1]);
				}
				if (x + 1 < width && !isChecked[y][x + 1] && grid[y][x + 1] != nullptr && grid[y][x + 1]->getColor() == color)
				{
					isChecked[y][x+1] = true;
					count++;
					py->connection |= POOYO_CONN_RT;
					q.push(grid[y][x+1]);
				}
			}

			if (count > 3)
			{
				pooyoToDelete.insert(pooyoToDelete.end(), connected.begin(), connected.end());
			}
		}
	}

	for (int i = 0; i < height; i++)
		delete[] isChecked[i];
	delete[] isChecked;
}

void Board::EraseOver4()
{
	for (auto pooyo : pooyoToDelete)
	{
		grid[pooyo->getY()][pooyo->getX()] = nullptr;
		delete pooyo;
	}
	pooyoToDelete.clear();
}