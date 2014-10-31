#include "Map.h"


Map::Map(void)
{
}

Map::Map(const char* objFile,const char* textureFile,bool addToSceneGraph)
{
	init(objFile,textureFile,addToSceneGraph);
}
Map::~Map(void)
{
}
