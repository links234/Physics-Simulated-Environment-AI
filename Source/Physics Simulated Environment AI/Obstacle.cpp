#include "Obstacle.h"

std::vector<Obstacle*> Obstacle::svObstacles;
CCamera *Obstacle::sCam;

Obstacle::Obstacle(Vec2 position, double radius)
	: mPosition(position), mRadius(radius)
{
	svObstacles.push_back(this);
}
	
void Obstacle::Draw(BITMAP* hBackBuffer)
{
	Circle(hBackBuffer,sCam->GetTransformedVec2(mPosition),mRadius*sCam->GetZoom(),CColor(255,0,0));
}

Obstacle::~Obstacle()
{
	Delete(this);
}

void Obstacle::DrawAll(BITMAP* hBackBuffer)
{
	for(register int i=0;i<(int)svObstacles.size();++i)
	{
		svObstacles[i]->Draw(hBackBuffer);
	}
}

void Obstacle::Delete(Obstacle* pObstacle)
{
	for(register int i=0;i<(int)svObstacles.size();++i)
	{
		if(svObstacles[i]==pObstacle)
		{
			svObstacles.erase(svObstacles.begin()+1);
			break;
		}
	}
}

void Obstacle::SetCamera(CCamera* pCamera)
{
	sCam=pCamera;
}