#ifndef __ISCHMENA_H__
#define __ISCHMENA_H__

#include <vector>
#include "projectGrafics.h"
#include "DataStructs.h"

class IConnectable;
struct Transform;
struct Vector3;

typedef unsigned int GobID;

class IGObject
{
private:
	GobID ID;
	bool _idIsSet;

protected:
	Transform transform;
	char Name[64];
	bool SetID(GobID);
	unsigned LockID(void);

public:
	IGObject(void);
	virtual ~IGObject(void);
	void InitializeObject(bool addToSceneGraph=true);
	bool IsVisible;
	virtual void draw(void)=0;
	Transform* getTransform(void);
	virtual Vector3 move(float,float,float) {return Vector3();};//=0;
	virtual Vector3 rotate(float,float,float) {return Vector3();};//=0;
	virtual Vector3 scale(Vector3) {return Vector3();};//=0;
	GobID GetID(void);
	char* GetName(void);
	void SetName(char*);
	IConnectable* conXtor;
	operator IConnectable();
	bool IsGrounded;

	template<typename T> T* AddConnectable(void)
	{
		return conXtor->AddConnectable<T>();
	}
	template<typename T> T* GetConnected(void)
	{
		return conXtor->GetConnected<T>();
	}
	template<typename T> bool HasConnected(void)
	{
		return conXtor->GetConnected<T>()!=NULL;
	}
};






#endif










