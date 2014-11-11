#include "CamTargetRotator.h"

CamTargetRotator::CamTargetRotator(void)
{

}

CamTargetRotator::~CamTargetRotator(void)
{

}

void
CamTargetRotator::RotateOnMouseHold(void)
{
	if(INPUT->Mouse.MIDDLE.HOLD)
	{
		this->Connection()->rotate(this->Connection()->getTransform()->rotation.x,this->Connection()->getTransform()->rotation.y+INPUT->Mouse.Movement.x,this->Connection()->getTransform()->rotation.z);
		this->Connection()->rotate(this->Connection()->getTransform()->rotation.x,this->Connection()->getTransform()->rotation.y,this->Connection()->getTransform()->rotation.z+INPUT->Mouse.Movement.y);
	}
}

void
CamTargetRotator::DoUpdate(void)
{
	if(SCENE->camera->GetTarget()->GetID() == this->Connection()->GetID())
	{
		RotateOnMouseHold();
	}
}