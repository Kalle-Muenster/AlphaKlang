#ifndef _RANDOMOVER_H_
#define _RANDOMOVER_H_

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


class Randomover 
	:
	public IConnectable,
	public IUpdateble
{
private:
	bool rotation, moving;
	float x,y,z,iX,iY,iZ;
	float speed;
protected:
	virtual bool Initialize(void);
public:
	void SetRotation(bool status);
	void SetMoving(bool status);
	void SetSpeed(float speed);
	virtual void DoUpdate(void);
	
};

#endif