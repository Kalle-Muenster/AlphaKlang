#include "VoxGrid.h"
#include "Utility.h"
#include "projectMacros.h"
#include "DataStructs.h"
#include "Connectable.h"



VoxGrid::VoxGrid(void)
{
	this->position.x = &this->transform.position.x;
	this->position.y = &this->transform.position.y;
	Initialize("FromDaCode",true);
}

VoxGrid::VoxGrid(string ppmfile,bool addToScene)
{
	this->position.x = &this->transform.position.x;
	this->position.y = &this->transform.position.y;
	Initialize(ppmfile,addToScene);
}



void
VoxGrid::Initialize(string PPMfileName,bool addToScene)
{
	this->Loade(PPMfileName,SetVoxelerBuffer(&Voxlers[0]));
	if(addToScene)
		SetID(SCENE->Add(this));
	LockID();
	IsVisible=true;
	SetTheZed(&this->getTransform()->position.z);
	Mode(NORMAL);
	conXtor = new VoxControl();
	conXtor->SetConnection(this);
	flipt = 'x';
	Zflipt = false;
}



void
VoxGrid::SetTheZed(float *theOtherZed)
{
	int count = mapWidth*mapHeight;
	for(int i=0;i<count;i++)
		voxels[i].TheOtherZED = theOtherZed;
}



VoxGrid::~VoxGrid(void)
{
	
}


Vector3
VoxGrid::scale(Vector3 s)
{
	this->getTransform()->scale = s;
   	this->MainSizzes.x = s.x;
	this->MainSizzes.y = s.y;
	return s;
}

void 
VoxGrid::flip(char direction)
{
	if(direction<0)
	{
		direction = flipt+1;
		flipZ();
	}

	if(direction>'z')
	   direction='x';

	flipt = direction;
}

void 
VoxGrid::flipZ(void)
{
	Zflipt = !Zflipt;
	SetTheZed(Zflipt? &getTransform()->position.x : &getTransform()->position.z);
}





void
VoxGrid::draw(void)
{
	if(!IsVisible)
		return;

	glColor4b(255,255,255,255);
	IsGrounded(false);

	glPushMatrix();
		switch(mode)
		{
		case NORMAL:
			this->Draw(this->position);
			break;
		case BUNT:
			this->DrawBunt(this->position);
			break;
		case BYTE:
			this->DrawByte(this->position);
			break;
		}
	glPopMatrix();

}

void
VoxGrid::Mode(ColorMode colorMode)
{
	mode=colorMode;
}

