#include <iostream>
#include "projectMacros.h"
//#include "DataStructs.h"
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
	this->_idIsLocked=false;
	this->ID=NULL;
	conXtor = new IConnectable();
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
	if(this->_idIsLocked==false)
	{
		this->ID = id;
		itoa(this->ID,&this->Name[0],10);
		usedIDs.push_back(id);
	//	return this->_idIsSet=true;
	}
	else
		return false;
}

unsigned
IGObject::LockID(void)
{
	if(!this->_idIsLocked)
	{
		if(this->ID==NULL)
		{
			while(!_IDIsFree(++objIDs));
			this->ID = objIDs;
			usedIDs.push_back(this->ID);
			itoa(this->ID,&this->Name[0],10);
		}
	this->_idIsLocked=true;
	}
	return this->ID;
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


GobID
IGObject::GetID(void)
{
	return this->ID;
}

// Ground
bool
IGObject::IsGrounded()
{
	return _isGrounded;
}

void
IGObject::IsGrounded(bool status)
{
	_isGrounded = status;
}

Vector3 
IGObject::move(Vector3 m) 
{
	getTransform()->movement+=( m - getTransform()->position);
	//getTransform()->position = m;
	if(AlwaysFaceMovingdirection)
	{
		Vector3 fow = getTransform()->movement.normalized();
		Vector3 vec = ((getTransform()->position + fow)-(getTransform()->position + getTransform()->forward));
		getTransform()->forward = fow; 
		 printf("testoutput 1v = %f,%f,%f\n",fow.x,fow.y,fow.z);
		fow = getTransform()->right=(((getTransform()->position+getTransform()->right)+vec)-m);
		printf("testoutput 1r = %f,%f,%f\n",fow.x,fow.y,fow.z);
		fow = getTransform()->up = (((getTransform()->position+getTransform()->up)+vec)-m);
		 printf("testoutput 1u = %f,%f,%f\n\n",fow.x,fow.y,fow.z);
		
		vec = getTransform()->forward;
		fow = getTransform()->right = -Vector3(-vec.y,vec.x,vec.z);
		printf("testoutput 2r = %f,%f,%f\n",fow.x,fow.y,fow.z);
		fow = getTransform()->up = -Vector3(vec.z,vec.x,-vec.y);
	    printf("testoutput 2u = %f,%f,%f\n",fow.x,fow.y,fow.z);

	}

	#ifdef OBJECT_DEBUG_MESSAGES_ON
		printf("OBJECT: %s-%i: moved to X: %f, Y: %f, Z: %f\n",GetName(),GetID(),getTransform()->position.x,getTransform()->position.y,getTransform()->position.z);
	#endif
	return getTransform()->position;


}

Vector3 
IGObject::rotate(Vector3 r) 
{
	getTransform()->rotation = r;
	angle=0;
	return getTransform()->rotation;
}

Vector3
IGObject::rotate(float rotationAngle,Vector3 axis)
{
	return rotate(axis);
	angle = rotationAngle;
}

Vector3 
IGObject::scale(Vector3 s) 
{
	getTransform()->scale = s;
	return getTransform()->scale;
}

Vector3 
IGObject::move(float X,float Y,float Z)
{
	return move(Vector3(X,Y,Z));
}

Vector3 
IGObject::rotate(float X,float Y,float Z)
{
	return rotate(Vector3(X,Y,Z));
}

Vector3 
IGObject::scale(float X,float Y,float Z)
{
	return scale(Vector3(X,Y,Z));
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


