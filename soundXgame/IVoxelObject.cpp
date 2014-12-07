#include "IVoxelObject.h"
#include "Utility.h"
#include "projectMacros.h"
#include "DataStructs.h"
#include "Connectable.h"

IVoxelObject::IVoxelObject(void)
{

	this->position.x = &this->transform.position.x;
	this->position.y = &this->transform.position.y;
}

void
IVoxelObject::InitializeObject(string ppmFileName,bool addToScene)
{
	this->Loade(ppmFileName,SetVoxelerBuffer(&Voxlers[0]));
	SetID(SCENE->Add(this));
	LockID();
	IsVisible=true;
}

IVoxelObject::~IVoxelObject(void)
{
	//voxels = NULL;
}

void
IVoxelObject::draw(void)
{
	if(!IsVisible)
		return;
//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_CULL_FACE);
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
IVoxelObject::Mode(ColorMode colorMode)
{
	mode=colorMode;
}