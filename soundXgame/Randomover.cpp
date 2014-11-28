#include "Randomover.h"
#include "projectMacros.h"

bool
Randomover::Initialize(void)
{
	rotation = true;
	moving = true;

//	speed = Utility::GetRandom();
	SetSpeed(Utility::Random()/10);
	Connection()->rotate(Utility::RandomDirection());
	x=y=z=0;
	iX =  Utility::Random(0.001);
	iY =  Utility::Random(0.002);
	iZ = -Utility::Random(-0.001);
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
		this->Connection()->move(getTransform()->rotation*0.005*speed);
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
