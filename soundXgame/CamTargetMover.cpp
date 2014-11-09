#include "CamTargetMover.h"


CamTargetMover::CamTargetMover(void)
{
	INPUT->attachMouseMove(this);
	this->IsActive = true;
}


CamTargetMover::~CamTargetMover(void)
{
	
}

void
CamTargetMover::mouseMotion(int x,int y)
{
	if(IsActive)
	{
		if(!INPUT->Mouse.RIGHT.HOLD)
		{
			if(SCENE->camera->GetTarget()->GetID() == this->Connection()->GetID())
			{
				this->getTransform()->position.x += INPUT->Mouse.Movement.x;
				if(!INPUT->Mouse.MIDDLE.HOLD)
					this->getTransform()->position.y += INPUT->Mouse.Movement.y;
				else
					this->getTransform()->position.z += INPUT->Mouse.Movement.y;
			}
		}
	}
}