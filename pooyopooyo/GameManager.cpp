#include "GameManager.h"

#include <SDL.h>
//#include "sdl_renderer.h"
//#include "SDL_image.h"
#include "Renderer.h"


GameManager* GameManager::mInstance = nullptr;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
	delete curScene;

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

	Renderer::Initialize(windowTitle);

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
	//SDL_RenderClear(Renderer::renderer);
	Renderer::RenderClear();

	curScene->Render();


	if (isSceneChanging)
	{
		// Draw Rectangle
		SDL_Rect rect;
		rect.x = 0; rect.y = 0; rect.w = SCREEN_WIDTH; rect.h = SCREEN_HEIGHT;
		
		SDL_SetRenderDrawColor(Renderer::renderer, 0, 0, 0, (Uint8)(255*alpha));
		SDL_RenderFillRect(Renderer::renderer, &rect);
	}

	//SDL_RenderPresent(renderer);
	Renderer::RenderPresent();
}

void GameManager::ChangeScene(int sceneIndex)
{
	isSceneChanging = true;
	nextSceneIndex = sceneIndex;
	deltaAlpha = 1.0f;
}