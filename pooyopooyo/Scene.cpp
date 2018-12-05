#include "Scene.h"

#include <iostream>
#include <cstdlib>
#include <SDL_image.h>
#include "GameManager.h"
#include "Renderer.h"
//#include "sdl_renderer.h"
//#include "res_path.h"

/////  CONST VARIABLES
const int BOARD_UP = 16;
const int BOARD_LEFT = 16;
const int BOARD_COLS = 6;	// TODO: FLEXIBLE
const int BOARD_ROWS = 12;	// TODO: FLEXIBLE
const int BOARD_WIDTH = 384;
const int BOARD_HEIGHT = 768;

template<typename T>
void BoardToWindow(T x, T y, int& xOut, int& yOut);

SceneManager* SceneManager::mInstance = nullptr;

SceneManager::SceneManager()
{
	gameManager = &GameManager::GetInstance();

	// Setting Scene
	scenes.push_back(new StartMenuScene());
	scenes.push_back(new SoloGameScene());

	gameManager->curScene = scenes[0];
	gameManager->curScene->Initialize();
}

SceneManager::~SceneManager()
{
	gameManager->curScene->Release();
	gameManager->curScene = nullptr;
	gameManager->StopRunning(); // »Ï...
	gameManager = nullptr;

	for (int i = scenes.size() - 1; 0 <= i; i--)
		delete scenes[i];
}

SceneManager& SceneManager::GetInstance()
{
	if (mInstance == nullptr)
		mInstance = new SceneManager();
	return *mInstance;
}

void SceneManager::ChangeScene(int sceneIndex)
{
#pragma warning(suppress:4018)
	if (sceneIndex < 0 && scenes.size() <= sceneIndex)
	{
		std::cerr << "ChangeScene Error : out of index" << std::endl;
		return;
	}
	gameManager->curScene->Release();
	gameManager->curScene = scenes[sceneIndex];
	gameManager->curScene->Initialize();
}


//////////////////////////////
// StartMenuScene
class PooyoParticle
{
public:
	Pooyo* pooyo;
	float x,y, rot, velX, velY, angularVel;

public:
	PooyoParticle(float _x, float _y)
	{
		int color = rand() % 4;
		pooyo = new Pooyo(0, 0, color);
		x = _x;
		y = _y;
		rot = rand() / RAND_MAX * 360.0f;
		velX = rand() / RAND_MAX * 400.0f - 200;
		velY = -(rand() / RAND_MAX * 400.0f + 400) ;
		angularVel = rand() / RAND_MAX * 500.0f - 250;
	}
	~PooyoParticle()
	{
		delete pooyo;
	}
};

void StartMenuScene::Initialize()
{
	BG_OffsetX = 0.0f;
	BG_OffsetY = 0.0f;

	// Load Texture
	if (-1 == Renderer::LoadTexture("UI\\StartMenuBackground.png", &background)
		|| -1 == Renderer::LoadTexture("UI\\StartMenuTitle.png", &title)
		|| -1 == Renderer::LoadTexture("UI\\StartMenuSpace.png", &bottomText) )
	{
		Renderer::CleanUp(&background, &title, &bottomText);
		logSDLError(std::cerr, "init StartMenuScene Texture");
		SDL_Quit();
		exit(1);
	}

	// Init texture
	SDL_QueryTexture(background.texture, NULL, NULL, &background.dest.w, &background.dest.h);
	SDL_QueryTexture(title.texture, NULL, NULL, &title.dest.w, &title.dest.h);
	SDL_QueryTexture(bottomText.texture, NULL, NULL, &bottomText.dest.w, &bottomText.dest.h);

}

StartMenuScene::~StartMenuScene()
{
	Release();
}
void StartMenuScene::Release()
{
	delete inputHandler;
	inputHandler = nullptr;

	for(int i = particles.size()-1; 0<=i; --i)
		delete particles[i];
	particles.clear();

	Renderer::CleanUp(&background, &title, &bottomText);
}

void StartMenuScene::HandleInput()
{
	static SDL_Event e;
	if (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			GameManager::GetInstance().StopRunning();
		if (e.key.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_RETURN:
				GameManager::GetInstance().ChangeScene(1);
				break;
			case SDLK_SPACE:
				spurt();
				break;
			}
		}
	}
}

void StartMenuScene::Update(float deltaTime)
{
	static float elapsedTime = 0.0f;
	
	elapsedTime += deltaTime;
	// Background offset
	BG_OffsetX = 0.2f *elapsedTime;
	BG_OffsetY = 0.2f *elapsedTime;
	// Spurt
	for (int i = particles.size() - 1; 0 <= i; --i)
	{
		particles[i]->velY += 500.0f*deltaTime; // Gravity
		particles[i]->x = particles[i]->x + particles[i]->velX * deltaTime;
		particles[i]->y = particles[i]->y + particles[i]->velY * deltaTime;
		particles[i]->rot += particles[i]->angularVel*deltaTime;

		// Check particles is out of screen
		if (particles[i]->x < -100 || 620 < particles[i]->x || 820 < particles[i]->y)
		{
			delete particles[i];
			particles.erase(particles.begin() + i);
		}
	}

}
void StartMenuScene::Render()
{
	Renderer::RenderTexture(background);
	Renderer::RenderTexture(title);
	Renderer::RenderTexture(bottomText);

	for (size_t i = 0; i < particles.size(); ++i)
	{
		particles[i]->pooyo->setX(particles[i]->x);
		particles[i]->pooyo->setY(particles[i]->y);
		particles[i]->pooyo->Render();
	}
}

void StartMenuScene::spurt()
{
	std::cout << " SPACE";
	for (int i = 0; i < 10; i++)
	{
		particles.push_back(new PooyoParticle(300, 800));
	}
}

//////////////////////////////
// SoloGameScene

void SoloGameScene::Initialize()
{
	///////////////////////////////
	// Initialize member variables
	board = new Board(BOARD_COLS, BOARD_ROWS);

	// Controller Setting
	inputHandler = new InputHandler();
	inputHandler->SetButtonW((Command*)new RotateCommand());
	inputHandler->SetButtonA((Command*)new MoveLeftCommand());
	inputHandler->SetButtonS((Command*)new MoveDownCommand());
	inputHandler->SetButtonD((Command*)new MoveRightCommand());
	inputHandler->SetButtonQ((Command*)new QuitCommand());
	inputHandler->SetButtonSpace((Command*)new DropDownCommand());


	// Load Texture
	if (-1 == Renderer::LoadTexture("UI\\tile.png", &tile)
		|| -1 == Renderer::LoadTexture("UI\\StatusUI.png", &statusUIBackground)
		|| -1 == Renderer::LoadTexture("Pooyo\\pooyo_sprite.png", &pooyoSprite))
	{
		Renderer::CleanUp(&tile, &statusUIBackground, &pooyoSprite);
		logSDLError(std::cerr, "init SoloGameScene Texture");
		SDL_Quit();
		exit(1);
	}

	tile.dest.x = 16;
	tile.dest.y = 16;
	tile.dest.w = 64 * BOARD_COLS;
	tile.dest.h = 64 * BOARD_ROWS;

	SDL_QueryTexture(statusUIBackground.texture, NULL, NULL, &statusUIBackground.dest.w, &statusUIBackground.dest.h);

	pooyoSprite.dest.x = 16;
	pooyoSprite.dest.y = 64+16;
	pooyoSprite.dest.w = 512;
	pooyoSprite.dest.h = 512;
}

SoloGameScene::~SoloGameScene()
{
	Release();
}
void SoloGameScene::Release()
{
	delete inputHandler;
	inputHandler = nullptr;

	delete board;

	Renderer::CleanUp(&tile, &statusUIBackground, &pooyoSprite);
}

void SoloGameScene::HandleInput()
{
	Command* command = inputHandler->handleInput();
	if (command)
		command->execute(this);
}

void SoloGameScene::Update(float deltaTime)
{
	board->Update(deltaTime);
	if (board->IsFinished())
		GameManager::GetInstance().StopRunning();
}

void SoloGameScene::Render()
{
	//Pooyo*** board = board->GetGrid();
	Board& bd = *board;
	const int Height = board->GetHeight();
	const int Width = board->GetWidth();

	// Draw BoardTile
	//Renderer::RenderTexture(tile);
	Renderer::RenderTextureRepeat(tile, 0, 0);

	// Draw pooyo which is on var board
	int color = 0;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if ((*board)[i][j])
				(*board)[i][j]->Render();
		}
	}

	// Draw pooyoToStack
	for (auto iter = board->pooyoToStack.begin();
		iter != board->pooyoToStack.end();
		iter++)
	{
		(*iter)->Render();
	}


	// Draw CurPooyo
	if (board->curPooyo[0])
	{
		int screenX, screenY;
		for (int i = 0; i < 2; i++)
		{
			board->curPooyo[i]->Render();
		}
	}

	// Draw status UI background
	Renderer::RenderTexture(statusUIBackground);

	//Renderer::RenderTexture(pooyoSprite);
}


template<typename T>
void BoardToWindow(T x, T y, int& xOut, int& yOut)
{
	x = (float)x / BOARD_COLS * BOARD_WIDTH + BOARD_LEFT;
	y = (float)y / BOARD_ROWS * BOARD_HEIGHT + BOARD_UP;
	xOut = (int)x;
	yOut = (int)y;
}