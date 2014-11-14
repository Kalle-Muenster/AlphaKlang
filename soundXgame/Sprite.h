#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "IMeshObject.h"


class Sprite :
	public IMeshObject
{
private:
	void SetUp(string textureFile,bool tranzparent,bool addCamRotator); 
public:
	Sprite(void);
	virtual ~Sprite(void);
};

#endif