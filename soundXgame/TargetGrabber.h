#ifndef _CAMERAMODE_TARGETGRABBER_H_
#define _CAMERAMODE_TARGETGRABBER_H_

#include "cam.h"


class TargetGrabber :
	public CameraMode
{
public:
	
	enum class MODE : short
	{get= -1,OFF=0,MOVE=1,ROTATE=2,SCALE=3, };
	MODE Mode(MODE = MODE::get);
	TargetGrabber(void);
	virtual ~TargetGrabber(void);
	static int StaticCamModeID;
	virtual bool Initialize(void);
	IGObject* GrabTarget(void);
	bool IsATargetGrabbed(void);
	void ReleaseTarget(void);

protected:
	virtual void UpdateMode(void);
	MODE _mode;
	bool _targetGRABBED;
};

#endif