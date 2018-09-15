#pragma once
#include "Pooyo.h"
#include <cstdlib>
#include <cmath>

float Pooyo::getX() { return x_actual; }
float Pooyo::getY() { return y_actual; }
void Pooyo::setX(float x) { x_actual = x; }
void Pooyo::setY(float y) { y_actual = y; }
//void Pooyo::setX_toBe(float x) { x_toBe = x; }
//void Pooyo::setY_toBe(float y) { y_toBe = y; }
int Pooyo::getColor() { return color; }
int Pooyo::getColor() const { return color; }

void Pooyo::moveLeft() { x_toBe--; }
void Pooyo::moveRight() { x_toBe++; }
void Pooyo::moveUp() { y_toBe--; }
void Pooyo::moveDown(float dist) { y_toBe += dist; }

int Pooyo::getGridX() { return (int)floorf(x_actual + 0.6f); }
int Pooyo::getGridY() { return (int)floorf(y_actual + 0.6f); }


void Pooyo::updateMoving(float deltaTime)
{
	float maxDistance =  deltaTime * 20.0f;

	float x_Diff = x_toBe - x_actual;
	if (x_Diff < -maxDistance)
		x_actual -= maxDistance;
	else if (maxDistance < x_Diff)
		x_actual += maxDistance;
	else
		x_actual += x_Diff;

	float y_Diff = y_toBe - y_actual;
	if (y_Diff < -maxDistance)
		y_actual -= maxDistance;
	else if (maxDistance < y_Diff)
		y_actual += maxDistance;
	else
		y_actual += y_Diff;
}