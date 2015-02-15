#include "SmoothObjectMover.h"


bool
SmoothObjectMover::Initialize(void)
{
	purzelnOn=false;
	movingVector.SetUserMode<SineControlled<float>>();
	movingVector.x.SetMIN(-30);
	movingVector.x.SetMAX(40);
	movingVector.x.SetMOVE(Circle/180);
	movingVector.y.SetMIN(20);
	movingVector.y.SetMAX(75);
	movingVector.y.SetMOVE(Circle/120);
	movingVector.z.SetMIN(-50);
	movingVector.z.SetMAX(50);
	movingVector.z.SetMOVE(Circle/220);
	movingVector.x = 10;
	movingVector.y = 25;
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