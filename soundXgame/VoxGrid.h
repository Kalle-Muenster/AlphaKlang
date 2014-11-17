#ifndef __VOXGRID_H__
#define __VOXGRID_H__
#include "Connectable.h"
#include "InputManager.h"
#include "DataStructs.h"
#include "IVoxelObject.h"


class VoxelInteractive : public IConnectable,public IInteractive
{
public:
	VoxelInteractive(void){};
	virtual ~VoxelInteractive(void){};

	virtual void mouseMotion(int iX,int Yps)
	{
		this->Connection()->getTransform()->position.x = (float)iX;
		this->Connection()->getTransform()->position.y = (float)Yps;
	}
};

class VoxGrid :
	public IVoxelObject
{
public:
	VoxGrid(void);
	VoxGrid(string ppmFileName);
	virtual ~VoxGrid(void);
	//virtual void draw(void);
};

#endif
