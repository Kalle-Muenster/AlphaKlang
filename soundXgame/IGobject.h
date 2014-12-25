#ifndef __ISCHMENA_H__
#define __ISCHMENA_H__

#include <vector>
#include "projectGrafics.h"
#include "DataStructs.h"
#include "UpdateManager.h"

class IConnectable;
//class Kollective;
//#include "Connectable.h"
struct Transform;
struct Vector3;


typedef unsigned int GobID;
typedef unsigned int ConID;

class IGObject //: public IUpdateble
{
private:
	GobID ID;
	bool _idIsSet;
	bool _isGrounded;
	
protected:

	Transform	transform;
	char		Name[64];
	bool		SetID(GobID);
	unsigned	LockID(void);

	void*		pUserData;
	

	// Ground
	float GroundValue;


public:
	IGObject(void);
	virtual ~IGObject(void);
	void			InitializeObject(bool addToSceneGraph=true);
	bool			IsVisible;
	virtual void	draw(void)=0;
	Transform*		getTransform(void);
	virtual Vector3 move(Vector3 m);
	virtual Vector3 rotate(Vector3 r);
	virtual Vector3	rotate(float rotationAngle,Vector3 axis);
	virtual Vector3 scale(Vector3 s);
	virtual Vector3 move(float X,float Y,float Z);
	virtual Vector3 rotate(float X,float Y,float Z);
	virtual Vector3 scale(float X,float Y,float Z);
	GobID			GetID(void);
	char*			GetName(void);
	void			SetName(char*);
	IConnectable*	conXtor;
	operator IConnectable();

	float			SomeValue;
	float			angle;
	bool			AlwaysFaceMovingdirection;

	// Ground
	bool IsGrounded();
	void IsGrounded(bool status);

	template<typename T> T* AddConnectable(void)
	{
		if(conXtor->GetConnected<T>()!=NULL)
			return conXtor->GetConnected<T>();

		return conXtor->AddConnectable<T>();
	}
	template<typename T> T* AddConnectable(ConID*id)
	{
			return conXtor->AddConnectable<T>(id);
	}
	template<typename T> T* GetConnected(ConID id=NULL)
	{
		if(id)
			return conXtor->GetConnected<T>(id);
		else
			return conXtor->GetConnected<T>();
	}
	template<typename T> bool HasConnected(void)
	{
		int i = -1;
		size_t t = typeid(T).hash_code();
		while((conXtor->ConIDs[++i]!=t)	&& (i<IConnectable::MaximumNumberOfConnectioms));
		if(i<IConnectable::MaximumNumberOfConnectioms)
			return true;
		else
			return false;
	}
	template<typename T> void Remove(ConID id=NULL)
	{
		if(this==NULL)
			return;
		if(id)
			conXtor->RemoveConnected<T>(id);
		else
			conXtor->RemoveConnected<T>();
		
		
	}

};


#endif