#ifndef _VOXJECT_
#define _VOXJECT_

#include "IGObject.h"
#include "VoxelMap.h"
struct Vector3;


typedef unsigned int ConID;
typedef IConnectable* Connectable;

class IVoxelObject : public VoxelMap ,public IGObject
{
protected:
	
	ConID		  conID;
	Voxel		  Voxlers[65536];

public:

	IVoxelObject(void);
	virtual ~IVoxelObject(void);
	virtual void Initiate(const char* ppmFileName);
	virtual void draw(void);
	virtual Vector3 move(float,float,float);
	virtual Vector3 scale(Vector3){return transform.scale;}
	virtual Vector3 rotate(float,float,float);

	template<typename C> C* AddConnectable(void)
	{
		return this->conXtor->AddConnectable<C>();
	}

	template<typename C> C* GetConnected(void)
	{
		return this->conXtor->GetConnected<C>();
	}

	template<typename C> bool HasConnectable(void);

	operator Connectable(void)
	{
		return this->conXtor;
	}

};

#endif