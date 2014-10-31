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
IConnectable::SetConnection(AbsGobject* gobject)
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
