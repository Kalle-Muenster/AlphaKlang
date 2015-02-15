#ifndef _SMOTHOBJECTMOVER_H_
#define _SMOTHOBJECTMOVER_H_

#include "connectable.h"
#include "UpdateManager.h"

class SmoothObjectMover 
	: 
	public IConnectable, 
	public IUpdateble
{
public:
	bool purzelnOn;
	ControlledVector3 movingVector;
	virtual bool Initialize(void);
	virtual void DoUpdate(void);
};

#endif