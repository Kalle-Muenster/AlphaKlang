#include "VoxGrid.h"
#include "projectMacros.h"
#include "VoxControl.h"

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
	flipped=false;
	InitializeObject(PPMfileName,true);
	SetTheZed();
	Mode(NORMAL);
	conXtor = new VoxControl();
	conXtor->SetConnection(this);
	
	pUserData = (new Transform());
//	this->AddConnectable<CTransform>(&conXtor->ConIDs[0]);
//	this->GetConnected<CTransform>()->Initiator(new Transform());
}

Transform*
VoxGrid::extraTransform(void)
{
	return  (Transform*)pUserData;
}

void
VoxGrid::SetTheZed(void)
{
	int count = mapWidth*mapHeight;
	for(int i=0;i<count;i++)
		voxels[i].TheOtherZED = &this->getTransform()->position.z;
}

VoxGrid::~VoxGrid(void)
{
	
}

Transform*
VoxGrid::getTransform(void)
{
	return flipped? extraTransform():&transform;
}




void 
VoxGrid::flip(Vector3 direction)
{
	
	Vector3 vec(getTransform()->position-direction+getTransform()->forward);
	flipped = !flipped;
	getTransform()->forward -= direction;
	getTransform()->right = getTransform()->forward.cros(direction);
	getTransform()->up = getTransform()->right.cros(direction);
	getTransform()->position = vec;

	SetTheZed();
}

//
//VoxGrid::operator VoxControll(void)
//{
//	return GetConnected<VoxControl>(conXtor->ConIDs[0]);
//}
