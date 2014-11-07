#include "Cubus.h"
#include "Utility.h"
#include "projectMacros.h"
#include "AudioObject.h"

Cubus::Cubus(void)
{
	InitializeCubus("cube.obi","tex_wendy.jpg");
	this->IsVisible=true;

}


Cubus::~Cubus(void)
{
}

void
Cubus::InitializeCubus(string meshfile,string texturefile)
{
	this->SetID(SCENE->Add(this));
	Utility::loadObj(meshfile,verts,uvs,norms);
	this->textureID = Utility::loadTexture(texturefile);
	
	this->SetName("Cubus");

	this->AddConnectable<AudioEmitter>();
	this->GetConnected<AudioEmitter>()->DoUpdate();
}