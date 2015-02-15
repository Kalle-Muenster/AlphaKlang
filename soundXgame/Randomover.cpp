#include "Randomover.h"
#include "Ground.h"
#include "projectMacros.h"

bool
Randomover::Initialize(void)
{
	rotation = true;
	moving = true;

//	speed = Utility::GetRandom();
	SetSpeed(Utility::Random()/2);
	Connection()->rotate(Utility::RandomDirection());
	x=y=z=0;
	iX =  Utility::Random(0.002);
	iY =  Utility::Random(0.003);
	iZ = -Utility::Random(-0.002);
	return true;
}

void Randomover::DoUpdate(void)
{
	speed = this->Connection()->getTransform()->speed;

	x+=iX;
	y+=iY;
	z+=iZ;

	if(x>1 || x<-1)
		iX= -iX;
	if(y>1 || y<-1)
		iY= -iY;
	if(x>1 || x<-1)
		iZ= -iZ;

	if(this->rotation)
		this->Connection()->rotate(getTransform()->rotation.x+x*speed,getTransform()->rotation.y+y*speed,getTransform()->rotation.z+z*speed);
	if(this->moving)
	{
		Vector3 vec = getTransform()->rotation*0.003*speed;
		float ground = Ground::getInstance()->GetGroundY(vec.x, vec.z);
		if(this->Connection()->getTransform()->position.y<ground)
			offsetY*= -1;
		vec.y = ground +  offsetY;
		this->Connection()->move(vec);
	}
}

void
Randomover::SetYoffset(float offset)
{
   offsetY =  offset;
}

void
Randomover::SetRotation(bool status)
{
	this->rotation = status;
}

void
Randomover::SetMoving(bool status)
{
	this->moving = status;
}

void
Randomover::SetSpeed(float speed)
{
	this->Connection()->getTransform()->speed = speed;
}



