#pragma once
#include "Pooyo.h"
#include <cstdlib>
#include <cmath>

int Pooyo::getX() { return x; }
int Pooyo::getY() { return y; }
void Pooyo::setX(float _x) { xAnim = x; x = (int)ceilf(_x); }
void Pooyo::setY(float _y) { yAnim = y; y = (int)ceilf(_y); }
int Pooyo::getColor() { return color; }
int Pooyo::getColor() const { return color; }
bool Pooyo::IsOnHalf() { return isOnHalf; }

void Pooyo::moveLeft() { xAnim = x; x--; }
void Pooyo::moveRight() { xAnim = x; x++; }
void Pooyo::moveUp() { yAnim = y; y--; }
void Pooyo::moveDown()
{
	if (!isOnHalf)
	{
		yAnim = y;
		++y;
	}
	isOnHalf = !isOnHalf;
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