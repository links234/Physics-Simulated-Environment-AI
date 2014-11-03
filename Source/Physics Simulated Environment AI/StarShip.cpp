#include "StarShip.h"

#define fScaleFactor 2.0f;

std::vector<StarShip*> StarShip::svShips;
CCamera *StarShip::sCam;

StarShip::StarShip(Vec2 position, CColor color, double safeDist, double thrusterAcceleration, double thrusterAngularAcceleration, double dynamicFriction, Vec2 orientation, Vec2 velocity, double angularVelocity, bool left, bool right)
	: mPosition(position), mColor(color), mThrusterAcceleration(thrusterAcceleration), mThrusterAngularAcceleration(thrusterAngularAcceleration), mDynamicFriction(dynamicFriction), mOrientation(orientation), mVelocity(velocity), mAngularVelocity(angularVelocity), leftThruster(left), rightThruster(right),
	frontThruster(false), backThruster(false), mSafeDist(safeDist)
{
	mOrientation.Normalize();
	svShips.push_back(this);
}

void StarShip::Draw(BITMAP* hBackBuffer)
{
	double angle=mOrientation.Argument();
	Vec2 P1(-20,15),P2(-20,-15),P3(20,-15),P4(25,0),P5(20,15);
	P1=P1/fScaleFactor; P1.Rotate(angle); P1+=mPosition;
	P2=P2/fScaleFactor; P2.Rotate(angle); P2+=mPosition;
	P3=P3/fScaleFactor; P3.Rotate(angle); P3+=mPosition;
	P4=P4/fScaleFactor; P4.Rotate(angle); P4+=mPosition;
	P5=P5/fScaleFactor; P5.Rotate(angle); P5+=mPosition;

	P1=sCam->GetTransformedVec2(P1);
	P2=sCam->GetTransformedVec2(P2);
	P3=sCam->GetTransformedVec2(P3);
	P4=sCam->GetTransformedVec2(P4);
	P5=sCam->GetTransformedVec2(P5);

	Line(hBackBuffer,P1,P2,mColor);
	Line(hBackBuffer,P2,P3,mColor);
	Line(hBackBuffer,P3,P4,mColor);
	Line(hBackBuffer,P4,P5,mColor);
	Line(hBackBuffer,P5,P1,mColor);
}
	
void StarShip::Update(float fDelta)
{
	double mAngularAcceleration=0.0f;
	double mAcceleration=0.0f;

	if(leftThruster)
	{
		mAngularAcceleration+=mThrusterAngularAcceleration;
	}
	if(rightThruster)
	{
		mAngularAcceleration-=mThrusterAngularAcceleration;
	}
	if(frontThruster)
	{
		mAcceleration-=mThrusterAcceleration/fScaleFactor;
	}
	if(backThruster)
	{
		mAcceleration+=mThrusterAcceleration*2/fScaleFactor;
	}
	if((leftThruster || rightThruster) && (!frontThruster || !backThruster))
	{
		mAcceleration+=mThrusterAcceleration/fScaleFactor;
	}

	mVelocity+=mOrientation*mAcceleration*fDelta;
	mVelocity-=mVelocity*mDynamicFriction;
	mPosition+=mVelocity*fDelta;

	mAngularVelocity+=mAngularAcceleration*fDelta;
	mAngularVelocity-=mAngularVelocity*mDynamicFriction;
	mOrientation.Rotate(DEG2RAD(mAngularVelocity*fDelta));
}

void StarShip::SetThrusters(bool front, bool back, bool left, bool right)
{
	if(left==right)
	{
		left=false;
		right=false;
	}
	if(front==back)
	{
		front=false;
		back=false;
	}
	leftThruster=left;
	rightThruster=right;
	frontThruster=front;
	backThruster=back;
}

void StarShip::SetColor(CColor color)
{
	mColor=color;
}

Vec2 StarShip::GetPosition()
{
	return mPosition;
}
	
Vec2 StarShip::GetVelocity()
{
	return mVelocity;
}

Vec2 StarShip::GetOrientation()
{
	return mOrientation;
}

double StarShip::GetAngularVelocity()
{
	return mAngularVelocity;
}

StarShip::~StarShip()
{
	Delete(this);
}

void StarShip::UpdateAll(double fDelta)
{
	for(register int i=0;i<(int)svShips.size();++i)
	{
		svShips[i]->Update(fDelta);
	}
}

void StarShip::DrawAll(BITMAP* hBackBuffer)
{
	for(register int i=0;i<(int)svShips.size();++i)
	{
		svShips[i]->Draw(hBackBuffer);
	}
}

void StarShip::Delete(StarShip *pShip)
{
	for(register int i=0;i<(int)svShips.size();++i)
	{
		if(svShips[i]==pShip)
		{
			svShips.erase(svShips.begin()+i);
			break;
		}
	}
}

void StarShip::SetCamera(CCamera *pCamera)
{
	sCam=pCamera;
}
