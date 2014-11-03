#ifndef _TESTCONNECTABLES_H_
#define _TESTCONNECTABLES_H_

#include "observer.h"
#include "Cam.h"

class YetiInteractive :
	public CInteractive
{
public:
	YetiInteractive(void){}
	virtual YetiInteractive::~YetiInteractive(void){}

	virtual void keyPress(char key);
//	virtual void mouseMotion(int x, int y);

};

class CameraTargetRotator : public IConnectable , public IUpdateble
{
private:
	void RotateOnMouseHold(void)
	{
		/*if(INPUT->Mouse.LEFT.HOLD)
			this->Connection()->rotate(glm::vec3(this->Connection()->getTransform()->rotation.x+INPUT->Mouse.Movement.x,this->Connection()->getTransform()->rotation.y,this->Connection()->getTransform()->rotation.z));*/
		if(INPUT->Mouse.MIDDLE.HOLD)
			this->Connection()->rotate(this->Connection()->getTransform()->rotation.x,this->Connection()->getTransform()->rotation.y,this->Connection()->getTransform()->rotation.z+INPUT->Mouse.Movement.x);
		else if(INPUT->Mouse.RIGHT.HOLD)
		{
			this->Connection()->rotate(this->Connection()->getTransform()->rotation.x,this->Connection()->getTransform()->rotation.y+INPUT->Mouse.Movement.x,this->Connection()->getTransform()->rotation.z);
			this->Connection()->rotate(this->Connection()->getTransform()->rotation.x,this->Connection()->getTransform()->rotation.y,this->Connection()->getTransform()->rotation.z+INPUT->Mouse.Movement.y);
		}
	}
public:
	CameraTargetRotator(void){}
	virtual ~CameraTargetRotator(void){}
	
	virtual void DoUpdate(void)
	{
		
		if(SCENE->camera->GetTarget()->GetObjectID() == this->Connection()->GetObjectID())
		{
			RotateOnMouseHold();

		}

	}
};

#endif