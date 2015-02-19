#include "Edit.h"
#include "ButtonControl.h"
#include "SliderX.h"
#include "SceneGraph.h"


int
Edit::ID = -1;
GuiObject* _panel;
bool _NoPanelGenerated=true;
int lastCamMode = FIRSTPERSON;
bool _mousereleased_edit = true;

void
_button_fps_click(IConnectable* sender)
{	
	if(_mousereleased_edit)
	{
		if(SCENE->ShowFPS)
		{
			GUI->Panel("Main-Menu")->GetConnected<ButtonControl>(2)->SetText("Hide FPS");
			SCENE->ShowFPS = false;
		}
		else
		{	
			GUI->Panel("Main-Menu")->GetConnected<ButtonControl>(2)->SetText("Show FPS");
			SCENE->ShowFPS = true;
		}
		_mousereleased_edit = false;
	}
}

void
_backButtonClick(IConnectable* sender)
{
	if(_mousereleased_edit)
	{
		GUI->Panel("Main-Menu")->isVisible(false);
		SceneGraph::getInstance()->camera->Mode(lastCamMode);
		_mousereleased_edit = false;
	}
}

void
_exitButtonClick(IConnectable* sender)
{
	if(_mousereleased_edit)
	{
		ProjectMappe::EXIT(true);
		_mousereleased_edit = false;
	}
}

void
Edit::switchBack(void)
{
	_backButtonClick(GUI->Panel("Main-Menu")->GetConnected<ButtonControl>(1));
}


void
Edit::BindGuiObject(GuiObject* menu)
{
	this->IsActive=false;
	GUI->Panel("Main-Menu")->isVisible(this->IsActive);
	GUI->Panel("Main-Menu")->GetConnected<ButtonControl>(1)->SetClickerFunc(_backButtonClick);
	GUI->Panel("Main-Menu")->GetConnected<ButtonControl>(2)->SetClickerFunc(_button_fps_click);
	GUI->Panel("Main-Menu")->GetConnected<ButtonControl>(3)->SetClickerFunc(_exitButtonClick);
}

Edit::Edit(void)
{
	ModeName = "Edit";
	isPrimarMode=true;
}


Edit::~Edit(void)
{
}

bool 
Edit::Initialize(void)
{
	angle		= 0;
	lx			= 0;
	lz			= -1;
	x			= -17.0f*3.5f;
	y			= 0;
	z			= 17.0f*3.5f;
	eyeY		= 1;
	moveSpeed	= 0.5f;
	mouseSpeed	= 1.0f;
	mouseX = SCREENWIDTH/2;
	mouseY = SCREENHEIGHT/2;
	InputManager::getInstance()->attachKey(this);
	InputManager::getInstance()->attachMouseMove(this);
	InputManager::getInstance()->attachMouseWheel(this);
//	InputManager::getInstance()->attachSpecial(this);
	ID = this->CamModeID();
	accelerate = false;
	return true;
}


void 
Edit::UpdateMode(void)
{ 
	camera->move(x, eyeY, z);
	camera->rotate(x+lx, y + 1.7f, z+lz); 
	
	gluLookAt(camera->transform.position.x, camera->transform.position.y, camera->transform.position.z,
			  camera->transform.rotation.x,camera->transform.rotation.y,camera->transform.rotation.z,	0, 1, 0);

	if(INPUT->Mouse.LEFT.RELEASE)
		_mousereleased_edit=true;

	GUI->Write("Press Enter to start the Experience of Sound",256,236);

	this->IsDirty=false;
}


void 
Edit::keyPress(char key)
{
	
	if(IsActive)
	{
		switch(key)
		{
			case 13: // W
				switchBack();
				break;
		}
		UpdateMode();
	}
}

//void
//Edit::specialKeyPressed(int key)
//{
//	if(IsActive)
//	{
//	switch (key) {
//		case 13:   //Enter
//			switchBack();
//			break;
//		}
//	UpdateMode();
//	}
//}

void
Edit::mouseMotion(int newX, int newY)
{

	if(!IsActive)
	{
		GUI->Panel("Main-Menu")->isVisible(false);
		return;
	}

	if(INPUT->Mouse.RIGHT.HOLD)
	{
		// if mouse haven't change -> return
		if(newX == mouseX && newY == mouseY)
			return;

		// check difference between last-frame mouse pos
		int diffX = newX - mouseX;
		int diffY = newY - mouseY;

		// calculate
		angle += 0.005f * diffX * mouseSpeed;
		lx = sin(angle);
		lz = -cos(angle);
		eyeY += (float)diffY / 300;


		// set mouse pos center to screen
		mouseX = SCREENWIDTH / 2;
		mouseY = SCREENHEIGHT / 2;

		//center cursor...
		glutWarpPointer(mouseX, mouseY);

		// flag check data uptodate
		IsDirty=true;
		UpdateMode();
	}

}

void
Edit::OnActivate(void)
{
	glutSetCursor(GLUT_CURSOR_INHERIT);
	//update view
	camera->NeedViewUpdate=true;
	glutReshapeWindow(SCREENWIDTH-1,SCREENHEIGHT-1);

	GUI->Panel("Main-Menu")->isVisible(true);
	_mousereleased_edit=true;


	eyeY = 4.0f;
}


