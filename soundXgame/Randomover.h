#ifndef _RANDOMOVER_H_
#define _RANDOMOVER_H_

#include "connectable.h"
#include "UpdateManager.h"

class testDing : public IConnectable, public IUpdateble
{
public:
	Controlled<Vector3> vec;
	virtual bool Initialize(void)
	{
		vec.SetUp(Vector3(20,20,10),Vector3(-20,-20,10),*Vector3::Zero,Vector3(0.1,0.1,0),vec.PingPong);
	}
	virtual void DoUpdate(void)
	{
		this->Connection()->move(vec);
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