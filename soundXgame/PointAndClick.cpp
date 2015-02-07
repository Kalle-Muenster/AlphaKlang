#include "PointAndClick.h"
#include "ButtonControl.h"
#include "SliderX.h"
#include "SceneGraph.h"
#include "CameraModesIncluder.h"

int
PointAndClick::ID = POINTANDCLICK;



bool mousereleased = true;

void
_backButtonClick_pNc(IConnectable* sender)
{	
	if(mousereleased)
	{
		SCENE->camera->ModeSocket->Get<PointAndClick>(POINTANDCLICK)->switchBack();
		mousereleased = false;
	}
}


void
PointAndClick::OnActivate(void)
{
	if(this->guiObject!=NULL)
	{
		this->guiObject->isVisible(true);

		camera->ModeSocket->Get<FirstPerson>(FIRSTPERSON)->DisableMouseCapture();
		glutSetCursor(GLUT_CURSOR_INHERIT);

		if(camera->GetTarget())
			if(camera->GetTarget()->conXtor->ShowDialog())
				{
					targetBindings = ((IObjection<Connectable<IEditable>>*)camera->GetTarget())->conXtor->operator IEditable *()->GetDialogBindings();
					((IObjection<Connectable<IEditable>>*)camera->GetTarget())->conXtor->operator IEditable *()->BindMenuObject(this->guiObject);
					((IObjection<Connectable<IEditable>>*)camera->GetTarget())->conXtor->operator IEditable *()->ShowDialog();
				}
			else
				this->guiObject->SetName(camera->GetTarget()->GetName());

			this->guiObject->GetConnected<ButtonControl>(1)->SetClickerFunc(_backButtonClick_pNc);
	}
	
	//this->guiObject->GetConnected<ButtonControl>(2)->SetClickerFunc(_button_fps_click);
	//this->guiObject->GetConnected<ButtonControl>(3)->SetClickerFunc(_exitButtonClick);

}

void
PointAndClick::switchBack(void)
{
	camera->ModeSocket->GetCameraMode<FirstPerson>()->EnableMouseCapture();
   this->guiObject->isVisible(false);
}


void
PointAndClick::BindGuiObject(GuiObject* bindToGUIobject)
{
	this->guiObject = bindToGUIobject;
	this->IsActive=false;
	this->guiObject->GetConnected<ButtonControl>(1)->SetClickerFunc(_backButtonClick_pNc);
	this->guiObject->isVisible(this->IsActive);

}

PointAndClick::PointAndClick(void)
{
	ModeName = "PointAndClick";
	isPrimarMode=false;
	lastCamMode = FIRSTPERSON;
}


PointAndClick::~PointAndClick(void)
{

}

bool 
PointAndClick::Initialize(void)
{
	InputManager::getInstance()->attachKey(this);
	InputManager::getInstance()->attachMouseMove(this);
	InputManager::getInstance()->attachMouseWheel(this);
	InputManager::getInstance()->attachSpecial(this);
	ID = this->CamModeID();
	return true;
}


void 
PointAndClick::UpdateMode(void)
{ 
	if(camera->GetTarget() != target)
	{
		target = camera->GetTarget();
		OnActivate();
	}

	
	if(INPUT->Mouse.LEFT.RELEASE)
		mousereleased=true;

	this->IsDirty=false;
}


void 
PointAndClick::keyPress(char key)
{

	if(IsActive)
	{
		if(key==27) //esc...
		  switchBack();
		
			
	}
}

void
PointAndClick::specialKeyPressed(int key)
{
	if(IsActive)
	{

		//UpdateMode();
	}
}

void
PointAndClick::mouseMotion(int newX, int newY)
{
	if(!IsActive)
	{
		this->guiObject->isVisible(false);
		return;
	}
}



