#include <iostream>
#include <cstdlib>
#include <conio.h>//kbhit, getch
#include <Windows.h>//gotoxy, Sleep, 콘솔 창크기 조절
#include <ctime>//clock

#include "render.h"

#include "GameManager.h"
#include "Pooyo.h"
#include "Input.h"
#include "Board.h"

using namespace std;


/////  CONST VARIABLES
const int HEIGHT = 12;
const int WIDTH = 6;
const int FPS = 60; //(ms)
const int FRAME_RATE = 1000 / FPS;

/////  VARIABLE DECLARATIONS
GameManager gameManager(WIDTH, HEIGHT);
InputHandler inputHandler;
Renderer renderer("PooyoPooyo!");

///// FUNCTION DECLARATIONS
void initialize();


int main(int argc, char** argv)
{
	
	initialize();

	clock_t lastTime = clock();
	clock_t elapsedTime = 0;


	// Main Loop
	while (gameManager.isRunning)
	{
		clock_t currentTime = clock();
		clock_t deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		if (gameManager.state == Moving)
		{
			Command* command = inputHandler.handleInput();
			if (command)
				command->execute(gameManager);
		}

		gameManager.update(deltaTime);

		elapsedTime += deltaTime;
		if (FRAME_RATE < elapsedTime)
		{
			renderer.render(gameManager);
			elapsedTime = 0;
		}
	}

	return 0;
}

void initialize()
{
	srand((unsigned int)time(NULL));
	
	// Controller Setting
	inputHandler.SetButtonW((Command*)new RotateCommand());
	inputHandler.SetButtonA((Command*)new MoveLeftCommand());
	inputHandler.SetButtonS((Command*)new MoveDownCommand());
	inputHandler.SetButtonD((Command*)new MoveRightCommand());
	inputHandler.SetButtonQ((Command*)new QuitCommand());
	inputHandler.SetButtonSpace((Command*)new DropDownCommand());

	return;
}
