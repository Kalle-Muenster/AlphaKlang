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
	void Initialize(string PPMfileName="FromDaCode");
	Transform transform2;
	

public:
	void flipZ(void);
	void flip(char);
	VoxGrid(void);
	VoxGrid(string ppmFileName);
	virtual ~VoxGrid(void);
	virtual Transform* getTransform(void);
	template<typename T> T* Get(ConID id = NULL)
	{
		if(id)
			return (T*)this->vConXtor()->GetConnected<T>(id);
		else
			return (T*)this->vConXtor()->GetConnected<T>();
	}
};

#endif
