#pragma once
#include <SDL_image.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800

class Renderer
{
private:
	Renderer();
	~Renderer();

public:
	static SDL_Window * window;
	static SDL_Renderer* renderer;

public:
	static void Initialize(const char* windowTitle);

	static void RenderClear();
	static void RenderPresent();

	static void RenderTexture(Texture* t);

};

class Texture
{
public:
	SDL_Texture * texture;
	SDL_Rect clip, dest;
	int x, y, w, h;
};