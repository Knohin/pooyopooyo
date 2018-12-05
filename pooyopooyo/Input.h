#pragma once

class Scene;
class Command
{
public:
	virtual ~Command() {}
	virtual void execute(Scene* scene) = 0;
};

class MoveLeftCommand : Command		{ virtual void execute(Scene* scene);};
class MoveRightCommand : Command	{ virtual void execute(Scene* scene);};
class MoveDownCommand : Command		{ virtual void execute(Scene* scene);};
class DropDownCommand : Command		{ virtual void execute(Scene* scene);};
class RotateCommand : Command		{ virtual void execute(Scene* scene);};
class QuitCommand : Command			{ virtual void execute(Scene* scene);};


class InputHandler
{
private:
	Command * buttonW;
	Command * buttonA;
	Command * buttonS;
	Command * buttonD;
	Command * buttonQ;
	Command * buttonSpace;
	Command * buttonEnter;

public:
	~InputHandler();

	void SetButtonW(Command* command);
	void SetButtonA(Command* command);
	void SetButtonS(Command* command);
	void SetButtonD(Command* command);
	void SetButtonQ(Command* command);
	void SetButtonSpace(Command* command);
	void SetButtonEnter(Command* command);

	Command* handleInput();
};