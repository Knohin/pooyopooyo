#pragma once
#include "Pooyo.h"
#include "Board.h"
#include <SDL_render.h>

enum eGameState
{
	Moving,
	Stacking,
	Poping
};

class GameManager
{
public:
	bool isRunning;
	bool isBlockHit;

	int level;
	int moveDownInterval;

	int totalErasedBlockCount;
	int comboCount;
	int score;

	eGameState state;

	int direction;
	static const int shape[4][2][2];
	
public:
	Pooyo * curPooyo[2];

	//Block * curBlock;
	Board board;
	int elapsedTime;

	GameManager(int width, int height);
	~GameManager();

	void update(int deltaTime);

	void rotateCurPooyo();
	void setCurPooyoOntoBoard();
	bool stackDownBlocksOnBoard(int distanceToMove);
};