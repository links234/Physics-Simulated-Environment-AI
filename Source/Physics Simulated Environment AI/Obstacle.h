#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Main.h"

class CCamera;

class Obstacle
{
public:
	Obstacle(Vec2 position, double radius);

	void Draw(BITMAP* hBackBuffer);

	~Obstacle();

	static void DrawAll(BITMAP* hBackBuffer);
	static void Delete(Obstacle* pObstacle);
	static void SetCamera(CCamera* pCamera);

	Vec2 mPosition;
	double mRadius;

	static std::vector<Obstacle*> svObstacles;
private:
	static CCamera *sCam;
};

#endif
