#pragma once
#include "Pooyo.h"

class Board
{
private:
	int width, height;
	/*************************************
	* board[y][x]
	* 0   : Blank Space
	* 1~4 : Four Different Color Block
	*************************************/
	int** grid;
	bool** linkedBlock;
	bool** isChecked;
	int linkedBlockCount = 0;

public:
	Board() {}
	Board(int _width, int _height);
	void init();
	~Board();
	void destroy();

	int** GetGrid();
	int GetHeight();;
	int GetWidth();

	bool IsCollideAt(int x, int y);
	void AddPooyo(Pooyo* newPooyo);

	bool EraseLinkedBlocks();

private:
	bool checkLinked(int x, int y, int color);

public:
	class Proxy {
	private:
		int* row;
	public:
		Proxy(int* _row) : row(_row) {}
		int& operator[](int x) { return row[x]; }
	};
	Proxy operator[](int y) { return Proxy(grid[y]); }

	Board& operator=(const Board& rVal)
	{
		destroy();
		width = rVal.width;
		height = rVal.height;
		init();
		linkedBlockCount = rVal.linkedBlockCount;

		return *this;
	}
};

