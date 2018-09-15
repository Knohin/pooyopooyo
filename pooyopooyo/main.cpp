#include <iostream>
#include <cstdlib>
#include <conio.h>//kbhit, getch
#include <Windows.h>//gotoxy, Sleep, 콘솔 창크기 조절
#include <ctime>//clock
#include <chrono>

#include "GameManager.h"
#include "Pooyo.h"
#include "Input.h"
#include "Board.h"

using namespace std;


const int FPS = 60; //(ms)
const float FRAME_RATE = 1.0f / FPS;

/////  VARIABLE DECLARATIONS
GameManager& gameManager = GameManager::GetInstance();



int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	gameManager.Initialize("PooyoPooyo!");

	std::chrono::time_point<std::chrono::system_clock> lastTime = std::chrono::system_clock::now();
	float elapsedTime = 0.0;

	// Main Loop
	while (gameManager.IsRunning())
	{
		std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - lastTime;

		lastTime = currentTime;
		gameManager.Update(deltaTime.count());

		elapsedTime += deltaTime.count();
		if (FRAME_RATE < elapsedTime)
		{
			gameManager.Render();
			elapsedTime = 0;
		}
	}

	return 0;
}
