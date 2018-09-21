#include "GameManager.h"

#include <SDL.h>
#include "sdl_renderer.h"
#include "SDL_image.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
//const int HEIGHT = 12;
//const int WIDTH = 6;

GameManager* GameManager::mInstance = nullptr;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
	delete curScene;

	cleanup(window, renderer);
	window = nullptr;
	renderer = nullptr;

	IMG_Quit();
	SDL_Quit();
}

void GameManager::Initialize(char* windowTitle)
{
	isRunning = true;
	isSceneChanging = false;
	nextSceneIndex = 0;
	alpha = 0;
	deltaAlpha = 0;

	// Renderer Setting
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		logSDLError(std::cerr, "SDL_Init");
		exit(1);
	}
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		logSDLError(std::cerr, "SDL_CreateWindow");
		SDL_Quit();
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		logSDLError(std::cerr, "SDL_CreateRenderer");
		cleanup(window);
		SDL_Quit();
		exit(1);
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SceneManager::GetInstance();
}

GameManager& GameManager::GetInstance()
{
	if (mInstance == nullptr)
		mInstance = new GameManager();
	return *mInstance;
}

bool GameManager::IsRunning() { return isRunning; }
void GameManager::StopRunning() { isRunning = false; }

void GameManager::Update(float deltaTime)
{
	if (isSceneChanging)
	{
		alpha += deltaAlpha * deltaTime;
		if (1 < alpha)
		{
			alpha = 1;
			deltaAlpha *= -1;
			SceneManager::GetInstance().ChangeScene(nextSceneIndex);
		}
		if (alpha < 0)
		{
			alpha = 0;
			nextSceneIndex = -1;
			isSceneChanging = false;
		}
	}
	else
	{
		curScene->HandleInput();
	}

	curScene->Update(deltaTime);
}

void GameManager::Render()
{
	SDL_RenderClear(renderer);

	curScene->Render();


	if (isSceneChanging)
	{
		// Draw Rectangle
		SDL_Rect rect;
		rect.x = 0; rect.y = 0; rect.w = SCREEN_WIDTH; rect.h = SCREEN_HEIGHT;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)(255*alpha));
		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_RenderPresent(renderer);
}

void GameManager::ChangeScene(int sceneIndex)
{
	isSceneChanging = true;
	nextSceneIndex = sceneIndex;
	deltaAlpha = 1.0f;
}