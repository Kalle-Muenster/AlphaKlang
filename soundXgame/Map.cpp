#include "Map.h"


Map::Map(void)
{
}

Map::Map(const char* objFile,const char* textureFile,bool addToSceneGraph)
{
		this->InitializeObject(objFile,addToSceneGraph);
		this->LoadTexture(textureFile);
}
Map::~Map(void)
{
}
