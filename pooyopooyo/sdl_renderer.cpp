#include "sdl_renderer.h"
#include <SDL_image.h>

void logSDLError(std::ostream& os, const std::string& msg)
{
	os << msg.c_str() << " Error: " << SDL_GetError() << std::endl;
}

// Before sdl_image.h
//SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren) {
//	//Initialize to nullptr to avoid dangling pointer issues
//	SDL_Texture *texture = nullptr;
//	//Load the image
//	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
//	//If the loading went ok, convert to texture and return the texture
//	if (loadedImage == nullptr) {
//		logSDLError(std::cout, "LoadBMP");
//	}
//	else {
//		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
//		SDL_FreeSurface(loadedImage);
//		//Make sure converting went ok too
//		if (texture == nullptr) {
//			logSDLError(std::cout, "CreateTextureFromSurface");
//		}
//	}
//	return texture;
//}
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr)
	{
		logSDLError(std::cerr, "LoadTexture");
	}
	return texture;
}


void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip)
{
	renderTexture(tex, ren, x, y, 0.0f, clip);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, float angle, SDL_Rect *clip)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr)
	{
		if (clip->w == 0 && clip->h == 0)
			SDL_QueryTexture(tex, NULL, NULL, &clip->w, &clip->h);
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

	SDL_Point center;
	center.x = dst.w / 2;
	center.y = dst.h / 2;
	renderTexture(tex, ren, dst, clip, angle, &center);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h, SDL_Rect *clip) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	//Query the texture to get its width and height to use
	renderTexture(tex, ren, dst, clip);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect& dst, SDL_Rect *clip)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect& dst, SDL_Rect *clip, float angle, SDL_Point* center)
{
	SDL_RenderCopyEx(ren, tex, clip, &dst, angle, center, SDL_FLIP_NONE);
}