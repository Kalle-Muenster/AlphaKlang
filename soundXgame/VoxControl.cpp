#include "VoxControl.h"
#include "IVoxelObject.h"
#include "projectMacros.h"

bool __ZedMode = false;
float _YPS=0.01;
string _files[5];


VoxControl::VoxControl(void)
{
	_firstStart=true;
	tempvector = *Vector3::Zero;
	bumpmapchannel=0;
		_files[0]="buntbild_128.ppm";
		_files[1]="drei_.ppm";
		_files[2]="ich.ppm";
		_files[3]="tr.ppm";
		_files[4]="bluobbber.ppm";
		bumpmapchannel=0;
		bumpmapIndex=0;
		imageIndex=0;
}


VoxControl::~VoxControl(void)
{
}


bool 
VoxControl::Initialize(void)
{
		INPUT->attachKey(this);
		return true;
}


int _currentSellection=0;
void 
VoxControl::keyPress(char key)
{
	//SCENE->Object((unsigned)0)->IsVisible=false;
	if(key=='x')
	{
		if(++_currentSellection>=5)
			_currentSellection=0;
		printf("File \"%s\" Sellected...\n",_files[_currentSellection]);
	}
	if(key == 'r')
	{
		this->vConnection()->ReLoade(_files[imageIndex=_currentSellection]);
		printf("Image \"%s\" geloadedt...\n",_files[_currentSellection]);
		vConnection()->MainSizzes.x=0.2;
		vConnection()->MainSizzes.y=0.045f;
	}

	if(key == 'l')
	{
		this->vConnection()->LoadMap(_files[bumpmapIndex=_currentSellection],bumpmapchannel);
		printf("BumpMap file-\"%s\"-Channel %i geloadedt !\n",_files[_currentSellection],bumpmapchannel);
		if(++bumpmapchannel>3)
			bumpmapchannel=0;
	}
	if(key=='b')
		this->vConnection()->Mode(IVoxelObject::ColorMode::BLURED);
	if(key=='n')
		this->vConnection()->Mode(IVoxelObject::ColorMode::NORMAL);

	if(key=='v')
		SCENE->camera->SetTarget(this->Connection());

	if(key=='t')
		glDisable(GL_BLEND);

	if(key=='z')
		__ZedMode=true;
	else
		__ZedMode=false;
}



IVoxelObject* 
VoxControl::vConnection(void)
{
	return (IVoxelObject*)this->connection;
}

void
VoxControl::DoUpdate(void)
{
	if(INPUT->Mouse.LEFT.HOLD)
	{
		this->vConnection()->MainSizzes.x *= 1-INPUT->Mouse.Movement.x/300;
		if(!__ZedMode)this->vConnection()->MainSizzes.y *= 1+INPUT->Mouse.Movement.y/600;
		else this->vConnection()->move(0,0, INPUT->Mouse.Movement.y);
		printf("MainSizerX: %f ,MainSizerY: %f\n",this->vConnection()->MainSizzes.x,this->vConnection()->MainSizzes.y);
	}
	if(INPUT->Mouse.MIDDLE.HOLD)
	{
		tempvector = this->vConnection()->getTransform()->rotation;
		vConnection()->getTransform()->position.z = ( tempvector.z += INPUT->Mouse.Position.x);
		tempvector.y += INPUT->Mouse.Movement.y;
		this->vConnection()->rotate(tempvector.x,tempvector.y,tempvector.z);
	}

	if(INPUT->Mouse.WheelV==WHEEL::UP)
	{
		SCENE->camera->transform.position += Vector3(0,_YPS,0);
	}
else 
	if(INPUT->Mouse.WheelV==WHEEL::DOWN)
	{
		SCENE->camera->transform.position -= Vector3(0,_YPS,0);
	}
}