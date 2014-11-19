#include "Cubus.h"
#include "Utility.h"
#include "projectMacros.h"
#include "AudioObject.h"
#include "CamTargetMover.h"
#include "CamTargetRotator.h"

Cubus::Cubus(void)
{
	InitializeCubus();
	SetCollisionSound("03.wav");
	this->GetConnected<AudioEmitter>()->DoUpdate();
	this->SetName("Cubus");
	this->IsVisible=true;
	this->scale(Vector3(0.5f,0.5f,0.5f));
}

Cubus::Cubus(string textureFile,bool backfaces)
{
	InitializeCubus(textureFile,"03.wav",backfaces);
	this->SetName("Cubus");
	this->IsVisible=true;
}

Cubus::Cubus(data32 color,bool backfaces)
{
	InitializeCubus();
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
Cubus::InitializeCubus(void)
{
	InitializeObject("cube_quads.obi",true);
	this->AddConnectable<CamTargetRotator>();
	this->NoBackfaceCulling = false;
}

void
Cubus::InitializeCubus(string texturefile,bool backfaces)
{
	InitializeCubus();
	LoadTexture(texturefile);
	this->NoBackfaceCulling = backfaces;
}

void 
Cubus::InitializeCubus(string texturefile,string audiofile,bool backfaces)
{
	InitializeCubus(texturefile,backfaces);
	SetCollisionSound(audiofile);
	this->GetConnected<AudioEmitter>()->DoUpdate();
}

void
Cubus::SetCollisionSound(string file)
{
	if(!this->HasConnected<AudioEmitter>())
		this->AddConnectable<AudioEmitter>();
	this->GetConnected<AudioEmitter>()->LoadeSample(file);
}

