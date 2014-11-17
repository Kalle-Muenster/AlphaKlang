#include "VoxGrid.h"
#include "projectMacros.h"

VoxGrid::VoxGrid(void)
{
	IGObject::InitializeObject(true);
	Loade("FromDaCode",&Voxlers[0]);
	SetID(SCENE->Add(this));
	LockID();
	IsVisible=true;
}

VoxGrid::VoxGrid(string ppmfile)
{
	InitializeObject((string)ppmfile);
	SetID(SCENE->Add(this));
	LockID();
	IsVisible=true;
}


VoxGrid::~VoxGrid(void)
{
	delete voxels;
}



