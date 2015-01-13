#ifndef __VOXGRID_H__
#define __VOXGRID_H__

#include "VoxelMap.h"
#include "Connectable.h"
#include "InputManager.h"
#include "DataStructs.h"
#include "VoxControl.h"
#include "Transform.h"
#include "IGobject.h"

struct Vector3;


typedef unsigned int ConID;


class VoxGrid :
	public VoxelMap ,public IGObject
{
public:
	enum ColorMode
	{NORMAL=0,BUNT=1,BYTE=2};
private:

	void			SetTheZed(float*);
	void			Initialize(string PPMfileName="FromDaCode",bool addToScene=true);
	bool			Zflipt;
	GLfloat			bumpFactor;
	ColorMode		mode;
	ConID			conID;
	Voxel			Voxlers[16384+64];

public:
					VoxGrid(void);
					VoxGrid(string ppmFileName,bool addToScene=true);
	virtual		   ~VoxGrid(void);
	virtual void	draw(void);
	void			Mode(ColorMode);
	void			flipZ(void);
	void			flip(char= -1);
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
