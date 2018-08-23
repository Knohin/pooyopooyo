#include "Board.h"


Board::Board(int _width=0, int _height=0)
	:width(_width), height(_height)
{
	init();
}
void Board::init()
{
	grid = new int*[height];
	for (int i = 0; i<height; ++i)
		grid[i] = new int[width]();
	linkedBlock = new bool*[height];
	for (int i = 0; i<height; ++i)
		linkedBlock[i] = new bool[width]();

	isChecked = new bool*[height];
	for (int i = 0; i<height; ++i)
		isChecked[i] = new bool[width]();
}

Board::~Board()
{
	destroy();
}
void Board::destroy()
{
	if (grid)
	{
		for (int i = 0; i < height; i++)
			delete[] grid[i];
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
}

int** Board::GetGrid() { return grid; }
int Board::GetHeight() { return height; }
int Board::GetWidth() { return width; }

bool Board::IsCollideAt(int x, int y)
{
	if (x < 0 || width <= x || y < 0 || height <= y ||
		grid[y][x] != 0)
		return true;

	return false;
}

void Board::AddPooyo(Pooyo* newPooyo)
{
  	grid[newPooyo->getGridY()][newPooyo->getGridX()] = newPooyo->getColor()+1;
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
			if (grid[y][x] == 0)
				continue;
			linkedBlockCount = 0;
			checkLinked(x, y, grid[y][x]);
			for (int y = 0; y < height; ++y)
				for (int x = 0; x < width; ++x)
				{
					if (linkedBlock[y][x] == true)
					{
						if (linkedBlockCount > 3)
						{
							// Erase
							grid[y][x] = 0;
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
	if (x < 0 || width <= x || y < 0 || height <= y ||
		isChecked[y][x] == true
		|| grid[y][x] != color)
		return false;

	linkedBlockCount++;
	linkedBlock[y][x] = true;
	isChecked[y][x] = true;

	checkLinked(x + 1, y, color);
	checkLinked(x - 1, y, color);
	checkLinked(x, y + 1, color);
	checkLinked(x, y - 1, color);
}