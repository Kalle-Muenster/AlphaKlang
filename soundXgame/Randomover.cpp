#include "Randomover.h"
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
		this->Connection()->move(getTransform()->rotation*0.003*speed);
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



bool
SmoothObjectMover::Initialize(void)
{
	purzelnOn=false;
 	movingVector.SetUserMode<SineControlled<float>>();
	movingVector.x.SetMIN(-30);
	movingVector.x.SetMAX(40);
	movingVector.x.SetMOVE(Circle/180);
	movingVector.y.SetMIN(0.5);
	movingVector.y.SetMAX(25);
	movingVector.y.SetMOVE(Circle/120);
	movingVector.z.SetMIN(-50);
	movingVector.z.SetMAX(50);
	movingVector.z.SetMOVE(Circle/220);
	movingVector.x = 10;
	movingVector.y = 1;
	movingVector.z = 0;
	movingVector.ControllersActive(true);
	return true;
}



void 
SmoothObjectMover::DoUpdate(void)
{
	this->Connection()->move(movingVector);
	//if(purzelnOn)
	//{
	//	Vector3 vec = this->Connection()->getTransform()->forward;
	//	this->Connection()->rotate(Vector3(vec.y*180,(vec.x*180) + (vec.z*90),0));
	//}
}