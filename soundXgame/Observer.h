#ifndef _CONNECTABLE_OBSERVER_
#define _CONNECTABLE_OBSERVER_

#include "projectMacros.h"
#include "InputManager.h"
#include "Connectable.h"
#include "VoxGrid.h"


class CInteractive : 
	public IInteractive, public IConnectable
{
public:
	CInteractive(void){}
	virtual ~CInteractive(void){};

	virtual void mouseQlicks(int button,bool IsPressed,glm::vec2 position)
	{
		if(IsPressed)
		printf("button \"%i\" pressed! ",button);
	}
	//virtual void mouseMotion(int x, int y)
	//{
	//	((VoxGrid*) this->Connection())->getTransform()->position.x = x;
	//	((VoxGrid*) this->Connection())->getTransform()->position.y = y;
	//}
	//virtual void mouseWheel(int wheel,WHEEL state)
	//{
	//	if(state!=WHEEL::NONE)
	//		((VoxGrid*) this->Connection())->MainSizzes.x += (float)state/10;
	//}
};






#endif