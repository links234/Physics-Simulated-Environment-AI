#ifndef MOUSE_H
#define MOUSE_H

struct structButtons
{
	bool Left,Right,Middle;
};

struct structMouse
{
	structButtons Button;
	Vec2 Pos;
	int Wheel;
};

#endif