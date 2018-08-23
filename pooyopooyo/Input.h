#pragma once
#include <conio.h>
#include "GameManager.h"
//#include "Block.h"

#define UP (72)
#define LEFT (75)
#define DOWN (80)
#define RIGHT (77)
#define SPACE (32)

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(GameManager& gameManager) = 0;
};

class MoveLeftCommand : Command
{
	virtual void execute(GameManager& gameManager);
};
class MoveRightCommand : Command
{
	virtual void execute(GameManager& gameManager);
};
class MoveDownCommand : Command
{
	virtual void execute(GameManager& gameManager);
};
class DropDownCommand : Command
{
	virtual void execute(GameManager& gameManager);
};
class RotateCommand : Command
{
	virtual void execute(GameManager& gameManager);
};
class QuitCommand : Command
{
	virtual void execute(GameManager& gameManager);
};


class InputHandler
{
private:
	Command * buttonW;
	Command * buttonA;
	Command * buttonS;
	Command * buttonD;
	Command * buttonQ;
	Command * buttonSpace;

public:
	void SetButtonW(Command* command) { buttonW = command; }
	void SetButtonA(Command* command) { buttonA = command; }
	void SetButtonS(Command* command) { buttonS = command; }
	void SetButtonD(Command* command) { buttonD = command; }
	void SetButtonQ(Command* command) { buttonQ = command; }
	void SetButtonSpace(Command* command) { buttonSpace = command; }

	Command* handleInput();
};