#ifndef _CONNECTABLE_H_
#define _CONNECTABLE_H_

//#include "IGObject.h"
#include "Transform.h"

struct Transform;

#define MAXIMUM_NUMBER_OF_CONNECTIONS (10)
#define EMPTY (4294967295u)

typedef unsigned int ConID;
class IConXtor;


class IDrawable
{
public:
	bool			IsVisible;
	data32			color;
	virtual void	draw(void){};
	virtual bool	isVisible(BOOL=3);
	virtual void	SetColor(data32 newColor);
	virtual void	SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

 class ILocatable 
{
protected:
	bool		_isGrounded;
	Transform	transform;
	float		GroundValue;
	

public:
	ILocatable(void);
	virtual Transform*	getTransform(void);
	virtual Vector3		move(float X,float Y,float Z);
	virtual Vector3		move(Vector3 p);
	virtual Vector3		rotate(float X,float Y,float Z);
	virtual Vector3		rotate(Vector3 r);
	virtual Vector3		rotate(float rotationAngle,Vector3 rotationAxis);
	virtual Vector3		scale(float X,float Y,float Z);
	virtual Vector3		scale(Vector3 s);
	bool				IsGrounded();
	void				IsGrounded(bool status);
	bool				AlwaysFaceMovingdirection;
	float				angle;
};


class IConnectable;
struct Transform;
struct Vector3;


typedef unsigned int GobID;
typedef unsigned int ConID;

template<class Objected>
class IObjection : public ILocatable, public IDrawable
{
//#define this ((IObjection<Objected>*)this)
public:
	Objected* conXtor;


	template<typename cT> cT* AddConnectable(void)
	{
		return this->conXtor->AddConnectable<cT>(); 
	}
	template<typename cT> cT* AddConnectable(ConID* id)
	{
		return conXtor->AddConnectable<cT>(id);
	}
	template<typename uT> uT* AddUnconnectable(void)
	{
		return (uT*)conXtor->AddConnectable<Connectable<uT>>();
	}
	template<typename uT> uT* GetUnconnectable(void)
	{
		return (uT*)conXtor->GetConnected<Connectable<uT>>();
	}
	template<typename cT> void Remove(ConID id=NULL)
	{
		if(this==NULL)
			return;
		if(id)
			conXtor->RemoveConnected<cT>(id);
		else
			conXtor->RemoveConnected<cT>();
	}
	template<typename cT> cT* GetConnected(ConID id=EMPTY)
	{
		if(id == EMPTY) return this->conXtor->GetConnected<cT>();
		else if(id == NULL) return this->conXtor->GetConnected<cT>(0);
		else return this->conXtor->GetConnected<cT>(id);
	}
	template<typename cT> cT* Set(cT* connectableInstance)
	{
		return this->conXtor->ConnectConnectable<cT>(connectableInstance);
	}
	template<typename cT> cT* Disconnect(ConID id=EMPTY)
	{
		cT* detached = (id==EMPTY)? this->conXtor->GetConnected<cT>() :this->conXtor->GetConnected<cT>(id);
		if(id==EMPTY) this->conXtor->Remove<cT>();
		else this->conXtor->Remove<cT>(id);
		return detached;
	}

	template<class cT> cT* GetOrAdd(void)
	{
		cT* get = conXtor->GetConnected<cT>();
		if(!get)
			get = conXtor->AddConnectable<cT>();

		return get;
	}

	virtual GobID GetID(void)
	{
		return GetConnected<IConXtor>(0)->GetID();
	}

	virtual const char* GetName(void)
	{
		return GetConnected<IConXtor>(0)->GetName();
	}

	virtual void SetName(char* name)
	{
		GetConnected<IConXtor>(0)->SetName(name);
	}

	operator IDrawable*(void)
	{
		return GetOrAdd<Connectable<IDrawable>>();
	}

	operator ILocatable*(void)
	{
		return GetOrAdd<Connectable<ILocatable>>();
	}

	//operator IConXtor(void)
	//{
	//	return GetOrAdd<IConXtor>();
	//}

	virtual Transform* getTransform()
	{
		return ((ILocatable*)this)->getTransform();
	}

	virtual void draw(void)
	{
		((IDrawable*)this)->draw();
	}

	virtual IObjection<Objected>* Connection(void)
	{
		return this;
	}
//#undef this
};

class IConnectable
{
protected:
	void AddCombiner(void*,ConID*,ConID*,int);
	bool needOneMoreStartupCycle;
	unsigned TypeHashCode;
	bool canHaveMultipleInstances;

public:
	
	static int MaximumNumberOfConnectioms;
	unsigned ConnectionID;
	void* connection;
	bool _initialized;
	IConnectable* Connectables[MAXIMUM_NUMBER_OF_CONNECTIONS];
	unsigned ConnectTo(IConnectable*);
	unsigned ConIDs[MAXIMUM_NUMBER_OF_CONNECTIONS];
	bool Not_hasInitialized(void);
	int current;
	int NumberOfConnectedObjects;
	virtual Transform* getTransform(void);
	bool IsActive;
	

public:
	IConnectable(void)
	{
		canHaveMultipleInstances=false;
		_initialized=false;
		Not_hasInitialized();
		NumberOfConnectedObjects=0;
		current= -1;
		TypeHashCode=(unsigned)typeid(IConnectable).hash_code();
	}

	virtual ~IConnectable(void);


	//Returns the Connaction's owner...
	//The "Main"-Object where all other Connectables are connected to
	//or the "Head" of the Connection...
	virtual IObjection<IConnectable>* Connection(void);
	
	//StartUp-Initializer...
	//override and put code in it for handling dependencies on other Components
	//or objects if there where some. it will bee executed each frame, as long as
	//you return "false" (-initialization-proces not completed). If everything
	//than is Setup right, return "true" to let the Component enter it's normal Runtime-Cycling...
	virtual bool Initialize(void)
	{return true;}


	void SetConnection(void*);
	void SetConnection(IConnectable*);

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
			newcon->SetConnection((IObjection<IConnectable>*)this->connection);	
			newcon->ConnectionID=typeid(T).hash_code(); 
			setConnectables(i,(T*)newcon);
			NumberOfConnectedObjects++;
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
			NumberOfConnectedObjects++;
			return (T*)getConnectables(i);
		}
		return NULL;
	}


	//Get's the Component defined by "T" of the Object where it's called on... 
	//Its slower at Runtime but faster at "Write-time" then calling by ID...
	template<typename T> T* GetConnected(void)
	{
		ConID TypeHash = (ConID)typeid(T).hash_code();
		if(TypeHash==TypeHashCode)
			return (T*)this;
		for(int i = 0; i < MAXIMUM_NUMBER_OF_CONNECTIONS ;i++)
			if(ConIDs[i] == TypeHash)
				return (T*)getConnectables(i);
		return NULL;
	}

	//Gets a component of an Object by giving its Connection-ID...
	//Works much faster....
	template<typename T> T* GetConnected(ConID conid)
	{
		return (T*)getConnectables(conid-1);
	}


	//Get's the ID of a "IConnectable" Component...
	//use this, if you want to call a Component by it's ID , but it was
	//previously Added by Type,.. and not by ID...
	template<typename T> ConID GetConnectionID(void)
	{
		ConID TypeHash = (ConID)typeid(T).hash_code();
		for(int i = 0; i < MAXIMUM_NUMBER_OF_CONNECTIONS ;i++)
			if(ConIDs[i] == TypeHash)
				return i+1;
		return NULL;
	}

	/*Removes a connected Component by it's type...*/
	template<typename T> void RemoveConnected(void)
	{
		ConID TypeHash = (ConID)typeid(T).hash_code();
		for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
			if(ConIDs[i]==TypeHash)
			{
				delete getConnectables(i);
				ConIDs[i] = NULL;
				NumberOfConnectedObjects--;
				printf("%s-ID:%i: connectable %s removed !\n",Connection()->GetName(),Connection()->GetID(),typeid(T).name());
			}		
	}

	/*Remove a connected Component by it's ID...*/
	template<typename T> void RemoveConnected(ConID id)
	{
		if(getConnectables(id-1)!=NULL)
		{
			printf("%s-ID:%i: removing connectable %s at slot %i !\n",Connection()->GetName(),Connection()->GetID(),typeid(*getConnectables(id-1)).name(),id-1);
			delete getConnectables(id-1);
			ConIDs[id-1]=NULL;
			NumberOfConnectedObjects--;
		}
	}

	//UNREADY-SECTION...
	// helper-functions and "under-construction" stuff...
	// most of it should be private next time...

	IConnectable* getConnectables(int index);
	void setConnectables(int index,IConnectable* connectable);
	int GetNumberOfConnected(void);


	template<typename T> T* AddConnectable(IObjection<IConnectable>* gobject)
	{

	Not_hasInitialized();
		
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(gobject->conXtor->ConIDs[i]==0)
		{
			IConnectable* newcon = new T();
			newcon->SetConnection(gobject);
			newcon->ConnectionID=ConIDs[i]=(ConID)typeid(T).hash_code();
			setConnectables(i,(T*)newcon);
			NumberOfConnectedObjects++;
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
			NumberOfConnectedObjects++;
			return &this->ConIDs[i];
		}
			return NULL;
	}




	// Link's two Connections conXtor<->conXtor-wise and returns a pointer to the newly Added Connection's ConID,
	// which then will contain this Connection's ConID-Key as it's value. the ConID of this Object
	// (where the AddConection-function has been called on) will contain the newAdded-Connection's
	// ConID-Key as its value in opposition. so both Object's connected Components and Member-functions
	// are Accessible by each other. So it is Possible to "merge" two objects from different Classes to one new
	// object at runtime. the new "merged" object will contain all functionality and Components-sets of both Instances it is merged from...
	// -> calling "Connection()" on this Object's Components, will return this Object. (as usual..)
	// -> calling "Connection()" on the other Object's Components, will also return this Object, (not the other Object itself)
	ConID* AddConnection(IObjection<IConnectable>* instance)
	{
		int x=0;
		for(int i= 0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		{
			if(ConIDs[i]==0)
			{
				for(int n = 0;n<MAXIMUM_NUMBER_OF_CONNECTIONS;n++)
					if((instance)->conXtor->ConIDs[n]==0)
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
		return NULL;
	}

	
};

template<typename NonConnectableType> 
class Connectable 
	: 
	public NonConnectableType,
	public IConnectable 
{
public:
	virtual bool Initialize(void)
	{
		TypeHashCode = typeid(this).hash_code();
		return true;
	}
};

typedef void(*action)(IConnectable*);

  class IConXtor : 
	public IConnectable
{
private:
	GobID ID;
	char Name[64];
	bool _idLocked;
protected:

		

	action ConnectionAction;
public:
		IConXtor(void);
		virtual ~IConXtor(void);
	GobID GetID(void);
	char* GetName(void);
	void SetName(char*);
	virtual bool Initialize(void);
	bool AddToSceneAndLockID(void);
	unsigned SetID(unsigned);
	bool LockID(unsigned);
};

class cTransform :
	public IConnectable,
	public Transform
{
public:
//	bool useTestTransform;
//	ITransform testTransform;
	cTransform(void);
	virtual ~cTransform(void);
	virtual bool Initialize(void);
};

class TransformPointer : 
	public IConnectable
{
private:
	Transform* transform;

public:
	TransformPointer(void);
	virtual void Initiator(Transform* origin)
	{
		transform = origin;
	}
	virtual ~TransformPointer(void);
	virtual Transform* getTransform(void);
	virtual operator Transform*();
};


 

class Dimensionality  : 
	public IConnectable, 
	public ITransform 
{
public:
	Dimensionality(void);
	virtual ~Dimensionality(void);
	virtual bool Initialize(void);
	Transform* getTransform(void);
};



#endif