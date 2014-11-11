#include "IVoxelObject.h"
#include "Utility.h"
#include "projectMacros.h"
#include "DataStructs.h"
#include "Connectable.h"

IVoxelObject::IVoxelObject(void)
{
	conXtor = new IConnectable();
	conXtor->SetConnection(this);

	this->position.x = &this->transform.position.x;
	this->position.y = &this->transform.position.y;
}

void
IVoxelObject::Initiate(const char* ppmFileName)
{
	this->Loade(ppmFileName,SetVoxelerBuffer(&Voxlers[0]));
	SCENE->Add(this);
	IsVisible=true;
}

IVoxelObject::~IVoxelObject(void)
{
	delete conXtor;
}

void
IVoxelObject::draw(void)
{
	this->Draw(this->position);
}

Vector3
IVoxelObject::move(float iX,float Yps,float Zed)
{
	this->transform.position.x = iX;
	this->transform.position.y = Yps;
	this->transform.position.z = Zed;

	return this->getTransform()->position;
}

Vector3
IVoxelObject::rotate(float iX,float Yps,float Zed)
{
	this->getTransform()->rotation.x = iX;
	this->getTransform()->rotation.y = Yps;
	this->getTransform()->rotation.z = Zed;

	return	this->getTransform()->rotation;
}
	
template<typename C> bool 
IVoxelObject::HasConnectable(void)
{
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(this->conXtor->ConIDs[i] == typeid(C))
			return true;
	return false;
}