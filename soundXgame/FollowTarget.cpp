//#include "FollowTarget.h"
//
//int FollowTarget::StaticCamModeID = -1;
//
//FollowTarget::FollowTarget(void)
//{
//	camera=SCENE->camera;
//}
//
//
//FollowTarget::~FollowTarget(void)
//{
//}
//
//bool
//FollowTarget::Initialize(void)
//{
//	
//	StaticCamModeID = this->CamModeID();
//	return true;
//}
//
//void
//FollowTarget::UpdateMode(void)
//{
//	camera->move(camera->transform.position);
//	camera->rotate(*_targetPosition);
//	this->IsDirty=false;
//}
//
//void
//FollowTarget::SetTarget(Vector3 *position)
//{
//	camera->SetTarget(position);
//}
//
//IGObject*
//FollowTarget::SetTarget(IGObject *targetObject)
//{
//	return camera->SetTarget(targetObject);
//}
//
//IGObject* 
//FollowTarget::GetTarget(void)
//{
//	return camera->GetTarget();
//}
//
//void
//FollowTarget::follow()
//{
//	camera->followTarget();
//}
//
//void
//FollowTarget::StopFollowing()
//{
//	this->IsActive=false;
//	camera->stopFollowing();
//}
//
//float
//FollowTarget::GetDistance(void)
//{
//	return camera->GetTargetDistance();
//}
//
//Vector3		
//FollowTarget::GetPosition(void)
//{
//	return camera->GetTargetPosition();
//}