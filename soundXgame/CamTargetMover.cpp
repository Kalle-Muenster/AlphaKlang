#include "CamTargetMover.h"
#include "FollowTarget.h"

CamTargetMover::CamTargetMover(void)
{
	INPUT->attachMouseMove(this);
}

bool
CamTargetMover::Initialize(void)
{
	return this->IsActive = this->Connection()!=NULL;
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
				if(SCENE->camera->Mode()==FIRSTPERSON)
				{
					this->Connection()->getTransform()->position = SCENE->camera->transform.forward * SCENE->camera->GetTargetDistance();
					if(!INPUT->Mouse.MIDDLE.HOLD)
						this->Connection()->getTransform()->position += SCENE->camera->transform.forward * INPUT->Mouse.Movement.y/100.0f;
				}
				else
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
}