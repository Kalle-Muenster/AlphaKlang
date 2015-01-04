#include "TestConectables.h"
#include "CameraModesIncluder.h"

void
YetiInteractive::keyPress(char key)
{
	if(this->Connection()->GetID() == (SCENE->camera->GetTarget()->GetID()) )
	{
		bool ismoving = false;
		float X,Z;
		X=this->Connection()->getTransform()->position.x;
		Z=this->Connection()->getTransform()->position.z;
		switch (key)
			{
			case 'w':
				X+=0.01f;
				ismoving = true;
				break;
			case 's':
				X-=0.01f;
				ismoving = true;
				break;
			case 'a':
				Z+=0.01f;
				ismoving = true;
				break;
			case 'd':
				Z-=0.01f;
				ismoving = true;
				break;
			}
		if(ismoving)
			this->Connection()->move(X,this->Connection()->getTransform()->position.y,Z);

		printf("\nKeypress (\"%c\") recieved in Object: %s !\n",key,this->Connection()->GetName());
	}
}

