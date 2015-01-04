#ifndef __VOXGRID_H__
#define __VOXGRID_H__

#include "Connectable.h"
#include "InputManager.h"
#include "DataStructs.h"
#include "IVoxelObject.h"
#include "VoxControl.h"
#include "Transform.h"


 //struct VectorA
 //{
 // float &x;
 // float &y;
 // float &z;
 //};




class VoxGrid :
	public IVoxelObject
{
private:

	void SetTheZed(float*);
	void Initialize(string PPMfileName="FromDaCode",bool addToScene=true);
	Transform transform2;
	

public:
	void flipZ(void);
	void flip(char);
	VoxGrid(void);
	VoxGrid(string ppmFileName,bool addToScene=true);
	virtual ~VoxGrid(void);
	virtual Transform* getTransform(void);
	virtual Vector3 scale(Vector3);
	template<typename T> T* Get(ConID id = NULL)
	{
		if(id)
			return (T*)this->conXtor->GetConnected<T>(id);
		else
			return (T*)this->conXtor->GetConnected<T>();
	}
};

#endif
