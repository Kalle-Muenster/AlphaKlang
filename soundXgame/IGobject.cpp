#include <iostream>
#include "projectMacros.h"
#include "Utility.h"
#include "Connectable.h"
#include "IGObject.h"
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
	IsVisible=true;
	conXtor = new IConXtor();
	conXtor->SetConnection(this);
	GroundValue = 0;
	IsGrounded(false);
	AlwaysFaceMovingdirection=false;
}

IGObject::~IGObject(void)
{
	delete conXtor;
}

bool
IGObject::SetID(GobID id)
{
	if(conXtor->SetID(id)==id)
		return true;
	else
		return false;
}

unsigned
IGObject::LockID(void)
{
	if(conXtor->LockID(conXtor->GetID()))
		return conXtor->GetID();
	else 
		return EMPTY;
}

void
IGObject::InitializeObject(bool addToSceneGraph)
{
	if(addToSceneGraph)
	{
		SetID(SCENE->Add(this));
		LockID();
	}
}


void
IGObject::Action(IConnectable* sender)
{

}

GobID
IGObject::GetID(void)
{
	return this->conXtor->GetID();
}


const char* 
IGObject::GetName(void)
{
	return conXtor->GetName();
}

void
IGObject::SetName(char* name)
{
	conXtor->SetName(name);
}

Transform*
IGObject::getTransform()
{
	return &transform;
}


IGObject::operator IConnectable(void)
{
	return *conXtor;
}

#undef This;

