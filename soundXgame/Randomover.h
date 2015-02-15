#ifndef _RANDOMOVER_H_
#define _RANDOMOVER_H_

#include "connectable.h"
#include "UpdateManager.h"




class Randomover 
	:
	public IConnectable,
	public IUpdateble
{
private:
	bool rotation, moving;
	float x,y,z,iX,iY,iZ;
	float speed;
	float offsetY;

protected:
	virtual bool Initialize(void);
public:
	void SetRotation(bool status);
	void SetMoving(bool status);
	void SetSpeed(float speed);
	void SetYoffset(float offset);
	virtual void DoUpdate(void);
	
};

#endif