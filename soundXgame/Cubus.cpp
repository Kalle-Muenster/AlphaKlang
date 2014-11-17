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
	this->scale(Vector3(0.5f,0.5f,0.5f));
}

Cubus::Cubus(string textureFile,bool transparent)
{
	InitializeCubus(textureFile,"03.wav",transparent);
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
	this->AddConnectable<CamTargetRotator>();
}

void
Cubus::InitializeCubus(string texturefile,bool useTransparenz)
{
	InitializeCubus();
	LoadTexture(texturefile);
	this->UseTransparenz = useTransparenz;
}

void 
Cubus::InitializeCubus(string texturefile,string audiofile,bool useTransparenz)
{
	InitializeCubus(texturefile,useTransparenz);
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

