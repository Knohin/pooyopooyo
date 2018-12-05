#include "Renderer.h"
//#include "sdl_renderer.h"
#include "res_path.h"
#include <utility>
// #include "cleanup.h"

SDL_Window* Renderer::window = 0;
SDL_Renderer* Renderer::renderer = 0;

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
		CleanUp(window);
		SDL_Quit();
		exit(1);
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Renderer::RenderClear() { SDL_RenderClear(renderer);}
void Renderer::RenderPresent() { SDL_RenderPresent(renderer); }

int Renderer::LoadTexture(const std::string &fileName, Texture* tex)
{
	tex->texture = IMG_LoadTexture(renderer, (getResourcePath() + fileName).c_str());
	if (tex->texture == nullptr)
	{
		logSDLError(std::cerr, "LoadTexture");
		/*
		SDL_Quit();
		exit(1);
		*/
		return -1;
	}
	return 0;
}


void Renderer::RenderTexture(Texture& tex)
{
	if (tex.clip.w == 0)
	{
		tex.clip.w = tex.dest.w;
		tex.clip.h = tex.dest.h;
	}
	SDL_RenderCopy(renderer, tex.texture, &tex.clip, &tex.dest);
}
void Renderer::RenderTexture(Texture& tex, int x, int y)
{
	SDL_Rect d = tex.dest;
	d.x += x;
	d.y += y;

	if (tex.clip.w == 0)
	{
		tex.clip.w = tex.dest.w;
		tex.clip.h = tex.dest.h;
	}

	SDL_RenderCopy(renderer, tex.texture, &tex.clip, &d);
}
void Renderer::RenderTextureRepeat(Texture& tex, float offsetX, float offsetY)
{
	SDL_Rect quater, clip;
	int w, h;
	int remainW = tex.dest.w, remainH = tex.dest.h;
	offsetX = offsetX - (int)offsetX;
	offsetY = offsetY - (int)offsetY;
	SDL_QueryTexture(tex.texture, NULL, NULL, &w, &h);

	quater = tex.dest;
	clip.w = w;
	clip.h = h;
	clip.y = offsetY * h;
	quater.w = w;
	quater.h = h;
	quater.y = tex.dest.y;

	while (0 < remainH)
	{
		quater.x = tex.dest.x;
		clip.x = offsetX * w;

		clip.h = h;
		if (clip.y + clip.h > h)
			clip.h = clip.h - clip.y;
		if (remainH < clip.h)
			clip.h = remainH;
		remainH -= clip.h;
		quater.h = clip.h;

		remainW = tex.dest.w;
		while (0 < remainW)
		{
			clip.w = w;
			if (clip.x + clip.w > w)
				clip.w = clip.w - clip.x;
			if (remainW < clip.w)
				clip.w = remainW;
			remainW -= clip.w;
			quater.w = clip.w;

			//renderTexture(tex, renderer, quater, &clip);
			SDL_RenderCopy(renderer, tex.texture, &clip, &quater);

			quater.x += quater.w;
			clip.x = 0;
		}
		quater.y += quater.h;
		clip.y = 0;
	}
}


void logSDLError(std::ostream& os, const std::string& msg)
{
	os << msg.c_str() << " Error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr)
	{
		logSDLError(std::cerr, "LoadTexture");
	}
	return texture;
}
