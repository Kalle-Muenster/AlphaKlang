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
	Voxel		  Voxlers[16384+64];

public:

	IVoxelObject(void);
	virtual ~IVoxelObject(void);
	virtual void InitializeObject(string ppmFileName,bool addToScene=true);
	virtual void draw(void);



};

#endif