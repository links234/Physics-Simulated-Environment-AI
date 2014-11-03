#include "AllegroEx.h"

void Line(BITMAP* hBackBuffer, Vec2 A, Vec2 B, CColor color)
{
	line(hBackBuffer, (int)A.x, (int)A.y, (int)B.x, (int)B.y, makecol(color.mRed,color.mGreen,color.mBlue));
}

void CircleFill(BITMAP* hBackBuffer, Vec2 C, double radius, CColor color)
{
	circlefill(hBackBuffer, (int)C.x, (int)C.y, radius, makecol(color.mRed,color.mGreen,color.mBlue));
}

void Circle(BITMAP* hBackBuffer, Vec2 C, double radius, CColor color)
{
	circle(hBackBuffer, (int)C.x, (int)C.y, radius, makecol(color.mRed,color.mGreen,color.mBlue));
}

void StretchBlit(BITMAP *hSprite, BITMAP *hBackBuffer, Vec2 A, Vec2 B)
{
	//BITMAP *hTemp=create_bitmap((int)B.x-A.x+1,(int)B.y-A.y+1);
	stretch_sprite(hBackBuffer,hSprite,(int)A.x,(int)A.y,(int)B.x-A.x+1,(int)B.y-A.y+1);
	//draw_sprite(hBackBuffer,hTemp,(int)A.x,(int)A.y);
	//draw_trans_sprite(hBackBuffer,hSprite,(int)A.x,(int)A.y);
	//destroy_bitmap(hTemp);
}
