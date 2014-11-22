#include "VoxControl.h"
#include "IVoxelObject.h"
#include "projectMacros.h"

bool __ZedMode = false;

VoxControl::VoxControl(void)
{
	_firstStart=true;
}


VoxControl::~VoxControl(void)
{
}


bool 
VoxControl::Initialize(void)
{
	//if(_firstStart)
	//{
	//	if(!(typeid(this->Connection())==typeid(IVoxelObject)))
	//		delete this;
	////	return _firstStart = false;
	////}
	////else
	////{
	//	return _firstStart = false;
	//}

	INPUT->attachKey(this);
	INPUT->attachMouseMove(this);
	INPUT->attachMouseWheel(this);

	return true;
}

void 
VoxControl::keyPress(char key)
{
	SCENE->Object((unsigned)0)->IsVisible=false;
	//this->Connection()->IsGrounded = false;

	if(key=='l')
		this->vConnection()->LoadMap("tr.ppm");

		
	if(key=='v')
		SCENE->camera->SetTarget(this->Connection());

	if(key=='t')
		glDisable(GL_BLEND);

	if(key=='z')
		__ZedMode=true;
	else
		__ZedMode=false;
}
void
VoxControl::mouseMotion(int x,int y)
{
	if(INPUT->Mouse.LEFT.HOLD)
	{
		this->vConnection()->MainSizzes.x += INPUT->Mouse.Movement.x/100;
		if(!__ZedMode)this->vConnection()->MainSizzes.y += INPUT->Mouse.Movement.y/100;
		else this->vConnection()->move(0,0, INPUT->Mouse.Movement.y);
	}
}
float _YPS = 0.5f;
void
VoxControl::mouseWheel(int wheel, WHEEL state)
{
	
	if(state==WHEEL::UP)
	{
		SCENE->camera->transform.position += Vector3(0,_YPS,0);
	}
else 
	if(state==WHEEL::DOWN)
	{
		SCENE->camera->transform.position -= Vector3(0,_YPS,0);
	}
}

IVoxelObject* 
VoxControl::vConnection(void)
{
	return (IVoxelObject*)this->connection;
}