#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Texture.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800

class Texture;

class Renderer
{
public:
	static SDL_Window * window;
	static SDL_Renderer* renderer;

	static const int ScreenWidth = 600;
	static const int ScreenHeight = 800;

public:
	static void Initialize(const char* windowTitle);

	static void RenderClear();
	static void RenderPresent();

	static int LoadTexture(const std::string &fileName, Texture* tex);
	template <typename T, typename... Args>
	static void CleanUp(T *t, Args*... args);
	
	static void RenderTexture(Texture& texture);
	static void RenderTexture(Texture& texture, int x, int y);
	static void RenderTextureRepeat(Texture& texture, float offsetX, float offsetY);

};



void logSDLError(std::ostream& os, const std::string& msg);

/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);


/*
* Recurse through the list of arguments to clean up, cleaning up
* the first one in the list each iteration.
*/
template<typename T, typename... Args>
void Renderer::CleanUp(T *t, Args*... args) {
	CleanUp(t);
	//CleanUp(std::forward<Args>(args)...);
	CleanUp(args...);
}
/*
* These specializations serve to free the passed argument and also provide the
* base cases for the recursive call above, eg. when args is only a single
* item one of the specializations below will be called by
* cleanup(std::forward<Args>(args)...), ending the recursion
*/
template<>
inline void Renderer::CleanUp<SDL_Window>(SDL_Window *win) {
	if (!win) {
		return;
	}
	SDL_DestroyWindow(win);
}
template<>
inline void Renderer::CleanUp<SDL_Renderer>(SDL_Renderer *ren) {
	if (!ren) {
		return;
	}
	SDL_DestroyRenderer(ren);
}
template<>
inline void Renderer::CleanUp<SDL_Texture>(SDL_Texture *tex) {
	if (!tex) {
		return;
	}
	SDL_DestroyTexture(tex);
}
template<>
inline void Renderer::CleanUp<SDL_Surface>(SDL_Surface *surf) {
	if (!surf) {
		return;
	}
	SDL_FreeSurface(surf);
}
template<>
inline void Renderer::CleanUp<Texture>(Texture *tex)
{
	if (!tex || !tex->texture)
		return;
	SDL_DestroyTexture(tex->texture);
}