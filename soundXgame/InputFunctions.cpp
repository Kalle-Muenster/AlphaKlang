#include "projectMacros.h"
#include "globalfunctions.h"
#include "Camera.h"




//Keyboard:
void ProjectMappe::keyboardInput(unsigned char key,int x,int y)
{
	INPUT->registerKey(key);
}

void ProjectMappe::keyboardUpInput(unsigned char key,int x,int y)
{
	INPUT->registerKeyUp(key);

	
	/* Switches Cam-Modes..*/
	if(key=='p')
	{	
		SCENE->camera->Mode(FIRSTPERSON);
	}
	else if(key=='f')
	{	
		SCENE->camera->Mode(FOLLOWTARGET);
	}
	else if(key=='o')
	{
		SCENE->camera->Mode(SPECTATOR);
	}
	else if(key=='v')
	{
		SCENE->camera->Mode(StrangeChaoticView::ID);
	}
	else if(key == 27) // ESC
	{
		if(GUI->Element("Main-Menu")->IsVisible)
		{
			SCENE->camera->ModeSocket->GetCameraMode<Edit>()->switchBack();
		}
		else
		{
			SCENE->camera->Mode(Edit::ID);
		}
	}
}

void ProjectMappe::processSpecialKeys(int key, int xx, int yy)
{
	INPUT->notifySpecialKey(key);
}

//Mouse:
void ProjectMappe::MouseMoveFunc(int X,int Y)
{
	INPUT->notifyMouse(X,Y);
}

void ProjectMappe::MouseClicks(int button,int state,int X,int Y)
{
	INPUT->UpdateMouseButtons(button,state,X,Y);
}

void ProjectMappe::MouseWheelFunc(int wheel,int state,int X,int Y)
{
	INPUT->UpdateMouseWheel(wheel,state,X,Y);
}