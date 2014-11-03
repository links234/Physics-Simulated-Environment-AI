#ifndef AI_H
#define AI_H

#include "Main.h"

#define AI_NO_ACTION				0
#define AI_ACTION_PFB				1

#define AI_ACTIONTARGET_NOTARGET	0
#define AI_ACTIONTARGET_CHASE		1
#define AI_ACTIONTARGET_INTERCEPT	2
#define AI_ACTIONTARGET_GOTO		3

class StarShip;

class StarShipAI
{
public:
	StarShipAI(StarShip *pShip, double warningDist=200.0, double evadeDist=200.0, double interceptCoefficient=1.5, double safeCoefficient=0.90);

	void Update();
	void Wait();
	void GoTo(Vec2 mPosition);
	void Chase(StarShip *pShip);
	void Intercept(StarShip *pShip);
	void Evade(StarShip *pShip);
	void DeleteFromBlacklist(StarShip *pShip);
	void ClearBlacklist();

	static void Delete(StarShipAI *pAI);
	static void UpdateAll();

	~StarShipAI();

private:
	int mActionAtTarget;
	int mAction;
	StarShip *myShip;
	StarShip *targetShip;
	Vec2 targetPosition;
	std::vector<StarShip*> blacklist;
	double warningDistance;
	double evadeDistance;
	double mInterceptCoefficient;
	double mSafeCoefficient;

	bool BlacklistSearch(StarShip *pShip);
	double LenardJones(double A, double B, double n, double m, double r, double scale=1.0, double ydisp=0, double xdisp=0, double atenuation=1);
	void MaintainVelocity(Vec2 velocity);

	static std::vector<StarShipAI*> svEntities;
};

#endif