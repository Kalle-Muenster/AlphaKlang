#ifndef _CAMTARGETROTATOR_H_
#define _CAMTARGETROTATOR_H_

#include "observer.h"
#include "Cam.h"

class CamTargetRotator : public IConnectable , public IUpdateble
{
private:
	void RotateOnMouseHold(void);
public:
	CamTargetRotator(void);
	virtual ~CamTargetRotator(void);
	
	virtual void DoUpdate(void);
};

#endif