#include "Cubus.h"
#include "Utility.h"
#include "projectMacros.h"
#include "AudioObject.h"
#include "CamTargetMover.h"
#include "CamTargetRotator.h"

Cubus::Cubus(void)
{
	InitializeCubus(true);
	SetCollisionSound("03.wav");
	this->GetConnected<AudioEmitter>()->DoUpdate();
	this->SetName("Cubus");
	this->IsVisible=true;
}

Cubus::Cubus(bool intialize)
{
	InitializeCubus(intialize);
	SetCollisionSound("03.wav");
	this->GetConnected<AudioEmitter>()->DoUpdate();
	this->SetName("Cubus");
	this->IsVisible=true;
}

Cubus::Cubus(string textureFile,bool backfaces,bool addToScene)
{
	InitializeCubus(textureFile,"03.wav",backfaces,addToScene);
	this->SetName("Cubus");
	this->IsVisible=true;
	
}

Cubus::Cubus(data32 color,bool backfaces,bool addToScene,Vector3 positionOffset)
{
	InitializeCubus(addToScene,positionOffset);
	this->color=color;
	this->SetName("Cubus");
	this->IsVisible=true;
	this->NoBackfaceCulling = backfaces;
}

Cubus::~Cubus(void)
{
	delete conXtor;
}

void 
Cubus::InitializeCubus(bool addToScene,Vector3 positionOffset)
{
	InitializeObject("cube_quads.obi",addToScene,positionOffset);
	this->AddConnectable<CamTargetRotator>();
	this->NoBackfaceCulling = false;
}

void
Cubus::InitializeCubus(string texturefile,bool backfaces,bool addToScene)
{
	InitializeCubus(addToScene);
	LoadTexture(texturefile);
	this->NoBackfaceCulling = backfaces;
}

void 
Cubus::InitializeCubus(string texturefile,string audiofile,bool backfaces,bool addToScene)
{
	InitializeCubus(texturefile,backfaces,addToScene);
	SetCollisionSound(audiofile);
	this->GetConnected<AudioEmitter>()->DoUpdate();
}

void
Cubus::SetCollisionSound(string file)
{
	if(!this->HasConnected<AudioEmitter>())
		this->AddConnectable<AudioEmitter>();
	//this->GetConnected<AudioEmitter>()->LoadeSample(file);
}

//Cubus*
//Cubus::CreateCuBus(IGObject* parent,int amount)
//{
//	theNext = new Cubus(false);
//	theNext->SetID(amount);
//	LockID();
//	theNext->conXtor=parent->conXtor;
//	theNext->transform = ((Cubus*)parent)->transform;
//	theNext->transform.position += ((Cubus*)parent)->transform.forward * ((Cubus*)parent)->transform.scale.z;
//	if(amount > 0)
//		return CreateCuBus(theNext,--amount);
//	else return theNext;
//}