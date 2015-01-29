#include "projectMacros.h"
#include "globalfunctions.h"
#include "Camera.h"




//Keyboard:
unsigned char lastKey='\0';
void ProjectMappe::keyboardInput(unsigned char key,int x,int y)
{
	/* Switches Cam-Modes..*/
	if(key=='p')
	{	
		if(lastKey!=key)
			SCENE->camera->Mode(FIRSTPERSON);
	}
	else if(key=='f')
	{	
		if(lastKey!=key)
			SCENE->camera->Mode(FOLLOWTARGET);
	}
	else if(key=='o')
	{
		if(lastKey!=key)
			SCENE->camera->Mode(SPECTATOR);
	}
	else if(key=='v')
	{
		if(lastKey!=key)
			SCENE->camera->Mode(StrangeChaoticView::ID);
	}
	else if(key=='e')
	{
		if(lastKey!=key)
			SCENE->camera->Mode(Edit::ID);
	}
	else if(key == 27) // ESC
	{
		if(lastKey!=key)
			ProjectMappe::EXIT(true);
	}

	lastKey = key;
	INPUT->registerKey(key);
}

void ProjectMappe::keyboardUpInput(unsigned char key,int x,int y)
{
	INPUT->registerKeyUp(key);
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