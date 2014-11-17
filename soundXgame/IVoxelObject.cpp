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
	delete voxels;
}

void
IVoxelObject::draw(void)
{
	if(!IsVisible)
		return;
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glColor4b(255,255,255,255);
	IsGrounded=false;
	glPushMatrix();
		this->Draw(this->position);
	glPopMatrix();
}

