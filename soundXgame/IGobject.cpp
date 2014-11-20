#include <iostream>
#include "projectMacros.h"
#include "DataStructs.h"
#include "Utility.h"
#include "Connectable.h"
#include "IGObject.h"
#include "kollective.h"
#include "Ground.h"

#define This this->conXtor->Connection()
static unsigned objIDs = 99;
std::vector<GobID> usedIDs = std::vector<GobID>();

bool
_IDIsFree(GobID id)
{
	for(auto it=usedIDs.begin();it!=usedIDs.end();it++)
		if((*it)==id)
			return false;
	return true;
}

IGObject::IGObject(void)
{
	IsVisible=false;
	this->_idIsSet=false;
	conXtor = new IConnectable();
	conXtor->SetConnection(this);
	IsGrounded=false;

}

IGObject::~IGObject(void)
{
	delete conXtor;
}

bool
IGObject::SetID(GobID id)
{
	if(this->_idIsSet==false)
	{
		this->ID = id;
		itoa(this->ID,&this->Name[0],10);
		usedIDs.push_back(id);
		return this->_idIsSet=true;
	}
	else
		return false;
}

unsigned
IGObject::LockID(void)
{
	if(!_idIsSet)
	{
		while(!_IDIsFree(++objIDs));
		this->ID = objIDs;
		usedIDs.push_back(this->ID);
		itoa(this->ID,&this->Name[0],10);
		this->_idIsSet=true;
		return this->ID;
	}
}

void
IGObject::InitializeObject(bool addToSceneGraph)
{
	if(addToSceneGraph)
		SetID(SCENE->Add(this));

	LockID();
}


GobID
IGObject::GetID(void)
{
	return this->ID;
}



char* 
IGObject::GetName(void)
{
	return &this->Name[0];
}

void
IGObject::SetName(char* name)
{
	int i = 0;
	while(i<63)
	{
		this->Name[i] = name[i];
		if(this->Name[i]=='\0')
			return;
		i++;
	}
	Name[63]='\0';
}

Transform*
IGObject::getTransform()
{
	return &This->transform;
}


IGObject::operator IConnectable(void)
{
	return *conXtor;
}







#undef This;


