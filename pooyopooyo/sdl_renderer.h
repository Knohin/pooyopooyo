#pragma once
#include <iostream>
#include <SDL_render.h>
//#include "res_path.h"
#include "cleanup.h"

void logSDLError(std::ostream& os, const std::string& msg);

/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*
* @param dst The destination rectangle to render the texture to
* @param slip The sub-section of the texture to draw (clipping rect)
*				default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = NULL);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, float angle, SDL_Rect *clip = NULL);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, SDL_Rect *clip = NULL);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect& dst, SDL_Rect *clip = NULL);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect& dst, SDL_Rect *clip, float angle, SDL_Point* center);