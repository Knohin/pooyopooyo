#include "Renderer.h"
#include "sdl_renderer.h"

void Renderer::Initialize(const char* windowTitle)
{
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
}

void Renderer::RenderClear() { SDL_RenderClear(renderer);}
void Renderer::RenderPresent() { SDL_RenderPresent(renderer); }

