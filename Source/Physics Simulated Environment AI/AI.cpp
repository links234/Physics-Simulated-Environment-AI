#include "AI.h"

extern BITMAP* hBackBuffer;
extern StarShipAI *pEntity;
extern StarShip *pPlayerShip;

std::vector<StarShipAI*> StarShipAI::svEntities;

StarShipAI::StarShipAI(StarShip *pShip, double warningDist, double evadeDist, double interceptCoefficient, double safeCoefficient)
	: myShip(pShip), mAction(AI_ACTION_PFB), targetShip(NULL), mActionAtTarget(AI_ACTIONTARGET_NOTARGET), warningDistance(warningDist+warningDist*RandomDouble(0.0,100.0)/100.0),
	evadeDistance(evadeDist+evadeDist*RandomDouble(0.0,50.0)/100.0), mInterceptCoefficient(interceptCoefficient), mSafeCoefficient(safeCoefficient)
{
	svEntities.push_back(this);
	blacklist.resize(0);
	myShip->SetThrusters(false,false,false,false);
}

double StarShipAI::LenardJones(double A, double B, double n, double m, double r, double scale, double ydisp, double xdisp, double atenuation)
{
	if(atenuation==0)
	{
		atenuation=1;
	}
	r-=xdisp;
	return ((-A/pow(r,n)+B/pow(r,m))*scale+ydisp)/atenuation;
}

void StarShipAI::Update()
{
	if(mAction==AI_ACTION_PFB)
	{
		Vec2 mypos=myShip->GetPosition();
		Vec2 myvel=myShip->GetVelocity();
		Vec2 targetPoint=mypos;
		if(mActionAtTarget==AI_ACTIONTARGET_CHASE)
		{
			targetPoint=targetShip->GetPosition();
		}
		else if(mActionAtTarget==AI_ACTIONTARGET_INTERCEPT)
		{
			Vec2 Sc=targetShip->GetPosition()-mypos;
			Vec2 Vc=targetShip->GetVelocity()-myvel;
			double tc=0;
			if(Vc.Magnitude()>0)
			{
				tc=Sc.Magnitude()/Vc.Magnitude();
			}
			tc*=mInterceptCoefficient;
			targetPoint=targetShip->GetPosition()+targetShip->GetVelocity()*tc;
		}
		else if(mActionAtTarget==AI_ACTIONTARGET_GOTO)
		{
			targetPoint=targetPosition;
		}
		Vec2 u;
		Vec2 ideal_velocity=Vec2(0,0);
		double d;
		Vec2 target_velocity;
		if(targetPoint!=mypos)
		{
			u=targetPoint-myShip->GetPosition();
			u.Normalize();
			d=targetPoint.Distance(myShip->GetPosition());

			ideal_velocity+=u*LenardJones(2,4,0.9,1.2,d,1e6,17.5e3,-85,100);
		}
		target_velocity=ideal_velocity;

		for(register int i=0;i<(int)StarShip::svShips.size();++i)
		{
			if(StarShip::svShips[i]!=myShip && (StarShip::svShips[i]!=targetShip || mActionAtTarget!=AI_ACTIONTARGET_NOTARGET))
			{
				StarShip *neighborShip=StarShip::svShips[i];
				d=neighborShip->GetPosition().Distance(mypos);
				double safeDistance=(myShip->mSafeDist+neighborShip->mSafeDist)*mSafeCoefficient;
				//safeDistance=75.0*mSafeCoefficient;
				if(d<safeDistance)
				{
					u=neighborShip->GetPosition()-mypos;
					if(u.Magnitude()==0)
					{
						u=Vec2(1,0);
						u.Rotate(DEG2RAD(RandomDouble(0.0,720.0)));
					}
					u.Normalize();
					ideal_velocity-=u*(pow(safeDistance-d,1));
				}
			}
		}

		for(register int i=0;i<(int)Obstacle::svObstacles.size();++i)
		{
			d=Obstacle::svObstacles[i]->mPosition.Distance(mypos);
			u=Obstacle::svObstacles[i]->mPosition-mypos;
			double radius=Obstacle::svObstacles[i]->mRadius;
			if(u.Magnitude()==0)
			{
				u=Vec2(1,0);
				u.Rotate(DEG2RAD(RandomDouble(0.0,720.0)));
			}
			if(d<radius)
			{
				u.Normalize();
				ideal_velocity-=u*(pow(radius-d,2));
			}
			Vec2 feeler=myShip->GetOrientation()*(myvel.Magnitude()+warningDistance);
			Vec2 p=u.Projection(feeler);
			Vec2 b=p-u;
			if(b.Magnitude()<radius && p.Magnitude()<feeler.Magnitude())
			{
				double m;
				u.Rotate(-feeler.Argument());
				if(RAD2DEG(u.Argument())>0)
				{
					m=1;
				}
				else
				{
					m=-1;
				}
				u=Obstacle::svObstacles[i]->mPosition-mypos;
				Vec2 perp=-u;
				perp.Rotate(DEG2RAD(m*-90.0));
				perp.Normalize();
				perp=perp*(radius+100*warningDistance);
				perp+=u;
				perp=-perp*10;
				ideal_velocity+=perp;
			}
		}
		
		bool in_danger=false;
		for(register int i=0;i<(int)blacklist.size();++i)
		{
			d=blacklist[i]->GetPosition().Distance(mypos);
			if(d<evadeDistance)
			{
				u=blacklist[i]->GetPosition()-myShip->GetPosition();
				u.Normalize();

				double LJ=LenardJones(2,4,0.9,1.2,evadeDistance-d,1e6,17.5e3,-85,100);
				ideal_velocity+=-u*pow(LJ,2);
				in_danger=true;
			}
		}
		if(in_danger)
		{
			ideal_velocity-=target_velocity;
		}

		if(ideal_velocity.Magnitude()>25.0)
		{
			MaintainVelocity(ideal_velocity);
		}
		else
		{
			myShip->SetThrusters(false,false,false,false);
		}
	}
}
	
void StarShipAI::Wait()
{
	mAction=AI_NO_ACTION;
}

void StarShipAI::Chase(StarShip *pShip)
{
	mAction=AI_ACTION_PFB;
	mActionAtTarget=AI_ACTIONTARGET_CHASE;
	targetShip=pShip;
	DeleteFromBlacklist(pShip);
}

void StarShipAI::Intercept(StarShip *pShip)
{
	mAction=AI_ACTION_PFB;
	mActionAtTarget=AI_ACTIONTARGET_INTERCEPT;
	targetShip=pShip;
	DeleteFromBlacklist(pShip);
}

void StarShipAI::GoTo(Vec2 mPosition)
{
	targetPosition=mPosition;
	mAction=AI_ACTION_PFB;
	mActionAtTarget=AI_ACTIONTARGET_GOTO;
}

void StarShipAI::Evade(StarShip *pShip)
{
	if(!BlacklistSearch(pShip))
	{
		blacklist.push_back(pShip);
	}
}

bool StarShipAI::BlacklistSearch(StarShip *pShip)
{
	bool found=false;
	for(register int i=0;i<(int)blacklist.size();++i)
	{
		if(blacklist[i]==pShip)
		{
			found=true;
			break;
		}
	}
	return found;
}

void StarShipAI::DeleteFromBlacklist(StarShip *pShip)
{
	for(register int i=0;i<(int)blacklist.size();++i)
	{
		if(blacklist[i]==pShip)
		{
			blacklist.erase(blacklist.begin()+i);
			break;
		}
	}
}

void StarShipAI::ClearBlacklist()
{
	blacklist.clear();
}

void StarShipAI::MaintainVelocity(Vec2 velocity)
{
	if(velocity==Vec2(0,0))
	{
		myShip->SetThrusters(false,false,false,false);
		return;
	}
	const double ANGULARVELOCITY_MIN=15.0;
	const double ANGULARVELOCITY_MAX=90.0;
	const double ANGLE_MIN=0.0;
	const double ANGLE_MAX=15.0;
	Vec2 u=velocity;
	bool left=false;
	bool right=false;
	bool front=false;
	bool back=false;
	u.Rotate(-myShip->GetOrientation().Argument());
	double angle=RAD2DEG(u.Argument());

	double angularVelocity=myShip->GetAngularVelocity();
	double distance=myShip->GetVelocity().Distance(velocity);

	double LIMIT_ANGULARVELOCITY=ANGULARVELOCITY_MIN+(ANGULARVELOCITY_MAX-ANGULARVELOCITY_MIN)*Fuzzy::Grade(angle,0.0,90.0);
	LIMIT_ANGULARVELOCITY+=LIMIT_ANGULARVELOCITY*Fuzzy::ReverseGrade(distance,0.0,500.0);
	double LIMIT_VELOCITY=velocity.Magnitude();
	double LIMIT_ANGLE=ANGLE_MIN+(ANGLE_MAX-ANGLE_MIN)*(Fuzzy::ReverseGrade(distance,0.0,200.0)*0.5);
	double ahead=Fuzzy::Trapezoid(angle,-60.0,-15.0,15.0,60.0);

	if(angle>LIMIT_ANGLE && !(angularVelocity>LIMIT_ANGULARVELOCITY) || angularVelocity<-LIMIT_ANGULARVELOCITY)
	{
		left=true;
	}
	else if(angle<-LIMIT_ANGLE && !(angularVelocity<-LIMIT_ANGULARVELOCITY || angularVelocity>LIMIT_ANGULARVELOCITY))
	{
		right=true;
	}
	if(ahead>0.0)
	{
		if(myShip->GetVelocity().Magnitude()<LIMIT_VELOCITY)
		{
			back=true;
		}
		else
		{
			front=true;
		}
	}
	else
	{
		front=true;
	}
	
	myShip->SetThrusters(front,back,left,right);
}

void StarShipAI::Delete(StarShipAI *pAI)
{
	for(register int i=0;i<(int)svEntities.size();++i)
	{
		if(svEntities[i]==pAI)
		{
			svEntities.erase(svEntities.begin()+i);
			break;
		}
	}
}

void StarShipAI::UpdateAll()
{
	for(register int i=0;i<(int)svEntities.size();++i)
	{
		svEntities[i]->Update();
	}
}

StarShipAI::~StarShipAI()
{
	Delete(this);
}
