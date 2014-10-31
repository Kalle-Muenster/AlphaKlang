#ifndef _VOXJECT_
#define _VOXJECT_

#include "IGobject.h"
#include "VoxelMap.h"

typedef unsigned int ConID;
typedef IConnectable* Connectable;

class IvOxject : public VoxelMap ,public AbsGobject
{
protected:
	IConnectable* connector;
	ConID		  conID;
//	GobID		  gobID;
	Voxel		  Voxlers[65536];
	//TransformA    transform;

public:
	IvOxject(void);
	virtual ~IvOxject(void);
	virtual void Initiate(const char* ppmFileName);
	virtual void draw(void);
	virtual Vector3 move(float,float,float);
	virtual Vector3 scale(Vector3){return transform.scale;}
	virtual Vector3 rotate(float,float,float);
	template<typename C> C* AddConnectable(void)
	{
		return this->connector->AddConnectable<C>();
	}
	template<typename C> C* GetConnected(void)
	{
		return this->connector->GetConnected<C>();
	}
	template<typename C> bool HasConnectable(void);

	operator Connectable(void)
	{
		return this->connector;
	}

};

#endif