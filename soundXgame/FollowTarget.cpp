#include "FollowTarget.h"

int FollowTarget::ID = -1;

FollowTarget::FollowTarget(void)
{
	ModeName="FollowTarget";
	isPrimarMode = false;
}


FollowTarget::~FollowTarget(void)
{
}

bool
FollowTarget::Initialize(void)
{
	
	ID = this->CamModeID();
	return true;
}

void
FollowTarget::UpdateMode(void)
{
	if(camera->GetTarget()==NULL)
		this->IsActive=false;
	else
	{
		camera->move(camera->transform.position);
		camera->rotate(camera->GetTargetPosition());
	}
	this->IsDirty=false;
}

void
FollowTarget::SetTarget(Vector3 *position)
{
	camera->SetTarget(position);
}

IGObject*
FollowTarget::SetTarget(IGObject *targetObject)
{
	return camera->SetTarget((IObjection<IConnectable> *)targetObject);
}

IObjection<IConXtor>* 
FollowTarget::GetTarget(void)
{
	return camera->GetTarget();
}

void
FollowTarget::follow()
{
//	camera->followTarget();
	if(GetTarget()!=NULL)
		this->IsActive = true;
}

void
FollowTarget::StopFollowing()
{
	this->IsActive=false;
//	camera->stopFollowing();
}

float
FollowTarget::GetDistance(void)
{
	return camera->GetTargetDistance();
}

Vector3		
FollowTarget::GetPosition(void)
{
	return camera->GetTargetPosition();
}