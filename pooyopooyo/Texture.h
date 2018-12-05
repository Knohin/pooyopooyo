#pragma once

#include <SDL.h>
#include <SDL_image.h>

class Texture
{
public:
	SDL_Texture * texture; // TODO : Shared ptr
	SDL_Rect clip, dest;
};