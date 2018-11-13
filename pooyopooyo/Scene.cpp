#include "Scene.h"

#include <iostream>
#include <cstdlib>
#include <SDL_image.h>
#include "GameManager.h"
#include "sdl_renderer.h"
#include "res_path.h"

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
		rot = rand() % 360;
		velX = rand() % 400 - 200;
		velY = -(rand() % 400+ 400) ;
		angularVel = rand() % 500 - 250;
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
	//renderer = GameManager::GetInstance().renderer;

	background = loadTexture(getResourcePath() + "UI\\StartMenuBackground.png", renderer);
	title = loadTexture(getResourcePath() + "UI\\StartMenuTitle.png", renderer);
	bottomText = loadTexture(getResourcePath() + "UI\\StartMenuSpace.png", renderer);
	pooyo[POOYO_COLOR_RED] = loadTexture(getResourcePath() + "Pooyo\\RedPooyo.png", renderer);
	pooyo[POOYO_COLOR_GREEN] = loadTexture(getResourcePath() + "Pooyo\\GreenPooyo.png", renderer);
	pooyo[POOYO_COLOR_BLUE] = loadTexture(getResourcePath() + "Pooyo\\BluePooyo.png", renderer);
	pooyo[POOYO_COLOR_YELLOW] = loadTexture(getResourcePath() + "Pooyo\\YellowPooyo.png", renderer);

	if (background == nullptr
		|| title == nullptr
		|| bottomText == nullptr
		|| pooyo[0] == nullptr
		|| pooyo[1] == nullptr
		|| pooyo[2] == nullptr
		|| pooyo[3] == nullptr)
	{
		cleanup(background, title, bottomText);
		cleanup(pooyo[0], pooyo[1], pooyo[2], pooyo[3]);
		logSDLError(std::cerr, "init StartMenuScene Texture");
		SDL_Quit();
		exit(1);
	}
}

StartMenuScene::~StartMenuScene()
{
	Release();
}
void StartMenuScene::Release()
{
	delete inputHandler;
	inputHandler = nullptr;
	renderer = nullptr;

	for(int i = particles.size()-1; 0<=i; --i)
		delete particles[i];
	particles.clear();

	cleanup(background, title, bottomText);
	cleanup(pooyo[0], pooyo[1], pooyo[2], pooyo[3]);
	background = title = bottomText = nullptr;
	pooyo[0] = pooyo[1] = pooyo[2] = pooyo[3] = nullptr;
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
	static bool flag = true;
	int w, h;
	SDL_QueryTexture(background, NULL, NULL, &w, &h);
	if (flag)
	{
		std::cout << "w, h : " << w << ", " << h << std::endl;
		flag = !flag;
	}


	renderTexture(background, renderer, 0, 0, w , h, BG_OffsetX, BG_OffsetY);
	renderTexture(title, renderer, 0, 0);
	renderTexture(bottomText, renderer, 0, 0);

	//std::cout << "particles.size : " << particles.size() << std::endl;
	for (int i = 0; i < particles.size(); ++i)
	{
		renderTexture(pooyo[particles[i]->pooyo->getColor()], renderer, particles[i]->x, particles[i]->y, particles[i]->rot);
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

	///////////////////////////
	// Load Texture
	//renderer = GameManager::GetInstance().renderer;
	//std::cout << "Resource Path is >> " << getResourcePath("") << std::endl;

	/*
	background = loadTexture(getResourcePath() + "UI\\Background.bmp", renderer);
	pooyo[POOYO_COLOR_RED] = loadTexture(getResourcePath() + "Pooyo\\RedPooyo.png", renderer);
	pooyo[POOYO_COLOR_GREEN] = loadTexture(getResourcePath() + "Pooyo\\GreenPooyo.png", renderer);
	pooyo[POOYO_COLOR_BLUE] = loadTexture(getResourcePath() + "Pooyo\\BluePooyo.png", renderer);
	pooyo[POOYO_COLOR_YELLOW] = loadTexture(getResourcePath() + "Pooyo\\YellowPooyo.png", renderer);

	if (background == nullptr
		|| pooyo[0] == nullptr
		|| pooyo[1] == nullptr
		|| pooyo[2] == nullptr
		|| pooyo[3] == nullptr)
	{
		cleanup(background, pooyo[0], pooyo[1], pooyo[2], pooyo[3]);
		logSDLError(std::cerr, "init SoloGameScene Texture");
		SDL_Quit();
		exit(1);
	}
	*/

	tile = loadTexture(getResourcePath() + "UI\\tile.png", renderer);
	statusUIBackground = loadTexture(getResourcePath() + "UI\\StatusUI.png", renderer);
	pooyoSprite = loadTexture(getResourcePath() + "Pooyo\\pooyo_sprite.png", renderer);

	if (tile == nullptr
		|| statusUIBackground == nullptr
		|| pooyoSprite == nullptr)
	{
		cleanup(tile, statusUIBackground, pooyoSprite);
		logSDLError(std::cerr, "init SoloGameScene Texture");
		SDL_Quit();
		exit(1);
	}

}

SoloGameScene::~SoloGameScene()
{
	Release();
}
void SoloGameScene::Release()
{
	delete inputHandler;
	inputHandler = nullptr;
	renderer = nullptr;

	delete board;

	cleanup(background, pooyo[0], pooyo[1], pooyo[2], pooyo[3]);
	background = pooyo[0] = pooyo[1] = pooyo[2] = pooyo[3] = nullptr;
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

	// Draw Background (Not needed)
	// renderTexture(background, renderer, 0, 0);

	// Draw pooyo which is on var board
	int color = 0;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if ((*board)[i][j] == nullptr)
				continue;
			int color = (*board)[i][j]->getColor();
			int x = (*board)[i][j]->getX();
			int y = (*board)[i][j]->getY();
			renderTexture(pooyo[color], renderer, 16 + x * 64, 16 + y * 64);
		}
	}

	// Draw pooyoToStack
	for (auto iter = board->pooyoToStack.begin();
		iter != board->pooyoToStack.end();
		iter++)
	{
		int color = (*iter)->getColor();
		float x = (*iter)->getX();
		float y = (*iter)->getY();
		renderTexture(pooyo[color], renderer, 16 + x * 64, 16 + y * 64);
	}


	// Draw CurPooyo
	if (board->curPooyo[0])
	{
		int screenX, screenY;
		for (int i = 0; i < 2; i++)
		{
			float x = board->curPooyo[i]->getX();
			float y = board->curPooyo[i]->getY();
			if (board->curPooyo[i]->IsOnHalf()) y -= 0.5f;
			BoardToWindow(x, y, screenX, screenY);
			renderTexture(pooyo[board->curPooyo[i]->getColor()], renderer, screenX, screenY);
		}
	}

	// Draw status UI background
	renderTexture(background, renderer, 0, 0);
}


template<typename T>
void BoardToWindow(T x, T y, int& xOut, int& yOut)
{
	x = (float)x / BOARD_COLS * BOARD_WIDTH + BOARD_LEFT;
	y = (float)y / BOARD_ROWS * BOARD_HEIGHT + BOARD_UP;
	xOut = (int)x;
	yOut = (int)y;
}