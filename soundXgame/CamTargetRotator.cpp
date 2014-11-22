#include "CamTargetRotator.h"
#include "CameraModesIncluder.h"

CamTargetRotator::CamTargetRotator(void)
{

}

CamTargetRotator::~CamTargetRotator(void)
{
	UPDATE->SignOutFromUpdate(this);
}

void
CamTargetRotator::RotateOnMouseHold(void)
{
		Vector3 temp = this->Connection()->getTransform()->rotation;
		temp.y += INPUT->Mouse.Movement.x/5;
		temp.z += INPUT->Mouse.Movement.y/5;

		this->Connection()->rotate(temp.x,temp.y,temp.z);
}

void
CamTargetRotator::DoUpdate(void)
{
	if(SCENE->camera->GetTarget()->GetID() == this->Connection()->GetID())
	{
		if(INPUT->Mouse.MIDDLE.HOLD)
		{
			RotateOnMouseHold();
		}
	}
}