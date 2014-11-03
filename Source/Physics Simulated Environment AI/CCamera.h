#ifndef CCAMERA_H
#define CCAMERA_H

#include "Main.h"

class StarShip;

class CCamera
{
public:
	CCamera();

	void SetTranslation(Vec2 translation);
	void SetTranslation(double x, double y);
	void SetZoom(double zoom);
	void SetCamTarget(StarShip *target);
	void EnableCamFallow();
	void DisableCamFallow();
	void Update();

	Vec2 GetTranslation();
	Vec2 GetTransformedVec2(Vec2 vec);

	double GetZoom();

private:
	StarShip *mTarget;
	Vec2 mTranslation;
	double mZoom;
	bool mCamFallow;
};

#endif
