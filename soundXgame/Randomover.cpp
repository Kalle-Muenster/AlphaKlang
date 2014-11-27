#include "Randomover.h"
#include "projectMacros.h"

bool
Randomover::Initialize(void)
{
	rotation = true;
	moving = true;

	speed=Utility::GetRandom();
	x=y=z=0;
	iX = -0.001 * Utility::GetRandom();
	iY = 0.002 * Utility::GetRandom();
	iZ = 0.001* Utility::GetRandom();
	return true;
}

void Randomover::DoUpdate(void)
{
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
		this->Connection()->move(getTransform()->rotation*0.01);
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
	this->speed = speed;
}
