#ifndef ALLEGROEX_H
#define ALLEGROEX_H

#include "Main.h"

void Line(BITMAP* hBackBuffer, Vec2 A, Vec2 B, CColor color);
void CircleFill(BITMAP* hBackBuffer, Vec2 C, double radius, CColor color);
void Circle(BITMAP* hBackBuffer, Vec2 C, double radius, CColor color);
void StretchBlit(BITMAP *hSprite, BITMAP *hBackBuffer, Vec2 A, Vec2 B);

#endif