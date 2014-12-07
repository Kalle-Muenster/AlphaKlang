#ifndef __VOXCONTROLL_H__
#define __VOXCONTROLL_H__

#include "TestConectables.h"
#include "Connectable.h"


class VoxControl :
	public IConnectable, public IInteractive, public IUpdateble
{
private:
//	int mode;
//	bool _firstStart;
	Vector3 tempvector;
	int bumpmapchannel;
	int imageIndex;
	int bumpmapIndex;
	bool transparenseEnabled;
	float timer;

public:
	VoxControl(void);
	virtual ~VoxControl(void);
	virtual bool Initialize(void);
	virtual void keyPress(char key);
	virtual IVoxelObject* vConnection(void);
	virtual void DoUpdate(void);
};

#endif