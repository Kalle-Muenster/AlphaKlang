#include <stdio.h>
#include "IGObject.h"
#include "Combiner.h"
#include "DataStructs.h"
#include "Connectable.h"

int
IConnectable::MaximumNumberOfConnectioms = MAXIMUM_NUMBER_OF_CONNECTIONS;


bool
IConnectable::Not_hasInitialized(void)
{
	if(!this->_initialized)
	{
		for(int i=0; i < MAXIMUM_NUMBER_OF_CONNECTIONS ; i++)
		{	this->ConIDs[i] = NULL; this->Connectables[i]=NULL;}
		this->NumberOfConnectedObjects = 0;
		this->current = -1;

		this->_initialized=IsActive=true;
		return this->_initialized;
	}

		if(needOneMoreStartupCycle)
			needOneMoreStartupCycle = !this->Initialize();

	return false;
}


IConnectable::~IConnectable(void)
{
	for(int i = 0; i < MAXIMUM_NUMBER_OF_CONNECTIONS ;i++)
	{
		if(Connectables[i]!=NULL)
		{
			RemoveConnected<IConnectable>(i);
			/*int index = 0;
			while(Connectables[i]->GetNumberOfConnected()>0)
			{
				if(Connectables[i]->getConnectables(index)!=NULL)
					Connectables[i]->RemoveConnected<IConnectable>(Connectables[i]->ConIDs[i]-1);
			}
			delete this->Connectables[i];*/
			NumberOfConnectedObjects--;
		}
	}
}


void
IConnectable::AddCombiner(IGObject* obje,ConID* id1,ConID* id2,int componentSlot)
{
		setConnectables(componentSlot,(new Combiner())->SetConiIDKeyPair(id1,id2));
}

IGObject* 
IConnectable::Connection(void)
{
	return this->connection;
}

IConnectable*
IConnectable::getConnectables(int index)
{
	return this->Connectables[index];
}

void
IConnectable::setConnectables(int index,IConnectable* connectable)
{
	this->Connectables[index] = connectable;
	this->ConIDs[index]= connectable->ConnectionID;
	this->needOneMoreStartupCycle = !connectable->Initialize();
}

int
IConnectable::GetNumberOfConnected(void)
{
	return this->NumberOfConnectedObjects;
}

void 
IConnectable::SetConnection(IGObject* gobject)
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

Transform*
IConnectable::getTransform(void)
{
		return this->Connection()->getTransform();
}


TransformPointer::TransformPointer(void)
{
}


TransformPointer::~TransformPointer(void)
{
}






Transform*
TransformPointer::getTransform(void)
{
	Transform* returner = new Transform();
	returner->position = this->Connection()->getTransform()->position + this->Connection()->getTransform()->position;
	returner->rotation = this->Connection()->getTransform()->rotation + this->Connection()->getTransform()->rotation;
	returner->scale = this->Connection()->getTransform()->scale + this->Connection()->getTransform()->scale;
	returner->movement = this->Connection()->getTransform()->movement;
	return returner;
}

TransformPointer::operator Transform*(void)
{
	return this->getTransform();
}


cTransform::cTransform(void)
{

}

cTransform::~cTransform(void)
{

}

bool
cTransform::Initialize(void)
{
//	this->testTransform = *ITransform::Zero;
//	this->useTestTransform = false;
	return	true;
}