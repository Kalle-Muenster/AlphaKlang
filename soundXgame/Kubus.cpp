#include "Kubus.h"

//

Kubus::Kubus(void)
{
	conXtor =  new ICubeXtor();
	conXtor->SetConnection(this);
	conXtor->SetName("last created");
	InitializeObject();
}

//#define this this->conXtor

Kubus::~Kubus(void)
{
}


void 
Kubus::InitializeObject(void)
{
	conXtor->AddToSceneAndLockID();
	color.byte[1] = 32;
	color.byte[2] = 128;
	color.byte[3] = 255;
	color.byte[0] = 128;
	conXtor->useTexture(false);
	conXtor->IsActive=true;
}

bool
Kubus::useTexture(BOOL use)
{
	return conXtor->useTexture(use);
}

void 
Kubus::LoadTexture(string textureFileName)
{
	scanf(textureFileName,"_%ix%i.",&conXtor->texture.w,&conXtor->texture.h);
	this->conXtor->texture.Loade(textureFileName,this->conXtor->texture.w,this->conXtor->texture.h,Texture::Format::RGBA);
}

void 
Kubus::draw(void)
{
	conXtor->draw();
}

Kubus::operator ICubeXtor(void)
{
	 return *conXtor;
}


//#undef this