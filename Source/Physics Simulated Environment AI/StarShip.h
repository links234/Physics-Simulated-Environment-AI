#ifndef STARSHIP_H
#define STARSHIP_H

#include "Main.h"

class StarShip
{
public:
	StarShip(Vec2 position=Vec2(0,0), CColor color=CColor(255,255,255), double safeDist=50, double thrusterAcceleration=100.0f, double thrusterAngularAcceleration=360.0f, double dynamicFriction=0.005f, Vec2 orientation=Vec2(1,0), Vec2 velocity=Vec2(0,0), double angularVelocity=0, bool left=false, bool right=false);

	void Draw(BITMAP* hBackBuffer);
	void Update(float fDelta);
	void SetThrusters(bool front, bool back, bool left, bool right);
	void SetColor(CColor color);

	Vec2 GetPosition();
	Vec2 GetVelocity();
	Vec2 GetOrientation();
	double GetAngularVelocity();

	~StarShip();

	static void UpdateAll(double fDelta);
	static void DrawAll(BITMAP* hBackBuffer);
	static void Delete(StarShip *pShip);
	static void SetCamera(CCamera *pCamera);

	static std::vector<StarShip*> svShips;
	double mSafeDist;

private:
	Vec2 mPosition;
	Vec2 mVelocity;
	Vec2 mOrientation;
	double mThrusterAcceleration;
	double mAngularVelocity;
	double mThrusterAngularAcceleration;
	double mDynamicFriction;
	bool frontThruster;
	bool backThruster;
	bool leftThruster;
	bool rightThruster;
	CColor mColor;

	static CCamera *sCam;
};

#endif
