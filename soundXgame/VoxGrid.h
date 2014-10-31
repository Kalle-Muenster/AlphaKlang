#ifndef __VOXGRID_H__
#define __VOXGRID_H__
#include "Connectable.h"
#include "InputManager.h"
#include "ivoxject.h"

class VoxelInteractive : public IConnectable,public IInteractive
{
public:
	VoxelInteractive(void){};
	virtual ~VoxelInteractive(void){};

	virtual void mouseMotion(int iX,int Yps)
	{
		this->Connection()->getTransform()->position.x = iX;
		this->Connection()->getTransform()->position.y = Yps;
	}
};

class VoxGrid :
	public IvOxject
{
public:
	VoxGrid(void);
	VoxGrid(const char* ppmfile);
	
	virtual ~VoxGrid(void);
//	virtual void draw(void);
};

#endif
