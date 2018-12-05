#pragma once
#include <cstdlib>
#include <cmath>
#include "Pooyo.h"
#include "Renderer.h"

std::map<PooyoConnection, SDL_Rect> Pooyo::spriteClip = {
	{ POOYO_CONN_NO , {0,0,64,64}},
	{ POOYO_CONN_UP , { 128,0,64,64 }},
	{ POOYO_CONN_DN ,{ 192,0,64,64 } },
	{ POOYO_CONN_LT ,{ 256,0,64,64 } },
	{ POOYO_CONN_RT ,{ 320,0,64,64 } },
	{ POOYO_CONN_UL ,{ 448,64,64,64 } },
	{ POOYO_CONN_UR ,{ 384,64,64,64 } },
	{ POOYO_CONN_DL ,{ 448,0,64,64 } },
	{ POOYO_CONN_DR ,{ 384,0,64,64 } },
	{ POOYO_CONN_UD ,{ 0,64,64,64 } },
	{ POOYO_CONN_LR ,{ 64,64,64,64 } },
	{ POOYO_CONN_UDL,{ 320,64,64,64 } },
	{ POOYO_CONN_UDR,{ 128,64,64,64 } },
	{ POOYO_CONN_ULR,{ 256,64,64,64 } },
	{ POOYO_CONN_DLR,{ 192,64,64,64 } },
	{ POOYO_CONN_UDLR,{ 64,0,64,64 } }
};

Pooyo::Pooyo(int _x, int _y, int _color)
	:x(_x), y(_y), xAnim((float)_x), yAnim((float)_y),
	color(_color), connection(POOYO_CONN_NO)
{
	Renderer::LoadTexture("Pooyo\\pooyo_sprite.png", &texture);
	texture.dest.x = 0;
	texture.dest.y = 0;
	texture.dest.w = 64;
	texture.dest.h = 64;
	texture.clip = spriteClip[POOYO_CONN_NO];
	texture.clip.y += color * 128;
}

Pooyo::~Pooyo() 
{
	Renderer::CleanUp(&texture);
}

int Pooyo::getX() { return x; }
int Pooyo::getY() { return y; }
void Pooyo::setX(int _x) { xAnim = (float)x; x = _x; }
void Pooyo::setY(int _y) { yAnim = (float)y; y = _y; }
void Pooyo::setX(float _x) { xAnim = (float)x; x = (int)ceilf(_x); }
void Pooyo::setY(float _y) { yAnim = (float)y; y = (int)ceilf(_y); }
int Pooyo::getColor() { return color; }
int Pooyo::getColor() const { return color; }

void Pooyo::moveLeft() { xAnim = (float)x; x--; }
void Pooyo::moveRight() { xAnim = (float)x; x++; }
void Pooyo::moveUp() { yAnim = (float)y; y--; }
void Pooyo::moveDown()
{
	if (!isHalfwayUp)
	{
		yAnim = (float)y;
		++y;
	}
	isHalfwayUp = !isHalfwayUp;
}

void Pooyo::Render()
{
	if(isHalfwayUp)
		Renderer::RenderTexture(texture, x*64+16, y*64-16);
	else
		Renderer::RenderTexture(texture, x*64+16, y*64+16);
}

//int Pooyo::getGridX() { return (int)floorf(x_actual + 0.6f); }
//int Pooyo::getGridY() { return (int)floorf(y_actual + 0.6f); }

//void Pooyo::updateMoving(float deltaTime)
//{
//	float maxDistance =  deltaTime * 20.0f;
//
//	float x_Diff = x_toBe - x_actual;
//	if (x_Diff < -maxDistance)
//		x_actual -= maxDistance;
//	else if (maxDistance < x_Diff)
//		x_actual += maxDistance;
//	else
//		x_actual += x_Diff;
//
//	float y_Diff = y_toBe - y_actual;
//	if (y_Diff < -maxDistance)
//		y_actual -= maxDistance;
//	else if (maxDistance < y_Diff)
//		y_actual += maxDistance;
//	else
//		y_actual += y_Diff;
//}