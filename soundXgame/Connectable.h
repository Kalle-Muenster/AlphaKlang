#ifndef _CONNECTABLE_H_
#define _CONNECTABLE_H_

#include "IGobject.h"
//class IGobject;
struct Transform;

#define MAXIMUM_NUMBER_OF_CONNECTIONS (10)

typedef unsigned int ConID;

class IConnectable
{
private:
	void AddCombiner(IGobject*,ConID*,ConID*,int);

public:
	unsigned ConnectionID;
	IGobject* connection;
	bool _initialized;
	IConnectable* Connectables[MAXIMUM_NUMBER_OF_CONNECTIONS];
	unsigned ConnectTo(IConnectable*);
	unsigned ConIDs[MAXIMUM_NUMBER_OF_CONNECTIONS];
	bool Not_hasInitialized(void);
	int current;
	int NumberOfConnectedObjects;
	virtual Transform* getTransform(void);

public:
	IConnectable(void)
	{
		_initialized=false;
		Not_hasInitialized();
		NumberOfConnectedObjects=0;
		current= -1;
	}

	virtual ~IConnectable(void);


	//Returns the Connaction's owner...
	//The "Main"-Object where all other Connectables are connected to
	//or the "Head" of the Connection...
	virtual IGobject* Connection(void);
	
	

	//Adds a Component to the Object were its called on...
	//the component later can be getted with the Objects "GetConnected<IConnectable>()"-function...
	//if you later want to call it by ID, you can get it's ID later by "GetConnectionID<IConnectable>()"
	template<typename T> T* AddConnectable(void)
	{

	Not_hasInitialized();
		
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(ConIDs[i]==0)
		{
			IConnectable* newcon = new T();
			newcon->SetConnection(this->connection);	
			newcon->ConnectionID=typeid(T).hash_code(); 
			setConnectables(i,(T*)newcon);
			return (T*)getConnectables(i);
		}
		return NULL;
	}

	//Adds a Component to the Object were its called on, and retrieves
	//the Connection's ID-number via the given Pointer witch later can be used
	//to get the component via the faster "GetConnected<IConnectable>(ConID)"-function...
	template<typename T> T* AddConnectable(ConID *id)
	{

	Not_hasInitialized();
		
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(ConIDs[i] == 0)
		{
			IConnectable* newcon = new T();
			newcon->SetConnection(this->connection);
			newcon->ConnectionID = i+1;
			setConnectables(i,(T*)newcon);
			*id=i+1;
			return (T*)getConnectables(i);
		}
		return NULL;
	}


	//Get's the Component defined by "T" of the Object where it's called on... 
	//Its slower at Runtime but faster at "Write-time" then calling by ID...
	template<typename T> T* GetConnected(void)
	{
		for(int i = 0; i < MAXIMUM_NUMBER_OF_CONNECTIONS ;i++)
			if(ConIDs[i] == (ConID)typeid(T).hash_code())
				return (T*)getConnectables(i);
		return NULL;
	}

	//Gets a component of an Object by givin its Connection-ID...
	//Workes mutch faster....
	template<typename T> T* GetConnected(ConID conid)
	{
		return (T*)getConnectables(conid-1);
	}


	//Get's the ID of a "IConnectable" Component...
	//use this, if you want to call a Component by it's ID , but it was
	//previously Addet by Type,.. and not by ID...
	template<typename T> ConID GetConnectionID(void)
	{
		for(int i = 0; i < MAXIMUM_NUMBER_OF_CONNECTIONS ;i++)
			if(ConIDs[i] == (ConID)typeid(T).hash_code())
				return ConIDs[i]-1;
	}




	//UNREADY-SECTION...
	// helper-functions and "under-construction" stuff...
	// most of it should be private next time...

	IConnectable* getConnectables(int index);
	void setConnectables(int index,IConnectable* connectable);
	int GetNumberOfConnected(void);


	template<typename T> T* AddConnectable(IGobject* gobject)
	{

	Not_hasInitialized();
		
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(gobject->conXtor->ConIDs[i]==0)
		{
			IConnectable* newcon = new T();
			newcon->SetConnection(gobject);
			newcon->ConnectionID=ConIDs[i]=(ConID)typeid(T).hash_code();
				setConnectables(i,(T*)newcon);
			return (T*)getConnectables(i);
		}
		return NULL;
	}

	ConID* ConnectConnectableInstance(IConnectable* inst)
	{
		for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
			if(inst->Connection()->conXtor->ConIDs[i]==0)
		{
			this->Connectables[i]=inst;
			inst->Connection()->conXtor->ConIDs[i]=i+1;
			this->ConIDs[i]=inst->Connection()->conXtor->ConIDs[i];
			setConnectables(i,inst);
			return &this->ConIDs[i];
		}
			return NULL;
	}


	void SetConnection(IGobject*);
	void SetConnection(IConnectable*);

	// Link's two Conections conXtor<->conXtor-whise and returns a pointer to the newly Added Conection's ConID,
	// wich then will contain this Conection's ConID-Key as it's value. the ConID of this Object
	// (where the AddConection-function has been called on) will contain the newAdded-Conection's
	// ConID-Key as its value in oposide. so both Object's connected Components and Member-functions
	// are Accsessible by eachother. So it is Possible to "merge" two objects from different Classes to one new
	// object at runtime. the new "merged" object will contain all functionality and Components-sets of both Instances it is merged from...
	// -> calling "Conection()" on this Object's Components, will return this Object. (as usual..)
	// -> calling "Conection()" on the other Object's Components, will also return this Object, (not the other Object itself)
	ConID* AddConnection(IGobject* instance)
	{
		int x=0;
		for(int i= 0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
			if(ConIDs[i]==0)
			{
				for(int n = 0;n<MAXIMUM_NUMBER_OF_CONNECTIONS;n++)
					if(instance->conXtor->ConIDs[n]==0)
					{
						this->ConIDs[i]=n+1;
						instance->conXtor->ConIDs[n]=i+1;
						instance->conXtor->SetConnection(this);

						setConnectables(i,instance->conXtor);
						
						while(ConIDs[i+(++x)]!=0);
						ConIDs[x+=i]=x+1;
											
						AddCombiner(this->Connection(),&this->ConIDs[i],&instance->conXtor->ConIDs[n],x);
						instance->conXtor->setConnectables(n,this);
						//this->SetConnection(instance);
						return &this->ConIDs[i];
					}
			}
	}

	
};

class CTransform : 
	public IConnectable
{
private:
	Transform* transform;

public:
	CTransform(void);
	virtual void Initiator(Transform* origin)
	{
		transform = origin;
	}
	virtual ~CTransform(void);
	virtual Transform* getTransform(void);
	virtual operator Transform*();
};

#endif