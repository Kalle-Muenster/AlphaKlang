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

class IGObject : public IUpdateble
{
private:
	GobID ID;
	bool _idIsSet;
	bool _isGrounded;
	
protected:
	Transform transform;
	char Name[64];
	bool SetID(GobID);
	unsigned LockID(void);
	IGObject* theNext;

	// Ground
	float GroundValue;

public:
	IGObject(void);
	virtual ~IGObject(void);
	void InitializeObject(bool addToSceneGraph=true);
	bool IsVisible;
	virtual void draw(void)=0;
	Transform* getTransform(void);
	virtual Vector3 move(float,float,float){return Vector3();}// =0;
	virtual Vector3 rotate(float,float,float){return Vector3();}// =0;
	virtual Vector3 scale(float,float,float){return Vector3();}// =0;
	virtual Vector3 move(Vector3 m) {return move(m.x,m.y,m.z);};//=0;
	virtual Vector3 rotate(Vector3 r) {return rotate(r.x,r.y,r.z);};//=0;
	virtual Vector3 scale(Vector3 s) {return scale(s.x,s.y,s.z);};//=0;
	GobID  GetID(void);
	char* GetName(void);
	void SetName(char*);
	IConnectable* conXtor;
	operator IConnectable();

	// Ground
	bool IsGrounded();
	void IsGrounded(bool status);
	virtual void DoLate(void);

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
		return conXtor->GetConnected<T>()!=NULL;
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
	//void through(void)
	//{
	//	if(((Kollective*)conXtor)->checkFail())
	//		return;

	//	if(((Kollective*)theNext->conXtor)->through(GetID()))
	//		((Kollective*)conXtor)->apear(this);
	//	else 
	//		theNext->through();
	//}
};


#endif