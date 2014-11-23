#include "Randomover.h"
#include "projectMacros.h"

//Randomover::Randomover(void)
//{
//}
//
//
//Randomover::~Randomover(void)
//{
//}
bool
Randomover::Initialize(void)
{
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

	Connection()->rotate(getTransform()->rotation.x+x,getTransform()->rotation.y+y,getTransform()->rotation.z+z);
	this->Connection()->move(getTransform()->rotation*0.01);
}



