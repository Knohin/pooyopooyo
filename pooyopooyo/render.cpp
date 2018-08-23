#include "render.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "sdl_renderer.h"
#include "res_path.h"


const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
const int BOARD_UP = 16;
const int BOARD_LEFT = 16;
const int BOARD_COLS = 6;	// TODO: FLEXIBLE
const int BOARD_ROWS = 12;	// TODO: FLEXIBLE
const int BOARD_WIDTH = 384;
const int BOARD_HEIGHT = 768 ;

template<typename T>
void BoardToWindow(T x, T y, int& xOut, int& yOut);

Renderer::Renderer(char* windowTitle)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		logSDLError(std::cerr, "SDL_Init");
		//return 1;
		exit(1);
	}
	window = SDL_CreateWindow(windowTitle, 200, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		logSDLError(std::cerr, "SDL_CreateWindow");
		SDL_Quit();
		//return 1;
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		logSDLError(std::cerr, "SDL_CreateRenderer");
		cleanup(window);
		SDL_Quit();
		//return 1;
		exit(1);
	}

	std::cout << "Resource Path is >> " << getResourcePath("") << std::endl;

	std::string path = getResourcePath("UI") + "Background.bmp";

	background = loadTexture(getResourcePath() + "UI\\Background.bmp", renderer);
	pooyo[0] = loadTexture(getResourcePath() + "Pooyo\\RedPooyo.png", renderer);
	pooyo[1] = loadTexture(getResourcePath() + "Pooyo\\GreenPooyo.png", renderer);
	pooyo[2] = loadTexture(getResourcePath() + "Pooyo\\BluePooyo.png", renderer);
	pooyo[3] = loadTexture(getResourcePath() + "Pooyo\\YellowPooyo.png", renderer);

	if (background == nullptr
		|| pooyo[0] == nullptr
		|| pooyo[1] == nullptr
		|| pooyo[2] == nullptr
		|| pooyo[3] == nullptr)
	{
		cleanup(background, renderer);
		logSDLError(std::cerr, "initGlobalTexture");
		SDL_Quit();
		exit(1);
	}
}

Renderer::~Renderer()
{
	cleanup(window, renderer);
	cleanup(background, pooyo[0], pooyo[1], pooyo[2], pooyo[3]);

	IMG_Quit();
	SDL_Quit();
}

void Renderer::render(GameManager& gameManager)
{
	const int** board = (const int**)gameManager.board.GetGrid();
	const int Height = gameManager.board.GetHeight();
	const int Width = gameManager.board.GetWidth();

	SDL_RenderClear(renderer);

	renderTexture(background, renderer, 0, 0);

	// Render board
	int color = 0;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			color = board[i][j] - 1;
			if (color == -1)
				continue;
			renderTexture(pooyo[board[i][j] - 1], renderer, 16 + j * 64, 16 + i * 64);
		}
	}
	if (gameManager.curPooyo[0])
	{
		int screenX, screenY;
		for (int i = 0; i < 2; i++)
		{
			float x = gameManager.curPooyo[i]->getX();
			float y = gameManager.curPooyo[i]->getY();
			BoardToWindow(x, y, screenX, screenY);
			renderTexture(pooyo[gameManager.curPooyo[i]->getColor()], renderer, screenX, screenY);
		}
	}



	SDL_RenderPresent(renderer);
}

template<typename T>
void BoardToWindow(T x, T y, int& xOut, int& yOut)
{
	x = (float)x / BOARD_COLS * BOARD_WIDTH + BOARD_LEFT;
	y = (float)y / BOARD_ROWS * BOARD_HEIGHT + BOARD_UP;
	xOut = x;
	yOut = y;
}