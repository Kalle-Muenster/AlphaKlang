#ifndef _CAMERAMODE_TARGETGRABBER_H_
#define _CAMERAMODE_TARGETGRABBER_H_

#include "cam.h"


class TargetGrabber :
	public CameraMode
{
public:
	TargetGrabber(void);
	virtual ~TargetGrabber(void);
	static int StaticCamModeID;
	virtual bool Initialize(void);
	IGObject* GrabTarget(void);
protected:
	virtual void UpdateMode(void);
	bool _targetGRABBED;

};

#endif