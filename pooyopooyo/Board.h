#pragma once
#include "Pooyo.h"
#include <vector>

enum eBoardState
{
	Moving,
	//PrepareStacking,
	Stacking,
	Popping
};

class Board
{
private:
	int width, height;
	eBoardState state;
	bool isFinished;

	/*************************************
	* board[y][x]
	* 0   : Blank Space
	* 1~4 : Four Different Color Block
	*************************************/
	Pooyo*** grid;
	bool** linkedBlock;
	//bool** isChecked;
	int linkedBlockCount;

	int direction;
	static const int shape[4][2][2];

	float moveDownInterval;
	float elapsedTime;

	int totalErasedBlockCount;
	int comboCount;


public:
	Pooyo * curPooyo[2];
	std::vector<Pooyo*> pooyoToStack;
	std::vector<Pooyo*> pooyoToDelete;

public:
	Board() {}
	Board(int _width, int _height);
	void Init();
	~Board();
	void Release();

	void Update(float deltaTime);

	Pooyo*** GetGrid();
	int GetHeight();
	int GetWidth();
	int GetDirection();
	bool IsFinished();
	void SetElapsedTime(float t);
	void SetIsFinished(bool);

	bool IsCollideAt(float x, float y);
	bool IsCollide(Pooyo* pooyo);
	bool IsCollide(Pooyo* pooyo, float x, float y);

	void AddPooyo(Pooyo* newPooyo);
	void RotateCurPooyo();
	void ChangeState(eBoardState newState);

private:
	void CheckPooyoToStack();
	//bool StackDownBlocks(float distanceToMove);

	bool EraseLinkedBlocks();
	bool checkLinked(int x, int y, int color);

	void CheckConnection();
	void EraseOver4();

public:
	class Proxy {
	private:
		Pooyo** row;
	public:
		Proxy(Pooyo** _row) : row(_row) {}
		Pooyo*& operator[](int x) { return row[x]; }
	};
	Proxy operator[](int y) { return Proxy(grid[y]); }

	Board& operator=(const Board& rVal)
	{
		Release();
		width = rVal.width;
		height = rVal.height;
		Init();
		linkedBlockCount = rVal.linkedBlockCount;

		return *this;
	}
};

