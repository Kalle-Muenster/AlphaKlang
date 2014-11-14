#ifndef __MAP_INCLUDET__
#define __MAP_INCLUDET__

#include "IGObject.h"
#include "IMeshObject.h"

class Map :
	public IMeshObject
{
public:
	Map(void);
	Map(const char* objFile,const char* textureFile,bool addToSceneGraph);
	virtual ~Map(void);
};

#endif