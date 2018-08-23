#pragma once

enum PooyoColor
{
	Red,
	Green,
	Blue,
	Yellow
};


class Pooyo
{
private:
	float x_actual, y_actual;
	float x_toBe, y_toBe;
	int color;
	char isConnected; // bitString. in order of 0000urdl

public:
	Pooyo(int _x, int _y, int _color)
		: x_actual(_x), y_actual(_y),
		x_toBe(_x), y_toBe(_y),
		color(_color), isConnected(0)
	{}

	float getX();
	float getY();
	void setX(float x);
	void setY(float y);
	//void setX_toBe(float x);
	//void setY_toBe(float y);
	int getColor();

	int getGridX();
	int getGridY();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown(float dist = 0.5f);

	void updateMoving(int deltaTIme);
};