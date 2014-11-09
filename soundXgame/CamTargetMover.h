#ifndef _CAMTARGETMOVER_H_
#define _CAMTARGETMOVER_H_

#include "connectable.h"
#include "projectMacros.h"
#include "Cam.h"

class CamTargetMover : public IConnectable, public IInteractive
{
public:
	CamTargetMover(void);
	virtual ~CamTargetMover(void);
	bool IsActive;
	virtual void mouseMotion(int x,int y);
};

#endif