#pragma once

enum PooyoColor
{
	POOYO_COLOR_RED,
	POOYO_COLOR_GREEN,
	POOYO_COLOR_BLUE,
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
	//float x_actual, y_actual;
	//float x_toBe, y_toBe;
	int x, y;
	float xAnim, yAnim;
	int color;
	bool isOnHalf = true;

public:
	char connection; // bitString. Using enum PooyoConnection

public:
	Pooyo(int _x, int _y, int _color)
		//: x_actual(_x), y_actual(_y),
		//x_toBe(_x), y_toBe(_y),
		:x(_x), y(_y), xAnim((float)_x), yAnim((float)_y),
		color(_color), connection(POOYO_CONN_NO)
	{}

	int getX();
	int getY();
	void setX(float _x);
	void setY(float _y);
	int getColor();
	int getColor() const;
	bool IsOnHalf();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	//void updateMoving(float deltaTIme);
};