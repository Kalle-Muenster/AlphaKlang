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
	SetTheZed(&this->getTransform()->position.z);
	Mode(NORMAL);
	conXtor = new VoxControl();
	conXtor->SetConnection(this);
	flipt = 'x';
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

Transform*
VoxGrid::getTransform(void)
{
	return IGObject::getTransform();
}


void 
VoxGrid::flip(char direction)
{
	if(direction>'z')
		flipt = 'x';
	else
		flipt = direction;
}

void 
VoxGrid::flipZ(void)
{
	SetTheZed(flipt? &getTransform()->position.x : &getTransform()->position.z);
}

//
//VoxGrid::operator VoxControll(void)
//{
//	return GetConnected<VoxControl>(conXtor->ConIDs[0]);
//}
