#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "IMeshObject.h"


class Sprite :
	public IMeshObject
{
private:
	void SetUp(string textureFile,bool backface=true,bool addCamRotator=false); 
	void SetUp(Texture texture,bool backface=true,bool addRotator=false);
public:
	Sprite(void);
	Sprite(char*);
	virtual ~Sprite(void);
};

#endif