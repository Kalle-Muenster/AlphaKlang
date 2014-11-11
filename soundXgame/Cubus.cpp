#include "Cubus.h"
#include "Utility.h"
#include "projectMacros.h"
#include "AudioObject.h"
#include "CamTargetMover.h"
#include "CamTargetRotator.h"

Cubus::Cubus(void)
{
	InitializeCubus("X-3.png","03.wav");
	this->SetName("Cubus");
	this->IsVisible=true;
}


Cubus::~Cubus(void)
{
	delete conXtor;
}

void 
Cubus::InitializeCubus(void)
{
	InitializeObject("cube-quads.obi",true);
//	this->AddConnectable<CamTargetMover>();
	this->AddConnectable<CamTargetRotator>();
}

void
Cubus::InitializeCubus(string texturefile)
{
	InitializeCubus();
	LoadTexture(texturefile);
}

void 
Cubus::InitializeCubus(string texturefile,string audiofile)
{
	InitializeCubus(texturefile);
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

