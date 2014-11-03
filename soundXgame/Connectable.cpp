#include <stdio.h>
#include "IGobject.h"
#include "Combiner.h"
#include "DataStructs.h"
#include "Connectable.h"


bool
IConnectable::Not_hasInitialized(void)
{
	if(!this->_initialized)
	{
		for(int i=0; i < MAXIMUM_NUMBER_OF_CONNECTIONS ; i++)
		{	this->ConIDs[i] = 0; this->Connectables[i]=NULL;}
		this->NumberOfConnectedObjects = 0;
		this->current = -1;

		this->_initialized=true;
		return this->_initialized;
	}
	return false;
}


IConnectable::~IConnectable(void)
{
	for(int i = 0; i < MAXIMUM_NUMBER_OF_CONNECTIONS ;i++)
		delete this->Connectables[i];
}


void
IConnectable::AddCombiner(IGobject* obje,ConID* id1,ConID* id2,int componentSlot)
{
		setConnectables(componentSlot,(new Combiner())->SetConiIDKeyPair(id1,id2));
}

IGobject* 
IConnectable::Connection(void)
{
	return (IGobject*)this->connection;
}

IConnectable*
IConnectable::getConnectables(int index)
{
	return this->Connectables[index];
}

void
IConnectable::setConnectables(int index,IConnectable* connectable)
{
	this->Connectables[index]=connectable;
	this->ConIDs[index]=connectable->ConnectionID;
}

int
IConnectable::GetNumberOfConnected(void)
{
	return this->NumberOfConnectedObjects;
}

void 
IConnectable::SetConnection(IGobject* gobject)
{
	connection=gobject;
	ConnectionID=0;
}



void
IConnectable::SetConnection(IConnectable* connectable)
{
	ConnectionID = ++connectable->current;
	connection = connectable->Connection();
}

TransformA*
IConnectable::getTransform(void)
{
		return this->Connection()->getTransform();
}


CTransform::CTransform(void)
{
}


CTransform::~CTransform(void)
{
}


//
//void
//CTransform::Initiator(void)
//{
//	
//}



TransformA*
CTransform::getTransform(void)
{
	TransformA* returner = new TransformA();
	returner->position = this->Connection()->getTransform()->position + this->Connection()->getTransform()->position;
	returner->rotation = this->Connection()->getTransform()->rotation + this->Connection()->getTransform()->rotation;
	returner->scale = this->Connection()->getTransform()->scale * this->Connection()->getTransform()->scale;
	returner->movement = this->Connection()->getTransform()->movement;
	return returner;
}

CTransform::operator TransformA*(void)
{
	return this->getTransform();
}

