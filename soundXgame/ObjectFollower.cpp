#include "ObjectFollower.h"
#include "ParticleSystem.h"

ObjectFollower::ObjectFollower(void)
{
	TypeHashCode = (unsigned)typeid(ObjectFollower).hash_code();
	IsActive=false;
	OffsetAsScale=true;
}

bool ObjectFollower::Initialize(void)
{
	return IsActive;
}

void ObjectFollower::SetTarget(IObjection<IConnectable>* targetObject)
{
	target = &targetObject->getTransform()->position;
	offset = &targetObject->getTransform()->movement;
	this->Connection()->AlwaysFaceMovingdirection = targetObject->AlwaysFaceMovingdirection;
	IsActive=true;
}

void ObjectFollower::SetTarget(Transform* TargetTransform)
{
	 target = &TargetTransform->position;
	 offset = &TargetTransform->movement;
	 this->Connection()->rotate(TargetTransform->rotation);
	 IsActive=true;
}

void ObjectFollower::SetTarget(Vector3 &TargetPosition)
{
	 target = &TargetPosition;
}

void ObjectFollower::SetOffSet(Vector3& offsetVector)
{
	offset = &offsetVector;
}

void ObjectFollower::DoUpdate(void)
{
	if(IsActive)
	{
		
		if(OffsetAsScale)
		{
			this->Connection()->move(*target);
			float scalar = (*offset).GetLength();
			this->Connection()->scale(scalar,scalar,scalar);
		}
		else
		{
		   	this->Connection()->move(*target + (*offset));
		}
	}
}							