#include "VoxGrid.h"
#include "projectMacros.h"

VoxGrid::VoxGrid(void)
{
	Initialize();
	//Loade("FromDaCode",&Voxlers[0]);
	//SetID(SCENE->Add(this));
	//LockID();
	//IsVisible=true;
}

VoxGrid::VoxGrid(string ppmfile)
{
	Initialize((string)ppmfile);
	//SetID(SCENE->Add(this));
	//LockID();
	//IsVisible=true;
	//SetTheZed();
}

void
VoxGrid::Initialize(string PPMfileName)
{
	InitializeObject(PPMfileName,true);
	SetTheZed();
	Mode(NORMAL);
}

void
VoxGrid::SetTheZed(void)
{
	int count = mapWidth*mapHeight;
	for(int i=0;i<count;i++)
		voxels[i].TheOtherZED = &transform.position.z;
}

VoxGrid::~VoxGrid(void)
{
	
}



