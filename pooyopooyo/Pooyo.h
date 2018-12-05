#pragma once
#include "Texture.h"
#include <map>

enum PooyoColor
{
	POOYO_COLOR_RED,
	POOYO_COLOR_BLUE,
	POOYO_COLOR_GREEN,
	POOYO_COLOR_YELLOW,
	POOYO_COLOR_COUNT
};

enum PooyoConnection
{
	POOYO_CONN_NO = 0b0000,
	POOYO_CONN_UP = 0b1000,
	POOYO_CONN_DN = 0b0100,
	POOYO_CONN_LT = 0b0010,
	POOYO_CONN_RT = 0b0001,
	POOYO_CONN_UL = 0b1010,
	POOYO_CONN_UR = 0b1001,
	POOYO_CONN_DL = 0b0110,
	POOYO_CONN_DR = 0b0101,
	POOYO_CONN_UD = 0b1100,
	POOYO_CONN_LR = 0b0011,
	POOYO_CONN_UDL = 0b1110,
	POOYO_CONN_UDR = 0b1101,
	POOYO_CONN_ULR = 0b1011,
	POOYO_CONN_DLR = 0b0111,
	POOYO_CONN_UDLR = 0b1111
};


class Pooyo
{
private:
	static std::map<PooyoConnection, SDL_Rect> spriteClip;

	int x, y;
	float xAnim, yAnim;
	int color;

	Texture texture;

public:
	bool isHalfwayUp = true;
	char connection; // bitString. Using enum PooyoConnection

public:
	Pooyo(int _x, int _y, int _color);
	~Pooyo();

	int getX();
	int getY();
	void setX(int _x);
	void setY(int _y);
	void setX(float _x);
	void setY(float _y);
	int getColor();
	int getColor() const;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void Render();
};