#pragma once
#include <SDL_render.h>
#include "GameManager.h"


class Renderer
{
private:
	SDL_Window * window;
	SDL_Renderer* renderer;

	SDL_Texture * background;
	SDL_Texture* pooyo[4]; // 0:Red 1:Green 2:Blue 3:Yellow


public:
	Renderer(char* windowTitle);
	~Renderer();

	void render(GameManager& gameManager);
};