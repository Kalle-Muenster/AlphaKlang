#include "PointAndClick.h"
#include "ButtonControl.h"
#include "SliderX.h"
#include "SceneGraph.h"
#include "CameraModesIncluder.h"

ObjectMenu::ObjectMenu(bool withBackButton)
{
	SetUp("GUI/panel_256x256.png",false,false);
	ShowTitle=true;
	if(withBackButton)
		GenerateBackButton();
}

ObjectMenu::ObjectMenu(const char* panelImage,bool withBackButton/* =true */)
{
	SetUp(panelImage,false,false);
	ShowTitle=true;
	if(withBackButton)
		GenerateBackButton();
}

ObjectMenu::~ObjectMenu(void)
{

}

void
ObjectMenu::BindToObject(IObjection<IConXtor>* obj)
{
	attachedObjection = obj;
}

bool
ObjectMenu::isAttachedTo(void* obj)
{
	return (obj==attachedObjection);
}


int
PointAndClick::ID = POINTANDCLICK;

bool _mousereleasedPnC = true;

void
_backButtonClick_pNc(IConnectable* sender)
{	
	if(_mousereleasedPnC)
	{
		((ControllElement*)sender)->Connection()->isVisible(false);
		_mousereleasedPnC = false;
	}
}

void
PointAndClick::OnActivate(void)
{
	
}

int
PointAndClick::OnDeactivate(void)
{
	if(guiObject!=NULL)
	{	
		guiObject->isVisible(false);
	}
}

void
PointAndClick::switchBack(void)
{
	if(this->guiObject!=NULL)
		this->guiObject->isVisible(false);
}


void
PointAndClick::BindGuiObject(GuiObject* bindToGUIobject)
{
	this->guiObject = (ObjectMenu*)bindToGUIobject;
}

PointAndClick::PointAndClick(void)
{
	ModeName = "PointAndClick";
	isPrimarMode=false;
	lastCamMode = FIRSTPERSON;		   

}



void
PointAndClick::GenerateGUIPanel(void)
{
	ObjectMenu* guiding = new ObjectMenu();
	guiding->scale(Vector3(512,512,1));
	guiding->move(Vector3(20,20,0));
	guiding->ResetHard(false);
	guiding->SetName("Editor-Panel");
	guiding->isVisible(false);
	BindGuiObject(guiding);
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
PointAndClick::mouseWheel(int wheel, WHEEL state)
{


}

void 
PointAndClick::UpdateMode(void)
{ 
	if(camera->GetTarget() != target)
	{
		target = camera->GetTarget();
		guiObject->BindToObject(target);
	}
	
	if(INPUT->Mouse.LEFT.RELEASE)
		_mousereleasedPnC = true;

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
		//if(guiObject!=NULL)
		//	this->guiObject->isVisible(false);
		return;
	}
}



