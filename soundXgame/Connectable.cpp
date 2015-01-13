#include <stdio.h>
#include "IGObject.h"
#include "Combiner.h"
#include "DataStructs.h"
#include "Connectable.h"
#include "projectMacros.h"

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
	IConnectable::AddCombiner(void* igobject,ConID* id1,ConID* id2,int componentSlot)
{
		setConnectables(componentSlot,(new Combiner())->SetConiIDKeyPair(id1,id2));
}

IObjection<IConnectable>* 
IConnectable::Connection(void)
{
	return (IObjection<IConnectable>* )this->connection;
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
	IConnectable::SetConnection(void* gobject)
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



  IConXtor::~IConXtor()
  {

  }

bool
IConXtor::Initialize(void)
{
	return LockID(SetID(SCENE->Add((IDrawable*)this->Connection())));
}

const char* 
IConXtor::GetName(void)
	{
		 return &this->Name[0];
	}

void 
IConXtor::SetName(const char* name)
	{
		this->Name[63]='\0';
		char i = 0;
		do this->Name[i] = name[i];
		while( (++i<63) && (Name[i-1]!='\0') );
	}


unsigned 
IConXtor::SetID(unsigned id)
{
	if(!_idLocked)
		this->ID = id;

	return this->ID;
}

GobID
IConXtor::GetID(void)
{
	return this->ID;
}

bool
IConXtor::LockID(unsigned lockTO)
{
	if(ID==EMPTY)
		lockTO = ID = ObjectManagement::getInstance()->GenerateID();
	else if(ID==lockTO)
		this->_idLocked=true;

	return _idLocked;
}

Dimensionality::Dimensionality(void)
{
	dimension.position = *Vector3::Zero;
	dimension.rotation = *Vector3::Zero;
	dimension.scale =	 *Vector3::Zero;
	dimension.DimensionDirty=false;

	direction.forward = Utility::GlobalZ;
	direction.right =	Utility::GlobalX;
	direction.up =		Utility::GlobalY;
	direction.DirectionsDirty=false;

	physics.movement	= *Vector3::Zero;
	physics.thickness	= 0.5;
	physics.depth		= 1;
	physics.speed.SetUp(Controlled<float>::ControllMode::Clamp,-10,100,0.1,0);
}

Dimensionality::~Dimensionality(void)
{
	physics.speed.~ControlledFloat();
}

bool 
Dimensionality::Initialize(void)
{
	return true;
}

Transform* 
Dimensionality::getTransform(void)
{
	Transform t;
	t.position = dimension.position;
	t.rotation = dimension.rotation;
	t.scale = dimension.scale;
	t.movement = physics.movement;
	t.speed = (float)physics.speed;
	t.forward = direction.forward;
	t.right = direction.right;
	t.up = direction.up;

	return &t;
}

 bool
IDrawable::isVisible(BOOL setter)
{
	if(setter<3)
		IsVisible = setter;
	return IsVisible;
}


ILocatable::ILocatable(void)
{
	transform.position = *Vector3::Zero;
	transform.rotation = *Vector3::Zero;
	transform.scale	   =  Vector3(1,1,1);
	transform.forward  =  Utility::GlobalZ;
	transform.right    =  Utility::GlobalX;
	transform.up       =  Utility::GlobalY;
	transform.movement = *Vector3::Zero;
	transform.speed	   =  0;
	GroundValue		   =  0;
	angle			   =  0;
	_isGrounded		   =  false;
	AlwaysFaceMovingdirection = false;
}



Transform*
ILocatable::getTransform(void)
{
	return &transform;
}


Vector3 
ILocatable::move(float X,float Y,float Z)
			{return move(Vector3(X,Y,Z));}
Vector3 
ILocatable::move(Vector3 p)
					{
						getTransform()->movement = p - getTransform()->position;
							if(AlwaysFaceMovingdirection)
							{
								Vector3 vec = getTransform()->movement.normalized();
								getTransform()->forward = vec;
								getTransform()->right = Vector3(vec.y,-vec.x,-vec.z);
								getTransform()->up = Vector3(-vec.z,-vec.x,vec.y);
								getTransform()->rotation = Vector3(vec.y*180,(vec.x*180) + (vec.z*90),0);
							}
						getTransform()->position = p;
						return p;
					}





Vector3		
ILocatable::rotate(float rotationAngle,Vector3 rotationAxis)
					{
						angle=rotationAngle;
						getTransform()->rotation = rotationAxis;
						return rotationAxis;
					}
Vector3
ILocatable::rotate(Vector3 r)
					{
						getTransform()->rotation = r;
						angle=0;
						return r;
					}
 Vector3 
ILocatable::rotate(float X,float Y,float Z)
			{return rotate(Vector3(X,Y,Z));}

Vector3 
ILocatable::scale(float X,float Y,float Z)
			{return scale(Vector3(X,Y,Z));}
Vector3 
ILocatable::scale(Vector3 s)
					{
						getTransform()->scale = s;
						return s;
					}
	
bool
ILocatable::IsGrounded()
{
	return _isGrounded;
}

void
ILocatable::IsGrounded(bool status)
{
	_isGrounded = status;
}
