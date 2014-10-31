#ifndef __MAP_INCLUDET__
#define __MAP_INCLUDET__

#include "IGobject.h"

class Map :
	public IGobject
{
public:
	Map(void);
	Map(const char* objFile,const char* textureFile,bool addToSceneGraph);
	virtual ~Map(void);
};

#endif