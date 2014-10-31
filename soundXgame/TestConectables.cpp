#include "TestConectables.h"


void
YetiInteractive::keyPress(char key)
{
	if(this->Connection()->GetObjectID() == (SCENE->camera->GetTarget()->GetObjectID()) )
	{
		bool ismoving = false;
		float X,Z;
		X=this->Connection()->getTransform()->position.x;
		Z=this->Connection()->getTransform()->position.z;
		switch (key)
			{
			case 'w':
				X+=0.01;
				ismoving = true;
				break;
			case 's':
				X-=0.01;
				ismoving = true;
				break;
			case 'a':
				Z+=0.01;
				ismoving = true;
				break;
			case 'd':
				Z-=0.01;
				ismoving = true;
				break;
			}
		if(ismoving)
			this->Connection()->move(Vector3(X,this->Connection()->getTransform()->position.y,Z));

		printf("\nKeypress (\"%c\") recieved in Object: %s !\n",key,this->Connection()->GetName());
	}
}


//void
//YetiInteractive::mouseMotion(int x, int y)
//{
//	if(this->Connection()->GetObjectID() == (SCENE->camera->GetTarget()->GetObjectID()) )
//	{
//		if(INPUT->Mouse.LEFT.HOLD)
//			this->Connection()->rotate(glm::vec3(this->Connection()->getTransform()->rotation.x+INPUT->Mouse.Movement.x,this->Connection()->getTransform()->rotation.y,this->Connection()->getTransform()->rotation.z));
//		else if(INPUT->Mouse.MIDDLE.HOLD)
//			this->Connection()->rotate(glm::vec3(this->Connection()->getTransform()->rotation.x,this->Connection()->getTransform()->rotation.y,this->Connection()->getTransform()->rotation.z+INPUT->Mouse.Movement.x));
//		else if(INPUT->Mouse.RIGHT.HOLD)
//		{
//			this->Connection()->rotate(glm::vec3(this->Connection()->getTransform()->rotation.x,this->Connection()->getTransform()->rotation.y+INPUT->Mouse.Movement.x,this->Connection()->getTransform()->rotation.z));
//			this->Connection()->rotate(glm::vec3(this->Connection()->getTransform()->rotation.x,this->Connection()->getTransform()->rotation.y,this->Connection()->getTransform()->rotation.z+INPUT->Mouse.Movement.y));
//		}
//	}
//}