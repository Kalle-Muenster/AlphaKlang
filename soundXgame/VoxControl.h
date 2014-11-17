#ifndef __VOXCONTROLL_H__
#define __VOXCONTROLL_H__

#include "TestConectables.h"
#include "Connectable.h"


class VoxControl :
	public IConnectable, public IInteractive
{
private:
	bool _firstStart;
public:
	VoxControl(void);
	virtual ~VoxControl(void);
	virtual bool Initialize(void);
	virtual void keyPress(char key);
	virtual void mouseMotion(int x,int y);
	virtual void mouseWheel(int wheel, WHEEL state);
	virtual IVoxelObject* vConnection(void);
};

#endif