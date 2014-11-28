#ifndef _VOXJECT_
#define _VOXJECT_

#include "IGObject.h"
#include "VoxelMap.h"

struct Vector3;


typedef unsigned int ConID;
typedef IConnectable* Connectable;

class IVoxelObject : public VoxelMap ,public IGObject
{
public:
	enum ColorMode
	{NORMAL=0,BUNT=1,BYTE=2};
	IVoxelObject(void);
	virtual ~IVoxelObject(void);
	virtual void	InitializeObject(string ppmFileName,bool addToScene=true);
	virtual void	draw(void);
	void			Mode(ColorMode);

protected:
	GLfloat		  bumpFactor;
	ColorMode	  mode;
	ConID		  conID;
	Voxel		  Voxlers[16384+64];
};

#endif