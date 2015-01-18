#ifndef _RANDOMOVER_H_
#define _RANDOMOVER_H_

#include "connectable.h"
#include "UpdateManager.h"

class testDing : public IConnectable, public IUpdateble
{
public:
	ControlledVector3 vec;
	virtual bool Initialize(void)
	{
		return true;
	}
	virtual void DoUpdate(void)
	{
		Vector3 temp = vec;
		this->Connection()->move(temp);
	}
};

class Randomover :
	public IConnectable ,public IUpdateble
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