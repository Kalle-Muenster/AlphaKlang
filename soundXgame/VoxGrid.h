#ifndef __VOXGRID_H__
#define __VOXGRID_H__
#include "Connectable.h"
#include "InputManager.h"
#include "DataStructs.h"
#include "IVoxelObject.h"
#include "VoxControl.h"

//class VoxelInteractive : public IConnectable,public IInteractive
//{
//public:
//	VoxelInteractive(void){};
//	virtual ~VoxelInteractive(void){};
//
//	virtual void mouseMotion(int iX,int Yps)
//	{
//		this->Connection()->getTransform()->position.x = (float)iX;
//		this->Connection()->getTransform()->position.y = (float)Yps;
//	}
//};



class VoxGrid :
	public IVoxelObject
{
private:
	bool flipped;
	Transform* extraTransform(void); 
	void SetTheZed(void);
	void Initialize(string PPMfileName="FromDaCode");
public:
	void flip(Vector3);
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
