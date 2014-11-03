#include "CCamera.h"

CCamera::CCamera()
	: mZoom(1), mTranslation(0,0), mCamFallow(false), mTarget(NULL)
{
}

void CCamera::SetTranslation(Vec2 translation)
{
	mTranslation=translation;
}

void CCamera::SetTranslation(double x, double y)
{
	mTranslation.x=x;
	mTranslation.y=y;
}

void CCamera::SetZoom(double zoom)
{
	mZoom=std::max(zoom,0.1);
}

Vec2 CCamera::GetTranslation()
{
	return mTranslation;
}

Vec2 CCamera::GetTransformedVec2(Vec2 vec)
{
	vec+=mTranslation;
	vec=vec*mZoom;
	return vec;
}

void CCamera::SetCamTarget(StarShip *target)
{
	mTarget=target;
}

void CCamera::EnableCamFallow()
{
	mCamFallow=true;
}
	
void CCamera::DisableCamFallow()
{
	mCamFallow=false;
}

double CCamera::GetZoom()
{
	return mZoom;
}

void CCamera::Update()
{
	if(mCamFallow)
	{
		Vec2 playerPos=mTarget->GetPosition();
		mTranslation = Vec2(-playerPos.x+((double)VIRTUALSCREEN_WIDTH)/2.0/mZoom,-playerPos.y+((double)VIRTUALSCREEN_HEIGHT)/2.0/mZoom);
	}
}

