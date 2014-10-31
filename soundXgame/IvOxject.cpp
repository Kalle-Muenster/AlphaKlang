#include "IvOxject.h"
#include "Utility.h"
#include "projectMacros.h"

IvOxject::IvOxject(void)
{
	connector = new IConnectable();
	connector->SetConnection((IGobject*)this);

	this->position.x = &this->transform.position.x;
	this->position.y = &this->transform.position.y;
}

void
IvOxject::Initiate(const char* ppmFileName)
{
	this->Loade(ppmFileName,SetVoxelerBuffer(&Voxlers[0]));
	SCENE->Add(this);
	IsVisible=true;
}

IvOxject::~IvOxject(void)
{
	delete connector;
}

void
IvOxject::draw(void)
{
	this->Draw(this->position);
}



Vector3
IvOxject::move(float iX,float Yps,float Zed)
{
this->transform.position.x = iX;
this->transform.position.y = Yps;
this->transform.position.z = Zed;



return this->getTransform()->position;
}

Vector3
IvOxject::rotate(float iX,float Yps,float Zed)
{
	this->getTransform()->rotation.x = iX;
	this->getTransform()->rotation.y = Yps;
	this->getTransform()->rotation.z = Zed;

return	this->getTransform()->rotation;
}
	




template<typename C> bool 
IvOxject::HasConnectable(void)
{
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(this->connector->ConIDs[i] == typeid(C))
			return true;
	return false;
}

//TransformA*	
//IvOxject::getTransform(void)
//{
//	return &transform;
//}