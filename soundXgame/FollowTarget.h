#ifndef _CAMERAMODE_FOLLOWTARGET_H_
#define _CAMERAMODE_FOLLOWTARGET_H_

#include "cam.h"


class FollowTarget :
	public CameraMode
{
protected:
	virtual void UpdateMode(void);
//	IGObject*				_targetObject;				//the object the camaera alwas "look's" at if in FOLLOWTARGET-Mode...
//	Vector3*				_targetPosition;
//	float					_distanceToTarget;		//hold's the actual distance to the cam's target if in FOLLOWTARGET-Mode...

public:
	FollowTarget(void);
	virtual ~FollowTarget(void);
	static int ID;
	virtual bool Initialize(void);

	void					SetTarget(Vector3*);
	IGObject*				SetTarget(IGObject*);
	IGObject*				GetTarget(void);
	Vector3					GetPosition(void);
	float					GetDistance(void);
	void					follow(void);
	void					StopFollowing(void);
};

#endif